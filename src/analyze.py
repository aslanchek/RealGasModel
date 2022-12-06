import matplotlib.pyplot as plt
from math import sqrt
import numpy as np
import csv

plt.ion()

dV = 0.15
Vmax = 5
NUM = 1000

file = open("../data/velocity_data.csv")

i = 0
x = np.array([])
y = np.array([])

while i <= Vmax:
    x = np.append(x, i)
    i += dV
    y = np.append(y, 0)

n = 1
timestep = 1

spamreader = csv.reader(file, delimiter=',')
for row in spamreader:
    if (timestep > 0 * NUM):
        if n < NUM - 1:
            V = sqrt(float(row[0]) ** 2 + float(row[1]) ** 2 + float(row[2]) ** 2)
            y[int(V // dV) - 1] += 1 / NUM
            n += 1
        else:
            plt.bar(x, y, width=dV, color="b", edgecolor="0")
            plt.draw()
            plt.pause(0.001)
            plt.clf()
    
            print(timestep // NUM)
   
            n = 1
            y = np.array([])
    
            i = 0
            while i <= Vmax:
                y = np.append(y, 0.0)
                i += dV

    timestep += 1
 

file.close()
