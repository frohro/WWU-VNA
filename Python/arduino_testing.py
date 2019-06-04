#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jun  3 14:41:47 2019

@author: frohro
"""

import sounddevice as sd
import numpy as np
import matplotlib.pyplot as plt
import serial
import datetime
import os
import time


fs = 48000
N = 48000  # points to take

sd.default.samplerate = fs
sd.default.channels = 2

myrecording = sd.rec(N, blocking=True)

# t=np.linspace(0,10,100)
t = np.arange(0, (N-1)/fs, 1/fs)
# plt.plot(t,np.cos(t),t,np.sin(t))
plt.figure()
plt.plot(t, myrecording)
plt.xlabel('t')
plt.title('What you recorded')
plt.legend(['left channel', 'right channel'])
plt.figure()

print("Enter the com port")
port = str(input())

print("Enter the frequency in MHz")
fMin = int(float(input()) * 1e6)

print("Enter number of samples")
samp = int(input())

start = time.time()

filename = str(os.path.splitext(os.path.basename(__file__))[0]) + "_" + \
    str(datetime.datetime.now()).replace(":", "-")\
    .replace(".", "-").replace(" ", "_") + ".dat"

if not os.path.exists("measurements"):
    os.makedirs("measurements")

file = open("measurements/" + filename, 'w+')

try:
    ser = serial.Serial(port, 115200, timeout=3)
except serial.serialutil.SerialException:
    print("Could not open the port")
    exit()

if not ser.is_open:
    print("Could not open port")
    exit()

time.sleep(1)
ser.flush()
print("Getting TIME Series:" + '\n')
command = "^TIME," + str(fMin) + "$\n"
ser.write(command.encode())
