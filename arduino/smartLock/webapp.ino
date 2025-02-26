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
        <button onclick = "lock()"> Lock Servo </button>
    </div>

    <script>
        function login(){
            const password = document.getElementById('password').value;
            var url = '/toggle-lock?password=' + encodeURIComponent(password);
            fetch(url)
            .then(response => response.text())
            .then (data => alert(data))
            .catch(error => console.error('Error:', error));
        }
    </script>
</body>
</html>)rawliteral";
}