/*
 * Web Interface for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 1
 */

#ifndef WEBUI_H
#define WEBUI_H

static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<!-- this code is written by pavank. -teenage engineering
     intelligenenergy management system
     update counter - 5
-->
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Energy Dashboard Demo</title>
  <link href="https://fonts.googleapis.com/css2?family=Quicksand:wght@400;600&family=Roboto:wght@300;500&family=Orbitron:wght@500&display=swap" rel="stylesheet">
  <script src="https://cdn.jsdelivr.net/npm/apexcharts"></script>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
  <style>
    :root {
      --primary: #2E7D32;
      --secondary: #81C784;
      --accent: #FFD54F;
      --tertiary: #17a2b8;
      --bg: #f4faf5;
      --card-bg: #ffffff;
      --text: #333333;
      --font: 'Quicksand', sans-serif;
      --shadow: 0 4px 6px rgba(0,0,0,0.1);
      --gradient-start: #34b3a0;
      --gradient-mid: #ffcc33;
      --gradient-end: #28a745;
    }

    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
    }

    body {
      font-family: var(--font);
      background: var(--bg);
      color: var(--text);
      min-height: 100vh;
    }

    .dashboard {
      display: grid;
      grid-template-columns: 280px 1fr;
      grid-template-rows: 80px 1fr;
      min-height: 100vh;
      gap: 24px;
      padding: 24px;
      max-width: 1920px;
      margin: 0 auto;
    }

    /* Sidebar Styles */
    .sidebar {
      grid-row: 1 / -1;
      background: var(--card-bg);
      padding: 2rem;
      border-radius: 20px;
      box-shadow: var(--shadow);
      display: flex;
      flex-direction: column;
      gap: 2.5rem;
      position: sticky;
      top: 24px;
      height: calc(100vh - 48px);
    }

    .sidebar-logo {
      display: flex;
      align-items: center;
      gap: 1rem;
      padding-bottom: 1.5rem;
      border-bottom: 2px solid var(--bg);
    }

    .sidebar-logo i {
      font-size: 2.2rem;
      color: var(--primary);
      background: var(--bg);
      padding: 12px;
      border-radius: 12px;
    }

    .sidebar-logo h2 {
      font-size: 1.8rem;
      font-weight: 600;
      color: var(--primary);
    }

    .nav-menu {
      list-style: none;
      display: flex;
      flex-direction: column;
      gap: 0.8rem;
    }

    .nav-item {
      padding: 1rem 1.2rem;
      border-radius: 12px;
      cursor: pointer;
      transition: all 0.3s ease;
      display: flex;
      align-items: center;
      gap: 1.2rem;
      font-weight: 500;
    }

    .nav-item:hover, .nav-item.active {
      background: #e8f5e9;
      color: #4CAF50;
      transform: translateX(5px);
      box-shadow: 0 2px 8px rgba(76, 175, 80, 0.15);
    }

    .nav-item:hover i, .nav-item.active i {
      color: #4CAF50;
    }

    .nav-item i {
      font-size: 1.4rem;
      color: var(--text);
      transition: color 0.3s ease;
    }

    .system-info {
      margin-top: auto;
      padding: 1.5rem;
      background: var(--bg);
      border-radius: 12px;
    }

    .system-info h4 {
      color: var (--primary);
      margin-bottom: 1rem;
      font-size: 1.1rem;
    }

    .system-info p {
      margin-bottom: 0.8rem;
      font-size: 0.95rem;
      color: var(--text);
      opacity: 0.9;
    }

    /* Header Styles */
    .header {
      grid-column: 2;
      grid-row: 1;
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 0 2rem;
      background: var(--card-bg);
      border-radius: 20px;
      box-shadow: var(--shadow);
    }

    .header h1 {
      font-size: 2rem;
      color: var(--primary);
      font-weight: 600;
    }

    .user-info {
      display: flex;
      align-items: center;
      gap: 1.2rem;
      padding: 0.8rem 1.2rem;
      background: var(--bg);
      border-radius: 12px;
      transition: all 0.3s ease;
    }

    .user-info:hover {
      transform: translateY(-2px);
    }

    .user-info i {
      font-size: 1.6rem;
      color: var(--primary);
    }

    .user-info span {
      font-weight: 500;
      color: var(--text);
    }

    /* Main Content */
    .main-content {
      grid-column: 2;
      grid-row: 2;
      display: grid;
      grid-template-columns: repeat(4, 1fr);
      grid-template-rows: auto minmax(400px, auto) minmax(350px, auto);
      gap: 24px;
    }

    /* Stats Grid */
    .stats-grid {
      grid-column: 1 / -1;
      display: grid;
      grid-template-columns: repeat(4, 1fr);
      gap: 24px;
    }

    .stat-item {
      background: var(--card-bg);
      border-radius: 20px;
      padding: 1.4rem 1.2rem;
      box-shadow: var(--shadow);
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 0.7rem;
      transition: all 0.3s ease;
      position: relative;
      overflow: hidden;
    }

    .stat-item::before {
      content: '';
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 4px;
      background: linear-gradient(to right, var(--gradient-start), var(--gradient-end));
      opacity: 0;
      transition: opacity 0.3s ease;
    }

    .stat-item:hover::before {
      opacity: 1;
    }

    .stat-item:hover {
      transform: translateY(-5px);
      box-shadow: 0 8px 15px rgba(0,0,0,0.1);
    }

    .stat-trend {
      display: flex;
      align-items: center;
      gap: 0.4rem;
      font-size: 0.85rem;
      padding: 0.25rem 0.6rem;
      border-radius: 8px;
      background: var(--bg);
    }

    .stat-trend.up {
      color: #4CAF50;
    }

    .stat-trend.down {
      color: #f44336;
    }

    .stat-trend.stable {
      color: #FF9800;
    }

    .quick-actions {
      grid-column: 1 / -1;
      display: flex;
      justify-content: center;
      gap: 1rem;
      margin-top: 2rem;
      padding: 1rem;
      background: var(--card-bg);
      border-radius: 20px;
      box-shadow: var(--shadow);
    }

    .action-btn {
      padding: 0.8rem 1.5rem;
      border: none;
      border-radius: 12px;
      background: var(--bg);
      color: var(--text);
      cursor: pointer;
      font-family: var(--font);
      font-weight: 600;
      font-size: 0.9rem;
      transition: all 0.3s ease;
      display: flex;
      align-items: center;
      gap: 0.8rem;
    }

    .action-btn:hover {
      background: var(--primary);
      color: white;
      transform: translateY(-2px);
    }

    .action-btn i {
      font-size: 1.1rem;
    }

    .comparison-matrix {
      font-size: 0.8rem;
      color: var(--text);
      opacity: 0.8;
      text-align: center;
      margin-top: 0.3rem;
    }

    .stat-item i {
      font-size: 2rem;
      color: var(--primary);
      background: var(--bg);
      padding: 0.8rem;
      border-radius: 15px;
    }

    .stat-item h3 {
      font-size: 1rem;
      color: var(--text);
      font-weight: 500;
    }

    .stat-item .value {
      font-size: 1.8rem;
      font-weight: bold;
      color: var(--primary);
      font-family: 'Orbitron', sans-serif;
    }

    /* Chart Cards */
    .chart-card {
      grid-column: 1 / 4;
      background: var(--card-bg);
      border-radius: 20px;
      padding: 1.8rem;
      box-shadow: var(--shadow);
    }

    .chart-card h3 {
      font-size: 1.3rem;
      color: var(--primary);
      margin-bottom: 1.5rem;
    }

    /* System Status */
    .system-status {
      grid-column: 4;
      background: var(--card-bg);
      border-radius: 20px;
      padding: 1.8rem;
      box-shadow: var(--shadow);
      display: flex;
      flex-direction: column;
      gap: 1.5rem;
    }

    .controls {
      display: grid;
      gap: 1rem;
    }

    .control-btn {
      padding: 1.2rem;
      border: none;
      border-radius: 12px;
      background: var(--secondary);
      color: white;
      cursor: pointer;
      font-family: var(--font);
      font-weight: 600;
      font-size: 1rem;
      transition: all 0.3s ease;
      display: flex;
      align-items: center;
      gap: 1rem;
      width: 100%;
      position: relative;
      padding-right: 3rem;
    }

    .control-btn i {
      font-size: 1.4rem;
      min-width: 24px;
      text-align: center;
      margin-right: 0.5rem;
    }

    .control-btn span:not(.status-indicator) {
      flex: 1;
      text-align: left;
      white-space: nowrap;
      overflow: hidden;
      text-overflow: ellipsis;
    }

    .status-indicator {
      display: inline-block;
      width: 10px;
      height: 10px;
      border-radius: 50%;
      background: rgba(255, 255, 255, 0.5);
      position: absolute;
      right: 1.2rem;
      top: 50%;
      transform: translateY(-50%);
      transition: all 0.3s ease;
      margin-left: auto;
    }

    .status-indicator.active {
      background: #fff;
      box-shadow: 0 0 8px rgba(255, 255, 255, 0.8);
    }

    .control-btn:hover {
      background: var (--primary);
      transform: translateY(-2px);
    }

    .control-btn.active {
      background: var(--primary);
      box-shadow: 0 4px 12px rgba(46, 125, 50, 0.2);
    }

    .status-info {
      margin-top: auto;
      padding-top: 1.5rem;
      border-top: 2px solid var(--bg);
    }

    .status-info h4 {
      color: var(--primary);
      margin-bottom: 1rem;
      font-size: 1.1rem;
    }

    .status-info p {
      margin-bottom: 0.8rem;
      font-size: 0.95rem;
      color: var (--text);
    }

    #lastUpdate {
      color: var (--primary);
      font-weight: 600;
    }

    /* Responsive Design */
    @media (max-width: 1400px) {
      .dashboard {
        grid-template-columns: 240px 1fr;
        padding: 20px;
        gap: 20px;
      }

      .stats-grid {
        grid-template-columns: repeat(2, 1fr);
      }
    }

    @media (max-width: 1024px) {
      .dashboard {
        grid-template-columns: 1fr;
      }
      
      .sidebar {
        display: none;
      }
      
      .header,
      .main-content {
        grid-column: 1;
      }

      .system-status {
        grid-column: 1 / -1;
      }

      .chart-card {
        grid-column: 1 / -1;
      }
    }

    @media (max-width: 768px) {
      .stats-grid {
        grid-template-columns: 1fr;
      }

      .header {
        padding: 1rem;
      }

      .header h1 {
        font-size: 1.5rem;
      }

      .controls {
        grid-template-columns: 1fr;
      }
    }

    /* Add new styles */
    .efficiency-meter {
      margin: 1.5rem 0;
      padding: 1rem;
      background: var(--bg);
      border-radius: 12px;
    }

    .progress-bar {
      height: 20px;
      background: #e0e0e0;
      border-radius: 10px;
      position: relative;
      margin-top: 10px;
      overflow: hidden;
    }

    .progress {
      height: 100%;
      background: linear-gradient(to right, var(--gradient-start), var(--gradient-end));
      width: 0%;
      transition: width 0.5s ease;
    }

    .efficiency-value {
      position: absolute;
      right: 10px;
      top: 50%;
      transform: translateY(-50%);
      color: var(--text);
      font-weight: 600;
      font-size: 0.9rem;
    }

    .alerts {
      margin-top: 1rem;
      max-height: 100px;
      overflow-y: auto;
    }

    .alert {
      padding: 0.5rem;
      margin-bottom: 0.5rem;
      border-radius: 6px;
      font-size: 0.9rem;
      display: flex;
      align-items: center;
      gap: 0.5rem;
    }

    .alert.warning {
      background: #fff3e0;
      color: #f57c00;
    }

    .alert.success {
      background: #e8f5e9;
      color: #2e7d32;
    }

    /* Add styles for system status in sidebar */
    .status-card {
      margin-top: auto;
      padding: 1.5rem;
      background: var(--bg);
      border-radius: 12px;
      margin-bottom: 1rem;
    }

    .status-card h4 {
      color: var(--primary);
      margin-bottom: 1rem;
      font-size: 1.1rem;
    }

    .status-card p {
      margin-bottom: 0.8rem;
      font-size: 0.95rem;
      color: var(--text);
      opacity: 0.9;
    }

    .status-card #lastUpdate {
      color: var(--primary);
      font-weight: 600;
    }

    /* Add Analytics page styles */
    .analytics-content {
      display: none;
      grid-column: 2;
      grid-row: 2;
      gap: 24px;
    }

    .analytics-content.active {
      display: grid;
      grid-template-columns: repeat(2, 1fr);
    }

    .analytics-card {
      background: var(--card-bg);
      border-radius: 20px;
      padding: 1.8rem;
      box-shadow: var(--shadow);
    }

    .analytics-card.full-width {
      grid-column: 1 / -1;
    }

    .analytics-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 1.5rem;
    }

    .time-filter {
      display: flex;
      gap: 1rem;
    }

    .time-filter button {
      padding: 0.5rem 1rem;
      border: none;
      border-radius: 8px;
      background: var(--bg);
      color: var(--text);
      cursor: pointer;
      transition: all 0.3s ease;
    }

    .time-filter button.active {
      background: var(--primary);
      color: white;
    }
  </style>
