% VNA 2017 test script.
% Rob Frohne, May 2017.

clc; clear;

fMin = 1e6;
fMax = 1e6;
nFreq = 1;

Sum = zeros(nFreq,2);
% Load the package
pkg load instrument-control
% Check if serial support exists
if (exist("serial") != 3)
  disp("No Serial Support");
endif
% Instantiate the Serial Port
% Naturally, set the COM port # to match your device
% Use this crazy notation for any COM port number: 1 - 255
%s1 = serial("/dev/pts/2");
%s1 = serial("/tmp/ttyDUMMY"); % $ interceptty /dev/ttyACM0 /tmp/ttyDUMMY
s1 = serial("/dev/ttyACM0");
pause(1); % Wait a second as it takes some ports a while to wake up
% Set the port parameters
set(s1,'baudrate', 115200);
set(s1,'bytesize', 8);
set(s1,'parity', 'n');
set(s1,'stopbits', 1);
set(s1,'timeout', 255); % 12.3 Seconds as an example here
% Optional commands, these can be 'on' or 'off'
%set(s1, 'requesttosend', 'on');
 % Sets the RTS line
%set(s1, 'dataterminalready', 'on'); % Sets the DTR line
% Optional - Flush input and output buffers
srl_flush(s1);
string_to_send = strcat("Testing! ^SWEEP,",num2str(uint64(fMin)),","...
                  ,num2str(uint64(fMax)),",",num2str(uint64(nFreq)),"$\n")
srl_write(s1,string_to_send);
for i=1:nFreq
  Sum(i,:) = str2num(ReadToTermination(s1, 10))
  %String = ReadToTermination(s1,10)
endfor
% Finally, Close the port
fclose(s1);
