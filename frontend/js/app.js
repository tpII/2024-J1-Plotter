const apiGatewayUrl = 'https://uqhtsxbehl.execute-api.us-east-1.amazonaws.com/';
const canvas = document.getElementById('drawCanvas');
const ctx = canvas.getContext('2d');
const resetButton = document.getElementById('resetButton');
const startButton = document.getElementById('startButton');
const canvasContainer = canvas.parentElement; // Contenedor del canvas
const overlayMessage = document.createElement('div');

let drawing = false;
let strokeBuffer = [];

// Rango de normalización
const X_RANGE = [0, 110];
const Y_RANGE = [0, 140];

// Modo de desarrollo
const isDevelopment = window.location.hostname === "localhost";

// URLs de Cognito
const cognitoLoginUrl = isDevelopment
  ? "https://plotter.auth.us-east-1.amazoncognito.com/login?client_id=32ad6ce6ub7eq69qetg3f151cj&response_type=token&scope=email+openid&redirect_uri=http://localhost:5500/frontend"
  : "https://plotter.auth.us-east-1.amazoncognito.com/login?client_id=32ad6ce6ub7eq69qetg3f151cj&response_type=token&scope=email+openid&redirect_uri=https://d212solchqqpyx.cloudfront.net/";

const cognitoLogoutUrl = isDevelopment
  ? "https://plotter.auth.us-east-1.amazoncognito.com/logout?client_id=32ad6ce6ub7eq69qetg3f151cj&logout_uri=http://localhost:5500/frontend"
  : "https://plotter.auth.us-east-1.amazoncognito.com/logout?client_id=32ad6ce6ub7eq69qetg3f151cj&logout_uri=https://d212solchqqpyx.cloudfront.net/";

let idToken = null;

// Normaliza las coordenadas al rango especificado
function normalizeCoordinates(x, y) {
  const normalizedX = x / canvas.width;   // Normaliza X entre 0 y 1
  const normalizedY = y / canvas.height; // Normaliza Y entre 0 y 1

  return [normalizedX, normalizedY];
}

// Enviar comandos específicos
async function sendCommand(command, data = "") {
  if (isDevelopment) {
    console.log(`Comando ${command} (modo local):`, JSON.stringify({ command, data }));
    return; // Evita enviar la solicitud en modo local
  }

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
  const [x, y] = getPosition(event);
  strokeBuffer.push([x, y]);
}


// Detener el dibujo y enviar STROKE
async function stopDrawing(event) {
  if (!drawing) return;
  drawing = false;

  if (strokeBuffer.length > 0) {
    const normalizedData = strokeBuffer.map(coord => normalizeCoordinates(coord[0], coord[1]));
    const data = {
      x: normalizedData.map(coord => coord[0]),
      y: normalizedData.map(coord => coord[1]),
    };
    await sendCommand("STROKE", data);
    strokeBuffer = [];
  }
}




// Dibujar en el canvas
function draw(event) {
  if (!drawing) return;
  event.preventDefault(); // Evita el scroll en dispositivos móviles

  const [x, y] = getPosition(event);

  ctx.beginPath();
  const lastPoint = strokeBuffer[strokeBuffer.length - 1];
  ctx.moveTo(lastPoint[0], lastPoint[1]); // Dibuja desde la última posición
  ctx.lineTo(x, y);                      // Dibuja a la posición actual
  ctx.stroke();

  strokeBuffer.push([x, y]);
}



// Obtener la posición del mouse
function getPosition(event) {
  const rect = canvas.getBoundingClientRect();
  const scaleX = canvas.width / rect.width;
  const scaleY = canvas.height / rect.height;

  let clientX, clientY;

  if (event.touches) {
    // Si es un evento táctil, usa la posición del primer toque
    clientX = event.touches[0].clientX;
    clientY = event.touches[0].clientY;
  } else {
    // Si es un evento de mouse
    clientX = event.clientX;
    clientY = event.clientY;
  }

  return [
    Math.round((clientX - rect.left) * scaleX),
    Math.round((clientY - rect.top) * scaleY),
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
// Eventos del canvas: mouse
canvas.addEventListener('mousedown', startDrawing);
canvas.addEventListener('mousemove', draw);
canvas.addEventListener('mouseup', stopDrawing);
canvas.addEventListener('mouseout', stopDrawing);

// Eventos del canvas: touch
canvas.addEventListener('touchstart', startDrawing);
canvas.addEventListener('touchmove', draw);
canvas.addEventListener('touchend', stopDrawing);
canvas.addEventListener('touchcancel', stopDrawing);
setupGamepad(); // Inicializa la funcionalidad del joystick


// Función para enviar el comando PING
function sendPing() {
  sendCommand("PING", "");
  console.log("Comando PING enviado");
}

// Ejecutar la función PING cada 5 segundos
setInterval(() => {
  if (idToken) { // Solo enviar PING si el usuario está autenticado
    // sendPing();
  }
}, 5000); // 5000 ms = 5 segundos

let joystickMode = false; // Variable para indicar si está activado el modo joystick

// Configurar la funcionalidad del joystick
function setupGamepad() {
  window.addEventListener("gamepadconnected", (e) => {
    console.log(`Gamepad conectado: ${e.gamepad.id}`);
    pollGamepad();
  });

  window.addEventListener("gamepaddisconnected", () => {
    console.log("Gamepad desconectado");
  });

  function pollGamepad() {
    const gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
    const gp = gamepads[0]; // Selecciona el primer gamepad conectado
    if (gp) {
      handleGamepadInput(gp);
    }
    requestAnimationFrame(pollGamepad);
  }
}

// Manejar las entradas del joystick
let buttonAPressed = false; // Estado del botón A (B0)
let buttonBPressed = false; // Estado del botón B (B1)

function handleGamepadInput(gp) {
  // Botón B9 (Menu) - Cambiar al modo joystick
  if (gp.buttons[9].pressed && !joystickMode) {
    joystickMode = true;
    sendCommand("MODESWITCH", ""); // Activar modo joystick
    console.log("Modo joystick activado");
  }

  // Si está en modo joystick
  if (joystickMode) {
    // Botón A (B0) - Enviar VERTDOWN solo una vez al presionar
    if (gp.buttons[0].pressed && !buttonAPressed) {
      sendCommand("VERTDOWN", "");
      console.log("VERTDOWN");
      buttonAPressed = true;
    } else if (!gp.buttons[0].pressed) {
      buttonAPressed = false; // Reinicia el estado
    }

    // Botón B (B1) - Enviar VERTUP solo una vez al presionar
    if (gp.buttons[1].pressed && !buttonBPressed) {
      sendCommand("VERTUP", "");
      console.log("VERTUP");
      buttonBPressed = true;
    } else if (!gp.buttons[1].pressed) {
      buttonBPressed = false; // Reinicia el estado
    }

    // Joystick principal (stick izquierdo) - Enviar MOVE
    const axisX = gp.axes[0]; // Eje X
    const axisY = gp.axes[1]; // Eje Y

    // Normalizar entre 0 y 1
    const dx = ((axisX + 1) / 2).toFixed(2); // Eje X (0 a 1)
    const dy = ((axisY + 1) / 2).toFixed(2); // Eje Y (0 a 1)

    // Solo enviar si hay movimiento significativo
    if (Math.abs(axisX) > 0.05 || Math.abs(axisY) > 0.05) {
      sendCommand("MOVE", { dx, dy });
      console.log(`MOVE: dx=${dx}, dy=${dy}`);
    }
  }
}

