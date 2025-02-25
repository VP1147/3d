import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def read_data(filename):
    """
    Reads the data from the file and extracts x, y, z coordinates.
    """
    data = np.loadtxt(filename, usecols=(0, 1, 2))  # Read only x, y, z columns
    x = data[:, 0]
    y = data[:, 1]
    z = data[:, 2]
    return x, y, z

def plot_3d_trajectory(x, y, z):
    """
    Plots the 3D trajectory using matplotlib.
    """
    plt.style.use('dark_background')
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Plot the trajectory
    ax.plot3D(x, y, z, 'bo-', markersize=1, linewidth=0, color="yellow", label='Trajetória do raio')

    # Plot the projections
    nully, nullx = [], []
    for i in range (0, len(x)):
        nully.append(y[0])

    for i in range (0, len(y)):
        nullx.append(x[0])

    ax.plot3D(nullx, y, z, 'bo-', markersize=0.5, linewidth=0, color="white", label='Projeção X')
    ax.plot3D(x, nully, z, 'bo-', markersize=0.5, linewidth=0, color="white", label='Projeção Y')
    ax.plot3D(x, y, 0, 'bo-', markersize=0.5, linewidth=0, color="white", label='Projeção Z')

    # Add labels and title
    ax.set_xlabel('X (m)')
    ax.set_ylabel('Y (m)')
    ax.set_zlabel('Z (m)')
    ax.set_title('Visualizador 3D')

    # Add a legend
    ax.legend()

    # Show the plot
    plt.show()

def main():
    # File containing the data
    filename = "raio3d1.dat"

    # Read the data
    x, y, z = read_data(filename)

    # Plot the 3D trajectory
    plot_3d_trajectory(x, y, z)

if __name__ == "__main__":
    main()
