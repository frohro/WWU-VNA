% VNA 2018 VNA test script.
% Rob Frohne, May 2017.
% This script will make nFreq measurements from fMin to fMax.
% It is designed to make S11 measurements and use calibration.
% The first step is to connect a reflection bridge to the analyzer
% and measure the reflection for a short, then an open and then a 50 
% ohm load, and lastly the DUT (device under test).

clc; clear;
close all;

Z0=50;

notDone = true;
if(!exist("fMin","var"))
  [fMin, fMax, nFreq] = getSweep();
endif

% Load the package
pkg load instrument-control
% Check if serial support exists
if (exist("serial") != 3)
  disp("No Serial Support");
endif

if (!exist("Z0","var"))
  Z0 = inputdlg({"Z_0 "},"Characteristic Impedance",[1 3]);
endif
switch (questdlg("Do you wish to load a calibration from disk?"));
  case 'Yes'
    [calFile,calFilePath] = uigetfile();
    if (calFile != 0)
      filename = fullfile(calFilePath,calFile);
      load(filename, 'fMin', 'fMax', 'nFreq', 'gamma_s', 'gamma_o', 'gamma_l');
    endif
  case {'No' 'Cancel'}  
    msgbox("Connect the short connection and hit return.");
    gamma_s = readVNA(fMin, fMax, nFreq);
    msgbox("Connect the open connection and hit return.");
    gamma_o = readVNA(fMin, fMax, nFreq);
    msgbox("Connect the Z_0 load connection and hit return.")
    gamma_l = readVNA(fMin, fMax, nFreq);
    switch (questdlg("Do you wish to save a calibration from disk?"));
      case 'Yes'
        [calFile,calFilePath] = uiputfile();
        if file != 0
          filename = fullfile(calFilePath,calFile);
          save(filename, 'fMin', 'fMax', 'nFreq', 'gamma_s', 'gamma_o', 'gamma_l');
        endif
       case {'No' 'Cancel'}
    end    
end
while notDone
  msgbox("Now connect your DUT, and push return.");
  gamma_m = readVNA(fMin, fMax, nFreq);
  % Do calibration.
  gamma_u = SOL(gamma_s, gamma_o, gamma_l, gamma_m);
  Z = Z0*(1+gamma_u)./(1-gamma_u);
  % Plot it on the Smith Chart.
  fileTitle = inputdlg({"Title"},"Sweep Title",[1 30]){1};
  figHandle = figure("name",fileTitle)
  SmithChart(gamma_u, figHandle, '-or');
  title('S_{11}');
  switch(questdlg("Do you wish to measure another network using this calibration?"));
    case 'Yes'
      notDone = true;
    case {'No' 'Cancel'}
      notDone = false;
  end
end
