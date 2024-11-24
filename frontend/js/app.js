// Select the canvas and get its context
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');

// Set up the canvas for drawing
let drawing = false; // Tracks if the user is currently drawing
let lastX = 0;       // Last X coordinate
let lastY = 0;       // Last Y coordinate

// The API Gateway URL
const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/'; // Replace with your API Gateway URL

// Function to start drawing
function startDrawing(event) {
  drawing = true;
  [lastX, lastY] = getMousePosition(event); // Get initial position
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

  // Get the current mouse position
  const [mouseX, mouseY] = getMousePosition(event);

  // Draw a line from the last position to the current position
  ctx.beginPath();
  ctx.moveTo(lastX, lastY);
  ctx.lineTo(mouseX, mouseY);
  ctx.stroke();

  // Send the current coordinates to the API Gateway
  sendCoordinates(lastX, lastY, mouseX, mouseY);

  // Update the last position
  lastX = mouseX;
  lastY = mouseY;
}

// Function to get the mouse position relative to the canvas
function getMousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  return [
    event.clientX - rect.left,
    event.clientY - rect.top
  ];
}

// Function to send coordinates to the API Gateway
async function sendCoordinates(x1, y1, x2, y2) {
  const payload = {
    coordinates: [
      { x: x1, y: y1 },
      { x: x2, y: y2 }
    ]
  };

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

// Add event listeners for mouse interactions
canvas.addEventListener('mousedown', startDrawing);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);
canvas.addEventListener('mousemove', draw);
