# README - Plotter 2D

## Descripción del Proyecto

**Plotter 2D** es una aplicación web full-stack diseñada para trazar gráficos en 2D de manera simple y eficiente. La aplicación está compuesta por dos componentes principales: un **frontend** que utiliza **nginx** para servir la interfaz de usuario, y un **backend** desarrollado con **Flask** (Python) que maneja la lógica de los gráficos y los datos.

El proyecto implementa una infraestructura en **AWS** para ejecutar ambos componentes dentro de contenedores **Docker**, los cuales son construidos y almacenados en **DockerHub**. La infraestructura está gestionada mediante **Terraform**, asegurando un despliegue escalable, legible y de bajo costo (aprovechando los servicios Free Tier de AWS).

## Arquitectura del Proyecto

1. **Frontend (nginx)**: Servido en una instancia EC2 de AWS usando una imagen Docker. El frontend expone la aplicación en el puerto 80 para recibir tráfico HTTP.

2. **Backend (Flask)**: También desplegado en una instancia EC2 con Docker, el backend maneja las solicitudes de la API en el puerto 5000.

3. **Red y Seguridad**: Se configuran grupos de seguridad para permitir tráfico HTTP (puerto 80) y tráfico para la API (puerto 5000). Las instancias EC2 están asociadas con direcciones IP elásticas (Elastic IP) para hacerlas accesibles desde cualquier lugar.

4. **Gestión de Dominio (Opcional)**: Si se utiliza un dominio personalizado, se puede gestionar mediante **AWS Route 53** para apuntar los registros DNS a las direcciones IP de las instancias.

## Requisitos Previos

- Cuenta de **AWS** con acceso a **Free Tier**.
- Claves de acceso AWS configuradas en tu entorno local.
- Par de claves de SSH (key pair) para acceder a las instancias EC2.
- Cuenta en **DockerHub** para almacenar las imágenes Docker.
- **Terraform** instalado en tu máquina local.

## Estructura del Proyecto

El proyecto está organizado en directorios para mantener la configuración clara y escalable:

```
terraform-aws-fullstack-app/
│
├── backend/
│   ├── main.tf           # Configuración de EC2 para el backend
│   ├── variables.tf      # Variables específicas del backend
│   └── outputs.tf        # Salidas (outputs) del backend
│
├── frontend/
│   ├── main.tf           # Configuración de EC2 para el frontend
│   ├── variables.tf      # Variables específicas del frontend
│   └── outputs.tf        # Salidas (outputs) del frontend
│
├── networking/
│   ├── main.tf           # Configuración de la red y seguridad
│   ├── variables.tf      # Variables de red
│   └── outputs.tf        # Salidas (outputs) de la red
│
├── provider.tf           # Configuración del proveedor AWS
├── terraform.tfvars      # Valores de las variables para Terraform
├── variables.tf          # Variables compartidas
└── README.md             # Este archivo
```

## Pasos para Desplegar

### 1. Clonar el Repositorio

Primero, clona el repositorio en tu máquina local:

```bash
git clone https://github.com/tu-usuario/plotter-2d.git
cd plotter-2d
```

### 2. Configurar las Variables

Actualiza el archivo `terraform.tfvars` con tus propios valores, como el nombre de la imagen Docker de **DockerHub**, el nombre del par de claves de AWS y la región donde quieres desplegar los recursos.

### 3. Inicializar y Aplicar Terraform

Ejecuta los siguientes comandos para desplegar la infraestructura en AWS:

```bash
terraform init
terraform apply
```

Confirma la aplicación de los cambios cuando se te solicite. Terraform comenzará a crear las instancias EC2, grupos de seguridad y asignar IPs elásticas.

### 4. Acceder a la Aplicación

Una vez que el despliegue haya terminado, podrás ver las direcciones IP de las instancias EC2 en las salidas de Terraform. El frontend estará disponible en el puerto 80 y el backend en el puerto 5000.



## Consideraciones de Costos

El proyecto está diseñado para utilizar servicios dentro del **Free Tier** de AWS, específicamente las instancias EC2 tipo **t2.micro**. Sin embargo, ten en cuenta que algunos costos pueden aplicarse si superas los límites del Free Tier o si utilizas servicios opcionales como **Elastic IP** o **Route 53**.
