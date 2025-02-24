    document.getElementById('start-btn').addEventListener('click', function() {
    document.getElementById('landing').classList.remove('active');
    document.getElementById('dashboard').classList.add('active');
  });
  
function getStarted(){
  auth.signInAnonymously()
    .then(() => {
      document.getElementById("rfid-section");
      document.getElementById("public-section");
      RFIDTags();
    })
}
  
