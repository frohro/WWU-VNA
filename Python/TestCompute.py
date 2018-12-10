"""
Python script to test the accuracy of the COMPUTE command.
Author: Jacob Priddy
Date: 12/7/18
"""

import time
# This is pyserial
import serial
import numpy
import statistics
import cmath
import datetime
import os


def average(arr):
    return sum(arr) / len(arr)


print("Enter the com port")
port = str(input())

print("Enter the frequency in MHz")
fMin = int(float(input()) * 1e6)

print("Enter number of samples")
samp = int(input())


filename = str(os.path.splitext(os.path.basename(__file__))[0]) + "_" + str(datetime.datetime.now()).replace(":", "-")\
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
command = "^SAMPLERATE,0"+"$\n"
ser.write(command.encode())

Fs = int(ser.readline())
N = int(ser.readline().decode().strip(' \n'))
F_IF = int(ser.readline().decode().strip(' \n'))

T = 1./float(Fs)

file.write("Compute Test\n")
file.write("Frequency = " + str(fMin) + '\n')
file.write("Samples = " + str(samp) + '\n')
file.write("Port = " + str(port) + '\n')

file.write("FS = " + str(Fs) + '\n')
file.write("N = " + str(N) + '\n')
file.write("F_IF = " + str(F_IF) + '\n')
file.write("T = " + str(T) + '\n\n\n')


ref = []
meas = []

for x in range(samp):
    print("Getting " + str(x + 1))
    command = "^COMPUTE," + str(fMin) + "$\n"
    ser.write(command.encode())
    computed = ser.readline().decode()

    computed = computed.split(',')
    rr, ri, mr, mi = [float(x.strip(' \n')) for x in computed if x.strip(' \n')]
    ref.append(rr + 1j * ri)
    meas.append(mr + 1j * mi)

ser.close()

H1 = []

for x in range(samp):
    file.write("Measurement " + str(x + 1) + '\n')
    file.write('Ref: ' + str(ref[x]) + '\n')
    file.write('Meas: ' + str(meas[x]) + '\n')
    H1.append(meas[x] / ref[x])
    file.write('H1: ' + str(H1[x]) + '\n')

magH1 = [numpy.absolute(x) for x in H1]

phaseH1 = [cmath.phase(x)*180/cmath.pi for x in H1]

# Store mean calculations for computing variances later
magH1bar = statistics.mean(magH1)

phaseH1bar = statistics.mean(phaseH1)

refmean = average([numpy.abs(x) for x in ref])
measmean = average([numpy.abs(x) for x in meas])

refstdv = statistics.stdev([numpy.abs(x) for x in ref])
measstdv = statistics.stdev([numpy.abs(x) for x in meas])

file.write('Reference Magnitude Mean: ' + str(refmean) + '\n')
file.write('Measured Magnitude Mean: ' + str(measmean) + '\n\n')

file.write('Reference Magnitude Standard Deviation: ' + str(refstdv) + '\n')
file.write('Measured Magnitude Standard Deviation: ' + str(measstdv) + '\n\n')

file.write('Standard deviation percent off mean ref: ' + str(refstdv/refmean * 100) + '%\n')
file.write('Standard deviation percent off mean meas: ' + str(measstdv/measmean * 100) + '%\n\n')


file.write('H1 Magnitude Mean: ' + str(magH1bar) + '\n')

file.write('H1 Magnitude Standard Deviation: ' + str(statistics.stdev(magH1, magH1bar)) + '\n')

file.write('H1 Magnitude Variance: ' + str(statistics.variance(magH1, magH1bar)) + '\n')

file.write('H1 Phase Mean: ' + str(phaseH1bar) + '\n')

file.write('H1 Phase Standard Deviation: ' + str(statistics.stdev(phaseH1, phaseH1bar)) + '\n')

file.write('H1 Phase Variance: ' + str(statistics.variance(phaseH1, phaseH1bar)) + '\n')

file.close()

print("DONE! CHECK measurements/" + filename + '\n')