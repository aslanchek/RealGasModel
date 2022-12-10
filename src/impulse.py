#!/bin/python
import matplotlib.pyplot as plt
import numpy as np
from math import sqrt
import csv


file = open("../data/velocity_data.csv")
spamreader = csv.reader(file, delimiter=',')
data = []
for row in spamreader:
    data.append(np.array([float(row[0]), float(row[1]), float(row[2])]))

file.close()

time_current = 0

time = np.array([])
impulse = np.array([])

NUM = 512
dt = 0.5

rowstep = 0

while rowstep < len(data):
    part = 0 
    summary_impulse = np.array([0, 0, 0])
    while part < NUM:
        summary_impulse = summary_impulse + data[rowstep + part]
        part += 1
    time_current += dt
    time = np.append(time, time_current) 
    impulse = np.append(impulse, np.linalg.norm(summary_impulse))
    rowstep += NUM

plt.scatter(time, impulse, 1)
plt.ylim(19.2, 19.4)
#plt.plot(time, impulse)
plt.show()
