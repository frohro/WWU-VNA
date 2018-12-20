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

% Put fMin, fMax, and nFreq here, if you want to avoid all the dialogs.

notDone = true;
pkg load instrument-control
% Check if serial support exists
if (exist("serial") != 3)
  msgbox("No Serial Support");
  return
endif

switch (questdlg("Do you wish to load a calibration from disk?"));
  case 'Yes'
    calFile = uigetfile();
    if calFile != 0
      load (calFile, 'fMin', 'fMax','nFreq', 'H1thru','H1iso');
    else
      return
    endif
  case {'No' 'Cancel'}
    if(!exist("fMin","var"))
      [fMin, fMax, nFreq] = getSweep()
    endif
    msgbox("Connect the through connection and hit okay.");
    H1thru = readVNA(fMin, fMax, nFreq);
    msgbox("Disconnect the through connection and hit okay.");
    H1iso = readVNA(fMin, fMax, nFreq);
    switch (questdlg("Do you wish to save a calibration from disk?"));
      case 'Yes'
        calFile = uiputfile();
        if calFile != 0
          save(calFile, 'fMin', 'fMax','nFreq', 'H1thru','H1iso');
        endif
    end    
end
while (notDone)
  msgbox("Now connect your DUT, and push okay.");
  H1comb = readVNA(fMin, fMax, nFreq);
  H1comb = H1comb - H1iso;
  H1dut = H1comb./H1thru;
  
  df = (fMax-fMin)/nFreq;
  f=fMin:df:fMax-df;
  % Assume this is for S21.
  fileTitle = inputdlg({"Title"},"Sweep Title",[1 30]){1};
  if fileTitle !=0 
    figure("name",fileTitle)
  endif
  plot(f,20*log10(abs(H1dut)),'bo')
  xlabel('Frequency (Hz)')
  title('|S_{21}|')
  ylabel('(dB)')
  figure("name",fileTitle)
  plot(f,angle(H1dut)*180/pi,'bo')
  xlabel('Frequency (Hz)')
  title('Angle of S_{21}')
  ylabel('(degrees)')
  switch(questdlg("Do you wish to measure another network using this calibration?"));
    case 'Yes'
      notDone = true;
    case 
      notDone = false;
  end
end

