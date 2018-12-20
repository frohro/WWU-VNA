% This function is to do an short, open, load (SOL) one port calibration
% for vector network analyzer calibration measurements.
% This is shown 
% This is worked out in my Electronics II class notes.  
%
%    The first photo shows how to calculate the reflection in a cascade of two 
%    scattering matrices.  Here is a link:  (See May 25, 2016.)
%    http://fweb.wallawalla.edu/~frohro//ClassNotes/ENGR357/2016/Background/index.php
%    Using this relationship, the second network is simplified into a one port 
%    so reflection r_2 = Gamma_L, the actual reflection at the DUT.  Then using 
%    this, the three different reflection coefficients (with DUT as open, short, 
%    and load), the reflection at the instrument is written down.
%    These are solved simultaneously to give the three independent parameters 
%    that describe the unknown scattering matrix between the instrument and DUT.  
%    (There is no photo of the algebra, just the three equations to start from.)
%    Then the unknown DUT reflection is measured at the instrument, and the 
%    reflection at the DUT is solved for.  This reflection is the calibrated 
%    measurement.
%
% The notes don't show all the algebra, but the problem is set up.  The algebra
% is done in a file in my Classes/Electronics/VNA Calibration One Port SLO.pdf
% gamma_m is the measured reflection for the DUT and gamma_u is the calibrated
% gamma returned.
% gamma_u = SOL(gamma_s, gamma_o, gamma_l, gamma_m);

function gamma_u = SOL(gamma_s, gamma_o, gamma_l, gamma_m)
  beta = (gamma_o - gamma_l)./(gamma_l -gamma_s); % beta is (1+r')/(1-r')
  rp = (beta -1)./(beta + 1);
  alpha = (gamma_l - gamma_s).*(2*beta./(beta+1)); % alpha is t*t' from web.
  gamma_u = (gamma_m -gamma_l)./((gamma_m -gamma_l).*rp +alpha);
end