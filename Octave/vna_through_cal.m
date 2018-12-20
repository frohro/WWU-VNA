% VNA 2018 VNA test script.
% Rob Frohne, May 2017.
% This script will make nFreq measurements from fMin to fMax.
% It is designed to make S21 measurements and use calibration.
% The first step is to connect your cables together, and do a
% a through calibration measurement.  Then you can measure the DUT.
% It will divide out the transfer function of the cables, so they
% don't mess up your measurement of the DUT.


% Start script here.
clc; clear;
close all;
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
  sweep = inputdlg({"fMin","fMax","N"},"Frequency Sweep",[1 10; 1 10; 1 3]);
    if isempty(sweep)
      fMin = 1.e6;
      fMax = 100.e6;
      nFreq = 100;
    elseif (isempty(sweep{1})||isempty(sweep{2})||isempty(sweep{3}))
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

switch (questdlg("Do you wish to load a calibration from disk?"));
  case 'Yes'
    calFile = uigetfile();
    load calFile f,H1thru;
  case 'No'  
    msgbox("Connect the through connection and hit okay.");
    H1thru = readVNA(fMin, fMax, nFreq);
    switch (questdlg("Do you wish to save a calibration from disk?"));
      case 'Yes'
        calFile = uiputfile();
        save calFile f,H1thru;
    end    
end
msgbox("Now connect your DUT, and push okay.");
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


