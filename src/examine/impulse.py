#!/bin/python
import matplotlib.pyplot as plt
import numpy as np
from math import sqrt
import csv
import json

DATA_DIR = "../../data/"

configs_file = open(DATA_DIR + "conf.json")
configs = json.load(configs_file)

NUM = int(configs["count"])
dt = float(configs["dt"]) * float(configs["log_step"])

configs_file.close()

data = []
data_file = open(DATA_DIR + "velocity_data.csv")
spamreader = csv.reader(data_file, delimiter=',')
for row in spamreader:
    data.append(np.array([float(row[0]), float(row[1]), float(row[2])]))
data_file.close()

time_current = 0

time = np.array([])
impulse = np.array([])

rowstep = 0

def truncate(n, decimals=0):
    multiplier = 10 ** decimals
    return int(n * multiplier) / multiplier

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

plt.ylim(truncate(impulse[1], 3) - 0.01, truncate(impulse[1], 3) + 0.01)
plt.scatter(time, impulse, 1)
plt.show()
