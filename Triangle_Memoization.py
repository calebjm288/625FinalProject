import numpy as np
import itertools
import time

# Assuming you have your data as a NumPy array called 'points' with shape (n, 3).

starttime = time.time()

# Parameters
r_max = 5.0  # The maximum side length for a valid triangle

# inputfile.dat has three columns of numbers
x, y, z = np.loadtxt("inputfile300.dat", usecols=(0, 1, 2), unpack=True)

points = np.column_stack((x, y, z))

# Initialize histograms
hist12 = []
hist23 = []
hist31 = []

# Dictionary for memoization
memo_dict = {}

def calculate_distance(point1, point2):
    # Use a single-level dictionary with tuple key
    key = (tuple(point1), tuple(point2))

    # Check if the distance has already been computed
    if key in memo_dict:
        return memo_dict[key]

    distance = np.linalg.norm(point1 - point2)

    # Memoize the result
    memo_dict[key] = distance

    return distance

count = 0

# Generate triangles and update histograms
for triplet in itertools.combinations(points, 3):
    # Calculate distances between points using memoization
    distances = [calculate_distance(triplet[i], triplet[j]) for i, j in [(0, 1), (1, 2), (2, 0)]]

    if all(dist < r_max for dist in distances):
        # Update the histograms
        count += 1
        hist12.append(distances[0])
        hist23.append(distances[1])
        hist31.append(distances[2])

endtime = time.time()

# Visualize the histograms
print(count)
print(endtime - starttime)
