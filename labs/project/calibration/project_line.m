function [v] = project_line(pc,M)
%%% Get the parameterized equation for a line p = v*t + v0 based on the
%%% camera coordinates pc and camera calibration matrix M.

m11 = M(1); m12 = M(4); m13 = M(7); m14 = M(10);
m21 = M(2); m22 = M(5); m23 = M(8); m24 = M(11);
m31 = M(3); m32 = M(6); m33 = M(9); m34 = M(12);

u = pc(1);
v = pc(2);

a1 = u*m31 - m11;
b1 = u*m32 - m12;
c1 = u*m33 - m13;
d1 = u*m34 - m14;

a2 = v*m31 - m21;
b2 = v*m32 - m22;
c2 = v*m33 - m23;
d2 = v*m34 - m24;


v = [...
    ((b2*c1)/(a2*b1 - a1*b2) - (b1*c2)/(a2*b1 - a1*b2));...
    (-((a2*c1)/(a2*b1 - a1*b2)) + (a1*c2)/(a2*b1 - a1*b2));...
    1 ...
    ];

v = -v / norm(v);
% v0 = [...
%     (b2*d1)/(a2*b1 - a1*b2) - (b1*d2)/(a2*b1 - a1*b2);...
%     -((a2*d1)/(a2*b1 - a1*b2)) + (a1*d2)/(a2*b1 - a1*b2);...
%     0 ...
%     ];
end