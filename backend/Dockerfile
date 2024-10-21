# Dockerfile for Backend (Flask API)

# Usa una imagen base con Python
FROM python:3.9-slim

# Establece el directorio de trabajo
WORKDIR /app

# Copia los archivos de requerimientos y luego los instala
COPY requirements.txt requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Copia el código de la aplicación
COPY . .

# Expone el puerto 5000 para la API Flask
EXPOSE 5000

# Comando para ejecutar la API
CMD ["python", "app.py"]
