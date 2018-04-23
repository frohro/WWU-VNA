"""
Python script to gather SParameters for calibration on the 2018 VNA Tester


Author: Jacob Priddy
Date: 4/23/18

WORK IN PROGRESS
"""

import serial
import Measurement

print("What is the low frequency?")
fMin = 1000000  # int(input())

print("What is the high frequency?")
fMax = 10000000  # int(input())

print("How many frequencies do you want to be taken in the middle?")
n = 10  # int(input())

print('Plug in the open circuit as the DUT. Press enter when ready.')
input()

print("What port is it on?")
port = 'COM5'  # input()

ser = serial.Serial(port, 115200, timeout=3)

if not ser.is_open:
    print("Could not open port")
    exit()

deltaFreq = (fMax-fMin)/n

measurementIndex = 0

command = '^SWEEP,' + str(fMin) + ',' + str(fMax) + ',' + str(n) + '$\n'

ser.write(command.encode())

measurements = []

while True:
    line = ser.readline().decode()
    if line is None:
        break
    lineParts = line.split(',')
    map(lambda s: s.strip(' \n'), lineParts)
    x = Measurement.Measurement(fMin+measurementIndex*deltaFreq, lineParts[0], lineParts[1], lineParts[2], lineParts[3])
    measurements.append(x)
    measurementIndex += 1


print("done")
ser.close()
