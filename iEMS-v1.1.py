import sys
import socket
import math
import time
from collections import deque
import pyqtgraph as pg
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QPushButton, QLabel, QWidget, QTextEdit, QTableWidget, QTableWidgetItem, QFrame, QHeaderView, QPushButton, QVBoxLayout
)
from PySide6.QtGui import QPainter, QPen, QColor, QBrush, QFont, QPixmap, QIcon
from PySide6.QtCore import QTimer, QThread, Qt, Signal, QSize
from PySide6.QtSvg import QSvgRenderer

ESP32_IP = "192.168.137.72"  # Replace with your ESP32 IP
PORT = 12345
class Worker(QThread):
    data_received = Signal(float, float, float, float, float, float)
    connection_status = Signal(bool)

    def __init__(self):
        super(Worker, self).__init__()
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connected = False
        self.stop_flag = False


    def run(self):
        while not self.stop_flag:
            if not self.connected:
                self.connect_to_esp()
            else:
                self.fetch_data()

    def connect_to_esp(self):
        try:
            print("Attempting to connect to ESP...")
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(5)  # Set a timeout for connection attempts
            self.socket.connect((ESP32_IP, PORT))
            self.connected = True
            self.connection_status.emit(True)
            print("Connection established.")
        except (socket.timeout, socket.error) as e:
            print(f"Connection error: {e}")
            self.connection_status.emit(False)
            self.connected = False
            self.sleep(3)  # Wait before retrying

    def fetch_data(self):
        try:
            data = self.socket.recv(1024).decode("utf-8").strip()
            lines = data.split("\n")
            if len(lines) >= 7:
                voltage = float(lines[0])
                current = float(lines[1])
                temperature = float(lines[3])
                grid_value = float(lines[4])
                solar_value = float(lines[5])
                battery_value = float(lines[6])
                self.data_received.emit(voltage, current, temperature, grid_value, solar_value, battery_value)
        except (socket.timeout, socket.error) as e:
            print(f"Data fetch error: {e}")
            self.connected = False
            self.connection_status.emit(False)
            self.reconnect()

    def reconnect(self):
        try:
            self.socket.close()
        except Exception:
            pass
        self.connect_to_esp()

    def stop(self):
        self.stop_flag = True
        self.socket.close()


class MultiRingGaugeWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.grid_value = 0
        self.solar_value = 0
        self.battery_value = 0

    def update_values(self, grid_value, solar_value, battery_value):
        self.grid_value = grid_value
        self.solar_value = solar_value
        self.battery_value = battery_value
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        rect = self.rect().adjusted(10, 10, -10, -10)
        radius = min(rect.width(), rect.height()) // 2 - 20
        center = rect.center()

        self.draw_ring(painter, rect, radius, self.grid_value, 300, QColor("#F05454"))
        self.draw_ring(painter, rect.adjusted(30, 30, -30, -30), radius - 30, self.solar_value, 100, QColor("#30BFBF"))
        self.draw_ring(painter, rect.adjusted(60, 60, -60, -60), radius - 60, self.battery_value, 100, QColor("#F5ED8E"))

    def draw_ring(self, painter, rect, radius, value, max_value, color):
        span_angle = (value / max_value) * 360
        painter.setPen(QPen(QColor(50, 50, 50), 15, Qt.SolidLine, Qt.RoundCap))
        painter.drawArc(rect, 0, 360 * 16)

        painter.setPen(QPen(color, 15, Qt.SolidLine, Qt.RoundCap))
        painter.drawArc(rect, -90 * 16, -span_angle * 16)

