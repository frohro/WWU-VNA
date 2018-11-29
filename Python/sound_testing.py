import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd
#Data on recording, etc on: https://python-sounddevice.readthedocs.io/en/0.3.12/usage.html#recording
fs = 48000
T = 1/fs
duration = 2  # seconds
N = int(duration*fs)
x = sd.rec(N, samplerate=fs, channels=2)
print(str(x.shape))
sd.wait()
#sd.play(x,samplerate=fs)
t = np.arange(0,duration,1/fs) # time vector
plt.plot(t,x)
plt.show()
X0 = np.fft.fftshift(np.fft.fft(x[:,0])/N)
X1 = np.fft.fftshift(np.fft.fft(x[:,1])/N)
f = np.arange(-1/(2*T),1/(2*T),1/(N*T))
plt.plot(f,np.abs(X0))
plt.show()

