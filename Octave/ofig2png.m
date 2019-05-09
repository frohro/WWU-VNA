
msgbox("Select the .ofig octave figure file you wist to convert to png.\
  Then select the file to save it as with .png on the end.");

[figFile,figFilePath] = uigetfile();
if (figFile != 0)
  filename = fullfile(figFilePath,figFile);
  h1 = hgload(filename);
else
  msgbox("Sorry, I didn't get that.");
endif
[saveFile,saveFilePath] = uiputfile();
if (saveFile != 0)
  filename = fullfile(saveFilePath,saveFile);
  print( h1, filename,"-dpng"); 
else
  msgbox("Sorry, I didn't get that.");
endif
