String webApp() {
  return R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>smartlocker</title>
    <style>
        body{
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
        }

        .Main{
          background-color: #7ea7c2;
          border-radius: 10px;
          padding: 30px;
          width: 90%;
          max-width: 800px;
          text-align: center;
          margin: auto;
          margin-bottom: 20px;
        }

        h1{
          font-size: 28px;
        }

        h2{
          font-size: 22px;
        }
        button{
            background-color: black;
            border: none;
            border-radius: 5px;
            color: white;
            padding: 15px 32px;
            text-align: center;
            cursor: pointer;
            font-size: 16px;
        }
        button:hover{
          background-color: red;
        }

        #control{
          display: none;
        }

        #lock{
          font-weight: bold;
          color: light-green;
        }

        #rfidUID{
          font-size: 18px;
          font-weight: normal;
          color: red;
        }

        #voltage{
          font-size: 18px;
          font-weight: normal;
          color: yellow;
        }

        .infoBox{
            background-color: #4c76ad;
            border-radius: 8px;
            padding: 20px;
            margin: auto;
            max-width: 800px;
            margin-top: 20px;
            margin-bottom: 20px;
        }

    </style>
</head>
<body>
<div class = "Main"> 
    <h1> ESP32 SmartLocker </h1>
    <div id = "loginAdmin">
    <input type="password" id="adm-pw" placeholder="Enter Admin Password">
    <button onclick = "login()">Login</button>
    </div>
    <div id = "control">
        <h2> Status: <span id="lock">Locked</span></h2>
        <button onclick = "lock()"> Lock Servo </button>
        <button onclick = "unlock()"> Unlock Servo </button>
    </div>
  </div>

  <div class = "infoBox">
    <h3>Last RFID Tag: <span id = "rfidUID"> None </span></h3>
    
    /*</div>
    <div class = "infoBox">
    <h3>Last Voltage Measured: <span id = "voltage"> None </span><h3>
    </div>*/

    <script>
        function login(){
            const password = document.getElementById('adm-pw').value;
            
            fetch("/admin-login?password=" + encodeURIComponent(password))
            .then(response => response.text())
            .then (data => {
              if(data.includes("Access Granted")){
                document.getElementById("loginAdmin").style.display = "none";
                document.getElementById("control").style.display = "block";
              }
            })
            .catch(error => console.error('Error:', error));
        }

        function unlock(){
          let startTime = performance.now();
          fetch('/unlock')
          .then(() => {
            document.getElementById("lock").innerText = "Unlocked";
            lastMoved = performance.now() - startTime;
            console.log("Unlock Time: " + lastMoved.toFixed(2) + "ms");
          })
          .catch(error => console.error('Error:', error));
        }

        function lock(){
          let startTime = performance.now();
          fetch('/lock')
          .then(() => {
            document.getElementById("lock").innerText = "Locked";
            lastMoved = performance.now() - startTime;
            console.log("Lock Time: " + lastMoved.toFixed(2)+  "ms");
            })
            .catch(error => console.error('Error:', error));
        }

        function fetchRFID(){
          fetch('/scannedUID')
          .then(response => response.text())
          .then(data => document.getElementById("rfidUID").innerText = data)
          .catch(error => console.error('Error:', error));
        }

        /*function fetchVoltage(){
          fetch('/voltage')
          .then(response => response.text())
          .then(data => document.getElementById("voltage").innerText = data)
          .catch(error => console.error('Error:', error));
        }*/

        setInterval(fetchRFID, 1000); 
        //setInterval(fetchVoltage, 1000);
    </script>
</body>
</html>)rawliteral";
}