import numpy as np

def pmatch(P1, PR1, P2, PR2, INFO):
    """
    This algorithm calculates the possible x, y, z coordinates, assuming that the two input points P1 and P2 are corresponding.
    It also returns the value db, which calculates the correspondence (the smaller the db, the higher the chance that P1 and P2 are the same point in space).
    xpr and ypr correspond to the location of the reference point of the image from which the match point was taken. xci and yci are the location of the center of the image.
    """
    # Extract values from PR1 and PR2
    xpr1, ypr1, xci1, yci1 = PR1[0], PR1[1], PR1[2], PR1[3]
    xpr2, ypr2, xci2, yci2 = PR2[0], PR2[1], PR2[2], PR2[3]

    # Convert P1 and P2 coordinates to angles (inclination and azimuthal) in radians
    ANG1d = np.zeros(2)
    ANG1d[0] = INFO[0] - 180 * np.arctan((P1[1] - yci1) * INFO[6] / INFO[5]) / np.pi
    ANG1d[1] = INFO[1] - 180 * (np.arctan((P1[0] - xci1) * INFO[6] / INFO[5]) - np.arctan((xpr1 - xci1) * INFO[6] / INFO[5])) / np.pi

    ANG2d = np.zeros(2)
    ANG2d[0] = INFO[7] - 180 * np.arctan((P2[1] - yci2) * INFO[13] / INFO[12]) / np.pi
    ANG2d[1] = INFO[8] - 180 * (np.arctan((P2[0] - xci2) * INFO[13] / INFO[12]) - np.arctan((xpr2 - xci2) * INFO[13] / INFO[12])) / np.pi

    ANG1 = ANG1d * np.pi / 180
    ANG2 = ANG2d * np.pi / 180

    # Find the projection (xp, yp) on the horizontal plane for this pair
    xp = -(INFO[10] - INFO[3] - INFO[9] * np.tan(ANG2[1] - INFO[2] * np.tan(ANG1[1]))) / (np.tan(ANG2[1]) - np.tan(ANG1[1]))
    yp = (xp - INFO[2]) * np.tan(ANG1[1]) + INFO[3]

    # Calculate height estimates for the supposed 3D point (x, y, z)
    d1 = np.sqrt((INFO[2] - xp)**2 + (INFO[3] - yp)**2)
    d2 = np.sqrt((INFO[9] - xp)**2 + (INFO[10] - yp)**2)

    zp1 = d1 * np.tan(ANG1[0]) + INFO[4]
    zp2 = d2 * np.tan(ANG2[0]) + INFO[12]

    # Calculate db based on the difference between zp1 and zp2
    db = np.arctan(np.abs(zp1 - zp2) / min(d1, d2))

    # Return the estimated height as the average of the height values found for each point
    z = zp2 + (d2 / np.cos(ANG2[0])) * (zp1 - zp2) / (d2 / np.cos(ANG2[0]) + d1 / np.cos(ANG1[0]))
    xyz = np.array([xp, yp, z])

    return xyz, db