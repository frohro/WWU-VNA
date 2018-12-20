function [fMin, fMax, nFreq] =  getSweep()
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
endfunction
