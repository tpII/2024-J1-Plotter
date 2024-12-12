import boto3
import json
import os
from shapely.geometry import LineString

# Create IoT Data client
iot_client = boto3.client('iot-data', region_name='us-east-1')

# Environment variable for the MQTT topic
MQTT_TOPIC = os.environ.get('MQTT_TOPIC', 'robot/draw')

# Función para truncar valores a 2 decimales
def truncate(value, decimals=2):
    return round(value, decimals)

# Simplifica los puntos usando el algoritmo Ramer-Douglas-Peucker
def simplify_stroke(x_list, y_list, tolerance=0.01):
    points = [(x, y) for x, y in zip(x_list, y_list)]  # Combina x e y en puntos
    line = LineString(points)                         # Crea una línea a partir de los puntos
    simplified_line = line.simplify(tolerance, preserve_topology=False)  # Simplifica la línea
    
    # Separa los puntos simplificados en listas de x e y
    simplified_x = [truncate(coord[0]) for coord in simplified_line.coords]
    simplified_y = [truncate(coord[1]) for coord in simplified_line.coords]
    return simplified_x, simplified_y

def lambda_handler(event, context):
    try:
        # Parse the request body
        if isinstance(event['body'], str):
            payload = json.loads(event['body'])
        else:
            payload = event['body']

        # Validate the payload structure
        if 'command' not in payload:
            raise ValueError("Missing 'command' in payload")

        command = payload['command']
        data = payload.get('data', {})

        # Procesar solo si el comando es "STROKE"
        if command == "STROKE" and 'x' in data and 'y' in data:
            # Truncar las coordenadas originales
            x_list = [truncate(val) for val in data['x']]
            y_list = [truncate(val) for val in data['y']]

            # Simplificar las coordenadas usando RDP
            simplified_x, simplified_y = simplify_stroke(x_list, y_list, tolerance=0.01)

            # Reemplazar data con las nuevas coordenadas simplificadas
            data['x'] = simplified_x
            data['y'] = simplified_y

        # Preparar el mensaje para MQTT
        mqtt_message = {
            "command": command,
            "data": data
        }

        # Publicar el mensaje al topic MQTT
        iot_client.publish(
            topic=MQTT_TOPIC,
            qos=1,
            payload=json.dumps(mqtt_message)
        )

        # Respuesta exitosa
        return {
            'statusCode': 200,
            'body': json.dumps({
                'message': f"Command '{command}' published successfully!",
                'data': data
            })
        }

    except ValueError as ve:
        # Manejar errores de validación
        print(f"Validation Error: {str(ve)}")
        return {
            'statusCode': 400,
            'body': json.dumps({'error': str(ve)})
        }
    except Exception as e:
        # Manejar otros errores
        print(f"Error: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
