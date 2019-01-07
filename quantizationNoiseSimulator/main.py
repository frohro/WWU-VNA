import numpy as np
import matplotlib.pyplot as plt
import random

vpp = 2.5

adbits = 10
places = 2**adbits
step = vpp/places
# error = 0.0110703125  # in volts
error = 0
Fs = 8000  # sampling rate
Ts = 1.0/Fs  # sampling interval
ff = 200  # frequency of the signal
t = np.arange(0, 100/ff, Ts)  # Time vector 3 periods
y = np.cos(2*np.pi*ff*t)

y = [float((int(round((v*vpp/2) / step)) + random.randint(-int(error/step), int(error/step))) * step) for v in y]

n = len(y)  # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T  # two sides frequency range
frq = frq[range(int(n/2))]  # one side frequency range

Y = np.fft.fft(y)/n   # fft computing and normalization
Y = abs(Y[range(int(n/2))])

Y = [20 * np.log10(m) for m in Y]
# plt.plot(t, y, 'o-')
plt.plot(frq, Y, '.-')
plt.xlim(0, frq[len(frq) - 1])
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.show()

