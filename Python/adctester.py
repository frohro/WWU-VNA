#---------------
# Jacob Neal
# This program is meant to measure the standard deviation and average of a constant dc source
# 	into the adc (meas P4.0).
#---------------



import serial
import statistics
import time


print("Enter the com port")
port = str(input())

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
command = "^TIME,10$\n"
ser.write(command.encode())

ref = ser.readline().decode()
meas = ser.readline().decode()

#ref = ref.split(',')
meas = meas.strip(' \n')
meas = meas.split(',')
meas = [x.strip(' ') for x in meas]
#meas.remove('')
meas = [int(x) for x in meas]

average = statistics.mean(meas)
stde = statistics.stdev(meas)

print(meas)
print('Raw Average', average)
print('Raw Sample Standard Deviation', stde)


print('Average: ', (2.5/2**(14))*float(average))
print('Sample Standard Deviation: ', (2.5/2**(14))*float(stde))
