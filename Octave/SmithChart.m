function SmithChart(S, style_input, VSWRcircle)
%--------------------------------------------------------------------------
%SmithChart -- this function takes an input vector "S", the Sii parameter of
%a given system, and plots it on an impedance smith chart, along with the
%option of plotting VSWR circles.
%--------------------------------------------------------------------------
%author:  Steven Holland 
%
%--------------------------------------------------------------------------
%OPTIONS:  in addition to specifying the s-parameter vector, the linestyle
%of the impedance loci can be set, and VSWR circles (constant reflection)
%circles can be plotted at an arbitraty VSWR level.
%
%Loci linestyle:  the second input argument should be of the same form used
%in the regular plot function in matlab, i.e. 'r' for red, '--r' for dotted
%dashed line, etc.
%
%example usage:  SmithChart(S11, '-.k') --> this plots the S11 vector as a
%dash-dotted line in the color black.

%VSWR Circles: the third input argument is the circle(s) desired, and this
%can be input in either a single value (for 1 circle), or can be entered as
%a vector in i that contains the VSWR levels.
%
%
%example usage:  SmithChart(S11, '-or', 2) --> this plots the S11 vector as a
%line with circle point markers in the color red, with a VSWR circle of 2:1 plotted.
%
%example usage:  SmithChart(S11, '-or', [2, 5, 7]) --> this plots the S11 vector as a
%line with circle point markers in the color red, with a VSWR circle of
%2:1, 5:1, and 7:1 plotted on the smith chart.
%--------------------------------------------------------------------------

if nargin < 1
    error('Requires 1 or more input arguments.')
elseif nargin == 1
    LINE_STYLE = 'b';
    VSWRcircles = 0;
elseif nargin == 2
    LINE_STYLE = style_input;
    VSWRcircles = 0;
elseif nargin == 3
    LINE_STYLE = style_input;
    VSWRcircles = 1;
    VSWR_CIRCLE = VSWRcircle;
elseif nargin > 3
    error('Too many arguments!!')
end

%--------------------------------------------------------------------------
%--------------------------------------------------------------------------

plot([-1 1],[0 0],'k')
axis equal
axis off
hold on

for r = [0 .2 .5 1 2 5]         % specify the r=const lines you want to draw
        rr = 1/(r+1);
        cr = 1-rr;
        tr = 2*pi*(0:.01:1);
        plot(cr+rr*cos(tr),rr*sin(tr),'k')
end

for x = [.2 .5 1 2 5]           % specify the x=const lines you want to draw
        rx = 1/x;
        cx = rx;
        tx = 2*atan(x)*(0:.01:1);
        plot(1-rx*sin(tx),cx-rx*cos(tx),'k')
        plot(1-rx*sin(tx),-cx+rx*cos(tx),'k')
end

text(-1.05,0,'0','fontsize',8)
text(-1.05,.4,'+j.2','fontsize',8)
text(-1.05,-.4,'-j.2','fontsize',8)
text(-.7,.85,'+j.5','fontsize',8)
text(-.7,-.85,'-j.5','fontsize',8)
text(-.05,1.05,'+j1','fontsize',8)
text(-.05,-1.05,'-j1','fontsize',8)
text(.6,.85,'+j2','fontsize',8)
text(.6,-.85,'-j2','fontsize',8)
text(.95,.4,'+j5','fontsize',8)
text(.95,-.4,'-j5','fontsize',8)
text(-.72,0.03,'.2','fontsize',8)
text(-.38,0.03,'.5','fontsize',8)
text(-.03,0.03,'1','fontsize',8)
text(.3,0.03,'2','fontsize',8)
text(.62,0.03,'5','fontsize',8)
%--------------------------------------------------------------------------
%--------------------------------------------------------------------------
if VSWRcircles == 1
    ANGLE = linspace(0, 2*pi, 100);
    if size(VSWR_CIRCLE,2) == 1
        GAMMA = (VSWR_CIRCLE - 1)/(VSWR_CIRCLE + 1);
        plot(GAMMA.*cos(ANGLE), GAMMA.*sin(ANGLE), '-.k', 'LineWidth', 2);
    else
        GAMMA = (VSWR_CIRCLE - 1)./(VSWR_CIRCLE + 1);
        N = size(VSWR_CIRCLE,2)
        for n = 1:N
            plot(GAMMA(n).*cos(ANGLE), GAMMA(n).*sin(ANGLE), '-.k', 'LineWidth', 2);
        end
    end
end

 fh = figure(1); 
 set(fh, 'color', 'white'); % sets the color to white
 hold on

plot(S, LINE_STYLE, 'LineWidth', 2)
endfunction


