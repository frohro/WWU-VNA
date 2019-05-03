function data = readVNA(fMin, fMax, nFreq)
  % This is the function than reads VNA data from the serial port.
  %s1 = serial("/tmp/ttyDUMMY"); % $ interceptty /dev/ttyACM0 /tmp/ttyDUMMY
  if(exist(".port","file"))
    load ".port" port
  else
    port = inputdlg({"For example: COM1 or /dev/ttyACM0 "},"Serial Port",[1 10]);
    save ".port" port
  endif
  try
    serialPort = serial(port{1});
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
  data = (raw(:,1)+j*raw(:,2))./(raw(:,3)+j*raw(:,4));
  fclose(serialPort);
endfunction