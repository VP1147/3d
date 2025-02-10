import numpy as np
import matplotlib.pyplot as plt
from pmatch import pmatch

def main_atr(ENTR, PRR, ENTA, PRA, INFO):
    """
    This function calculates 3D coordinates (xyz) and a correspondence matrix (db) for pairs of points.
    It also generates 3D plots to visualize the results.
    """
    nR = ENTR.shape[0]  # Number of points in ENTR
    nA = ENTA.shape[0]  # Number of points in ENTA

    # Initialize arrays
    db = np.zeros((nR, nA))  # Correspondence matrix
    x = np.zeros((nR, nA))   # x coordinates
    y = np.zeros((nR, nA))   # y coordinates
    z = np.zeros((nR, nA))   # z coordinates

    # Calculate points and populate the db matrix
    for i in range(nR):
        for j in range(nA):
            xyz, db[i, j] = pmatch(ENTR[i, :], PRR, ENTA[j, :], PRA, INFO)
            x[i, j] = xyz[0]
            y[i, j] = xyz[1]
            z[i, j] = xyz[2]

    # Plot the db matrix
    plt.figure()
    plt.imshow(db**(1/4), cmap='viridis', aspect='auto')
    plt.colorbar(label='db^(1/4)')
    plt.title('Correspondence Matrix (db)')
    plt.xlabel('ENTA Index')
    plt.ylabel('ENTR Index')
    plt.show()

    # Choose points for ray construction
    kmin = 1
    K = np.zeros(nA, dtype=int)  # Array to store indices of matched points

    for j in range(nA):
        dbarrayj = db[:, j]**(1/4)
        min_db, k = np.min(dbarrayj), np.argmin(dbarrayj)

        # Handle edge cases
        if k < kmin - 1:
            raise ValueError("Points are 'returning'. Check input data.")
        if k == kmin - 1:
            k = kmin
        if k < nA and k > kmin:
            kmin = k
        if k == 1 or k == nR:
            k = 0  # No correspondence found

        K[j] = k

    # Extract valid 3D points
    xyzt = []
    for j in range(len(K)):
        if K[j] > 0:
            xyzt.append([x[K[j], j], y[K[j], j], z[K[j], j]])

    xyzt = np.array(xyzt)

    # Plot 3D representation of the points
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot3D(xyzt[:, 0], xyzt[:, 1], xyzt[:, 2], 'bo-')
    ax.set_xlabel('eixo x (m)')
    ax.set_ylabel('eixo y (m)')
    ax.set_zlabel('eixo z (m)')
    ax.set_title('Representação tridimensional dos pontos adquiridos')
    ax.grid(True)
    ax.set_box_aspect([1, 1, 1])  # Equal aspect ratio
    plt.show()

    # Plot from the perspective of the auxiliary camera
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot3D(xyzt[:, 0], xyzt[:, 1], xyzt[:, 2], 'bo-')
    ax.set_xlabel('eixo x (m)')
    ax.set_ylabel('eixo y (m)')
    ax.set_zlabel('eixo z (m)')
    ax.set_title('Visão da câmera auxiliar')
    ax.grid(True)
    ax.set_box_aspect([1, 1, 1])
    ax.view_init(elev=-INFO[8], azim=INFO[9] - 90)  # Set view angle
    plt.show()

    # Plot from the perspective of the fast camera
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot3D(xyzt[:, 0], xyzt[:, 1], xyzt[:, 2], 'bo-')
    ax.set_xlabel('eixo x (m)')
    ax.set_ylabel('eixo y (m)')
    ax.set_zlabel('eixo z (m)')
    ax.set_title('Visão da câmera rápida')
    ax.grid(True)
    ax.set_box_aspect([1, 1, 1])
    ax.view_init(elev=-INFO[1], azim=INFO[2] - 90)  # Set view angle
    plt.show()

    return xyzt, db

ENTR = np.array([[10, 20], [15, 25], [20, 30]])  # Points in the reference image
PRR = np.array([5, 15, 100, 150])  # Reference point and image center for the reference image
ENTA = np.array([[12, 22], [17, 27], [22, 32]])  # Points in the auxiliary image
PRA = np.array([6, 16, 110, 160])  # Reference point and image center for the auxiliary image
INFO = np.array([45, 30, 100, 50, 200, 1, 0.5, 45, 30, 100, 50, 200, 1, 0.5])  # Additional parameters

# Call the function
xyzt, db = main_atr(ENTR, PRR, ENTA, PRA, INFO)

# Print results
print("3D Coordinates (x, y, z):")
print(xyzt)
print("Correspondence Matrix (db):")
print(db)