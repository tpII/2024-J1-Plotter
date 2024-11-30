variable "function_name" {
  description = "Nombre de la función Lambda"
  type        = string
}

variable "ecr_repository_url" {
  description = "URL del repositorio ECR que contiene la imagen Docker"
  type        = string
}

variable "image_tag" {
  description = "Tag de la imagen Docker que usará la función Lambda"
  type        = string
}

variable "memory_size" {
  description = "Cantidad de memoria asignada a la función Lambda"
  type        = number
  default     = 256
}

variable "timeout" {
  description = "Timeout para la función Lambda en segundos"
  type        = number
  default     = 15
}


variable "tags" {
  description = "Etiquetas para aplicar a la función Lambda"
  type        = map(string)
  default     = {
    Environment = "Production"
    Project     = "UNLP Plotter"
  }
}

variable "environment_variables" {
  description = "Environment variables for the Lambda function"
  type        = map(string)
  default     = {
    MQTT_TOPIC = "robot/draw"
  }
}
