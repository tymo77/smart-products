test_image_label = imread('test_image_label.bmp');

imgpts1 = [
    80 47;
    75 50;
    71 53;
    67 55;
    80 42;
    75 45;
    71 48;
    67 50;
    79 37;
    74 40;
    70 43;
    66 46;
    79 32;
    75 35;
    70 38;
    66 40;
    ];

wrldpts1 = [
    1 0 1;
    2 0 1;
    3 0 1;
    4 0 1;
    1 0 2;
    2 0 2;
    3 0 2;
    4 0 2;
    1 0 3;
    2 0 3;
    3 0 3;
    4 0 3;
    1 0 4;
    2 0 4;
    3 0 4;
    4 0 4;
];

imgpts2 = [
    83 53
    80 56
    76 59
    71 62
    88 56
    85 59
    80 62
    76 65
    92 58
    89 62
    85 64
    81 68
    97 61
    93 64
    89 68
    85 71
];

wrldpts2 = [
    1 1 0
    2 1 0
    3 1 0
    4 1 0
    1 2 0
    2 2 0
    3 2 0
    4 2 0
    1 3 0
    2 3 0
    3 3 0
    4 3 0
    1 4 0
    2 4 0
    3 4 0
    4 4 0
];

imgpts3 = [
    87 46;
    91 49;
    96 52;
    100 54;
    87 42;
    92 45;
    95 47;
    101 50;
    87 37;
    91 40;
    96 42;
    101 45;
    87 32;
    92 35;
    96 37;
    101 39;
];

wrldpts3 = [
    0 1 1
    0 2 1
    0 3 1
    0 4 1
    0 1 2
    0 2 2
    0 3 2
    0 4 2
    0 1 3
    0 2 3
    0 3 3
    0 4 3
    0 1 4
    0 2 4
    0 3 4
    0 4 4
];

wrldpts = [wrldpts1; wrldpts2; wrldpts3]*8.5;
imgpts = [imgpts1; imgpts2; imgpts3];
%%
N = length(imgpts);

p = reshape(imgpts',[2*N 1]);

d = @(P,p) [...
    P(1) P(2) P(3) 1 0 0 0 0 -P(1)*p(1) -P(2)*p(1) -P(3)*p(1);...
    0 0 0 0 P(1) P(2) P(3) 1 -P(1)*p(2) -P(2)*p(2) -P(3)*p(2)];

D = zeros(N,11);

for i = 1:N
   
    Di = d(wrldpts(i,:),imgpts(i,:));
    D(2*i-1,:) = Di(1,:);
    D(2*i,:) = Di(2,:);
    
end

m = (inv(D'*D)*D')*p;

M = [m(1:4)'; m(5:8)'; m(9:11)' 1];

save('Mmatrix.mat','M');

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

error = zeros(N,1);
proj_pts = zeros(N,2);
for i = 1:N
    projection = M * [wrldpts(i,:)'; 1];
    proj_pts(i,:) = projection(1:2)'/projection(3);
    
    err_vec = imgpts(i,:)' - proj_pts(i,:)';
    error(i) = dot(err_vec,err_vec);
    
end
mse = sum(error)/N;

P_camera_origin = inv(S)*[0;0;0;1];

close all;
image(test_image_label);
hold on;
axis equal;
plot(imgpts(:,1),imgpts(:,2),'r.','MarkerSize',20);
plot(proj_pts(:,1),proj_pts(:,2),'bo','MarkerSize',10,'LineWidth',3);

%% Test backward ray projectiong
close all;
figure;
hold on; view(3); grid on;
scatter3(wrldpts(:,1),wrldpts(:,2),wrldpts(:,3),200,'.');
scatter3(P_camera_origin(1), P_camera_origin(2), P_camera_origin(3),200,'x','LineWidth',3);
axis equal;

for i = 1:length(imgpts)
    [v] = project_line(imgpts(i,:),M);
    p1 = P_camera_origin(1:3);
    p2 = 250*v + p1;
    line_pts = [p1'; p2'];
    plot3(line_pts(:,1),line_pts(:,2),line_pts(:,3),'r','LineWidth',3);
end

%% Test the two distance functions. They should be the same.
close all;
r_c = 10;
p_c = imgpts(40,:);


r_clist = linspace(1,120,100);
d1_list = 0*r_clist;
d2_list = d1_list;

R_sphere = 20;%mm regulation ping pong ball
for i = 1:length(r_clist)
    d1_list(i) = dist2Sphere(p_c,r_clist(i),M,R_sphere);
    d2_list(i) = dist2Sphere2(p_c,r_clist(i),M,R_sphere,0.001,100);
end

figure; hold on;
plot(r_clist,(d1_list - d2_list)./d1_list*100)




