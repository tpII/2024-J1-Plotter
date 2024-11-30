const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/'; // Replace with your API URL
const cognitoClientId = '32ad6ce6ub7eq69qetg3f151cj'; // Replace with your Cognito User Pool Client ID
const cognitoAuthUrl = 'https://your-cognito-domain.auth.us-east-1.amazoncognito.com/oauth2/token'; // Replace with your Cognito domain
let idToken = null;

// Login Elements
const loginContainer = document.getElementById('login-container');
const loginForm = document.getElementById('login-form');
const loginError = document.getElementById('login-error');

// Canvas Elements
const canvasContainer = document.getElementById('canvas-container');
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');
const logoutButton = document.getElementById('logout-button');

// Variables to track drawing state
let drawing = false;
let strokeBuffer = [];

// Function to handle login
loginForm.addEventListener('submit', async (event) => {
  event.preventDefault();

  const username = document.getElementById('username').value;
  const password = document.getElementById('password').value;

  try {
    const response = await fetch(`https://cognito-idp.us-east-1.amazonaws.com/`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/x-amz-json-1.1',
        'X-Amz-Target': 'AWSCognitoIdentityProviderService.InitiateAuth',
      },
      body: JSON.stringify({
        AuthParameters: { USERNAME: username, PASSWORD: password },
        AuthFlow: 'USER_PASSWORD_AUTH',
        ClientId: cognitoClientId,
      }),
    });

    const data = await response.json();
    if (response.ok) {
      idToken = data.AuthenticationResult.IdToken;
      console.log('JWT Token:', idToken); // Log the token
      loginContainer.style.display = 'none';
      canvasContainer.style.display = 'block';
    } else {
      throw new Error(data.message || 'Failed to login');
    }
    
  } catch (error) {
    loginError.textContent = error.message;
  }
});

// Logout functionality
logoutButton.addEventListener('click', () => {
  idToken = null;
  loginContainer.style.display = 'block';
  canvasContainer.style.display = 'none';
});

// Function to start drawing
function startDrawing(event) {
  drawing = true;
  strokeBuffer = []; // Reset the stroke buffer for a new stroke
  const [mouseX, mouseY] = getMousePosition(event);
  strokeBuffer.push({ x: mouseX, y: mouseY });
}

// Function to stop drawing
async function stopDrawing() {
  if (!drawing) return;
  drawing = false;
  if (strokeBuffer.length > 0) {
    await sendStroke(strokeBuffer);
  }
  strokeBuffer = [];
}

// Function to draw on the canvas
function draw(event) {
  if (!drawing) return;

  const [mouseX, mouseY] = getMousePosition(event);
  ctx.beginPath();
  const lastPoint = strokeBuffer[strokeBuffer.length - 1];
  ctx.moveTo(lastPoint.x, lastPoint.y);
  ctx.lineTo(mouseX, mouseY);
  ctx.stroke();
  strokeBuffer.push({ x: mouseX, y: mouseY });
}

// Function to get mouse position relative to the canvas
function getMousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  return [event.clientX - rect.left, event.clientY - rect.top];
}

// Function to send the completed stroke to the API
async function sendStroke(stroke) {
  if (!stroke || stroke.length === 0) return;

  try {
    console.log('Authorization Header:', `Bearer ${idToken}`);
    const response = await fetch(apiGatewayUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${idToken}`, // Ensure Bearer prefix
      },
      body: JSON.stringify({ stroke }),
    });

    if (!response.ok) {
      console.error('Failed to send stroke:', response.statusText);
    }
  } catch (error) {
    console.error('Error sending stroke:', error);
  }
}


// Add event listeners for canvas interactions
canvas.addEventListener('mousedown', startDrawing);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);
canvas.addEventListener('mousemove', draw);
