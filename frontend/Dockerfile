# Etapa de construcción (opcional si es necesario un build de JS/CSS)
FROM node:16-alpine AS builder
WORKDIR /app
COPY . /app

# Si tuviera que compilar JS o CSS 
# RUN npm install && npm run build

# Etapa de producción
FROM nginx:alpine
COPY --from=builder /app /usr/share/nginx/html

# Exponer el puerto 80 para nginx
EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
