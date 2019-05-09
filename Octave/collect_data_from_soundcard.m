% Test script to record the WWU-VNA using a soundcard.

% VNA 2017 test script.
% This script tests the TIME command, which is meant to get the analyzer
% to make a measurement at one frequency, and instead of downsampling to DC,
% it just sends the data back in the variable, "voltage".
% Rob Frohne, May 2017.

clc; clear;
close all;

fMin = 10e6; % Note:  We need to figure out what the aliased frequency is
% and make sure it doesn't interfere with our measurement even with the LPF.

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
%s1 = serial("/tmp/ttyDUMMY"); % $ 
if exist("/dev/ttyACM0","file")
  s1 = serial("/dev/ttyACM0"); 
elseif exist("/dev/ttyACM1","file")
    s1 = serial("/dev/ttyACM1");
else
  s1 = serial("/tmp/ttyDUMMY");
endif

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
%string_to_send = strcat("^SAMPLERATE,","$\n");
%srl_write(s1,string_to_send);
%Fs = str2num(ReadToTermination(s1,10));
%N = str2num(ReadToTermination(s1,10));
%F_IF = str2num(ReadToTermination(s1,10));
F_IF = 155;
N = 1024*4;  % was 3840/2;
Fs=53000;  
T = 1/Fs;
%srl_flush(s1);
string_to_send = strcat("^TIME,",num2str(uint64(fMin)),"$\n");
srl_write(s1,string_to_send);
ref(1,:) = str2num(ReadToTermination(s1,10));
pause(1);
meas(1,:)= str2num(ReadToTermination(s1,10));
ref=ref-2^13;
meas=meas-2^13;
t=0:T:(N-1)*T;
figure(1)
plot(t,ref);
hold on
plot(t,meas,"linewidth",1)
legend('ref','meas')
title('Results')
xlabel('Time (s)')

figure(2)
fS =96000;
nBits = 16;
nChannels = 2;
totalTime = 4;
t=0:1/fS:totalTime-1/fS;
recorder = audiorecorder(fS,nBits,nChannels)
recordblocking(recorder,totalTime);
data = getaudiodata(recorder,"single");
plot(t(fS:end),data(fS:end,1),t(fS:end),data(fS:end,2))