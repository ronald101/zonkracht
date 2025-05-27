#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Create instances
WebServer server(80);
BH1750 lightMeter;

// Variables for data storage
float currentLux = 0;
float maxLux = 0;
float minLux = 65535;
unsigned long lastReading = 0;
const unsigned long readingInterval = 2000; // Read every 2 seconds

// Structure to store historical data (last 24 readings)
struct Reading {
  float lux;
  String timestamp;
};

Reading readings[24];
int readingIndex = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize BH1750 sensor
  if (lightMeter.begin()) {
    Serial.println("BH1750 sensor initialized successfully");
  } else {
    Serial.println("Error initializing BH1750 sensor");
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Initialize readings array
  for (int i = 0; i < 24; i++) {
    readings[i].lux = 0;
    readings[i].timestamp = "";
  }
  
  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/reset", handleReset);
  
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  
  // Read sensor data at specified interval
  if (millis() - lastReading >= readingInterval) {
    readSensor();
    lastReading = millis();
  }
}

void readSensor() {
  currentLux = lightMeter.readLightLevel();
  
  if (currentLux > maxLux) {
    maxLux = currentLux;
  }
  
  if (currentLux < minLux) {
    minLux = currentLux;
  }
  
  // Store reading in circular buffer
  readings[readingIndex].lux = currentLux;
  readings[readingIndex].timestamp = getTimeString();
  readingIndex = (readingIndex + 1) % 24;
  
  Serial.print("Light: ");
  Serial.print(currentLux);
  Serial.println(" lx");
}

String getTimeString() {
  unsigned long currentTime = millis();
  unsigned long seconds = currentTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  return String(hours % 24) + ":" + 
         String((minutes % 60) < 10 ? "0" : "") + String(minutes % 60) + ":" +
         String((seconds % 60) < 10 ? "0" : "") + String(seconds % 60);
}

