import serial
import numpy
import statistics
import cmath
import datetime
import os
import time

def average(arr):
    return sum(arr) / len(arr)

print("Enter the com port")
port = str(input())

print("Enter the frequency in MHz")
fMin = int(float(input()) * 1e6)

print("Enter number of samples")
samp = int(input())

start = time.time()

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
command = "^SAMPLERATE,1"+"$\n"
ser.write(command.encode())

Fs = int(ser.readline())
N = int(ser.readline().decode().strip(' \n'))
F_IF = int(ser.readline().decode().strip(' \n'))

T = 1./float(Fs)

file.write("Time Test\n")
file.write("Frequency = " + str(fMin) + '\n')
file.write("Samples = " + str(samp) + '\n')
file.write("Port = " + str(port) + '\n')

file.write("FS = " + str(Fs) + '\n')
file.write("N = " + str(N) + '\n')
file.write("F_IF = " + str(F_IF) + '\n')
file.write("T = " + str(T) + '\n\n\n')

endRef = []
endMeas = []
refcomputed = []
meascomputed = []

for x in range(samp):
    print("Getting " + str(x) + '\n')
    command = "^TIMECOMPUTE," + str(fMin) + "$\n"
    ser.write(command.encode())
    refdat = ser.readline().decode()
    measdat = ser.readline().decode()
    computed = ser.readline().decode()

    computed = computed.split(',')
    rr, ri, mr, mi = [float(x.strip(' \n')) for x in computed if x.strip(' \n')]
    refcomputed.append(rr + 1j * ri)
    meascomputed.append(mr + 1j * mi)

    refdat = refdat.split(',')
    measdat = measdat.split(',')
    refdat = [int(x.strip(' \n')) for x in refdat if x.strip(' \n')]
    measdat = [int(x.strip(' \n')) for x in measdat if x.strip(' \n')]

    endRef.append(refdat)
    endMeas.append(measdat)

ser.close()

H1computed = []

ref = []
meas = []
H1 = []

window = numpy.hanning(N)

for x in range(samp):
    print("Computing " + str(x) + '\n')

    for y in range(len(endRef[x])):
        endRef[x][y] *= window[y]

    for y in range(len(endMeas[x])):
        endMeas[x][y] *= window[y]

    reffft = numpy.fft.fft(endRef[x])
    measfft = numpy.fft.fft(endMeas[x])

    binIndex = int(F_IF * N / Fs + 1)

    ref.append(reffft[binIndex])
    meas.append(measfft[binIndex])

    H1.append(measfft[binIndex] / reffft[binIndex])
    file.write("Measurement from raw data " + str(x + 1) + '\n')
    file.write('Ref: ' + str(ref[x]) + '\n')
    file.write('Meas: ' + str(meas[x]) + '\n')
    file.write('H1: ' + str(H1[x]) + '\n')
    file.write('H1 mag: ' + str(numpy.abs(H1[x])) + '\n\n')

    H1computed.append(meascomputed[x] / refcomputed[x])
    file.write("Board computed measurement " + str(x + 1) + '\n')
    file.write('Ref: ' + str(refcomputed[x]) + '\n')
    file.write('Meas: ' + str(meascomputed[x]) + '\n')
    file.write('H1: ' + str(H1computed[x]) + '\n')
    file.write('H1 mag: ' + str(numpy.abs(H1computed[x])) + '\n\n\n')

magH1 = [numpy.absolute(x) for x in H1]
magH1computed = [numpy.absolute(x) for x in H1computed]

refmean = average([numpy.abs(x) for x in ref])
measmean = average([numpy.abs(x) for x in meas])

refmeancomputed = average([numpy.abs(x) for x in refcomputed])
measmeancomputed = average([numpy.abs(x) for x in meascomputed])

refstdv = statistics.stdev([numpy.abs(x) for x in ref])
measstdv = statistics.stdev([numpy.abs(x) for x in meas])

refstdvcomputed = statistics.stdev([numpy.abs(x) for x in refcomputed])
measstdvcomputed = statistics.stdev([numpy.abs(x) for x in meascomputed])


magH1bar = statistics.mean(magH1)
magH1barcomputed = statistics.mean(magH1computed)

file.write("Numpy computed values:\n")
file.write('Reference Magnitude Mean: ' + str(refmean) + '\n')
file.write('Measured Magnitude Mean: ' + str(measmean) + '\n')
file.write('Reference Magnitude Standard Deviation: ' + str(refstdv) + '\n')
file.write('Measured Magnitude Standard Deviation: ' + str(measstdv) + '\n')
file.write('Standard deviation percent off mean ref: ' + str(refstdv/refmean * 100) + '%\n')
file.write('Standard deviation percent off mean meas: ' + str(measstdv/measmean * 100) + '%\n')
file.write('H1 Magnitude Mean: ' + str(magH1bar) + '\n')
file.write('H1 Magnitude Standard Deviation: ' + str(statistics.stdev(magH1)) + '\n')
file.write('H1 Magnitude standard deviation percentage: ' + str(statistics.stdev(magH1) / magH1bar * 100) + '%\n\n')


file.write("Board computed values:\n")
file.write('Reference Magnitude Mean: ' + str(refmeancomputed) + '\n')
file.write('Measured Magnitude Mean: ' + str(measmeancomputed) + '\n')
file.write('Reference Magnitude Standard Deviation: ' + str(refstdvcomputed) + '\n')
file.write('Measured Magnitude Standard Deviation: ' + str(measstdvcomputed) + '\n')
file.write('Standard deviation percent off mean ref: ' + str(refstdvcomputed/refmeancomputed * 100) + '%\n')
file.write('Standard deviation percent off mean meas: ' + str(measstdvcomputed/measmeancomputed * 100) + '%\n')
file.write('H1 Magnitude Mean: ' + str(magH1barcomputed) + '\n')
file.write('H1 Magnitude Standard Deviation: ' + str(statistics.stdev(magH1computed)) + '\n')
file.write('H1 Magnitude standard deviation percentage: ' + str(statistics.stdev(magH1computed)/magH1barcomputed * 100) + '%\n\n')

file.close()

print("DONE! CHECK measurements/" + filename + '\n')
print("Total TIme: " + str(time.time() - start) + " seconds\n")
