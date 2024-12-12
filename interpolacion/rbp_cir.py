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

    def perpendicular_distance(point, start, end):
        """Distancia perpendicular de un punto a una línea."""
        if start == end:
            return np.linalg.norm(np.array(point) - np.array(start))
        return abs(np.cross(np.array(end) - np.array(start), np.array(start) - np.array(point))) / np.linalg.norm(np.array(end) - np.array(start))

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

# Generar un círculo como curva cerrada
theta = np.linspace(0, 2 * np.pi, 1000)  # 1000 puntos en la circunferencia
r = 1  # Radio del círculo
x = r * np.cos(theta)  # Coordenadas X
y = r * np.sin(theta)  # Coordenadas Y

# Crear lista de puntos [(x1, y1), (x2, y2), ...]
circle_points = list(zip(x, y))

# Aplicar el algoritmo Ramer-Douglas-Peucker
epsilon = 0.01  # Umbral de simplificación (ajústalo)
simplified_points = ramer_douglas_peucker(circle_points, epsilon)

# Separar los puntos simplificados en listas X e Y para graficar
simplified_x = [p[0] for p in simplified_points]
simplified_y = [p[1] for p in simplified_points]

# Graficar el círculo original y el simplificado
plt.figure(figsize=(8, 8))
plt.plot(x, y, label="Círculo Original (1000 puntos)", color="blue", linewidth=1)
plt.plot(simplified_x, simplified_y, label=f"Círculo Simplificado ({len(simplified_points)} puntos)", color="red", linestyle="--", linewidth=2)
plt.scatter(simplified_x, simplified_y, color="red", s=10, label="Puntos Simplificados")

# Configuración del gráfico
plt.title("Simplificación de Círculo con Ramer-Douglas-Peucker")
plt.axis("equal")  # Ejes proporcionales
plt.legend()
plt.grid()
plt.show()
