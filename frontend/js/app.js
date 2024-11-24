// Select the canvas and get its context
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');

// Variables to track drawing state
let drawing = false;
let strokeBuffer = [];
const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/'; // Replace with your API Gateway URL

// Function to start drawing
function startDrawing(event) {
  drawing = true;
  strokeBuffer = []; // Reset the stroke buffer for a new stroke
  const [mouseX, mouseY] = getMousePosition(event);

  // Start the stroke
  strokeBuffer.push({ x: mouseX, y: mouseY });
}

// Function to stop drawing
async function stopDrawing() {
  if (!drawing) return;
  drawing = false;

  // Send the completed stroke to the API
  if (strokeBuffer.length > 0) {
    await sendStroke(strokeBuffer);
  }

  strokeBuffer = []; // Clear the buffer
}

// Function to draw on the canvas
function draw(event) {
  if (!drawing) return;

  const [mouseX, mouseY] = getMousePosition(event);

  // Draw on the canvas
  ctx.beginPath();
  const lastPoint = strokeBuffer[strokeBuffer.length - 1];
  ctx.moveTo(lastPoint.x, lastPoint.y);
  ctx.lineTo(mouseX, mouseY);
  ctx.stroke();

  // Add the current point to the stroke buffer
  strokeBuffer.push({ x: mouseX, y: mouseY });
}

// Function to get mouse position relative to the canvas
function getMousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  return [
    event.clientX - rect.left,
    event.clientY - rect.top
  ];
}

// Function to send the completed stroke to the API
async function sendStroke(stroke) {
  if (!stroke || stroke.length === 0) return; // No stroke to send

  // Prepare the payload
  const payload = { stroke };

  try {
    const response = await fetch(apiGatewayUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(payload),
    });

    if (!response.ok) {
      console.error('Failed to send stroke:', response.statusText);
    }
  } catch (error) {
    console.error('Error sending stroke:', error);
  }
}

// Add event listeners for mouse interactions
canvas.addEventListener('mousedown', startDrawing);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);
canvas.addEventListener('mousemove', draw);
