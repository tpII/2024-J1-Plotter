// Función adaptada para obtener posición del mouse o del toque
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

// Iniciar el dibujo (para mouse y touch)
function startDrawing(event) {
  if (!idToken) return;
  drawing = true;
  strokeBuffer = [];
  const [x, y] = getPosition(event);
  strokeBuffer.push([x, y]);
}

// Dibujar en el canvas (para mouse y touch)
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

// Detener el dibujo y enviar STROKE (para mouse y touch)
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
