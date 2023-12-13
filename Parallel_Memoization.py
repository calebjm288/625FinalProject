import numpy as np
import itertools
import time
import concurrent.futures
import threading

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

# Use ThreadLocal for thread-specific counts
thread_local = threading.local()

def calculate_distance(point1, point2, memo_dict):
    # Use a single-level dictionary with tuple key
    key = (tuple(point1), tuple(point2))

    # Check if the distance has already been computed
    if key in memo_dict:
        return memo_dict[key]

    distance = np.linalg.norm(point1 - point2)

    # Memoize the result
    memo_dict[key] = distance

    return distance

def process_chunk(chunk_points, memo_dict):
    local_hist12 = []
    local_hist23 = []
    local_hist31 = []
    
    local_count = 0

    for triplet in itertools.combinations(chunk_points, 3):
        # Calculate distances between points using memoization
        distances = [calculate_distance(triplet[i], triplet[j], memo_dict) for i, j in [(0, 1), (1, 2), (2, 0)]]

        if all(dist < r_max for dist in distances):
            # Update local histograms
            local_count += 1
            local_hist12.append(distances[0])
            local_hist23.append(distances[1])
            local_hist31.append(distances[2])

    # Store the local count in the ThreadLocal variable
    thread_local.count = local_count

    return local_hist12, local_hist23, local_hist31

# Split points into chunks for parallel processing
num_threads = 8  # Adjust as needed
chunk_size = len(points) // num_threads
point_chunks = [points[i:i+chunk_size] for i in range(0, len(points), chunk_size)]

# Dictionary for memoization per thread
memo_dicts = [{} for _ in range(num_threads)]

with concurrent.futures.ThreadPoolExecutor() as executor:
    futures = [executor.submit(process_chunk, chunk, memo_dict) for chunk, memo_dict in zip(point_chunks, memo_dicts)]

    # Collect results from all threads and sum the counts
    count = sum(future.result()[0].count for future in concurrent.futures.as_completed(futures))
    # You can also collect histograms if needed
    for future in concurrent.futures.as_completed(futures):
        local_hist12, local_hist23, local_hist31 = future.result()
        hist12.extend(local_hist12)
        hist23.extend(local_hist23)
        hist31.extend(local_hist31)

endtime = time.time()

# Visualize the count
print(count)
print(endtime - starttime)
