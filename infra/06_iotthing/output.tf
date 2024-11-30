output "iot_endpoint" {
  description = "The IoT Core endpoint for MQTT communication"
  value       = data.aws_iot_endpoint.endpoint.endpoint_address
}

output "robot_certificate_arn" {
  description = "ARN of the robot's IoT certificate"
  value       = aws_iot_certificate.robot_cert.arn
}

output "robot_private_key" {
  description = "Private key for the robot's IoT certificate"
  value       = aws_iot_certificate.robot_cert.private_key
  sensitive   = true
}

output "robot_certificate_pem" {
  description = "Certificate PEM for the robot"
  value       = aws_iot_certificate.robot_cert.certificate_pem
  sensitive   = true
}

output "robot_thing_name" {
  description = "Name of the IoT Thing for the robot"
  value       = aws_iot_thing.robot_thing.name
}
