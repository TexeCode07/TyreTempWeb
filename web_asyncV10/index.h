const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Temperature Readings</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body {
      font-family: Arial, sans-serif;
      font-size:smaller;
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: flex-start;
      height: 100vh;
      background-color: #f5f5f5;
    }

    .container {
      display: flex;
      width: 98%;
      height: 98%;
      justify-content: center;
    }

    .side-panel {
      width: 110px;
      display: flex;
      flex-direction: column;
      gap: 10px;
      height: fit-content;
      margin-top: 10px;
    }

    .data-display {
      flex-grow: 1;
    }

    .theme-switcher {
      margin-top: 20px;
    }

    /* Light Theme */
    .light-theme {
      --background-color: #ffffff;
      --text-color: #000000;
      --canvas-border-color: #dddddd;
      /* Adjust border color for light theme */
      background-color: var(--background-color);
      color: var(--text-color);
    }

    /* Dark Theme */
    .dark-theme {
      --background-color: #363537;
      --text-color: #FAFAFA;
      --canvas-border-color: #666666;
      /* Adjust border color for dark theme */
      background-color: var(--background-color);
      color: var(--text-color);
    }

    .header {
      width: 60%;
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 20px 0;
    }

    .header-title {
      font-size: 20px;
      text-align: center;
    }

    .info-text {
      right: 0;
      font-size: 16px;
      text-align: right;
      margin-top: 5px;
    }

    table,
    th,
    td {
      border: 1px solid black;
      border-collapse: collapse;
      padding: 3px;
      text-align: center;
      border: 3px solid #BB86FC;
    }

    table {
      margin-bottom: 20px;
      margin-left: auto;
      margin-right: auto;
      border-collapse: collapse;
    }

    canvas {
      border: 1px solid ;
      border-radius: 5px;
    }

    options: {  
        responsive: true,
        maintainAspectRatio: false
    }

    /* Button Styling */
    button {
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
    }
    .button-normal {
      color: black; /* Or whatever your default color is */
    }

    .button-active {
      color: red; /* This will be the active state */
    }


    .configuration-container {
      margin-left: 50px; 
      width: 140px
    }
    
    .configuration-table {
      margin-left: 40px;
      /* Adjust spacing between items */
    }

    .input-container {
      position: relative;
    }

    .input-container input {
      padding-right: 10px;
      /* Adjust spacing between input and divider */
    }

    .divider {
      flex-grow: 1;
      height: 1px;
      background-color: #000;
      margin: 0 10px;
      /* Adjust spacing between inputs */
      position: relative;
    }

    .divider::after {
      content: "";
      position: absolute;
      top: 50%;
      left: 0;
      right: 0;
      border-top: 1px dashed #000;
    }

    .green-button {
      background-color: #4CAF50;
      /* Green */
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 5px;
    }

    .red-button {
      background-color: #FF0000;
      /* Red */
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 5px;
    }

    .blue-button {
      background-color:  #24A0ED;
      /* Red */
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 5px;
    }
    
    .radio-container {
      display: block;
      position: relative;
      padding-left: 35px;
      margin-bottom: 12px;
      cursor: pointer;
      font-size: 16px;
    }

    .radio-container input {
      position: absolute;
      opacity: 0;
      cursor: pointer;
    }

    .radio-checkmark {
      position: absolute;
      top: 0;
      left: 0;
      height: 25px;
      width: 25px;
      background-color: #ccc;
      border-radius: 50%;
    }

    .radio-container input:checked~.radio-checkmark:after {
      display: block;
    }

    .radio-container input:checked~.radio-checkmark {
      background-color: #2196F3;
    }

    .radio-checkmark:after {
      content: "";
      position: absolute;
      display: none;
    }

    .radio-container:hover input~.radio-checkmark {
      background-color: #ccc;
    }

    .radio-container input:checked~.radio-checkmark:after {
      display: block;
    }

    .radio-container .radio-checkmark:after {
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
      width: 12px;
      height: 12px;
      border-radius: 50%;
      background: white;
    }
  </style>
</head>

