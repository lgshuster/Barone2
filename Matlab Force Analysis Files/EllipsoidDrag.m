close all
clear all
clc
%constants
V=4.03;%cubic meters
rho = 1.225; %kg/m^3 density of air 1.225
U =8.9408; %5mph in meters per second 2.23

%variables
D = 0.01 : 0.01 :1.3716;%hight 0 to 48 inches converted to meters 1.22meters
%D = .508;
L = sqrt(V./((4/3)*pi*D));%length
%L = 1.3716;

Cd=(0.44*(D/L))+(0.016*(L/D))+(0.016*((D/L).^(1/2)));%drag coefficent
A = (pi*(2*D).^2)./4; % doubled since this uses diameter
Fd = (Cd*A*rho*U^2*(1/2));
R = L./D; %ratio

figure 
subplot(2,2,1)
plot(D,L)
xlabel('D')
ylabel('L')
title('D vs L at Volume')

subplot(2,2,2)
plot(D,Cd)
xlabel('Hight radius')
ylabel('coefficent')
title('drag coefficent at diffrent ratios')

subplot(2,2,3)
plot(D,A)
xlabel('Hight radius')
ylabel('Area')
title('Frontal area of ellipsoid')

subplot(2,2,4)
plot(D,Fd)
xlabel('Hight radius (m)')
ylabel('Drag force (N)')
title('Drag force of an ellipsoid')

figure 

subplot(3,1,1)
plot(R,Cd)
xlabel('L/D ratio')
xlim([0, 10]) 
ylabel('coefficent')
title('drag coefficent at diffrent ratios')

subplot(3,1,2)
plot(R,A)
xlabel('L/D ratio')
xlim([0, 10]) 
ylabel('Area')
title('Frontal area of ellipsoid')

subplot(3,1,3)
plot(R,Fd)
xlabel('L/D ratio')
xlim([0, 10]) 
ylabel('Drag force')
title('Drag force of an ellipsoid')