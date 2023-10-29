import numpy as np
import itertools
import matplotlib.pyplot as plt

# Assuming you have your data as a NumPy array called 'points' with shape (n, 3).

# Parameters
r_max = 5.0  # The maximum side length for a valid triangle

# Sample data: Three points in 3D space
point1 = [1.0, 2.0, 3.0]
point2 = [4.0, 5.0, 3.3]
point3 = [2.0, 3.0, 4.0]
point4 = [1.5, 1.5, 2.6]
point5 = [3.0, 2.2, 1.9]

# Create the 'points' array
points = np.array([point1, point2, point3, point4, point5])

# Initialize histograms

hist12 = []
hist23 = []
hist31 = []

# Generate triangles and update histograms
for triplet in itertools.combinations(points, 3):
    # Calculate distances between points
    distances = [np.linalg.norm(triplet[i] - triplet[j]) for i, j in [(0, 1), (1, 2), (2, 0)]]

    if all(dist < r_max for dist in distances):
        # Update the histograms
        hist12.append(distances[0])
        hist23.append(distances[1])
        hist31.append(distances[2])

# Visualize the histograms
plt.figure()
plt.hist(hist12, bins=10, alpha=0.5, label='r12')
plt.legend()
plt.xlabel('Side Length')
plt.ylabel('Count')
plt.title('Histogram of Triangle Side Lengths')
plt.show()

# Visualize the histograms
plt.clf()
plt.hist(hist23, bins=10, alpha=0.5, label='r23')
plt.legend()
plt.xlabel('Side Length')
plt.ylabel('Count')
plt.title('Histogram of Triangle Side Lengths')
plt.show()

# Visualize the histograms
plt.clf()
plt.hist(hist31, bins=10, alpha=0.5, label='r31')
plt.legend()
plt.xlabel('Side Length')
plt.ylabel('Count')
plt.title('Histogram of Triangle Side Lengths')
plt.show()

