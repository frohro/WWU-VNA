% This script is to make sure the DSP in the 2017 VNA is correct.
% The A/D converters are not synchronized so the Q is delayed by
% Td from the I signal.  From this it appears I can't do anything
% with FIR filters to improve the sampling error, it is so small. 

pkg load signal;
clear all;
close all;

T=1/8000; % Sample rate.  In real life 1/8000.
ADC14CLK = 48e6;  % In real life 48e6.
Td = 16/ADC14CLK

N=200;
ti=0:T:(N-1)*T;
tq=Td:T:(N-1)*T+Td;
t=0:T/10:(N-1)*T;

fIF = 200;
theta = 0;

via = cos(2*pi*fIF*t+theta);
vi = cos(2*pi*fIF*ti+theta);
vq = sin(2*pi*fIF*(tq-Td)+theta);
vqa = sin(2*pi*fIF*t+theta);

% Compensate for delay here.
f = -1/2/T:1/N/T:(N-1)/2/N/T;

H = exp(j*2*pi*f*Td);%.*kaiser(N)';
Hsh = fftshift(H);
h=real(ifft(Hsh));
h(1)= 1;
%h = h.*kaiser(N)';

vqs = filter(h,1,vq);

plot(tq,vq,'r*',t,vqa)

