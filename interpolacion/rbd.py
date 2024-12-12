import numpy as np
import matplotlib.pyplot as plt

def ramer_douglas_peucker(points, epsilon):
    """
    Simplifica una curva usando el algoritmo Ramer-Douglas-Peucker.

    Args:
        points (list): Lista de puntos [(x, y), ...]
        epsilon (float): Umbral de simplificación.

    Returns:
        list: Lista de puntos simplificados.
    """
    if len(points) < 3:
        return points

    # Función para calcular la distancia perpendicular de un punto a una línea
    def perpendicular_distance(point, start, end):
        if start == end:
            return np.linalg.norm(np.array(point) - np.array(start))
        else:
            return abs(np.cross(np.array(end) - np.array(start), np.array(start) - np.array(point))) / np.linalg.norm(np.array(end) - np.array(start))

    # Encuentra el punto más alejado de la línea
    start, end = points[0], points[-1]
    distances = [perpendicular_distance(point, start, end) for point in points]
    max_distance = max(distances)
    index = distances.index(max_distance)

    if max_distance > epsilon:
        # Divide y conquista
        left = ramer_douglas_peucker(points[:index+1], epsilon)
        right = ramer_douglas_peucker(points[index:], epsilon)

        return left[:-1] + right
    else:
        return [start, end]

# Generar una curva sinusoidal como ejemplo
x = np.linspace(0, 2 * np.pi, 1000)  # 1000 puntos de la curva
y = np.sin(x)                       # Función sinusoidal

# Crear lista de puntos [(x1, y1), (x2, y2), ...]
curve_points = list(zip(x, y))

# Aplicar el algoritmo Ramer-Douglas-Peucker
epsilon = 0.1  # Tolerancia (ajústala para mayor o menor simplificación)
simplified_points = ramer_douglas_peucker(curve_points, epsilon)

# Separar los puntos simplificados en listas X e Y para graficar
simplified_x = [p[0] for p in simplified_points]
simplified_y = [p[1] for p in simplified_points]

# Graficar la curva original y la simplificada
plt.figure(figsize=(10, 6))
plt.plot(x, y, label="Curva Original (1000 puntos)", color="blue", linewidth=1)
plt.plot(simplified_x, simplified_y, label=f"Curva Simplificada ({len(simplified_points)} puntos)", color="red", linestyle="--", linewidth=2)
plt.scatter(simplified_x, simplified_y, color="red", s=10, label="Puntos Simplificados")

# Configuraciones del gráfico
plt.title("Simplificación de Curva con Ramer-Douglas-Peucker")
plt.xlabel("X")
plt.ylabel("Y")
plt.legend()
plt.grid()
plt.show()
