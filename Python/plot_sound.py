#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jun  3 14:41:47 2019

@author: frohro
"""

import sounddevice as sd
import numpy as np
import matplotlib.pyplot as plt


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
