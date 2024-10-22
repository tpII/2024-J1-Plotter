# Usa una imagen base de Python
FROM python:3.9-slim

# Establece el directorio de trabajo dentro del contenedor
WORKDIR /app

# Copia el archivo de dependencias y luego instálalas
COPY requirements.txt requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Copia el código de la aplicación dentro del contenedor
COPY . .

# Expone el puerto 5000 para Flask
EXPOSE 5000

CMD ["python", "app.py"]
