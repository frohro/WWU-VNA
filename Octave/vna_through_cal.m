% VNA 2018 VNA test script.
% Rob Frohne, May 2017.
% This script will make nFreq measurements from fMin to fMax.
% It is designed to make S21 measurements and use calibration.
% The first step is to connect your cables together, and do a
% a through calibration measurement.  Then you can measure the DUT.
% It will divide out the transfer function of the cables, so they
% don't mess up your measurement of the DUT.

clc; clear;
close all;

function s21 = readVNA(fMin, fMax, nFreq)
  % Instantiate the Serial Port
  % Naturally, set the COM port # to match your device
  % Use this crazy notation for any COM port number: 1 - 255
  %s1 = serial("/dev/pts/2");
  %s1 = serial("/tmp/ttyDUMMY"); % $ interceptty /dev/ttyACM0 /tmp/ttyDUMMY
  serialPort = serial("/dev/ttyACM0");
  pause(1); % Wait a second as it takes some ports a while to wake up
  % Set the port parameters
  set(serialPort,'baudrate', 115200);
  set(serialPort,'bytesize', 8);
  set(serialPort,'parity', 'n');
  set(serialPort,'stopbits', 1);
  set(serialPort,'timeout', 255); % 12.3 Seconds as an example here
  % Optional commands, these can be 'on' or 'off'
  %set(s1, 'requesttosend', 'on');
   % Sets the RTS line
  %set(s1, 'dataterminalready', 'on'); % Sets the DTR line
  srl_flush(serialPort);
  string_to_send = strcat("^SWEEP,",num2str(uint64(fMin)),","...
                    ,num2str(uint64(fMax)),",",num2str(uint64(nFreq)),"$\n")
  srl_write(serialPort,string_to_send);
  for i=1:nFreq
    raw(i,:) = str2num(ReadToTermination(serialPort, 10));
    if(mod(i,10) == 0) 
      disp(i)
    endif
  endfor
  s21 = (raw(:,1)+j*raw(:,2))./(raw(:,3)+j*raw(:,4));
##  for i=1:nFreq
##    y = (raw(i,1)+j*raw(i,2))./(raw(i,3)+j*raw(i,4));
##    s21[i]=y;
##  endfor
  % Finally, Close the port
  fclose(serialPort);
endfunction

% Load the package
pkg load instrument-control
% Check if serial support exists
if (exist("serial") != 3)
  msgbox("No Serial Support");
  return
endif

% If you wish to hardcode fMin, fMax, and Nfreq do it here, and you won't
% be bothered with entering it everytime.
if(!exist("fMin","var"))
  sweep = inputdlg({"fMin","fMax","N"},"Frequency Sweep",[1 10; 1 10; 1 3])
    if (isempty(sweep))
      fMin = 1.e6;
      fMax = 100.e6;
      nFreq = 100;
    else
      fMin = sweep{1};
      fMax = sweep{2};
      N = sweep{3};
    endif
endif

df = (fMax-fMin)/nFreq;
f=fMin:df:fMax-df;

switch (questdlg("Do you wish to load a calibration from disk?"))
  case 'Yes'
    calFile = uigetfile()
    load calFile f,H1thru;
  case 'No'  
    msgbox("Connect the through connection and hit okay.")
    H1thru = readVNA(fMin, fMax, nFreq);
end
msgbox("Now connect your DUT, and push okay.")
H1comb = readVNA(fMin, fMax, nFreq);
H1dut = H1comb./H1thru;
% Assume this is for S21.
figure(1)
plot(f,20*log10(abs(H1dut)),'bo')
xlabel('Frequency (Hz)')
title('|S_{21}|')
ylabel('(dB)')
figure(2)
plot(f,angle(H1dut)*180/pi,'bo')
xlabel('Frequency (Hz)')
title('Angle of S_{21}')
ylabel('(degrees)')


