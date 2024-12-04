const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/';
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');
const canvasContainer = canvas.parentElement; // Contenedor del canvas
const overlayMessage = document.createElement('div'); // Overlay message
const resetButton = document.getElementById('resetButton');

let drawing = false;
let strokeBuffer = [];

// URLs de Cognito
const cognitoLoginUrl = "https://plotter.auth.us-east-1.amazoncognito.com/login?client_id=32ad6ce6ub7eq69qetg3f151cj&response_type=token&scope=email+openid&redirect_uri=https://d212solchqqpyx.cloudfront.net/";
const cognitoLogoutUrl = "https://plotter.auth.us-east-1.amazoncognito.com/logout?client_id=32ad6ce6ub7eq69qetg3f151cj&logout_uri=https://d212solchqqpyx.cloudfront.net/";

let idToken = null;

// Modo de desarrollo
const isDevelopment = false;

// Maneja login y logout según el estado del usuario
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

// Revisa el estado del canvas según la autenticación
function checkCanvasState() {
  if (!idToken) {
    overlayMessage.style.display = 'flex';
    canvasContainer.style.opacity = '0.5'; // Ventana transparente
    canvas.style.pointerEvents = 'none';
    canvas.style.opacity = '0.5';
  } else {
    overlayMessage.style.display = 'none';
    canvasContainer.style.opacity = '1'; // Ventana sin transparencia
    canvas.style.pointerEvents = 'auto';
    canvas.style.opacity = '1';
  }
}

// Inicializa el mensaje de overlay sobre el canvas
// Inicializa el mensaje de overlay sobre el canvas
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
  overlayMessage.style.lineHeight = '2rem'; // Añade espacio entre líneas para mejor diseño

  const canvasContainer = canvas.parentElement;
  canvasContainer.style.position = 'relative';
  canvasContainer.appendChild(overlayMessage);

  checkCanvasState();
}


// Limpia el contenido del canvas
function clearCanvas() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
}

// Maneja el clic en el botón "Erase"
resetButton.addEventListener('click', () => {
  if (!idToken) {
    alert("Debes iniciar sesión para borrar el canvas.");
    return;
  }
  clearCanvas();
});

// Función para iniciar el dibujo
function startDrawing(event) {
  if (!idToken) return; // Desactiva si no hay sesión
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

// Función para obtener la posición del mouse relativa al canvas (ajustado para offset y scroll)
function getMousePosition(event) {
  const rect = canvas.getBoundingClientRect();
  const scrollLeft = window.scrollX || document.documentElement.scrollLeft;
  const scrollTop = window.scrollY || document.documentElement.scrollTop;
  return [event.clientX - rect.left - scrollLeft, event.clientY - rect.top - scrollTop];
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

// Configuración inicial
setupCanvasOverlay();
