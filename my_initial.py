import numpy as np
import itertools
import time
#import matplotlib.pyplot as plt
from memory_profiler import memory_usage

# Assuming you have your data as a NumPy array called 'points' with shape (n, 3).

# Parameters
r_max = 5.0  # The maximum side length for a valid triangle


# inputfile.dat has three columns of numbers
x, y, z = np.loadtxt("inputfile300.dat", usecols =(0, 1, 2), unpack = True)

print(x)

points = np.column_stack((x,y,z))
print(points)
# Sample data: Three points in 3D space
#point1 = [1.0, 2.0, 3.0]
#point2 = [4.0, 5.0, 3.3]
#point3 = [2.0, 3.0, 4.0]
#point4 = [1.5, 1.5, 2.6]
#point5 = [3.0, 2.2, 1.9]

# Create the 'points' array
#points = np.array([point1, point2, point3, point4, point5])

# Initialize histograms

def calc(points):
    hist12 = []
    hist23 = []
    hist31 = []

    count = 0

    # Generate triangles and update histograms
    for triplet in itertools.combinations(points, 3):
        # Calculate distances between points
        distances = [np.linalg.norm(triplet[i] - triplet[j]) for i, j in [(0, 1), (1, 2), (2, 0)]]

        if all(dist < r_max for dist in distances):
            # Update the histograms
            count += 1
            hist12.append(distances[0])
            hist23.append(distances[1])
            hist31.append(distances[2])

#starting time
start = time.time()

mem = memory_usage((calc, (points,),{}),interval=0.1)

# end time
end = time.time()


# Visualize the histograms
#print(count)

print(max(mem))

print(end-start)
