String webApp(){
    return R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>smartlocker</title>
    <style>
        body{
            font-family: Arial, Helvetica, sans-serif;
            text-align: center;
            margin-top: 50px;
        }
        button{
            background-color: black;
            border: none;
            color: white;
            padding: 15px 32px;
            text-align: center;
        }
    </style>
</head>
<body>
    <h1> ESP32 SmartLocker </h1>
    <div id = "loginAdmin">
    <input type="password" id="adm-pw" placeholder="Enter Admin Password">
    <button onclick = "login()">Login</button>
    </div>
    <div id = "control" style = "display: none;">
        <h2> Status: <span id="lockStatus">Locked</span></h2>
        <button onclick = "lock()"> Lock Servo </button>
        <button onclick = "unlock()"> Unlock Servo </button>
    </div>

    <script>
        function login(){
            const password = document.getElementById('adm-pw').value;
            
            fetch(`/admin-login?password=${encodeURIComponent(password)}`)
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
            document.getElementById("lockStatus").innerText = "Unlocked";
            lastMoved = performance.now() - startTime;
            console.log(`Unlock Time: ${lastMoved.toFixed(2)} ms`);
          })
          .catch(error => console.error('Error:', error));
        }

        function lock(){
            let startTime = performance.now();
            fetch('/lock')
            .then(() => {
              document.getElementById("lockStatus").innerText = "Locked";
              lastMoved = performance.now() - startTime;
              console.log(`Lock Time: ${lastMoved.toFixed(2)} ms`);
            })
            .catch(error => console.error('Error:', error));
        }
    </script>
</body>
</html>)rawliteral";
}