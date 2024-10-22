from flask import Flask, jsonify

app = Flask(__name__)

# Ruta principal (home) que devuelve un mensaje de saludo
@app.route('/')
def home():
    return jsonify({"message": "Hello, World! This is your Flask API"}), 200

# Iniciar la aplicación
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
