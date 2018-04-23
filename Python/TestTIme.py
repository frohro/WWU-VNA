"""
Python script to get the accuracy of the TIME command.

Author: Jacob Priddy
Date: 4/23/18

WORK IN PROGRESS
"""

import time, serial, numpy, cmath


def average(arr):
    return sum(arr) / len(arr)


def stdev(arr):
    avg = average(arr)
    squareSum = 0
    for element in arr:
        squareSum += (element - avg)**2
    return cmath.sqrt(squareSum / len(arr))


def variance(arr):
    avg = average(arr)
    squareSum = 0
    for element in arr:
        squareSum += (element - avg) ** 2
    return squareSum / len(arr)

fMin = 60e6

print("Enter the com port")
port = input()

print("Enter number of samples")
samp = int(input())

ser = serial.Serial(port, 115200, timeout=3)

if not ser.is_open:
    print("Could not open port")
    exit()

time.sleep(1)
ser.flush()
command = "^SAMPLERATE,"+"$\n"
ser.write(command.encode())

# Fs =
ser.readline()
N = int(ser.readline().decode().strip(' \n'))
F_IF = int(ser.readline().decode().strip(' \n'))

# Setting next line to fix frequency correction temporarily
Fs = 8000

T = 1/Fs


endRef = []
endMeas = []

for x in range(samp):
    command = "^TIME," + str(fMin) + "$\n"
    ser.write(command.encode())
    ref = ser.readline().decode()
    meas = ser.readline().decode()

    ref = ref.split(',')
    meas = meas.split(',')
    refSanitized = [int(x.strip(' \n')) for x in ref if x.strip(' \n')]
    measSanitized = [int(x.strip(' \n')) for x in meas if x.strip(' \n')]

    ref = [x - average(refSanitized) for x in refSanitized]
    meas = [x - average(measSanitized) for x in measSanitized]

    endRef.append(ref)
    endMeas.append(meas)

ser.close()

ref = []
meas = []
H1 = []
H3 = []
H5 = []
H7 = []

for x in range(samp):
    reffft = numpy.fft.fft(endRef[x])
    measfft = numpy.fft.fft(endMeas[x])
    ref.append(reffft[int(F_IF*N/Fs+1)])
    meas.append(measfft[int(F_IF*N/Fs+1)])
    H1.append(measfft[int(F_IF * N / Fs + 1)] / reffft[int(F_IF * N / Fs + 1)])
    H3.append(measfft[int(3 * F_IF * N / Fs + 1)] / reffft[int(3 * F_IF * N / Fs + 1)])
    H5.append(measfft[int(5 * F_IF * N / Fs + 1)] / reffft[int(5 * F_IF * N / Fs + 1)])
    H7.append(measfft[int(7 * F_IF * N / Fs + 1)] / reffft[int(7 * F_IF * N / Fs + 1)])

    print('Ref: ' + str(ref[x]) + '\n')
    print('Meas: ' + str(meas[x]) + '\n')
    print('H1: ' + str(H1[x]) + '\n')
    print('H3: ' + str(H3[x]) + '\n')
    print('H5: ' + str(H5[x]) + '\n')
    print('H7: ' + str(H7[x]) + '\n')

print('H1 Magnitude Mean: ' + str(abs(average(H1))) + '\n')
print('H3 Magnitude Mean: ' + str(abs(average(H3))) + '\n')
print('H5 Magnitude Mean: ' + str(abs(average(H5))) + '\n')
print('H7 Magnitude Mean: ' + str(abs(average(H7))) + '\n')

print('H1 Magnitude Standard Deviation: : ' + str(abs(stdev(H1))) + '\n')
print('H3 Magnitude Standard Deviation: : ' + str(abs(stdev(H3))) + '\n')
print('H5 Magnitude Standard Deviation: : ' + str(abs(stdev(H5))) + '\n')
print('H7 Magnitude Standard Deviation: : ' + str(abs(stdev(H7))) + '\n')

print('H1 Magnitude Variance: ' + str(abs(variance(H1))) + '\n')
print('H3 Magnitude Variance: ' + str(abs(variance(H3))) + '\n')
print('H5 Magnitude Variance: ' + str(abs(variance(H5))) + '\n')
print('H7 Magnitude Variance: ' + str(abs(variance(H7))) + '\n')
