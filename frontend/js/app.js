const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/';
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');

let drawing = false;
let strokeBuffer = [];

// URLs de Cognito
const cognitoLoginUrl = "https://plotter.auth.us-east-1.amazoncognito.com/login?client_id=32ad6ce6ub7eq69qetg3f151cj&response_type=token&scope=email+openid&redirect_uri=https://d212solchqqpyx.cloudfront.net/";
const cognitoLogoutUrl = "https://plotter.auth.us-east-1.amazoncognito.com/logout?client_id=32ad6ce6ub7eq69qetg3f151cj&logout_uri=https://d212solchqqpyx.cloudfront.net/";

let idToken = null;

// Modo de desarrollo
const isDevelopment = true;

// Maneja login y logout según el estado del usuario
function handleAuth() {
  if (idToken) {
    if (isDevelopment) {
      console.log("Simulando cierre de sesión en desarrollo");
      idToken = null;
      updateAuthButton();
    } else {
      window.location.href = cognitoLogoutUrl;
    }
  } else {
    if (isDevelopment) {
      console.log("Simulando inicio de sesión en desarrollo");
      idToken = "fake-development-token";
      updateAuthButton();
    } else {
      window.location.href = cognitoLoginUrl;
    }
  }
}

// Actualiza dinámicamente el texto del botón
function updateAuthButton() {
  const loginButton = document.getElementById("loginButton");
  if (idToken) {
    loginButton.textContent = "Log out";
  } else {
    loginButton.textContent = "Log in";
  }
}

// Extrae el token del hash de la URL después del login (solo producción)
if (!isDevelopment) {
  const hash = window.location.hash.substring(1);
  const params = new URLSearchParams(hash);
  const tokenFromUrl = params.get("id_token");

  if (tokenFromUrl) {
    localStorage.setItem("idToken", tokenFromUrl);
    idToken = tokenFromUrl;
    window.location.hash = "";
  } else {
    idToken = localStorage.getItem("idToken");
  }
}

updateAuthButton(); // Actualiza el botón al cargar la página

// Función para iniciar el dibujo
function startDrawing(event) {
  drawing = true;
  strokeBuffer = [];
  const [mouseX, mouseY] = getMousePosition(event);
  strokeBuffer.push({ x: mouseX, y: mouseY });
}

// Función para detener el dibujo
async function stopDrawing() {
  if (!drawing) return;
  drawing = false;
  if (strokeBuffer.length > 0) {
    if (!isDevelopment) {
      await sendStroke(strokeBuffer);
    } else {
      console.log("Simulación de trazo enviado:", strokeBuffer);
    }
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
  try {
    console.log('Authorization Header:', `Bearer ${idToken}`);
    const response = await fetch(apiGatewayUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${idToken}`,
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
