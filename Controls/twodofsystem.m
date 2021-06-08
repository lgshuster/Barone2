clc
clear

%script to help create plant for the drone. Should help with updates to
%the system definition and with tweaking stuff later on. 

%provide X vector here
syms pn dpn pe dpe pd dpd pitch dpitch yaw dyaw roll droll Fb;
x = transpose([pitch dpitch roll droll]);

%dx vector
syms ddpn ddpe ddpd ddpitch ddyaw ddroll
dx = transpose([dpitch ddpitch droll ddroll]);

%system inputs
syms Tflx Tflz Tfrx Tfrz Tblx Tblz Tbrx Tbrz Fb Fg
u = transpose([Tflx Tflz Tfrx Tfrz Tblx Tblz Tbrx Tbrz Fb]);

%other consatnts
syms CMflx CMfly CMflz CMfrx CMfry CMfrz ...
    CMblx CMbly CMblz CMbrx CMbry CMbrz mpd cmcbx cmcby cmcbz cosyaw ...
    sinyaw sinafl sinafr sinabl sinabr cosafl ...
    cosafr cosabl cosabr;

%rotation matrix
Rvb =[           cos(yaw)                          sin(yaw)              -pitch;
      roll*pitch*cos(yaw)-cos(yaw)*sin(yaw) roll*pitch*sin(yaw)+cos(yaw) roll*pitch;
          pitch*cos(yaw)+roll*sin(yaw)      pitch*sin(yaw)-roll*cos(yaw)     1];
              
%drag matrix
syms d1 d2 d3 d4 d5 d6 d7 d8 d9
dragMatrix = [d1 d2 d3; d4 d5 d6; d7 d8 d9];

%Inertia Matrix
syms ixx ixy ixz iyy izz iyz
iDrone = [ixx ixy ixz; ixy iyy iyz; ixz iyz izz];

ddAngle = (iDrone)^-1*(Rvb * ([Tfrx;0;Tfrz]*([CMfrx,CMfry,CMfrz]+mpd*[Tfrx, 0, Tfrz])+...
    [Tflx;0;Tflz]*([CMflx,CMfly,CMflz]+mpd*[Tflx, 0, Tflz])+...
    [Tbrx;0;Tbrz]*([CMbrx,CMbry,CMbrz]+mpd*[Tbrx, 0, Tbrz])+...
    [Tblx;0;Tblz]*([CMblx,CMbly,CMblz]+mpd*[Tblx, 0, Tblz]))+...
    cross((Rvb*[cmcbx;cmcby;cmcbz]),([0;0;-Fb])));

ddpitch = ddAngle(1);
ddyaw = ddAngle(2);
ddroll = ddAngle(3);

dpitch = dpitch + ddpitch;
droll = droll + ddroll;

%to find A and B
%take partial derivatives with respect to state vector
A = sym(length(x));
B = sym([length(x),length(u)]);

%dx = transpose([dpitch ddpitch droll ddroll]);
dx = transpose([dpitch ddpitch droll ddroll 0]);
for i1 = 1:length(x)
    for i2 = 1:length(x)
        A(i1,i2) = diff(dx(i1), x(i2));
    end
    
    for i3 = 1:length(u)
        B(i1,i3) = diff(dx(i1), u(i3));
    end
end

% substitute constants in matrices for solution
%dx sub

Is = [ixx ixy ixz iyy iyz izz];%dont touch this one
drags = [d1 d2 d3 d4 d5 d6 d7 d8 d9];%or this one
motorS = [CMflx CMfly CMflz CMfrx CMfry CMfrz CMblx CMbly CMblz CMbrx CMbry CMbrz];%still no
mpds = mpd;%nuh uh
buoyancys = [cmcbx cmcby cmcbz];%almost there
angs = [sinafl sinafr sinabl sinabr cosafl cosafr cosabl cosabr]; %next one!

%edit these for plugging in values. These are operating points
dxsub = [ 0 0 0 0 0];
usub = [0 .25 0 .25 0 .25 0 .25 39];
pyrsub = [0 0 0];
angsub = [1 1 1 1 .01 .01 .01 .01];

%These ones are constants. Plug in here
com = [.1174947, .0036073, -.21940985];
posFR = [0.97318664 1.1769786 0];
Isub = [1.561623 -.01205778 -.0547494 1.8811298 -.00144385 2.7494228];
dragsub = [0.536912752 0 0 0 0.536912752 0 0 0 0.536912752];
motorsub = [(posFR(1)-com(1)) (-posFR(2)-com(2)) -com(3) (posFR(1)-com(1)) (posFR(2)-com(2)) -com(3) (-posFR(1)-com(1)) (-posFR(2)-com(2)) -com(3) (-posFR(1)-com(1)) (posFR(2)-com(2)) -com(3)];
mpdsub = .04604;
buoyancysub = -com;

%for reference in plugging in
% dxsub = [dpn ddpn dpe ddpe dpd ddpd dpitch ddpitch dyaw ddyaw droll ddroll];
% usub = [Tflx Tflz Tfrx Tfrz Tblx Tblz Tbrx Tbrz Fb Fg];
% pyrsub = [pitch yaw roll];
% Isub = [ixx ixy ixz iyy iyz izz];
% dragsub = [d1 d2 d3 d4 d5 d6 d7 d8 d9];
%motorS = [CMflx CMfly CMflz CMfrx CMfry CMfrz CMblx CMbly CMblz CMbrx CMbry CMbrz];
%mpd = mpd;
%buoyancysub = [cmcbx cmcby cmcbz];

sb = horzcat(pyrsub,dxsub, usub, Isub, dragsub, motorsub, mpdsub, buoyancysub, angsub, 39);

An=double(subs(A, horzcat([pitch, yaw, roll], transpose(dx), transpose(u),...
    Is,drags, motorS, mpds, buoyancys, angs, Fb), sb));
Bn=double(subs(B, horzcat([pitch, yaw, roll], transpose(dx), transpose(u),...
    Is, drags, motorS, mpds, buoyancys, angs, Fb), sb));

C = [1 0 0 0 
     0 0 1 0];

D = zeros(2,9);

states = {'Pitch' 'dpitch' 'Roll' 'droll'};
inputs = {'Tflx' 'Tflz' 'Tfrx' 'Tfrz' 'Tblx' 'Tblz' 'Tbrx' 'Tbrz' 'Fb'};
outputs = {'Pitch' 'Roll'};

sys_mimo = ss(An, Bn, C, D, 'statename', states, 'inputname', inputs,...
    'outputname', outputs);
