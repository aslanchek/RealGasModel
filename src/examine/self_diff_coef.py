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
file = open(DATA_DIR + "absolute_position_data.csv")
spamreader = csv.reader(file, delimiter=',')
for row in spamreader:
    data.append(np.array([float(row[0]), float(row[1]), float(row[2])]))
file.close()

time_current = 0

time = np.array([])
msd = np.array([])

rowstep = 0
begin_positions = []

while rowstep < NUM:
    begin_positions.append(data[rowstep])
    rowstep += 1

rowstep = NUM

while rowstep < len(data):
    part = 0 
    msd_current = 0
    while part < NUM:
        msd_current += (np.linalg.norm(data[rowstep + part] - begin_positions[part]))**2
        part += 1
    time_current += dt
    msd_current /= NUM * 6
    time = np.append(time, time_current) 
    msd = np.append(msd, msd_current/time_current)
    rowstep += NUM

plt.scatter(time, msd, 1)
plt.show()