String getSunStatus() {
  if (currentLux > 10000) {
    return "Bright Sunlight";
  } else if (currentLux > 1000) {
    return "Moderate Sunlight";
  } else if (currentLux > 100) {
    return "Dim Light";
  } else if (currentLux > 10) {
    return "Very Dim";
  } else {
    return "Dark";
  }
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>☀️ Sunlight Monitor</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #74b9ff 0%, #0984e3 100%);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            backdrop-filter: blur(10px);
        }
        
        .header {
            text-align: center;
            margin-bottom: 30px;
        }
        
        .header h1 {
            font-size: 2.5em;
            color: #2d3436;
            margin-bottom: 10px;
        }
        
        .status-card {
            background: linear-gradient(135deg, #fdcb6e 0%, #e17055 100%);
            color: white;
            padding: 25px;
            border-radius: 15px;
            text-align: center;
            margin-bottom: 30px;
            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.1);
        }
        
        .current-value {
            font-size: 3em;
            font-weight: bold;
            margin-bottom: 10px;
        }
        
        .status-text {
            font-size: 1.2em;
            opacity: 0.9;
        }
        
        .stats-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .stat-card {
            background: white;
            padding: 20px;
            border-radius: 12px;
            text-align: center;
            border: 2px solid #f1f2f6;
            transition: transform 0.3s ease;
        }
        
        .stat-card:hover {
            transform: translateY(-5px);
        }
        
        .stat-value {
            font-size: 2em;
            font-weight: bold;
            color: #2d3436;
            margin-bottom: 5px;
        }
        
        .stat-label {
            color: #636e72;
            font-size: 0.9em;
        }
        
        .chart-container {
            background: white;
            padding: 20px;
            border-radius: 12px;
            margin-bottom: 20px;
            border: 2px solid #f1f2f6;
        }
        
        .chart-title {
            font-size: 1.2em;
            color: #2d3436;
            margin-bottom: 15px;
            text-align: center;
        }
        
        .chart {
            height: 200px;
            border: 1px solid #ddd;
            border-radius: 8px;
            background: #f8f9fa;
            display: flex;
            align-items: center;
            justify-content: center;
            color: #636e72;
        }
        
        .controls {
            display: flex;
            gap: 15px;
            justify-content: center;
            flex-wrap: wrap;
        }
        
        .btn {
            background: linear-gradient(135deg, #00b894 0%, #00a085 100%);
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 25px;
            font-size: 1em;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 5px 15px rgba(0, 184, 148, 0.3);
        }
        
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 8px 20px rgba(0, 184, 148, 0.4);
        }
        
        .btn.secondary {
            background: linear-gradient(135deg, #fd79a8 0%, #e84393 100%);
            box-shadow: 0 5px 15px rgba(232, 67, 147, 0.3);
        }
        
        .btn.secondary:hover {
            box-shadow: 0 8px 20px rgba(232, 67, 147, 0.4);
        }
        
        .footer {
            text-align: center;
            margin-top: 30px;
            color: #636e72;
            font-size: 0.9em;
        }
        
        @media (max-width: 600px) {
            .container {
                padding: 20px;
            }
            
            .current-value {
                font-size: 2.5em;
            }
            
            .stats-grid {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>☀️ Sunlight Monitor</h1>
            <p>Real-time light intensity monitoring</p>
        </div>
        
        <div class="status-card">
            <div class="current-value" id="currentLux">-- lx</div>
            <div class="status-text" id="sunStatus">Loading...</div>
        </div>
        
        <div class="stats-grid">
            <div class="stat-card">
                <div class="stat-value" id="maxLux">-- lx</div>
                <div class="stat-label">Peak Today</div>
            </div>
            <div class="stat-card">
                <div class="stat-value" id="minLux">-- lx</div>
                <div class="stat-label">Minimum Today</div>
            </div>
            <div class="stat-card">
                <div class="stat-value" id="avgLux">-- lx</div>
                <div class="stat-label">Average</div>
            </div>
        </div>
        
        <div class="chart-container">
            <div class="chart-title">Light Level History</div>
            <div class="chart" id="lightChart">
                📊 Historical data visualization
            </div>
        </div>
        
        <div class="controls">
            <button class="btn" onclick="refreshData()">🔄 Refresh</button>
            <button class="btn secondary" onclick="resetStats()">🗑️ Reset Stats</button>
        </div>
        
        <div class="footer">
            <p>Last updated: <span id="lastUpdate">--</span></p>
            <p>ESP32 Sunlight Monitor v1.0</p>
        </div>
    </div>

    <script>
        let chartData = [];
        
        function updateData() {
            fetch('/data')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('currentLux').textContent = data.current + ' lx';
                    document.getElementById('sunStatus').textContent = data.status;
                    document.getElementById('maxLux').textContent = data.max + ' lx';
                    document.getElementById('minLux').textContent = data.min + ' lx';
                    document.getElementById('avgLux').textContent = data.average + ' lx';
                    document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString();
                    
                    updateChart(data.history);
                })
                .catch(error => {
                    console.error('Error fetching data:', error);
                });
        }
        
        function updateChart(history) {
            const chart = document.getElementById('lightChart');
            let chartHTML = '<div style="display: flex; align-items: end; height: 160px; gap: 3px; padding: 20px;">';
            
            const maxVal = Math.max(...history.map(h => h.lux));
            
            history.forEach((reading, index) => {
                const height = maxVal > 0 ? (reading.lux / maxVal) * 120 : 0;
                const color = reading.lux > 1000 ? '#fdcb6e' : 
                            reading.lux > 100 ? '#74b9ff' : '#636e72';
                
                chartHTML += `<div style="
                    background: ${color}; 
                    height: ${height}px; 
                    min-height: 2px;
                    flex: 1; 
                    border-radius: 2px;
                    title: '${reading.lux} lux at ${reading.time}'
                "></div>`;
            });
            
            chartHTML += '</div>';
            chart.innerHTML = chartHTML;
        }
        
        function refreshData() {
            updateData();
        }
        
        function resetStats() {
            if (confirm('Reset all statistics? This cannot be undone.')) {
                fetch('/reset')
                    .then(() => {
                        updateData();
                    });
            }
        }
        
        // Update data every 3 seconds
        updateData();
        setInterval(updateData, 3000);
    </script>
</body>
</html>
  )";
  
  server.send(200, "text/html", html);
}

void handleData() {
  StaticJsonDocument<1000> doc;
  
  doc["current"] = currentLux;
  doc["max"] = maxLux;
  doc["min"] = minLux;
  doc["status"] = getSunStatus();
  
  // Calculate average
  float sum = 0;
  int count = 0;
  for (int i = 0; i < 24; i++) {
    if (readings[i].lux > 0) {
      sum += readings[i].lux;
      count++;
    }
  }
  doc["average"] = count > 0 ? round(sum / count) : 0;
  
  // Add history data
  JsonArray history = doc.createNestedArray("history");
  for (int i = 0; i < 24; i++) {
    JsonObject reading = history.createNestedObject();
    reading["lux"] = readings[i].lux;
    reading["time"] = readings[i].timestamp;
  }
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}

void handleReset() {
  maxLux = 0;
  minLux = 65535;
  
  // Clear readings array
  for (int i = 0; i < 24; i++) {
    readings[i].lux = 0;
    readings[i].timestamp = "";
  }
  readingIndex = 0;
  
  server.send(200, "text/plain", "Statistics reset successfully");
}
