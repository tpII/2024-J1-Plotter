# Proyecto Controlador de Plotter 2D

## Descripción General

Este proyecto es una aplicación web que permite controlar un plotter 2D, similar al robot Line-us. El sistema permite que un usuario dibuje en un **canva** dentro de una aplicación web, y este dibujo se replique en tiempo real utilizando un dispositivo físico (plotter) controlado por un ESP32. El flujo de trabajo es el siguiente:

1. El usuario dibuja en el **canva** de la aplicación web.
2. La interfaz web envía las coordenadas del dibujo a un servidor backend a través de una API REST.
3. El backend procesa los datos y los envía al plotter utilizando un protocolo IoT.
4. El ESP32, que ejecuta un sistema operativo en tiempo real (RTOS), transforma las coordenadas en movimientos físicos del plotter, replicando el dibujo.

## Estructura del Repositorio

El proyecto utiliza un **monorepo** para gestionar los diferentes componentes. Las siguientes carpetas están presentes en el repositorio:

- **frontend/**: Contiene la aplicación web (HTML, CSS, JS) que se comunica con el backend.
- **backend/**: Contiene la API REST desarrollada con Flask (Python), que recibe los datos del frontend y los envía al dispositivo IoT.
- **rtos/**: Contiene el código en C que se ejecuta en el ESP32 para controlar los movimientos del plotter.

## Workflow de Git

El equipo utiliza un workflow basado en **trunk based development**:

- La rama principal es **main**, donde se integran los cambios finales.
- Cada desarrollador trabaja en su propia rama (**guido**, **gian**, **nico**, **alex**), y luego fusiona sus cambios en **main** tras una revisión.
- Los cambios se prueban y verifican mediante pipelines automáticos configurados con GitHub Actions.

## Contenedores Docker

Cada componente del proyecto utiliza un Dockerfile para facilitar el desarrollo y despliegue.

- **frontend**: Dockerfile que permite desarrollar y construir la imagen de la aplicación web.
- **backend**: Dockerfile que permite desarrollar y construir la imagen de la API Flask.
- **rtos**: Dockerfile que simula el entorno de desarrollo del ESP32 para compilar el código, pero no construye una imagen. Los **artifacts** de compilación se suben a GitHub como **artifacts**.

## Docker Compose

El proyecto incluye un archivo **docker-compose.yml** que permite ejecutar y probar el frontend y el backend juntos en un entorno local para asegurar la correcta comunicación entre ellos.

## Pipelines de CI/CD

Se han configurado pipelines en GitHub Actions para automatizar el proceso de integración y despliegue. Cada componente tiene su propio pipeline:

- **Frontend**: Compila, prueba y sube la imagen Docker a DockerHub.
- **Backend**: Compila, prueba y sube la imagen Docker a DockerHub.
- **RTOS**: Compila el código para el ESP32 y sube los **artifacts** compilados a GitHub.

## Requisitos y Credenciales

### DockerHub
Se requieren credenciales para subir las imágenes Docker al repositorio de DockerHub.

### GitHub Artifacts
Se necesitan credenciales para subir los **artifacts** compilados del ESP32 al repositorio de **artifacts** de GitHub.

---

## Cómo Empezar

### Clonar el Repositorio

```bash
git clone https://github.com/tu-repositorio/proyecto-plotter-2d.git
cd proyecto-plotter-2d
```

### Ejecutar con Docker Compose

1. Asegúrate de tener Docker y Docker Compose instalados en tu máquina.
2. Ejecuta el siguiente comando para iniciar los servicios frontend y backend:

```bash
docker-compose up --build
```

El frontend estará disponible en `http://localhost:3000` y el backend en `http://localhost:5000`.

### Contribuir

1. Crea una rama nueva para tu desarrollo:

```bash
git checkout -b mi-nueva-funcionalidad
```

2. Haz tus cambios y súbelos:

```bash
git add .
git commit -m "Añadir nueva funcionalidad"
git push origin mi-nueva-funcionalidad
```

3. Crea un **Pull Request** para fusionar tus cambios en la rama principal.

---

## Licencia

Este proyecto está bajo la licencia [MIT](LICENSE).