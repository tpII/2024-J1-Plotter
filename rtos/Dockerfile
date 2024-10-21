# Dockerfile for RTOS (ESP32 Development)

# Usamos la imagen oficial del Espressif IDF para simular el entorno de desarrollo del ESP32
FROM espressif/idf:latest

# Establecemos el directorio de trabajo
WORKDIR /project

# Copiamos el código fuente al contenedor
COPY . /project

# Comando para compilar el proyecto
CMD ["bash", "-c", "idf.py build"]
