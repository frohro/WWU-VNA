function data = readVNA(fMin, fMax, nFreq)

  if exist("/dev/ttyACM0","file")
    port ="/dev/ttyACM0"; 
  elseif exist("/dev/ttyACM1","file")
    port = "/dev/ttyACM1";
##  elseif exist(".port","file")
##    load ".port" port
##    s1 = serial(port)
##  else
##    port = inputdlg({"For example: COM1 or /dev/ttyACM0 "},"Serial Port",[1 10]);
##    save ".port" port
endif
  %port = "/dev/ttyACM0";
  try
    %serialPort = serial(port{1});
    serialPort = serial(port)
  catch
    errordlg("Problem checkinng serial port.")
    delete(".port")  % In case the port got typed incorrectly.
  end
  pause(1); % Wait a second as it takes some ports a while to wake up
  set(serialPort,'baudrate', 115200);
  set(serialPort,'bytesize', 8);
  set(serialPort,'parity', 'n');
  set(serialPort,'stopbits', 1);
  set(serialPort,'timeout', 50); 
  srl_flush(serialPort);
  f = waitbar(0,"Getting your data...");
  string_to_send = strcat("^SWEEP,",num2str(uint64(fMin)),","...
                    ,num2str(uint64(fMax)),",",num2str(uint64(nFreq)),"$\n");
  srl_write(serialPort,string_to_send);
  for i=1:nFreq
    raw(i,:) = str2num(ReadToTermination(serialPort, 10));
    waitbar(i/nFreq, f);
  endfor
  close(f);
  data = 8*(raw(:,1)+j*raw(:,2))./(raw(:,3)+j*raw(:,4));
  fclose(serialPort);
endfunction