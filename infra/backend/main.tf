resource "aws_instance" "backend" {
  ami                    = "ami-0dc2d3e4c0f9ebd18"  # Amazon Linux 2
  instance_type          = var.instance_type
  key_name               = var.key_pair_name
  vpc_security_group_ids = [aws_security_group.app_sg.id]
  associate_public_ip_address = true

  user_data = <<-EOF
    #!/bin/bash
    sudo yum update -y
    sudo yum install docker -y
    sudo service docker start
    sudo usermod -a -G docker ec2-user
    sudo docker run -d -p 5000:5000 ${var.backend_docker_image}
  EOF

  tags = {
    Name = "backend-instance"
  }
}
