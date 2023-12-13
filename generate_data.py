import random

# Generating 1000 rows of random data in the specified format
num_rows = 100

with open("inputfile100.dat", "w") as file:
    for _ in range(num_rows):
        row = " ".join([f"{random.uniform(0, 100):.1f}" for _ in range(3)])
        file.write(f"{row}\n")

