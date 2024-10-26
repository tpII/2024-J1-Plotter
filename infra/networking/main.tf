resource "aws_security_group" "app_sg" {
  name   = var.security_group_name
  vpc_id = aws_vpc.main.id

  ingress {
    from_port   = 80
    to_port     = 80
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port = 22
    to_port = 22 
    protocol = "tcp"
  }

  ingress {
    from_port   = 5000
    to_port     = 5000
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }
}

resource "aws_eip" "frontend_eip" {
  vpc = true
  instance = aws_instance.frontend.id
}

resource "aws_eip" "backend_eip" {
  vpc = true
  instance = aws_instance.backend.id
}

resource "aws_vpc" "main" {
  cidr_block = "10.0.0.0/16"
}

output "frontend_ip" {
  value = aws_eip.frontend_eip.public_ip
}

output "backend_ip" {
  value = aws_eip.backend_eip.public_ip
}
