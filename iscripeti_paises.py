import random

paises = []

with open("paises_todos.txt", "r") as f:
    for l in f:
        paises.append(l)

for i in range(250):
    print(random.choice(paises), end="")
