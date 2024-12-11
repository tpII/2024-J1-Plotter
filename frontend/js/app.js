const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/';
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');
const resetButton = document.getElementById('resetButton');
const startButton = document.getElementById('startButton');
const canvasContainer = canvas.parentElement; // Contenedor del canvas
const overlayMessage = document.createElement('div');

let drawing = false;
let strokeBuffer = [];

// URLs de Cognito
const cognitoLoginUrl = "https://plotter.auth.us-east-1.amazoncognito.com/login?client_id=32ad6ce6ub7eq69qetg3f151cj&response_type=token&scope=email+openid&redirect_uri=https://d212solchqqpyx.cloudfront.net/";
const cognitoLogoutUrl = "https://plotter.auth.us-east-1.amazoncognito.com/logout?client_id=32ad6ce6ub7eq69qetg3f151cj&logout_uri=https://d212solchqqpyx.cloudfront.net/";

let idToken = null;

// Rango de normalización
const X_RANGE = [0, 110];
const Y_RANGE = [0, 140];

// Modo de desarrollo
const isDevelopment = false;

// Normaliza las coordenadas al rango especificado
function normalizeCoordinates(x, y) {
  const normalizedX = Math.round((x / canvas.width) * (X_RANGE[1] - X_RANGE[0]) + X_RANGE[0]);
  const normalizedY = Math.round((y / canvas.height) * (Y_RANGE[1] - Y_RANGE[0]) + Y_RANGE[0]);
  return [normalizedX, normalizedY];
}

// Enviar comandos específicos
async function sendCommand(command, data = "") {
  const payload = { command, data };
  try {
    const response = await fetch(apiGatewayUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${idToken}`,
      },
      body: JSON.stringify(payload),
    });

    if (!response.ok) {
      console.error(`Error al enviar comando ${command}:`, response.statusText);
    } else {
      console.log(`Comando ${command} enviado correctamente.`);
    }
  } catch (error) {
    console.error(`Error al enviar comando ${command}:`, error);
  }
}

// Configuración del login/logout
function handleAuth() {
  if (idToken) {
    if (isDevelopment) {
      console.log("Simulando cierre de sesión en desarrollo");
      idToken = null;
      updateAuthButton();
      checkCanvasState();
    } else {
      window.location.href = cognitoLogoutUrl;
    }
  } else {
    if (isDevelopment) {
      console.log("Simulando inicio de sesión en desarrollo");
      idToken = "fake-development-token";
      updateAuthButton();
      checkCanvasState();
    } else {
      window.location.href = cognitoLoginUrl;
    }
  }
}

function updateAuthButton() {
  const loginButton = document.getElementById("loginButton");
  loginButton.textContent = idToken ? "Log out" : "Log in";
}

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

updateAuthButton();
setupCanvasOverlay();
checkCanvasState();

// Limpia el canvas y envía STOP
resetButton.addEventListener('click', () => {
  if (!idToken) {
    alert("Debes iniciar sesión para borrar el canvas.");
    return;
  }
  clearCanvas();
  sendCommand("STOP");
});

// Inicia el dibujo al presionar Start
startButton.addEventListener('click', () => {
  if (!idToken) {
    alert("Debes iniciar sesión para comenzar a dibujar.");
    return;
  }
  sendCommand("START");
});

// Iniciar el dibujo
function startDrawing(event) {
  if (!idToken) return;
  drawing = true;
  strokeBuffer = [];
  const [mouseX, mouseY] = getMousePosition(event);
  strokeBuffer.push(normalizeCoordinates(mouseX, mouseY));
}

// Detener el dibujo y enviar STROKE
async function stopDrawing() {
  if (!drawing) return;
  drawing = false;

  if (strokeBuffer.length > 0) {
    const data = {
      x: strokeBuffer.map(coord => coord[0]),
      y: strokeBuffer.map(coord => coord[1]),
    };
    await sendCommand("STROKE", data);
  }
  strokeBuffer = [];
}

// Dibujar en el canvas
function draw(event) {
  if (!drawing) return;

  const [mouseX, mouseY] = getMousePosition(event);
  const [normalizedX, normalizedY] = normalizeCoordinates(mouseX, mouseY);

  ctx.beginPath();
  const lastPoint = strokeBuffer[strokeBuffer.length - 1];
  ctx.moveTo(lastPoint[0], lastPoint[1]);
  ctx.lineTo(normalizedX, normalizedY);
  ctx.stroke();

  strokeBuffer.push([normalizedX, normalizedY]);
}

// Obtener la posición del mouse
function getMousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  return [
    event.clientX - rect.left,
    event.clientY - rect.top,
  ];
}

// Limpia el contenido del canvas
function clearCanvas() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
}

// Overlay para el estado de autenticación
function setupCanvasOverlay() {
  overlayMessage.innerHTML = `
    <div style="text-align: center;">
      <span>Antes de dibujar debes </span>
      <a href="${cognitoLoginUrl}" class="underline text-blue-500">iniciar sesión</a>
    </div>`;
  overlayMessage.style.position = 'absolute';
  overlayMessage.style.top = '50%';
  overlayMessage.style.left = '50%';
  overlayMessage.style.transform = 'translate(-50%, -50%)';
  overlayMessage.style.background = 'rgba(0, 0, 0, 0.7)';
  overlayMessage.style.color = 'white';
  overlayMessage.style.fontSize = '1.5rem';
  overlayMessage.style.fontWeight = 'bold';
  overlayMessage.style.padding = '1rem 2rem';
  overlayMessage.style.borderRadius = '8px';
  overlayMessage.style.display = 'none';
  overlayMessage.style.zIndex = '10';

  canvasContainer.style.position = 'relative';
  canvasContainer.appendChild(overlayMessage);
}

function checkCanvasState() {
  if (!idToken) {
    overlayMessage.style.display = 'flex';
    canvas.style.pointerEvents = 'none';
    canvas.style.opacity = '0.5';
  } else {
    overlayMessage.style.display = 'none';
    canvas.style.pointerEvents = 'auto';
    canvas.style.opacity = '1';
  }
}

// Eventos del canvas
canvas.addEventListener('mousedown', startDrawing);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);
canvas.addEventListener('mousemove', draw);
