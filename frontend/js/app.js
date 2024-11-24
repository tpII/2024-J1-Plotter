// Select the canvas and get its context
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');

// Variables to track drawing state
let drawing = false;
let lastX = 0;
let lastY = 0;

// Buffer to store coordinates
const coordinatesBuffer = [];
const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/'; // Replace with your API Gateway URL

// Function to start drawing
function startDrawing(event) {
  drawing = true;
  [lastX, lastY] = getMousePosition(event);
}

// Function to stop drawing
function stopDrawing() {
  drawing = false;
  lastX = 0;
  lastY = 0;
}

// Function to draw on the canvas
function draw(event) {
  if (!drawing) return;

  const [mouseX, mouseY] = getMousePosition(event);

  // Draw on the canvas
  ctx.beginPath();
  ctx.moveTo(lastX, lastY);
  ctx.lineTo(mouseX, mouseY);
  ctx.stroke();

  // Add the coordinates to the buffer
  coordinatesBuffer.push({ x: lastX, y: lastY });
  coordinatesBuffer.push({ x: mouseX, y: mouseY });

  // Update the last position
  lastX = mouseX;
  lastY = mouseY;
}

// Function to get mouse position relative to the canvas
function getMousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  return [
    event.clientX - rect.left,
    event.clientY - rect.top
  ];
}

// Function to send buffered coordinates to the API
async function sendCoordinates() {
  if (coordinatesBuffer.length === 0) return; // No coordinates to send

  // Prepare the payload
  const payload = { coordinates: [...coordinatesBuffer] };

  // Clear the buffer after preparing the payload
  coordinatesBuffer.length = 0;

  try {
    const response = await fetch(apiGatewayUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(payload),
    });

    if (!response.ok) {
      console.error('Failed to send coordinates:', response.statusText);
    }
  } catch (error) {
    console.error('Error sending coordinates:', error);
  }
}

// Set up an interval to send buffered coordinates periodically
setInterval(sendCoordinates, 1000); // Send every 1 second

// Add event listeners for mouse interactions
canvas.addEventListener('mousedown', startDrawing);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);
canvas.addEventListener('mousemove', draw);
