% script to define the plant of the two DOF plant. Uses linearized system
clear
clc
%positions
MotorPosX = 0.97318664;
MotorPosY = 1.1769786;
MotorPosZ = 0;
Iyy = 1.8811298;
Ixx = 1.561623;
COMx = .1174947;
COMy = .0036073;
COMz = -.21940985;

Fb = 39;
Mass = 4;

%Thrusts
syms TFRX TFLX TBRX TBLX TFRZ TFLZ TBRZ TBLZ

%States
syms Pitch Roll dPitch dRoll Height dHeight

%motor positions
MotorPosFRx = MotorPosX - COMx;
MotorPosFRy = MotorPosY - COMy;
MotorPosFRz = MotorPosZ - COMz;

MotorPosFLx = MotorPosX - COMx;
MotorPosFLy = -MotorPosY - COMy;
MotorPosFLz = MotorPosZ - COMz;

MotorPosBRx = -MotorPosX - COMx;
MotorPosBRy = MotorPosY - COMy;
MotorPosBRz = MotorPosZ - COMz;

MotorPosBLx = -MotorPosX - COMx;
MotorPosBLy = -MotorPosY - COMy;
MotorPosBLz = MotorPosZ - COMz;

TorqueRoll = TFRZ*MotorPosFRy + TFLZ*MotorPosFLy + TBRZ*MotorPosBRy + TBLZ*MotorPosBLy - Fb*Roll*sqrt(COMy^2+COMz^2);
TorquePitch = TFRX*MotorPosFRz + TFLX*MotorPosFLz + TBRX*MotorPosBRz + TBLX*MotorPosBLz...
    -(TFRZ*MotorPosFRx + TFLZ*MotorPosFLx + TBRZ*MotorPosBRx + TBLZ*MotorPosBLx) -Fb*Pitch*sqrt(COMx^2+COMz^2);

AccelRoll = TorqueRoll/Ixx;
AccelPitch = TorquePitch/Iyy;

%to maintain height, we just use a linear combination of all z
%components/mass

A = [0 1 0 0 0 0; -5.4803 0 0 0 0 0; 0 0 0 1 0 0; 0 0 -5.16 0 0 0; 0 0 0 0 0 1; 0 0 0 0 0 0];
B = [0 0 0 0 0 0 0 0;...
    0 0 0 0 .7514 -.7560 .7514 -.756;...
    0 0 0 0 0 0 0 0;...
    .1166 .1166 .1166 .1166 -.4549 -.4549 .5798 .5798;
    0 0 0 0 0 0 0 0;
    0 0 0 0 1/Mass 1/Mass 1/Mass 1/Mass];
C = [1 0 0 0 0 0; 0 0 1 0 0 0; 0 0 0 0 1 0];
D = zeros(3, 8);
kp = place(A, B, [-8+.8i -8-.8i -9+.9i -9-.9i, -2+.1i, -2-.1i]);
sys_mimo = ss(A, B, C, D);
V = (C)*(-A+B*kp)^-1 * B;
kr = transpose(transpose(V)*(V*transpose(V))^-1);
%kr(kr == Inf) = 0;
largeAngleError = ss(A, B, C, D);

A_star = [transpose(zeros(size(C))) A; zeros(size(C,1)), C];
B_star = [B; zeros(rank(C),length(B))];
ki = .1 * [0 0 0 0 1 -1 1 -1;
      0 0 0 0 -1 -1 1 1;
      0 0 0 0 1 1 1 1];