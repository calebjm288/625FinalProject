import numpy as np
import itertools
import time
#import matplotlib.pyplot as plt
from memory_profiler import memory_usage
import sys

# Assuming you have your data as a NumPy array called 'points' with shape (n, 3).

# Parameters
r_max = 25.0  # The maximum side length for a valid triangle

num_points = sys.argv[1]

# inputfile.dat has three columns of numbers
x, y, z = np.loadtxt("inputfile{}.dat".format(num_points), usecols =(0, 1, 2), unpack = True)

#print(x)

points = np.column_stack((x,y,z))
#print(points)
# Sample data: Three points in 3D space
#point1 = [1.0, 2.0, 3.0]
#point2 = [4.0, 5.0, 3.3]
#point3 = [2.0, 3.0, 4.0]
#point4 = [1.5, 1.5, 2.6]
#point5 = [3.0, 2.2, 1.9]

# Create the 'points' array
#points = np.array([point1, point2, point3, point4, point5])

# Initialize histograms

count = 0

def calc(points):
    hist12 = []
    hist23 = []
    hist31 = []

    global count
    N = len(points)
    print(N)
    # Generate triangles and update histograms
    for i in range(N):
        for j in range(i+1,N):
            d12 = np.sqrt((x[i] - x[j])**2 + (y[i] - y[j])**2 + (z[i] - z[j])**2)
            #if d12 > r_max:
                #continue
            for k in range(j+1,N):
                d23 = np.sqrt((x[j] - x[k])**2 + (y[j] - y[k])**2 + (z[j] - z[k])**2)
                d31 = np.sqrt((x[k] - x[i])**2 + (y[k] - y[i])**2 + (z[k] - z[i])**2)
                # Calculate distances between points
                # distances = [np.linalg.norm(triplet[i] - triplet[j]) for i, j in [(0, 1), (1, 2), (2, 0)]]

                if d12 < r_max and d23 < r_max and d31 < r_max:
                    # Update the histograms
                    count += 1
                    hist12.append(d12)
                    hist23.append(d23)
                    hist31.append(d31)

#starting time
start = time.time()

mem = memory_usage((calc, (points,),{}),interval=0.1)

# end time
end = time.time()


# Visualize the histograms
#print(count)

print("DATA,OurInitialPython,{},{},{}".format(num_points,max(mem),(end-start)))
print(max(mem))

print(end-start)
print(count)
