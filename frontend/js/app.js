const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/'; // URL del API Gateway

// Canvas Elements
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');

// Variables para seguimiento del dibujo
let drawing = false;
let strokeBuffer = [];

// const cognitoLoginUrl = "https://plotter.auth.us-east-1.amazoncognito.com/login?client_id=32ad6ce6ub7eq69qetg3f151cj&response_type=token&scope=email+openid&redirect_uri=http://localhost:3000/";
// const cognitoLogoutUrl = "https://plotter.auth.us-east-1.amazoncognito.com/logout?client_id=32ad6ce6ub7eq69qetg3f151cj&logout_uri=http://localhost:3000/";

// URLs de Cognitocloudfront 
const cognitoLoginUrl = "https://plotter.auth.east-1.amazoncognito.com/login?client_id=32ad6ce6ub7eq69qetg3f151cj&response_type=token&scope=email+openid&redirect_uri=https://d212solchqqpyx.cloudfront.net/";
const cognitoLogoutUrl =
  "https://plotter.auth.us-east-1.amazoncognito.com/logout?client_id=32ad6ce6ub7eq69qetg3f151cj&logout_uri=https://d212solchqqpyx.cloudfront.net/";

// Manejo del token de usuario
let idToken = localStorage.getItem("idToken");

function handleAuth() {
  if (idToken) {
    // Redirige al logout de Cognito
    window.location.href = cognitoLogoutUrl;
  } else {
    // Redirige al login de Cognito
    window.location.href = cognitoLoginUrl;
  }
}

function updateAuthButton() {
  const loginButton = document.getElementById("loginButton");
  if (idToken) {
    loginButton.textContent = "Cerrar Sesión";
  } else {
    loginButton.textContent = "Iniciar Sesión";
  }
}

// Extrae el token después del redireccionamiento de Cognito
const hash = window.location.hash.substring(1);
const params = new URLSearchParams(hash);
const tokenFromUrl = params.get("id_token");

if (tokenFromUrl) {
  localStorage.setItem("idToken", tokenFromUrl); // Guarda el token
  idToken = tokenFromUrl; // Actualiza el estado
  window.location.hash = ""; // Limpia el hash de la URL
}

updateAuthButton(); // Actualiza el texto del botón al cargar la página

// Función para iniciar el dibujo
function startDrawing(event) {
  drawing = true;
  strokeBuffer = []; // Resetea el buffer de trazos para un nuevo trazo
  const [mouseX, mouseY] = getMousePosition(event);
  strokeBuffer.push({ x: mouseX, y: mouseY });
}

// Función para detener el dibujo
async function stopDrawing() {
  if (!drawing) return;
  drawing = false;
  if (strokeBuffer.length > 0) {
    await sendStroke(strokeBuffer);
  }
  strokeBuffer = [];
}

// Función para dibujar en el canvas
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

// Función para obtener la posición del mouse relativa al canvas
function getMousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  return [event.clientX - rect.left, event.clientY - rect.top];
}

// Función para enviar los trazos completados al API Gateway
async function sendStroke(stroke) {
  if (!stroke || stroke.length === 0) return;

  try {
    console.log('Authorization Header:', `Bearer ${idToken}`);
    const response = await fetch(apiGatewayUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${idToken}`, // Token con prefijo Bearer
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

// Agrega los eventos para el canvas
canvas.addEventListener('mousedown', startDrawing);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);
canvas.addEventListener('mousemove', draw);
