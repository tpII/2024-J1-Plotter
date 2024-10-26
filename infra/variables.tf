variable "aws_region" {
  description = "The AWS region to deploy resources"
  type        = string
  default     = "us-east-1"
}

variable "instance_type" {
  description = "EC2 instance type"
  type        = string
  default     = "t2.micro"
}

variable "frontend_docker_image" {
  description = "DockerHub image for the frontend"
  type        = string
  default     = "gianfrancolasala/plotter-frontend:latest"
}

variable "backend_docker_image" {
  description = "DockerHub image for the backend"
  type        = string
  default     = "gianfrancolasala/plotter-backend:latest"
}

variable "key_pair_name" {
  description = "Name of the AWS key pair to access EC2 instances"
  type        = string
}

variable "security_group_name" {
  description = "Security group name for the app"
  type        = string
  default     = "fullstack-app-sg"
}
