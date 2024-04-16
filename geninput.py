import random

n = input("Enter n: ")
out = input("Enter output filename: ")

n = int(n)

a = []
b = []
for i in range(n):
    a.append(random.randint(2**10, 2**29))
    b.append(random.randint(2**10, 2**29))


with open(out, "w") as f:
    f.write(str(n))
    f.write("\n")
    f.write(" ".join([str(i) for i in a]))
    f.write("\n")
    f.write(" ".join([str(i) for i in b]))


