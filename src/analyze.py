#!/bin/python
import matplotlib.pyplot as plt
from math import sqrt
import numpy as np
import csv

plt.ion()

dV = 0.1
Vmax = 6
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
    if n < NUM - 1:
        V = sqrt(float(row[0]) ** 2 + float(row[1]) ** 2 + float(row[2]) ** 2)
        # V = float(row[0]) ** 2
        # y[int(V // dV) - 1] += 1 / NUM
        try: 
            y[int(V // dV)] += 1 / NUM
        except:
            print("blya")
        n += 1
    else:
        y_show = np.array([])
        for i in y:
            y_show = np.append(y_show, i/timestep)

        #plt.bar(np.sqrt(x), np.log(y_show), width=dV, color="b", edgecolor="0")
        plt.bar(x, y_show, width=dV, color="b", edgecolor="0")
        plt.draw()
        plt.pause(0.001)
        plt.clf()

        print(timestep)
   
        n = 1

        timestep += 1
            
file.close()