</head>
<body>
  <div class="dashboard">
    <div class="sidebar">
      <div class="sidebar-logo">
        <i class="fas fa-solar-panel"></i>
        <h2>iEMS</h2>
      </div>
      <ul class="nav-menu">
        <li class="nav-item active">
          <i class="fas fa-home"></i>
          <span>Dashboard</span>
        </li>
        <li class="nav-item">
          <i class="fas fa-chart-line"></i>
          <span>Analytics</span>
        </li>
        <li class="nav-item">
          <i class="fas fa-cog"></i>
          <span>Settings</span>
        </li>
        <li class="nav-item">
          <i class="fas fa-bell"></i>
          <span>Notifications</span>
        </li>
        <li class="nav-item">
          <i class="fas fa-user"></i>
          <span>Profile</span>
        </li>
      </ul>
      <div class="status-card">
        <h4>System Status</h4>
        <p id="systemStatus">All systems operational</p>
        <p>Last updated: <span id="lastUpdate">Just now</span></p>
      </div>
      <div class="system-info">
        <h4>System Info</h4>
        <p>Version: 2.1.0</p>
        <p>Last Update: Today</p>
      </div>
    </div>

    <header class="header">
      <h1>Intelligent Energy Management System</h1>
      <div class="user-info">
        <i class="fas fa-user-circle"></i>
        <span>Welcome, User</span>
      </div>
    </header>

    <main class="main-content">
      <div class="stats-grid">
        <div class="card stat-item">
          <i class="fas fa-solar-panel"></i>
          <h3>Solar Generation</h3>
          <div class="value">3.5 kW</div>
          <div class="stat-trend up">
            <i class="fas fa-arrow-up"></i>
            <span>12% vs yesterday</span>
          </div>
          <div class="comparison-matrix">Peak: 4.2 kW | Avg: 3.1 kW</div>
        </div>
        <div class="card stat-item">
          <i class="fas fa-battery-three-quarters"></i>
          <h3>Battery Level</h3>
          <div class="value">75%</div>
          <div class="stat-trend down">
            <i class="fas fa-arrow-down"></i>
            <span>5% last hour</span>
          </div>
          <div class="comparison-matrix">Capacity: 100 kWh | Health: 98%</div>
        </div>
        <div class="card stat-item">
          <i class="fas fa-bolt"></i>
          <h3>Power Usage</h3>
          <div class="value">2.8 kW</div>
          <div class="stat-trend up">
            <i class="fas fa-arrow-up"></i>
            <span>8% vs average</span>
          </div>
          <div class="comparison-matrix">Peak: 3.5 kW | Low: 1.2 kW</div>
        </div>
        <div class="card stat-item">
          <i class="fas fa-temperature-high"></i>
          <h3>System Temperature</h3>
          <div class="value">24°C</div>
          <div class="stat-trend stable">
            <i class="fas fa-equals"></i>
            <span>Stable</span>
          </div>
          <div class="comparison-matrix">Max: 28°C | Min: 20°C</div>
        </div>
      </div>

      <div class="card chart-card">
        <h3>Power Consumption Trends</h3>
        <div id="graph"></div>
        <div class="quick-actions">
          <button class="action-btn">
            <i class="fas fa-download"></i>
            Export Data
          </button>
          <button class="action-btn">
            <i class="fas fa-calendar"></i>
            Schedule Report
          </button>
          <button class="action-btn">
            <i class="fas fa-chart-line"></i>
            Detailed Analysis
          </button>
        </div>
      </div>

      <div class="card system-status">
        <h3>Energy Management Controls</h3>
        <div class="controls">
          <button class="control-btn" onclick="toggleControl('solar')" id="solarBtn">
            <i class="fas fa-sun"></i> Solar Power
            <span class="status-indicator"></span>
          </button>
          <button class="control-btn" onclick="toggleControl('battery')" id="batteryBtn">
            <i class="fas fa-battery-full"></i> Battery
            <span class="status-indicator"></span>
          </button>
          <button class="control-btn" onclick="toggleControl('grid')" id="gridBtn">
            <i class="fas fa-plug"></i> Grid Power
            <span class="status-indicator"></span>
          </button>
          <button class="control-btn" onclick="toggleAutoMode()" id="autoBtn">
            <i class="fas fa-robot"></i> Auto Mode
            <span class="status-indicator"></span>
          </button>
        </div>
        <div class="efficiency-meter">
          <h4>System Efficiency</h4>
          <div class="progress-bar">
            <div class="progress" id="efficiencyBar"></div>
            <span class="efficiency-value">0%</span>
          </div>
        </div>
        <div class="alerts" id="alertsContainer"></div>
      </div>
    </main>
  </div>

  <script>
    let ws;
    let powerChart;
    let lastData = {};
    let chartData = {
      solar: [],
      battery: [],
      consumption: []
    };

    // WebSocket Connection
    function connectWebSocket() {
      console.log('Attempting WebSocket connection...');
      ws = new WebSocket(`ws://${window.location.hostname}/ws`);
      
      ws.onopen = () => {
        console.log('WebSocket Connected');
        document.getElementById('systemStatus').textContent = 'Connected';
        document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString();
        showAlert('WebSocket connected successfully', 'success');
      };
      
      ws.onclose = () => {
        console.log('WebSocket Disconnected');
        document.getElementById('systemStatus').textContent = 'Disconnected';
        showAlert('WebSocket disconnected - Reconnecting...', 'warning');
        // Attempt to reconnect
        setTimeout(connectWebSocket, 2000);
      };
      
      ws.onmessage = (event) => {
        try {
          const data = JSON.parse(event.data);
          console.log('Received data:', data); // Debug received data
          updateDashboard(data);
        } catch (e) {
          console.error('Error parsing WebSocket data:', e);
          showAlert('Error updating dashboard data', 'warning');
        }
      };
      
      ws.onerror = (error) => {
        console.error('WebSocket Error:', error);
        showAlert('WebSocket error occurred', 'warning');
      };
    }

    // Update Chart Data
    function updateChartData(data) {
      const now = new Date();
      const timeStr = now.toLocaleTimeString();
      
      // Add some natural variation to the values
      const variation = Math.sin(Date.now() / 10000) * 0.05; // Gentle sine wave variation
      
      // Solar power with daily pattern simulation
      const hour = now.getHours();
      let solarFactor = 0;
      if (hour >= 6 && hour <= 18) { // Daylight hours
        solarFactor = Math.sin((hour - 6) * Math.PI / 12); // Peak at noon
        solarFactor = Math.max(0, solarFactor);
      }
      const solarValue = data.solarEnabled ? (0.2 + variation) * solarFactor : 0;
      
      // Battery power with realistic consumption pattern
      const batteryValue = 0.15 + variation;
      
      // Total consumption with some randomness
      const baseConsumption = 0.3; // Base consumption
      const randomFactor = Math.random() * 0.1; // Random variation
      const consumptionValue = baseConsumption + randomFactor + (data.gridEnabled ? 0.2 : 0);
      
      chartData.solar.push({ x: timeStr, y: parseFloat(solarValue.toFixed(2)) });
      chartData.battery.push({ x: timeStr, y: parseFloat(batteryValue.toFixed(2)) });
      chartData.consumption.push({ x: timeStr, y: parseFloat(consumptionValue.toFixed(2)) });
      
      // Keep last 20 data points
      if (chartData.solar.length > 20) {
        chartData.solar.shift();
        chartData.battery.shift();
        chartData.consumption.shift();
      }
      
      // Update chart
      powerChart.updateSeries([
        { name: 'Solar Generation (kW)', data: chartData.solar },
        { name: 'Battery Storage (kW)', data: chartData.battery },
        { name: 'Energy Consumption (kW)', data: chartData.consumption }
      ]);
      
      // Update comparison matrix
      const solarStats = calculateStats(chartData.solar);
      const consumptionStats = calculateStats(chartData.consumption);
      
      document.querySelector('.stat-item:nth-child(1) .comparison-matrix').textContent = 
        `Peak: ${solarStats.max.toFixed(1)} kW | Avg: ${solarStats.avg.toFixed(1)} kW`;
      
      document.querySelector('.stat-item:nth-child(3) .comparison-matrix').textContent = 
        `Peak: ${consumptionStats.max.toFixed(1)} kW | Low: ${consumptionStats.min.toFixed(1)} kW`;
    }

    // Helper function to calculate statistics
    function calculateStats(data) {
      const values = data.map(point => point.y);
      return {
        max: Math.max(...values),
        min: Math.min(...values),
        avg: values.reduce((a, b) => a + b, 0) / values.length
      };
    }

    // Update Dashboard with Real-time Data
    function updateDashboard(data) {
      console.log('Updating dashboard with data:', data);
      lastData = data;
      
      // Update temperature display
      const tempElement = document.querySelector('.stat-item:nth-child(4) .value');
      const tempTrend = document.querySelector('.stat-item:nth-child(4) .stat-trend');
      
      if (data.temperature !== null && !isNaN(data.temperature)) {
        const temp = parseFloat(data.temperature).toFixed(1);
        tempElement.textContent = `${temp}°C`;
        
        if (data.tempSensorOk) {
          if (temp >= 45) {
            tempTrend.className = 'stat-trend up';
            tempTrend.innerHTML = '<i class="fas fa-arrow-up"></i><span>High</span>';
          } else if (temp <= 25) {
            tempTrend.className = 'stat-trend down';
            tempTrend.innerHTML = '<i class="fas fa-arrow-down"></i><span>Low</span>';
          } else {
            tempTrend.className = 'stat-trend stable';
            tempTrend.innerHTML = '<i class="fas fa-equals"></i><span>Stable</span>';
          }
        } else {
          tempElement.textContent = 'Sensor Error';
          tempTrend.className = 'stat-trend down';
          tempTrend.innerHTML = '<i class="fas fa-exclamation-triangle"></i><span>Error</span>';
        }
      } else {
        tempElement.textContent = 'N/A';
        tempTrend.className = 'stat-trend down';
        tempTrend.innerHTML = '<i class="fas fa-exclamation-triangle"></i><span>No Data</span>';
      }
      
      // Update solar generation (fixed at 0.2 kWh)
      document.querySelector('.stat-item:nth-child(1) .value').textContent = '0.2 kW';
      
      // Update battery level (fixed at 92%)
      document.querySelector('.stat-item:nth-child(2) .value').textContent = '92%';
      
      // Update power usage
      document.querySelector('.stat-item:nth-child(3) .value').textContent = 
        `${(data.batteryPower + (data.gridEnabled ? 0.2 : 0)).toFixed(1)} kW`;
      
      // Update system efficiency
      document.getElementById('efficiencyBar').style.width = `${data.efficiency}%`;
      document.querySelector('.efficiency-value').textContent = `${data.efficiency}%`;
      
      // Update control buttons with proper status indicators
      document.querySelector('#solarBtn .status-indicator').classList.toggle('active', data.solarEnabled);
      document.querySelector('#batteryBtn .status-indicator').classList.toggle('active', true); // Always active at 92%
      document.querySelector('#gridBtn .status-indicator').classList.toggle('active', data.gridEnabled);
      document.querySelector('#autoBtn .status-indicator').classList.toggle('active', data.autoMode);
      
      // Update chart data
      updateChartData(data);
      
      // Update last update time
      document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString();
    }

    // Control Functions
    function toggleControl(type) {
      if (!ws || ws.readyState !== WebSocket.OPEN) {
        showAlert('Not connected to system', 'warning');
        return;
      }
      
      const command = {
        command: type,
        enable: !lastData[`${type}Enabled`]
      };
      
      ws.send(JSON.stringify(command));
    }

    function toggleAutoMode() {
      if (!ws || ws.readyState !== WebSocket.OPEN) {
        showAlert('Not connected to system', 'warning');
        return;
      }
      
      const command = {
        command: 'auto',
        enable: !lastData.autoMode
      };
      
      ws.send(JSON.stringify(command));
    }

    function showAlert(message, type) {
      const alertsContainer = document.getElementById('alertsContainer');
      const alert = document.createElement('div');
      alert.className = `alert ${type}`;
      alert.innerHTML = `<i class="fas fa-${type === 'warning' ? 'exclamation-triangle' : 'check-circle'}"></i>${message}`;
      
      alertsContainer.prepend(alert);
      setTimeout(() => alert.remove(), 5000);
    }

    document.addEventListener('DOMContentLoaded', function() {
      // Initialize WebSocket connection
      connectWebSocket();
      
      // Power Consumption Chart
      const options = {
        series: [
          { name: "Solar Generation (kW)", data: [] },
          { name: "Battery Storage (kW)", data: [] },
          { name: "Energy Consumption (kW)", data: [] }
        ],
        chart: {
          type: "area",
          height: 350,
          zoom: { enabled: true },
          toolbar: { show: true, tools: { zoomin: true, zoomout: true, pan: true, reset: true } },
          animations: { enabled: true, easing: "easeout", dynamicAnimation: { speed: 600 } },
          fontFamily: 'Quicksand, sans-serif'
        },
        colors: ["var(--primary)", "var(--secondary)", "var(--tertiary)"],
        stroke: { curve: "smooth", width: 3 },
        fill: {
          type: 'gradient',
          gradient: { 
            shade: 'light', 
            gradientToColors: ['#34b3a0', '#ffcc33', '#28a745'], 
            opacityFrom: 0.5, 
            opacityTo: 0.1 
          }
        },
        xaxis: { 
          type: 'category',
          labels: {
            style: {
              colors: 'var(--text)',
              fontFamily: 'Quicksand, sans-serif'
            }
          }
        },
        yaxis: {
          labels: {
            style: {
              colors: 'var(--text)',
              fontFamily: 'Quicksand, sans-serif'
            },
            formatter: function(val) {
              return val.toFixed(1) + ' kW';
            }
          }
        },
        markers: { size: 5, hover: { sizeOffset: 4 } },
        grid: { borderColor: "#ddd", strokeDashArray: 3 },
        tooltip: { 
          theme: "dark", 
          shared: true, 
          followCursor: true,
          style: {
            fontSize: '12px',
            fontFamily: 'Quicksand, sans-serif'
          }
        }
      };

      // Initialize the power consumption chart
      powerChart = new ApexCharts(document.querySelector("#graph"), options);
      powerChart.render();

      // Temperature Gauge
      const tempOptions = {
        series: [24],
        chart: {
          type: 'radialBar',
          height: 300,
          fontFamily: 'Quicksand, sans-serif',
        },
        plotOptions: {
          radialBar: {
            startAngle: -135,
            endAngle: 135,
            hollow: {
              margin: 15,
              size: '70%'
            },
            track: {
              background: '#f2f2f2',
              strokeWidth: '97%',
              margin: 5,
              dropShadow: {
                enabled: true,
                top: 2,
                left: 0,
                blur: 4,
                opacity: 0.15
              }
            },
            dataLabels: {
              name: {
                offsetY: -10,
                color: '#888',
                fontSize: '17px',
                fontFamily: 'Quicksand, sans-serif',
              },
              value: {
                color: '#111',
                fontSize: '36px',
                fontWeight: 600,
                formatter: function(val) {
                  return val + '°C';
                }
              }
            }
          }
        },
        fill: {
          type: 'gradient',
          gradient: {
            shade: 'dark',
            type: 'horizontal',
            shadeIntensity: 0.5,
            gradientToColors: ['var(--gradient-end)'],
            inverseColors: true,
            opacityFrom: 1,
            opacityTo: 1,
            stops: [0, 100]
          }
        },
        stroke: {
          lineCap: 'round'
        },
        labels: ['Temperature']
      };

      // Initialize temperature gauge
      const tempChart = new ApexCharts(document.querySelector("#tempGauge"), tempOptions);
      tempChart.render();
    });

    // Add navigation handling
    document.addEventListener('DOMContentLoaded', function() {
      const mainContent = document.querySelector('.main-content');
      const analyticsContent = document.createElement('div');
      analyticsContent.className = 'analytics-content';
      analyticsContent.innerHTML = `
        <div class="analytics-card full-width">
          <div class="analytics-header">
            <h3>Energy Consumption Analysis</h3>
            <div class="time-filter">
              <button class="active">Day</button>
              <button>Week</button>
              <button>Month</button>
              <button>Year</button>
            </div>
          </div>
          <div id="consumptionChart"></div>
        </div>
        <div class="analytics-card">
          <h3>Peak Usage Hours</h3>
          <div id="peakUsageChart"></div>
        </div>
        <div class="analytics-card">
          <h3>Cost Analysis</h3>
          <div id="costChart"></div>
        </div>
      `;
      
      mainContent.parentNode.insertBefore(analyticsContent, mainContent.nextSibling);

      // Handle navigation
      const navItems = document.querySelectorAll('.nav-item');
      navItems.forEach(item => {
        item.addEventListener('click', function() {
          navItems.forEach(i => i.classList.remove('active'));
          this.classList.add('active');
          
          if(this.querySelector('span').textContent === 'Analytics') {
            mainContent.style.display = 'none';
            analyticsContent.classList.add('active');
          } else {
            mainContent.style.display = 'grid';
            analyticsContent.classList.remove('active');
          }
        });
      });
    });
  </script>
</body>
</html>
)rawliteral";

#endif // WEBUI_H 