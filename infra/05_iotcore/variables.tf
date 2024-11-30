variable "region" {
  description = "AWS Region"
  type        = string
  default     = "us-east-1"
}

variable "thing_name" {
  description = "Name of the IoT Thing"
  type        = string
  default     = "robot-drawing-device"
}

variable "policy_name" {
  description = "Name of the IoT policy"
  type        = string
  default     = "robot-policy"
}

variable "log_retention_in_days" {
  description = "Number of days to retain IoT logs in CloudWatch"
  type        = number
  default     = 7
}
