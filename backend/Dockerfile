# Usa la imagen base de Lambda para Python 3.8
FROM public.ecr.aws/lambda/python:3.8

# Copia el código de la función y las dependencias
COPY lambda_function.py /var/task/
COPY requirements.txt /var/task/

# Instala las dependencias desde requirements.txt
RUN pip install -r /var/task/requirements.txt

# Define el punto de entrada para Lambda (handler)
CMD ["lambda_function.lambda_handler"]
