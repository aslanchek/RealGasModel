#!/bin/python
import matplotlib.pyplot as plt
from math import sqrt
import numpy as np
from scipy.stats import maxwell
import csv
import json

dV = 0.1

DATA_DIR = "../../data_512/"

configs_file = open(DATA_DIR + "conf.json")
configs = json.load(configs_file)

NUM = int(configs["count"])

configs_file.close()

Vmax = 5
file = open(DATA_DIR + "velocity_data.csv")

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
        try: 
            y[int(V // dV)] += 1 / NUM
        except:
            print("outofrange: ", V)
        n += 1
    else:
        y_show = np.array([])
        for i in y:
            y_show = np.append(y_show, i/timestep)

        print(timestep)
   
        n = 1

        timestep += 1
            
file.close()

plt.bar(x, y_show, width=dV, color="b", edgecolor="0")
plt.plot(x, maxwell.pdf(x, loc=-0.05)/9.9, color="r")
plt.show()

