document.querySelectorAll('.locker').forEach(locker => {
    locker.addEventListener('click', function() {
      if (this.classList.contains('available')) {
        this.classList.remove('available');
        this.classList.add('reserved');
        this.textContent = 'Reserved';
        document.getElementById('status-msg').textContent = 'Locker Reserved!';
      } else {
        this.classList.remove('reserved');
        this.classList.add('available');
        this.textContent = 'Locker';
        document.getElementById('status-msg').textContent = 'Locker Available!';
      }
    });
  });

  document.getElementById('start-btn').addEventListener('click', function() {
    document.getElementById('landing').classList.remove('active');
    document.getElementById('dashboard').classList.add('active');
  });
  
  