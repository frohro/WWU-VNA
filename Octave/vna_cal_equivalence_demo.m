% This a test program to verify the VNA one port calibration is what I expect
% in the vna.py GUI.
% First I want to see if my way of doing it is equivalent to what is used
% in vna.py, and others.
% get some random data to check with.
gamma_o = rand(100,1).*exp(j*((rand(100,1)-.5)*pi));
gamma_s = rand(100,1).*exp(j*((rand(100,1)-.5)*pi));
gamma_l = rand(100,1).*exp(j*((rand(100,1)-.5)*pi));
gamma_m = rand(100,1).*exp(j*((rand(100,1)-.5)*pi));

beta = (gamma_o - gamma_l)./(gamma_l -gamma_s);
rp = (beta -1)./(beta + 1);
alpha = (gamma_l - gamma_s).*(2*beta./(beta+1));

gamma_u = (gamma_m -gamma_l)./((gamma_m -gamma_l).*rp +alpha);

z_u = 50*(1 + gamma_u)./(1-gamma_u)

% formula from vna.py
%  z = np.divide(50.0 * (open - load) * (dut - short), (load - short) * (open - dut))

z_c = 50*(gamma_o - gamma_l).*(gamma_m - gamma_s)./((gamma_l -gamma_s).*(gamma_o-gamma_m));
max (abs(z_u -z_c))  % Is zero to 10 digits.

%  This demonstrates they are the same.  Now just check to see if they recalculate 
%  the gamma_s from the corrected z.