<body class="light-theme">
  <div class="container">
    <div class="side-panel">
      <button class="blue-button">Main Page</button>
      <button class="blue-button">Data</button>
      <button class="blue-button">Calculate</button>
      <button class="blue-button">Setup</button>
      <label class="theme-switcher">
        <input type="checkbox" id="theme-toggle"> Dark / Light View
      </label>
    </div>
    <div class="data-display">
      <div class="info-text">WiFi Name: <span id="wifiname">%WiFi Name%</span></div>
      <div class="info-text">IP Address: <span id="ipaddr">%IP Address%</span></div>
      <div class="info-text">Date: <span id="date">%Date%</span></div>
      <div class="info-text" style="color: red;">Time: <span id="time">%Time%</span></div>
      <!-------------------------------------------------MainPage------------------------------------------->
      <div id="mainPageContent">
        <div class="header-title">Main Page</div>
        <table style="width: 40%; font-size: 16px;">
          <thead>
            <tr>
              <th>Temperature Type</th><th>Reading (°C)</th>
            </tr>
          </thead>
          <tbody id="temperatureTable">
            <tr>
              <td>TRACK-TEMP1</td><td><span id="ir1">%0.0%</span></td>
            </tr>
            <tr>
              <td>TRACK-TEMP2</td><td><span id="ir2">%0.0%</span></td>
            </tr>
            <tr>
              <td>TRACK-TEMP3</td><td><span id="ir3">%0.0%</span></td>
            </tr>
            <tr>
              <td>AIR-TEMP</td><td><span id="temp">%0.0%</span></td>
            </tr>
          </tbody>
          <tfoot>
              <tr>
                <td colspan="2" id="lastUpdatedCell">Last Updated: <span id="last_ts">%LAST_UPDATED%</span></td>              
              </tr>
          </tfoot>
        </table>
        <br><br>
        <div style="height: 450px;  width: 800px; display: flex; justify-content: center; margin-inline: auto;">
            <canvas id="temperatureChart" ></canvas>
        </div>
      </div>
      <!-------------------------------------------------DataPage------------------------------------------->
      <div id="dataPageContent" style="margin-left: 1px; display: flex; justify-content: center;">
        <div class="header-title">Data Page</div>
        <div style="display: flex; justify-content: center;">
          <button class="red-button" onclick="clearData()">Clear Data</button>
          <button class="blue-button" onclick="saveAsCSV()">Save as CSV</button>
        </div>
        <table style="width: 60%; font-size: 16px">
          <thead>
            <tr>
              <th>Time</th>
              <th>Temperature</th>
              <th>IRTemp1</th>
              <th>IRTemp2</th>
              <th>IRTemp3</th>
            </tr>
          </thead>
          <tbody id="dataRecords">
            <!-- Data records will be inserted here -->
          </tbody>
        </table>
      </div>
      <!-------------------------------------------------CalculatePage------------------------------------------->
      <div id="calculatePageContent" style="margin-left: 1px; display: flex; justify-content: center;">
        <div class="header-title">Calculate Page</div>
        <div style="display: flex;">
          <div style="margin-left:200px;">
            <label for="calc">TyreName-Set#</label>
            <input type="text" id="calc" name="calc" placeholder="Enter value...">
            <div style="margin-top: 20px;">
              <label>Select Option:</label><br>
              <label class="radio-container">Use last measured data
                <input type="radio" name="calcOption" value="lastMeasured" checked>
                <span class="radio-checkmark"></span>
              </label>
              <label class="radio-container">Use custom data
                <input type="radio" name="calcOption" value="customData">
                <span class="radio-checkmark"></span>
              </label>
            </div>
            <div style="margin-top: 20px; margin-left: 100px">
              <div>
                <label for="temp1">T_Air:</label>
                <input type="number" id="temp1" name="temp1" placeholder="Enter Temp#1">
              </div>
              <div style="margin-top: 20px;">
                <label for="temp2">T_Track:</label>
                <input type="number" id="temp2" name="temp2" placeholder="Enter Temp#2">
              </div>
            </div>
          </div>
          <div style="margin-left:100px; margin-top:50px;">
            <h2>Result:</h2>
            <table class="configuration-table" style="font-size: 16px;">
                 <tr>
                    <td style="width: 200px; text-align: center;" id="box1">Value 1</td>
                    <td style="width: 200px; text-align: center;" id="box2">Value 2</td>
                </tr>
                <tr>
                    <td style="width: 200px; text-align: center;" id="box3">Value 3</td>
                    <td style="width: 200px; text-align: center;" id="box4">Value 4</td>
                </tr>
            </table>
          </div>

        </div>

        <div style="display: flex; justify-content: center;">
          <button class="green-button"   onclick="calculate()">Calculate</button>
          <button class="red-button"   onclick="clearCalc()">Clear Calculate</button>
          <button class="blue-button" onclick="saveCalcCSV()">Save CALC</button>
        </div>

        <table style="font-size: 10px;">
          <thead>
            <tr>
              <th>Date & Time</th>
              <th>CALC#</th>
              <th>Temp</th>
              <th>IRTemp1</th>
              <th>IRTemp2</th>
              <th>IRTemp3</th>
              <th>Temp#1</th>
              <th>Temp#2</th>
              <th>Value1</th>
              <th>Value2</th>
              <th>Value3</th>
              <th>Value4</th>
              <th>ValueTGT1</th>
              <th>ValueTGT2</th>
              <th>ValueTGT3</th>
              <th>ValueTGT4</th>
              <th>ValueP1</th>
              <th>ValueP2</th>
            </tr>
          </thead>
          <tbody id="dataCalculate">
            <!-- Data records will be inserted here -->
          </tbody>
        </table>

      </div>
      <!-------------------------------------------------SetupPage------------------------------------------->
      <div id="setupPageContent" style="margin-left: 1px; display: flex; justify-content: center;">
        <div class="header-title">Setup Page</div>
          
            <p style="text-align:center;">This is the setup page where you can configure various settings for your application. Use the input field below to enter necessary configuration and press the button to save your changes.</p>
            <p style="text-align:center;">Additional information and guidelines can be added here to assist users in correctly configuring their application. Ensure all inputs are correctly filled to avoid errors during application use.</p>
            <p style="text-align:center;">Remember to save your settings before leaving this page to ensure all changes are applied. If you encounter any issues, refer to the documentation or contact support for further assistance.</p>
            <p style="text-align:center;">Customization and configuration are key to maximizing the efficiency and effectiveness of your application. Take the time to properly set up your application according to your needs.</p>
            <p style="text-align:center;">Thank you for using our application. Your feedback is valuable to us, so please let us know if there are additional features or improvements you would like to see in future updates.</p>
              <form id="setupForm" method="POST" >
              <div class="container">
                <div class="configuration-container">
                    <div class="configuration-item">
                        <label for="temp_target_gas">TARGET AIR Temp:</label>
                        <input type="number" step="0.1" style="text-align: center; width:120px;margin-bottom: 10px;" id="temp_target_gas" name="temp_target_gas"  placeholder="Enter value...">
                    </div>
                    <div class="configuration-item">
                        <label for="temp_target_tr">TARGET TRACK Temp:</label>
                        <input type="number" step="0.1" style="text-align: center; width:120px;" id="temp_target_tr" name="temp_target_tr"    placeholder="Enter value...">
                    </div>
                </div>
                <div class="configuration-section">
                    <div class="value-tgt">
                        <span style="margin-left: 50px;"></span>
                        <label for="value_tg1">TARGET FL</label>
                        <span style="margin-left: 70px;"></span> <!-- Adjust the margin value as needed -->
                        <label for="value_tg2">TARGET FR</label>
                    </div>
                    <br>
                    <table class="configuration-table">
                        <tr>
                            <td><input type="number" step="0.01" style="text-align: center; width:120px;" id="value_tg1" name="value_tg1" class="box-input"  placeholder="Enter value for Box 1"></td>
                            <td><input type="number" step="0.01" style="text-align: center; width:120px;" id="value_tg2" name="value_tg2" class="box-input"  placeholder="Enter value for Box 2"></td>
                        </tr>
                        <tr>
                            <td><input type="number" step="0.01" style="text-align: center; width:120px;" id="value_tg3" name="value_tg3" class="box-input"  placeholder="Enter value for Box 3"></td>
                            <td><input type="number" step="0.01" style="text-align: center; width:120px;" id="value_tg4" name="value_tg4" class="box-input"  placeholder="Enter value for Box 4"></td>
                        </tr>
                    </table>
                    <div class="value-tgt">
                        <span style="margin-left: 50px;"></span>
                        <label for="value_tg3">TARGET RL</label>
                        <span style="margin-left: 70px;"></span>
                        <label for="value_tg4">TARGET RR</label>
                    </div>
                </div>
                
                <div class="configuration-container">
                    <label for="valuep1">TARGET FA:</label>
                    <input type="number" step="0.01" style="text-align: center; width:120px;" id="valuep1" name="value_p1"  placeholder="Input 1">
                    <div class="divider" style="margin-top: 10px;margin-bottom: 10px;"></div>
                    <input type="number" step="0.01" style="text-align: center; width:120px;" id="valuep2" name="value_p2"  placeholder="Input 2">
                    <label for="valuep2">TARGET RA:</label>
                </div>
               
                <span style="margin-left: 30px;"></span>
                <button id="saveChangesBtn" type="submit" class="green-button" style="margin-top: 70px;">Save Changes</button>
              </div> 
              </form>
          
      </div>
    </div>
  </div>
  <script>
  
    var irt1, irt2, irt3,temp;
 
    document.addEventListener('DOMContentLoaded', function() {
        // Correctly define the chart context and other elements
        var ctx                 = document.getElementById('temperatureChart').getContext('2d');
        var themeToggle         = document.getElementById('theme-toggle');
        var body                = document.querySelector('body');
        // Get r eferences to the content divs and buttons
        var mainPageContent     = document.getElementById('mainPageContent');
        var dataPageContent     = document.getElementById('dataPageContent');
        var calculatePageContent= document.getElementById('calculatePageContent');
        var setupPageContent    = document.getElementById('setupPageContent');

        var mainPageButton      = document.querySelector('button:nth-child(1)');
        var dataPageButton      = document.querySelector('button:nth-child(2)');
        var calculatePageButton = document.querySelector('button:nth-child(3)');
        var setupPageButton     = document.querySelector('button:nth-child(4)');

        var temperatureTable    = document.getElementById('temperatureTable');
        
        var temperatureChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [
                    {
                        label: 'Temp',
                        data: [],
                        borderColor: 'rgba(255, 99, 132, 1)',
                        borderWidth: 1,
                        fill: false
                    },
                    {
                        label: 'IR Temp1',
                        data: [],
                        borderColor: 'rgba(54, 162, 235, 1)',
                        borderWidth: 1,
                        fill: false
                    },
                    {
                        label: 'IR Temp2',
                        data: [],
                        borderColor: 'rgba(255, 206, 86, 1)',
                        borderWidth: 1,
                        fill: false
                    },
                    {
                        label: 'IR Temp3',
                        data: [],
                        borderColor: 'rgba(75, 192, 192, 1)',
                        borderWidth: 1,
                        fill: false
                    }
                ]
            },
            options: {
                scales: {
                    y: {
                        beginAtZero: true,
                        grid: {
                            color: 'rgba(0, 0, 0, 0.1)' // Cyan for grid lines
                        },
                        ticks: {
                            color: '#000000' // Neon green for text
                        }
                    },
                    x: {
                        ticks: {
                            color: '#000000' // Neon green for text
                        },
                        grid: {
                            color: 'rgba(0, 255, 255, 0.5)' // Cyan for grid lines
                        }
                    }
                },
                plugins: {
                    legend: {
                        labels: {
                            color: '#000000', // Neon green for legend text
                        }
                    },
                }
            }
        });
        
      
      //----------------------------------------Main Page Function---------------------------------------------------
      function showMainPage() {
          mainPageContent.style.display = 'block'; // Show the main page content
          dataPageContent.style.display = 'none';
          calculatePageContent.style.display = 'none';
          setupPageContent.style.display = 'none';

          // Reset button colors
          resetButtonColors();
          // Set current button color to red
          mainPageButton.style.color = 'red';
      }
      //----------------------------------------DATA Page Function---------------------------------------------------
      function showDataPage() {
        mainPageContent.style.display = 'none';
        dataPageContent.style.display = 'block';
        calculatePageContent.style.display = 'none';
        setupPageContent.style.display = 'none';

        // Reset button colors
        resetButtonColors();
        // Set current button color to red
        dataPageButton.style.color = 'red';
      }
      //----------------------------------------Calculate Page Function------------------------------------------------
      function calculateDataPage() {
        mainPageContent.style.display = 'none';
        dataPageContent.style.display = 'none';
        calculatePageContent.style.display = 'block';
        setupPageContent.style.display = 'none';

        // Reset button colors
        resetButtonColors();
        // Set current button color to red
        calculatePageButton.style.color = 'red';
      }
      //----------------------------------------Setup Page Function---------------------------------------------------
      function showSetupPage() {
        mainPageContent.style.display = 'none';
        dataPageContent.style.display = 'none';
        calculatePageContent.style.display = 'none';
        setupPageContent.style.display = 'block';

        // Reset button colors
        resetButtonColors();
        // Set current button color to red
        setupPageButton.style.color = 'red';
      }

      // Event listeners for buttons and theme toggle
      mainPageButton.addEventListener('click', showMainPage);
      dataPageButton.addEventListener('click', showDataPage);
      setupPageButton.addEventListener('click', showSetupPage);
      calculatePageButton.addEventListener('click', calculateDataPage);
      themeToggle.addEventListener('change', function() {
        if (this.checked) {
          body.classList.replace('light-theme', 'dark-theme');
          updateChartColors('dark'); 
        } else {
          body.classList.replace('dark-theme', 'light-theme');
          updateChartColors('light');
        }
      });

      // Function to update chart colors
      function updateChartColors(theme) {
          // Define colors for "dark" and "light" themes
          const darkThemeColors = {
              ticksColor: '#00FF00', // Neon green for text in dark theme
              gridColor: 'rgba(0, 255, 255, 0.5)', // Cyan for grid lines in dark theme
              legendColor: '#00FF00' // Neon green for legend text in dark theme
          };
          const lightThemeColors = {
              ticksColor: '#000000', // Black for text in light theme
              gridColor: 'rgba(0, 0, 0, 0.1)', // Light grey for grid lines in light theme
              legendColor: '#000000' // Black for legend text in light theme
          };

          // Apply colors based on the theme
          const colors = theme === 'dark' ? darkThemeColors : lightThemeColors;

          temperatureChart.options.scales.y.ticks.color = colors.ticksColor;
          temperatureChart.options.scales.y.grid.color = colors.gridColor;
          temperatureChart.options.scales.x.ticks.color = colors.ticksColor;
          temperatureChart.options.scales.x.grid.color = colors.gridColor;
          temperatureChart.options.plugins.legend.labels.color = colors.legendColor;

          // Update the chart to apply new colors
          temperatureChart.update();
      }


      // Initially show main page
      showMainPage();

      function updateTime() {
        var currentTime = new Date().toLocaleTimeString();
        document.getElementById("time").innerHTML = currentTime;
      }
      
      // Update the time every second
      window.onload = function() {
          setInterval(updateTime, 1000);
      }

      function resetButtonColors() {
          // Assuming you have the buttons defined similar to mainPageButton, dataPageButton, etc.
          mainPageButton.style.color = 'black'; // or your default color
          dataPageButton.style.color = 'black';
          calculatePageButton.style.color = 'black';
          setupPageButton.style.color = 'black';
      }

      document.getElementById('setupForm').addEventListener('submit', function(event) {
            event.preventDefault(); // Prevent the form from submitting in the traditional way

            const formData = new FormData(this); // 'this' refers to the form
            
            fetch('/', {
                method: 'POST',
                body: formData,
            })
            .then(response => {
                if (response.ok) {
                    return response.text();
                }
                throw new Error('Network response was not ok.');
            })
            .then(data => {
                console.log(data); // Handle the response data
                alert('Form submitted successfully!');
            })
            .catch(error => {
                console.error('There has been a problem with your fetch operation:', error);
                alert('Error submitting form. Please try again later.');
            });
      });

  
      if (!!window.EventSource) {
        var source = new EventSource('/events');
        
        source.addEventListener('open', function(e) {
          console.log("Events Connected");
        }, false);

        source.addEventListener('error', function(e) {
          if (e.target.readyState != EventSource.OPEN) {
            console.log("Events Disconnected");
          }
        }, false);
        
        source.addEventListener('message', function(e) {
          console.log("message", e.data);
        }, false);
        
        source.addEventListener('wifiname', function(e) {
          console.log("wifiname", e.data);
          document.getElementById("wifiname").innerHTML = e.data;
        }, false);

        source.addEventListener('ipaddr', function(e) {
          console.log("ipaddr", e.data);
          document.getElementById("ipaddr").innerHTML = e.data;
        }, false);

        source.addEventListener('date', function(e) {
          console.log("date", e.data);
          document.getElementById("date").innerHTML = e.data;
        }, false);

        source.addEventListener('clearTable', function(e) {
            document.getElementById("box1").innerHTML = "Value1";
            document.getElementById("box2").innerHTML = "Value2";
            document.getElementById("box3").innerHTML = "Value3";
            document.getElementById("box4").innerHTML = "Value4";
            document.getElementById("dataCalculate").innerHTML = "";
        }, false);

        source.addEventListener('clearDataTable', function(e) {
            document.getElementById("dataRecords").innerHTML = "";
        }, false);

        source.addEventListener('DataData', function(e) {
            const dataString = e.data.trim(); // Trim leading and trailing whitespace
            console.log("Data Data", dataString);
            try {
                const DataData = JSON.parse(dataString); // Parse the JSON string into an array

                // Ensure DataData is an array before proceeding
                if (!Array.isArray(DataData)) {
                    console.error("Parsed data is not an array:", DataData);
                    return;
                }

                const tbody = document.getElementById("dataRecords");
                tbody.innerHTML = ""; // Clear existing table rows

                // Prepare to collect the latest 20 records for the chart
                const labels = [];
                const tempData = [];
                const irt1Data = [];
                const irt2Data = [];
                const irt3Data = [];

                // Variables to store the latest record
                let latestRecord;

                DataData.forEach(function(record) {
                    const row = document.createElement("tr");
                    row.innerHTML = `
                        <td>${record.timestamp}</td>
                        <td>${record.temp.toFixed(2)}</td>
                        <td>${record.irt1.toFixed(2)}</td>
                        <td>${record.irt2.toFixed(2)}</td>
                        <td>${record.irt3.toFixed(2)}</td>
                    `;
                    tbody.appendChild(row);

                    // Update the latest record
                    latestRecord = record;
                });

                // Get the latest 20 records for chart display
                const startIndex = Math.max(0, DataData.length - 20);
                const latestData = DataData.slice(startIndex);

                latestData.forEach(record => {
                    labels.push(record.timestamp);
                    tempData.push(record.temp);
                    irt1Data.push(record.irt1);
                    irt2Data.push(record.irt2);
                    irt3Data.push(record.irt3);
                });

                // Update the chart with the latest 20 data entries
                temperatureChart.data.labels = labels;
                temperatureChart.data.datasets[0].data = tempData;
                temperatureChart.data.datasets[1].data = irt1Data;
                temperatureChart.data.datasets[2].data = irt2Data;
                temperatureChart.data.datasets[3].data = irt3Data;

                temperatureChart.update();

                // Update the main page table with the last data
                if (latestRecord) {
                    document.getElementById("ir1").textContent = latestRecord.irt1.toFixed(2);
                    document.getElementById("ir2").textContent = latestRecord.irt2.toFixed(2);
                    document.getElementById("ir3").textContent = latestRecord.irt3.toFixed(2);
                    document.getElementById("temp").textContent = latestRecord.temp.toFixed(2);
                    document.getElementById("last_ts").textContent = latestRecord.timestamp;
                }
            } catch (error) {
                console.error("Error parsing JSON:", error);
                console.log("Raw data:", dataString); // Log raw data for debugging
            }
        }, false);

        
        source.addEventListener('calculateData', function(e) {
            const trimmedData = e.data.trim(); // Trim leading and trailing whitespace
            const dataRows = trimmedData.split('\n'); // Split the data into individual JSON objects

            try {
                const calculateData = dataRows.map(row => JSON.parse(row)); // Parse each JSON object
                console.log("Calculate data", calculateData);
                
                // Get the tbody element where the data will be inserted
                const tbody = document.getElementById("dataCalculate");

                // Clear existing table rows
                tbody.innerHTML = "";

                // Iterate over the data and create table rows
                calculateData.forEach(function(record) {
                    const row = document.createElement("tr");
                    row.innerHTML = `
                        <td>${record.date}</td>
                        <td>${record.calc}</td>
                        <td>${record.temp}</td>
                        <td>${record.irt1}</td>
                        <td>${record.irt2}</td>
                        <td>${record.irt3}</td>
                        <td>${record.temp1}</td>
                        <td>${record.temp2}</td>
                        <td>${record.value1}</td>
                        <td>${record.value2}</td>
                        <td>${record.value3}</td>
                        <td>${record.value4}</td>
                        <td>${record.value_tg1}</td>
                        <td>${record.value_tg2}</td>
                        <td>${record.value_tg3}</td>
                        <td>${record.value_tg4}</td>
                        <td>${record.valuep1}</td>
                        <td>${record.valuep2}</td>
                    `;
                    tbody.appendChild(row);
                });
            } catch (error) {
                console.error("Error parsing JSON:", error);
                console.log("Raw data:", trimmedData); // Log raw data for debugging
            }
        }, false);


        source.addEventListener('setupData', function(e) {
          console.log("setupData", e.data);
          var setupData = JSON.parse(e.data);

          temp_target_gas = setupData.ttg;
          temp_target_tr  = setupData.ttr;
          value_tg1       = setupData.vt1;
          value_tg2       = setupData.vt2;
          value_tg3       = setupData.vt3;
          value_tg4       = setupData.vt4;
          valuep1         = setupData.vp1;
          valuep2         = setupData.vp2;

          document.getElementById("temp_target_gas").value  = temp_target_gas.toFixed(1);
          document.getElementById("temp_target_tr").value   = temp_target_tr.toFixed(1);
          document.getElementById("value_tg1").value        = value_tg1.toFixed(2);
          document.getElementById("value_tg2").value        = value_tg2.toFixed(2);
          document.getElementById("value_tg3").value        = value_tg3.toFixed(2);
          document.getElementById("value_tg4").value        = value_tg4.toFixed(2);
          document.getElementById("valuep1").value          = valuep1.toFixed(2);
          document.getElementById("valuep2").value          = valuep2.toFixed(2);

        }, false);

        source.addEventListener('sensorData', function(e) {
          var data = JSON.parse(e.data);
          
          // Store sensor data in global variables
          irt1 = parseFloat(data.irt1);
          irt2 = parseFloat(data.irt2);
          irt3 = parseFloat(data.irt3);
          
          temp = parseFloat(data.temp);

          // --------------------------------------Main Page----------------------------------------- 
          document.getElementById("ir1").innerHTML     = irt1.toFixed(2);
          document.getElementById("ir2").innerHTML     = irt2.toFixed(2);
          document.getElementById("ir3").innerHTML     = irt3.toFixed(2);
          document.getElementById("temp").innerHTML    = temp.toFixed(2);
          document.getElementById("last_ts").innerHTML = data.timestamp;


          // Format current time as YYYY-MM-DD HH:MM:SS
          var now = new Date();
          var year = now.getFullYear();
          var month = (now.getMonth() + 1).toString().padStart(2, '0');   // Ensure two digits, January is 0!
          var day = now.getDate().toString().padStart(2, '0');            // Ensure two digits
          var hours = now.getHours().toString().padStart(2, '0');         // Ensure two digits
          var minutes = now.getMinutes().toString().padStart(2, '0');     // Ensure two digits
          var seconds = now.getSeconds().toString().padStart(2, '0');     // Ensure two digits
          var currentTime = `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`; // Format date and time as YYYY-MM-DD HH:MM:SS

  
          temperatureChart.data.labels.push(currentTime);

          // Assuming the order of datasets in temperatureData matches the order of sensor data
          temperatureChart.data.datasets[0].data.push(temp); // Temp
          temperatureChart.data.datasets[1].data.push(irt1); // IR Temp1
          temperatureChart.data.datasets[2].data.push(irt2); // IR Temp2
          temperatureChart.data.datasets[3].data.push(irt3); // IR Temp3

          // Keep chart data to a reasonable length, e.g., last 20 readings
          if (temperatureChart.data.labels.length > 20) {
              temperatureChart.data.labels.shift(); // Remove the first label
              temperatureChart.data.datasets.forEach(dataset => dataset.data.shift()); // Remove the first data point from each dataset
          }

          temperatureChart.update(); // Update the chart to reflect new data

          // --------------------------------------Data Page----------------------------------------- 
          updateDataTable(data);
        }, false);
      }

    });

    
    // --------------------------------------Data Page----------------------------------------- 
    // Function to clear the data table
    function clearData() {
        var tableBody = document.getElementById("dataRecords");
        tableBody.innerHTML = ""; // Remove all rows
        fetch('/clearData')
          .then(response => {
              if (response.ok) {
                  console.log("Data cleared successfully");
              } else {
                  throw new Error('Network response was not ok.');
              }
          })
          .catch(error => {
              console.error('Error clearing data:', error);
              alert('Error clearing data. Please try again later.');
          });
    }

    // Function to update the data table with new sensor readings
    function updateDataTable(data) {
        var tableBody = document.getElementById("dataRecords");
        var newRow = document.createElement("tr");
        newRow.innerHTML = `
            <td>${data.timestamp}</td>
            <td>${data.temp.toFixed(2)}</td>
            <td>${data.irt1.toFixed(2)}</td>
            <td>${data.irt2.toFixed(2)}</td>
            <td>${data.irt3.toFixed(2)}</td>
        `;
        tableBody.appendChild(newRow);
    }
    
    // Function to save the data table as CSV
    function saveAsCSV() {
        var table = document.getElementById("dataRecords");
        var rows = table.getElementsByTagName("tr");
        var csvContent = "data:text/csv;charset=utf-8,";

        // Iterate over table rows and concatenate cell values into CSV format
        for (var i = 0; i < rows.length; i++) {
            var cells = rows[i].getElementsByTagName("td");
            var rowData = [];
            for (var j = 0; j < cells.length; j++) {
                rowData.push(cells[j].innerText);
            }
            csvContent += rowData.join(",") + "\n";
        }

        // Create a hidden link element to trigger download
        var encodedUri = encodeURI(csvContent);
        var link = document.createElement("a");
        link.setAttribute("href", encodedUri);
        link.setAttribute("download", "sensor_data.csv");
        document.body.appendChild(link);

        // Trigger download
        link.click();
    }

    function saveCalcCSV() {
        var table = document.getElementById("dataCalculate");
        var rows = table.getElementsByTagName("tr");
        var csvContent = "data:text/csv;charset=utf-8,";

        // Iterate over table rows and concatenate cell values into CSV format
        for (var i = 0; i < rows.length; i++) {
            var cells = rows[i].getElementsByTagName("td");
            var rowData = [];
            for (var j = 0; j < cells.length; j++) {
                rowData.push(cells[j].innerText);
            }
            csvContent += rowData.join(",") + "\n";
        }

        // Create a hidden link element to trigger download
        var encodedUri = encodeURI(csvContent);
        var link = document.createElement("a");
        link.setAttribute("href", encodedUri);
        link.setAttribute("download", "sensor_calc.csv");
        document.body.appendChild(link);

        // Trigger download
        link.click();
    }


    // --------------------------------------Setup Page----------------------------------------- 
    // Get a reference to the button element
    var button = document.getElementById('saveChangesBtn');

    // Add an event listener to the button, listening for the 'click' event
    button.addEventListener('click', storeValues);
    
    var temp_target_gas, temp_target_tr, value_tg1, value_tg2, value_tg3, value_tg4,valuep1,valuep2;

    function storeValues() {
        // Prevent the default form submission
        //event.preventDefault();

        temp_target_gas = parseFloat(document.getElementById("temp_target_gas").value);
        temp_target_tr  = parseFloat(document.getElementById("temp_target_tr").value);
        value_tg1       = parseFloat(document.getElementById("value_tg1").value);
        value_tg2       = parseFloat(document.getElementById("value_tg2").value);
        value_tg3       = parseFloat(document.getElementById("value_tg3").value);
        value_tg4       = parseFloat(document.getElementById("value_tg4").value);
        valuep1         = parseFloat(document.getElementById("valuep1").value);
        valuep2         = parseFloat(document.getElementById("valuep2").value);

        // Check if any value is NaN (not a number)
        if (isNaN(temp_target_gas) || isNaN(temp_target_tr) || isNaN(value_tg1) || isNaN(value_tg2) || isNaN(value_tg3) || isNaN(value_tg4) || isNaN(valuep1) || isNaN(valuep2)) {
            alert("Please enter valid float values for all boxes.");
            return;
        }

        // Print the stored values to the console
        console.log("Temp AIR:", temp_target_gas);
        console.log("Temp Track:", temp_target_tr);
        console.log("Target FL:", value_tg1);
        console.log("Target FR:", value_tg2);
        console.log("Target RL:", value_tg3);
        console.log("Target RR:", value_tg4);
        console.log("Target P FA:", valuep1);
        console.log("Target P RA:", valuep2);

        // Show a dialog box to indicate values are saved
        // alert("Values are saved successfully!");
    }


    // --------------------------------------Calculate Page----------------------------------------- 
    var value1, value2, value3, value4;

    function calculate(){
      var calcValue = document.getElementById("calc").value.trim();
      if (calcValue === "") {
          alert("Tyre-SET-NAME#:");
          return;
      }
      
      var calcOption = document.querySelector('input[name="calcOption"]:checked').value;
      if (calcOption === "lastMeasured") {  
        console.log(irt1);console.log(irt2);console.log(irt3);console.log(temp);
        console.log(temp_target_gas);console.log(temp_target_tr);
        console.log(value_tg1);console.log(valuep1);
        console.log(typeof irt1, irt1);
        console.log(typeof temp_target_gas,temp_target_gas);
        console.log(temp_target_gas + temp_target_tr);
        console.log(value_tg1 * valuep1);

        value1 = value_tg1 + (temp - temp_target_gas) * (0.04 / 5) - ((0.05 / 10) * (((irt1 + irt2 + irt3) / 3) - temp_target_tr)); 
        value2 = value_tg2 + (temp - temp_target_gas) * (0.04 / 5) - ((0.05 / 10) * (((irt1 + irt2 + irt3) / 3) - temp_target_tr)); 
        value3 = value_tg3 + (temp - temp_target_gas) * (0.04 / 5) - ((0.05 / 10) * (((irt1 + irt2 + irt3) / 3) - temp_target_tr)); 
        value4 = value_tg4 + (temp - temp_target_gas) * (0.04 / 5) - ((0.05 / 10) * (((irt1 + irt2 + irt3) / 3) - temp_target_tr)); 
      
      } else if (calcOption === "customData") {
        var temp1val = parseFloat(document.getElementById("temp1").value);
        if (temp1val === "") {
            alert("Please enter a value for Temp Air");
            return;
        }

        var temp2val = parseFloat(document.getElementById("temp2").value);
        if (temp2val === "") {
            alert("Please enter a value for Temp Track");
            return;
        }

        value1 = value_tg1 + (temp1val-temp_target_gas)*(0.04/5)-((0.05/10)*(temp2val-temp_target_tr));
        value2 = value_tg2 + (temp1val-temp_target_gas)*(0.04/5)-((0.05/10)*(temp2val-temp_target_tr));
        value3 = value_tg3 + (temp1val-temp_target_gas)*(0.04/5)-((0.05/10)*(temp2val-temp_target_tr));
        value4 = value_tg4 + (temp1val-temp_target_gas)*(0.04/5)-((0.05/10)*(temp2val-temp_target_tr));
      }

      // Update the table with calculated values
      document.getElementById("box1").textContent = value1.toFixed(2);
      document.getElementById("box2").textContent = value2.toFixed(2);
      document.getElementById("box3").textContent = value3.toFixed(2);
      document.getElementById("box4").textContent = value4.toFixed(2);

      // Inserting the calculated values into the table
      var table = document.getElementById("dataCalculate");
      var newRow = table.insertRow();

      newRow.innerHTML = `
        <td>${new Date().toLocaleString()}</td>
        <td>${document.getElementById("calc").value}</td>
        <td>${temp.toFixed(2)}</td>
        <td>${irt1.toFixed(2)}</td>
        <td>${irt2.toFixed(2)}</td>
        <td>${irt3.toFixed(2)}</td>
        <td>${temp1val}</td>
        <td>${temp2val}</td>
        <td>${value1.toFixed(2)}</td>
        <td>${value2.toFixed(2)}</td>
        <td>${value3.toFixed(2)}</td>
        <td>${value4.toFixed(2)}</td>
        <td>${value_tg1.toFixed(2)}</td>
        <td>${value_tg2.toFixed(2)}</td>
        <td>${value_tg3.toFixed(2)}</td>
        <td>${value_tg4.toFixed(2)}</td>
        <td>${valuep1.toFixed(2)}</td>
        <td>${valuep2.toFixed(2)}</td>
      `;


      fetch('/saveCalcData', {
        method: 'POST', // or 'PUT'
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          date: new Date().toLocaleString(),
          calc: document.getElementById("calc").value,
          temp: temp.toFixed(2),
          irt1: irt1.toFixed(2),
          irt2: irt2.toFixed(2),
          irt3: irt3.toFixed(2),
          temp1: temp1val,
          temp2: temp2val,
          value1: value1.toFixed(2),
          value2: value2.toFixed(2),
          value3: value3.toFixed(2),
          value4: value4.toFixed(2),
          value_tg1: value_tg1.toFixed(2),
          value_tg2: value_tg2.toFixed(2),
          value_tg3: value_tg3.toFixed(2),
          value_tg4: value_tg4.toFixed(2),
          valuep1: valuep1.toFixed(2),
          valuep2: valuep2.toFixed(2)
        })
      }).then(response => {
        console.log('Data sent successfully');
      }).catch((error) => {
        console.error('Error:', error);
      });
    }
    
    function clearCalc(){
        var tableBody = document.getElementById("dataCalculate");
        tableBody.innerHTML = ""; // Remove all rows
        fetch('/clearCalc')
          .then(response => {
              if (response.ok) {
                  console.log("Data cleared successfully");
              } else {
                  throw new Error('Network response was not ok.');
              }
          })
          .catch(error => {
              console.error('Error clearing data:', error);
              alert('Error clearing data. Please try again later.');
          });
    } 

  </script>
</body>

</html>
)rawliteral";