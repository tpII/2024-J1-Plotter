output "frontend_eip" {
  value = aws_eip.frontend_eip.public_ip
}

output "backend_eip" {
  value = aws_eip.backend_eip.public_ip
}
