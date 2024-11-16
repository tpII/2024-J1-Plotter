 // Initialize canvas and API details
const canvas = document.getElementById('drawCanvas');
const context = canvas.getContext('2d');
const apiUrl = 'YOUR_API_GATEWAY_URL_HERE';  // Replace with the actual API Gateway URL

let drawing = false;
let lastX = 0;
let lastY = 0;

// Function to start drawing
canvas.addEventListener('mousedown', (e) => {
  drawing = true;
  [lastX, lastY] = [e.offsetX, e.offsetY];
});

// Function to stop drawing
canvas.addEventListener('mouseup', () => drawing = false);
canvas.addEventListener('mouseout', () => drawing = false);

// Function to draw and send coordinates
canvas.addEventListener('mousemove', (e) => {
  if (!drawing) return;

  // Draw on the canvas
  context.beginPath();
  context.moveTo(lastX, lastY);
  context.lineTo(e.offsetX, e.offsetY);
  context.stroke();
  [lastX, lastY] = [e.offsetX, e.offsetY];

  // Send coordinates to backend
  sendDrawingData(lastX, lastY);
});

// Function to send drawing data to API Gateway
function sendDrawingData(x, y) {
  fetch(apiUrl, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ x, y })
  })
  .then(response => {
    if (!response.ok) {
      console.error("Failed to send drawing data:", response.statusText);
    }
  })
  .catch(error => {
    console.error("Error sending data:", error);
  });
}
 