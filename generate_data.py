import random

# Generating 1000 rows of random data in the specified format
num_rows = 1000

with open("inputfile1000.dat", "w") as file:
    for _ in range(num_rows):
        row = " ".join([f"{random.uniform(0, 100):.1f}" for _ in range(3)])
        file.write(f"{row}\n")

