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
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Plot the trajectory
    ax.plot3D(x, y, z, 'bo-', markersize=4, linewidth=1, label='Trajectory')

    # Add labels and title
    ax.set_xlabel('X (m)')
    ax.set_ylabel('Y (m)')
    ax.set_zlabel('Z (m)')
    ax.set_title('3D Trajectory Visualization')

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
