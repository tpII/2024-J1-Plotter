import json
import boto3
import os
from botocore.exceptions import ClientError

# Inicializa el cliente de AWS IoT y DynamoDB
iot_client = boto3.client('iot-data', endpoint_url=os.environ['IOT_ENDPOINT'])
dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table(os.environ['DYNAMODB_TABLE'])

# Nombre del tema de MQTT en AWS IoT al que se suscribe el ESP32
MQTT_TOPIC = "drawing_topic"

def lambda_handler(event, context):
    # Extrae las coordenadas x e y del evento
    x = event.get("x")
    y = event.get("y")

    if x is None or y is None:
        return {
            'statusCode': 400,
            'body': json.dumps('Invalid input: Missing x or y coordinates')
        }
    
    # Construye el mensaje para el ESP32
    message = {
        "x": x,
        "y": y
    }

    try:
        # Publica el mensaje en AWS IoT Core
        response = iot_client.publish(
            topic=MQTT_TOPIC,
            qos=0,
            payload=json.dumps(message)
        )
        
        return {
            'statusCode': 200,
            'body': json.dumps('Message sent to IoT Core')
        }
        
    except ClientError as e:
        print("Error publishing to IoT Core:", e)
        
        # Si falla la publicación, guarda el mensaje en DynamoDB
        try:
            table.put_item(
                Item={
                    'drawing_id': str(int(context.aws_request_id, 16)),
                    'x': x,
                    'y': y
                }
            )
            return {
                'statusCode': 500,
                'body': json.dumps('Failed to send to IoT Core. Saved in DynamoDB')
            }
        except ClientError as db_error:
            print("Error saving to DynamoDB:", db_error)
            return {
                'statusCode': 500,
                'body': json.dumps('Failed to send to IoT Core and DynamoDB')
            }
