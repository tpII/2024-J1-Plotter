# Dockerfile for Frontend

# Etapa de construcción (opcional si es necesario un build de JS/CSS)
FROM node:16-alpine AS builder
WORKDIR /app
COPY . /app

# Instala dependencias si es una aplicación con algún build step
# (opcional, si usas frameworks como React, Vue, etc)
# RUN npm install && npm run build

# Etapa de producción
FROM nginx:alpine
COPY --from=builder /app /usr/share/nginx/html

# Copiar el archivo de configuración de nginx (si es necesario)
# COPY nginx.conf /etc/nginx/nginx.conf

EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
