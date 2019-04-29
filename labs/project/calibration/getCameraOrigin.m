function P = getCameraOrigin(M)

A = M(1:3,1:3);
b = M(1:3,4);

a1 = A(1,:)';
a2 = A(2,:)';
a3 = A(3,:)';

a13 = cross(a1,a3);
a23 = cross(a2,a3);


rho = 1/norm(a3);
xc_o = rho^2 * (dot(a1,a3));
yc_o = rho^2 * (dot(a2,a3));
theta = acos(-(dot(a13,a23))/(norm(a13)*norm(a23)));
alpha_x = rho^2*norm(a13)*sin(theta);
alpha_y = rho^2*norm(a23)*sin(theta);

K = [alpha_x -alpha_x*cos(theta) xc_o;
    0 alpha_y/sin(theta) yc_o;
    0 0 1];

r3 = rho*a3;
r1 = a23/norm(a23);
r2 = cross(r3,r1);
t = rho*inv(K)*b;

R = [r1';r2';r3'];

S = [R t];
S = [S;0 0 0 1];

P = inv(S)*[0;0;0;1];
P = P(1:3);

end