% -- S = cascadeS1S2(S1,S2)
%
% From:
%   --S1--S2--
% To:
%      --S--
%
%   --S is the resulting composite scattering matrix, (2x2xm), where m allows
%       these to be a function of frequency if you like.
%   --S1 is the left scattering matrix, (2x2xm), where m allows
%       these to be a function of frequency if you like.
%   --S2 is the right scattering matrix, (2x2xm), where m allows
%       these to be a function of frequency if you like.
%   
% This function calculates the composite scattering matrix, give the scattering
% matrices of the cascaded networks.  It uses the Redheffer rules derived in Rob
% Frohne's Class Notes for Propagation and Radiation, January 21, 2005.
%
% To start with we will only do 2x2 S matrices, because that is what we need
% now, but the derivation is much more general.

function S = cascadeS1S2(S1,S2)
  for m =1:size(S1,3)
    S(1,1,m) = S1(1,1,m)+S1(1,2,m)/(1-S2(1,1,m)*S1(2,2,m))*S2(1,1,m)*S1(2,1,m);
    S(1,2,m) = S2(1,2,m)/(1-S1(2,2,m)*S2(1,1,m))*S1(1,2,m);
    S(2,1,m) = S1(2,1,m)/(1-S2(1,1,m)*S1(2,2,m))*S2(2,1,m);
    S(2,2,m) = S2(2,2,m)+S2(2,1,m)/(1-S1(2,2,m)*S2(1,1,m))*S1(2,2,m)*S2(1,2,m);
  endfor
end

