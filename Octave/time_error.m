%  This script is to investigate a time delay due to having on multiplexed A/D
%  on the MSP432.  The upshot is that shifting the sine in the down conversion
%  signal by Td makes it almost perfect.

pkg load signal;
clear all;
close all;

T=1/8000; % Sample rate.  In real life 1/8000.
ADC14CLK = 48e6;  % In real life 48e6.
Td = 16/ADC14CLK

N=2000;
ti=0:T:(N-1)*T;
tq=Td:T:(N-1)*T+Td;
t=0:T/10:(N-1)*T;

fIF = 3500;
theta = pi/6;
thetad = theta*180/pi

vin = cos(2*pi*fIF*t+theta)+j*sin(2*pi*fIF*(t-Td)+theta);
vin1 = cos(2*pi*fIF*t+theta)+j*sin(2*pi*fIF*(t)+theta);
dc=cos(2*pi*fIF*t)-j*sin(2*pi*fIF*(t-Td));
x=dc.*vin;
x1 = exp(-j*2*pi*fIF*t).*vin1;
temp1=mean(x1)
temp=mean(x)
mag1=abs(temp1)
mag=abs(temp)
ang=angle(temp);
ang1=angle(temp1);
angd1=ang1*180/pi
angd=ang*180/pi