class CombinedGraph(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.graph_widget = pg.PlotWidget(self)
        self.graph_widget.setGeometry(0, 0, 540, 250)
        self.graph_widget.setTitle("Voltage & Current Over Time")
        self.graph_widget.setLabel('left', "Value")
        self.graph_widget.setLabel('bottom', "Time (s)")
        self.graph_widget.showGrid(x=True , y=True)

        self.x_data = deque(maxlen=100)
        self.voltage_data = deque(maxlen=100)
        self.current_data = deque(maxlen=100)

        self.voltage_plot = self.graph_widget.plot(pen=pg.mkPen(color='r', width=2), name="Voltage")
        self.current_plot = self.graph_widget.plot(pen=pg.mkPen(color='b', width=2), name="Current")

    def update_graph(self, voltage=None, current=None):
        new_x_value = self.x_data[-1] + 1 if self.x_data else 0
        
        self.x_data.append(new_x_value)
        self.voltage_data.append(voltage if voltage is not None else 0)
        self.current_data.append(current if current is not None else 0)
        
        self.voltage_plot.setData(list(self.x_data), list(self.voltage_data))
        self.current_plot.setData(list(self.x_data), list(self.current_data))

class TemperatureGauge(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.value = 0

    def set_value(self, value):
        self.value = value
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        center_x, center_y = self.width() // 2, self.height() // 2
        radius = min(self.width(), self.height()) // 2 - 10

        self.draw_gauge(painter, center_x, center_y, radius)
        self.draw_needle(painter, center_x, center_y, radius)
        self.draw_text(painter, center_x, center_y)

    def draw_gauge(self, painter, cx, cy, r):
        arc_rect = cx - r, cy - r, 2 * r, 2 * r
        segments = [(Qt.red, 0, 36), (QColor(255, 165, 0), 36, 36), 
                    (Qt.yellow, 72, 36), (QColor(144, 238, 144), 108, 36), 
                    (QColor(0, 100, 0), 144, 36)]
        for color, start, span in segments:
            painter.setPen(QPen(color, 6))
            painter.drawArc(*arc_rect, start * 16, span * 16)

    def draw_needle(self, painter, cx, cy, r):
        angle = math.radians(180 + (self.value / 100) * 180)
        x_end = cx + int(r * math.cos(angle))
        y_end = cy + int(r * math.sin(angle))
        painter.setPen(QPen(Qt.white, 6))
        painter.drawLine(cx, cy, x_end, y_end)
        painter.setBrush(QBrush(Qt.gray))
        painter.drawEllipse(cx - 5, cy - 5, 10, 10)

    def draw_text(self, painter, cx, cy):
        painter.setPen(Qt.white)
        font = QFont("Arial", 15)
        painter.setFont(font)
        painter.drawText(cx - 90, cy + 36, f"{self.value:.1f}\u00b0C     Temperature")

class VoltageDashboard(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Voltage Sensor Dashboard")
        self.setGeometry(100, 100, 1280, 720)
        self.setStyleSheet("background-color: #1c1c1c;")

        self.worker = Worker()
        self.worker.data_received.connect(self.update_dashboard)
        self.worker.connection_status.connect(self.update_connection_status)
        
        self.battery_image_label = None

        self.create_box(15, 65, 265, 165, "#2e2e2e", 20)  
        self.create_box(295, 65, 265, 105, "#2e2e2e", 20)
        self.create_box(295, 180, 265, 50, "#2e2e2e",20)
        self.create_box(575, 65, 265, 165, "#2e2e2e", 20) 
        self.create_box(15, 243, 545, 212, "#FFFFFF", 0) 
        self.create_box(575, 243, 265, 102, "#2e2e2e", 20)
        self.create_box(575, 400, 265, 50, "#2e2e2e", 20)
        self.create_box(15, 470, 825, 210, "#2e2e2e", 20)
        

        # Connection Status
        self.connection_status_label = QLabel("Status: Disconnected", self)
        self.connection_status_label.setGeometry(591, 398, 265, 50)
        self.connection_status_label.setStyleSheet("color: Red; font-size: 25px; background : transparent;")
        
        self.voltage_label = self.create_value_label("Voltage: 0.00 V", 140, 120, 200, 50)
        self.battery_text_label = self.create_value_label("Battery: 100%", 140, 80, 200, 50)
        self.current_label = self.create_value_label("Current: 0.00 V", 140, 160, 200, 50)

        # Temperature Gauge
        self.temperature_gauge = TemperatureGauge(self)
        self.temperature_gauge.setGeometry(560, 68, 290, 230)

        # Multi-Ring Gauge
        self.ring_gauge = MultiRingGaugeWidget(self)
        self.ring_gauge.setGeometry(35, 478, 195, 195)

        # ----> Energy Supply Heading <----
        self.energy_supply_heading = QLabel("Energy Supply", self)
        self.energy_supply_heading.setGeometry(250, 450, 200, 30)  # Adjust position as needed
        self.energy_supply_heading.setAlignment(Qt.AlignCenter)
        self.energy_supply_heading.setStyleSheet(
            """
            QLabel {
                background-color: transparent;  
                color: white;
                font-size: 18px;
                font-weight: bold;
                text-decoration: underline; 
            }
            """
        )

        # ----> Labels for the ring gauge values <----
        self.grid_text_label = self.create_value_label("Grid Usage: 0 W", 250, 490, 200, 50, font_size=14, color="#F05454")  # Red
        self.solar_text_label = self.create_value_label("Solar Generation: 0 W", 250, 540, 200, 50, font_size=14, color="#30BFBF")  # Teal
        self.battery_text_label = self.create_value_label("Battery Usage: 0 W", 250, 590, 200, 50, font_size=14, color="#F5ED8E")  # Yellow


        # ----> Energy Usage Heading <----
        self.energy_usage_heading = QLabel("Energy Usage", self)
        self.energy_usage_heading.setGeometry(470, 450, 200, 30)  # Adjust position as needed
        self.energy_usage_heading.setAlignment(Qt.AlignCenter)
        self.energy_usage_heading.setStyleSheet(
            """
            QLabel {
                background-color: transparent;  
                color: white;
                font-size: 18px;
                font-weight: bold;
                text-decoration: underline;  
            }
            """
        )

        # ----> Labels for Energy Usage <----
        self.grid_usage_label = self.create_value_label("Grid: 0 W", 470, 490, 200, 50, font_size=14, color="#F05454")  # Red
        self.home_usage_label = self.create_value_label("Home: 0 W", 470, 540, 200, 50, font_size=14, color="#30BFBF")  # Teal
        self.battery_storage_label = self.create_value_label("Battery Storage: 0 W", 470, 590, 200, 50, font_size=14, color="#F5ED8E")  # Yellow

        # Combined Graph
        self.graph = CombinedGraph(self)
        self.graph.setGeometry(18, 246, 539, 206)
        # Set an initial image based on 100% battery
        
        
        self.output_table = QTableWidget(self)
        self.output_table.setGeometry(15, 65, 1170, 625)
        self.output_table.setStyleSheet("""
            QTableWidget {
                background-color: #f0f0f0;  /* Light gray background */
                color: black;                /* Black text */
            }
            QHeaderView::section {
                background-color: #4CAF50;   /* Green header */
                color: white;                /* White text in header */
                padding: 5px;                /* Padding in header */
            }
            QTableWidget::item {
                padding: 10px;               /* Padding in table items  */
            }
        """)
        self.create_output_buttons()
        self.create_change_output_heading()
        
        self.update_battery_image(100)
        self.output_table.setColumnCount(6)
        self.output_table.resizeColumnsToContents()
        self.output_table.setHorizontalHeaderLabels(["Voltage (V)", "Current (A)", "Temperature (°C)","Grid-Value (V)","Solar Value(V)","Battery Value(%)"])
        header = self.output_table.horizontalHeader()
        header.setSectionResizeMode(QHeaderView.Stretch)
        self.output_table.raise_()
        self.output_table.hide()
        
        self.toggle_table_button = self.create_header_button("Output Table")
        self.toggle_table_button.setGeometry(1035, 10, 120, 30)
        self.toggle_table_button.clicked.connect(self.toggle_output_table)
        self.toggle_table_button.setParent(self)

        self.worker.start()
        
    def create_output_buttons(self):
      button_configurations = [
          ("icons/home.png", self.toggle_home_relay, 855, 140, "HOME"),
          ("icons/battery.png", self.toggle_battery_relay, 962, 140, "BATTERY"),
          ("icons/power-grid.png", self.toggle_grid_relay, 1068, 140, "GRID"),
          ("AUTO", self.toggle_auto_mode, 1175, 140, "AUTO"),
      ]
    
      for icon_or_text, callback, x, y, relay_id in button_configurations:
          self.create_button(icon_or_text, callback, x, y, relay_id)
          
    def create_button(self, icon_or_text, callback, x, y, relay_id):
        button = QPushButton(self)
        button.setGeometry(x, y, 90, 90)
        button.relay_id = relay_id
        if icon_or_text.endswith(".png"):
            button.setIcon(QIcon(icon_or_text))
            button.setIconSize(QSize(64, 64))
        else:
            button.setText(icon_or_text)
        button.clicked.connect(callback)
        button.setStyleSheet(
            """
            QPushButton {
                background-color: #2e2e2e;
                color: white;
                border: 2px solid white;
                border-radius: 10px;
                font-size: 18px;
            }
            QPushButton:hover {
                background-color: #555555;
            }
        """
        )

    def create_change_output_heading(self):
        heading = QLabel("Change Output", self)
        heading.setGeometry(855, 65, 410, 60)
        heading.setAlignment(Qt.AlignCenter)
        heading.setStyleSheet(
            """
            QLabel {
                background-color: #2e2e2e;
                color: white;
                border: 2px solid white;
                border-radius: 10px;
                font-size: 28px;
                font-weight: bold;
            }
        """
        )
        
    def send_relay_command(self, relay_id, state):
        command = f"RELAY,{relay_id},{state}\n"
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(5)
                s.connect((ESP32_IP,PORT))
                s.sendall(command.encode("utf-8"))
                print(f"Command sent: {command.strip()}")
        except socket.timeout:
            print("Connection timed out. Ensure ESP32 is reachable.")
        except Exception as e:
            print(f"Error sending command: {e}")
            
            
    def toggle_relay(self, relay_id):
        self.send_relay_command(relay_id, "TOGGLE")
        
    def toggle_home_relay(self):
        self.toggle_relay("HOME")
        
    def toggle_battery_relay(self):
        self.toggle_relay("BATTERY")
        
    def toggle_grid_relay(self):
        self.toggle_relay("GRID")
        
    def toggle_auto_mode(self):
        self.toggle_relay("AUTO")
  
    def create_box(self, x, y, width, height, color, radius):
        box = QFrame(self)
        box.setGeometry(x, y, width, height)
        box.setStyleSheet(f"background-color: {color}; border-radius: {radius}px;")


    def update_dashboard(self, voltage, current, temperature, grid, solar, battery):
        self.temperature_gauge.set_value(temperature)
        self.ring_gauge.update_values(grid, solar, battery)
        self.graph.update_graph(voltage, current)
        self.update_battery_image(battery)
        self.add_to_table(voltage, current, temperature, grid, solar ,battery)
        self.update_value_label(self.voltage_label, voltage, "V")
        self.update_value_label(self.battery_text_label, battery, "%")
        self.update_value_label(self.current_label, grid, "V")

        # ----> Update ring gauge text labels <----
        self.update_value_label(self.grid_text_label, grid, "W")  
        self.update_value_label(self.solar_text_label, solar, "W")
        self.update_value_label(self.battery_text_label, battery, "W") 

        # ----> Update Energy Usage labels <----
        self.update_value_label(self.grid_usage_label, grid, "W")  # You'll need to calculate the actual grid usage
        self.update_value_label(self.home_usage_label, 100, "W")  # Replace 100 with the actual home usage value
        self.update_value_label(self.battery_storage_label, battery, "W")  # You'll need to calculate the actual battery storage
        
    def add_to_table(self, voltage, current, temperature,grid, solar, battery):
        row_position = self.output_table.rowCount()
        self.output_table.insertRow(row_position)
        self.output_table.setItem(row_position, 0, QTableWidgetItem(f"{voltage:.2f}"))
        self.output_table.setItem(row_position, 1, QTableWidgetItem(f"{current:.2f}"))
        self.output_table.setItem(row_position, 2, QTableWidgetItem(f"{temperature:.1f}"))
        self.output_table.setItem(row_position , 3, QTableWidgetItem(f"{grid:.1f}"))
        self.output_table.setItem(row_position, 4, QTableWidgetItem(f"{solar:.1f}"))
        self.output_table.setItem(row_position, 5, QTableWidgetItem(f"{battery:.1f}"))

    def update_connection_status(self, status):
        if status:
            self.connection_status_label.setText("Status: Connected" if status else "Status: Disconnected")
            self.connection_status_label.setStyleSheet("color: Lime;font-size: 25px; background : transparent;" if status else "color: red;")
        else:
            self.connection_status_label.setText("Status: Disconnected")
            self.connection_status_label.setStyleSheet("color: red; font-size: 25px; background : transparent;")
            
    def toggle_output_table(self):
        # Toggle the visibility of the output table
        if self.output_table.isVisible():
            self.output_table.hide()
            self.update_battery_image(self.current_battery_value)  # Show battery image when table is hidden
        else:
            self.output_table.show()
            self.remove_battery_image() 
        
    def update_battery_image(self, battery):
        self.current_battery_value = battery
        # Determine the correct image path
        if battery is None or battery > 100 or battery < 0:
            image_path = r"C:\Users\adars\OneDrive\Desktop\SIH\none-battery.svg"
        elif battery == 100:
            image_path = r"C:\Users\adars\OneDrive\Desktop\SIH\FlatColorIconsFullBattery.svg"
        elif battery >= 75:
            image_path = r"C:\Users\adars\OneDrive\Desktop\SIH\FlatColorIconsHighBattery.svg"
        elif battery >= 50:
            image_path = r"C:\Users\adars\OneDrive\Desktop\SIH\FlatColorIconsMiddleBattery.svg"
        elif battery > 0:
            image_path = r"C:\Users\adars\OneDrive\Desktop\SIH\FlatColorIconsLowBattery.svg"
        else:
            image_path = r"C:\Users\adars\OneDrive\Desktop\SIH\FlatColorIconsEmptyBattery.svg"
    
        if self.output_table and self.output_table.isVisible():
            self.remove_battery_image()
        else:
            # Pass only the file path to create_svg_image
            self.create_svg_image(image_path, 1, 62, 150, 167)
            
    def create_svg_image(self, image_path, x, y, width, height):
        """Create an SVG image."""
        if self.battery_image_label:  # Remove existing battery image if it exists
            self.battery_image_label.deleteLater()
        
        self.battery_image_label = QLabel(self)
        self.battery_image_label.setGeometry(x, y, width, height)
        renderer = QSvgRenderer(image_path)  # Ensure image_path is a string
        pixmap = QPixmap(width, height)
        pixmap.fill(Qt.transparent)
    
        painter = QPainter(pixmap)
        renderer.render(painter)
        painter.end()
    
        self.battery_image_label.setPixmap(pixmap)
        self.battery_image_label.setStyleSheet("background: transparent;")
        self.battery_image_label.show()
        
    def remove_battery_image(self):
        """Remove the battery image from the dashboard."""
        if self.battery_image_label:
            self.battery_image_label.deleteLater()
            self.battery_image_label = None

    
    def create_value_label(self, text, x, y, width, height, font_size=15, color="#7CAAF8", background_color="transparent"):
        """Create a label for displaying a specific value."""
        label = QLabel(text, self)  # Create a QLabel with the specified text and set the parent to self
        label.setGeometry(x, y, width, height)  # Set the geometry of the label
        label.setStyleSheet(f"color: {color}; font-size: {font_size}px; background: {background_color};font-weight: bold;")  # Set the style
        label.show()  # Show the label
        return label  # Return the created label

    def update_value_label(self, label, value, unit):
        """Update the text of the label with the new value."""
        label.setText(f"{label.text().split(':')[0]}: {value:.2f} {unit}")  # Update the label text
            
            
    def create_header_button(self, text):
        button = QPushButton(text)
        button.setFont(QFont("Arial", 13, QFont.Bold))
        button.setStyleSheet(""" 
            QPushButton { color: white; background-color: #444444; border: 1px solid white; padding: 2px; border-radius: 5px;font-weight: bold; }
            QPushButton:hover { background-color: #555555; border: 3px solid white; border-radius: 15px; }
        """)
        return button


if __name__ == "__main__":
    app = QApplication(sys.argv)
    dashboard = VoltageDashboard()
    dashboard.show()
    sys.exit(app.exec())