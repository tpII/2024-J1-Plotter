# Use the AWS Lambda Python 3.9 runtime as the base image
FROM public.ecr.aws/lambda/python:3.9

# Copy the requirements file
COPY requirements.txt .

# Install dependencies
RUN pip install -r requirements.txt

# Copy the Lambda function code into the container
COPY lambda_function.py ./

# Set the CMD to your Lambda function handler
CMD ["lambda_function.lambda_handler"]
