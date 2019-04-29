clearvars; close all;
load('Mmatrix.mat')


R_ball = 42.037/2; % Ping pong balls are about 20mm in radius.
[Xball,Yball,Zball] = sphere();
Xball = Xball*R_ball; Yball = Yball*R_ball; Zball = Zball*R_ball;

% Calculate camera parameters from calibration matrix M
P_cam_origin = getCameraOrigin(M);

%% Step 1: Gather a bunch of points P_W_i and p_c_star_i, r_c_star

camera_xyrrgb = dlmread('Camera-Robot-Calibration/camera_xyr.txt',',',1,0);% format: header + index, x, y, r
ball_xyz = dlmread('Camera-Robot-Calibration/ball_xyz.txt',',',1,0);% format: header + index, x, y ,z
robot_xyz = dlmread('Camera-Robot-Calibration/robot_xyz.txt',',',1,0);% format: header + index, x, y, z

Nballs = max(ball_xyz(:,1));
Npts = length(robot_xyz);

%Correct for z offset from center of ball due to radius.

ball_xyz(:,4) = ball_xyz(:,4) - R_ball;


%% Compute average ball position and plot.
ball_avg_xyz = zeros(Nballs,3);
for i = 1:Nballs
    count = 0;
    temp = [0 0 0];
    for j = 1:length(ball_xyz)
        if ball_xyz(j,1) == i
            temp = temp + ball_xyz(j,2:4);
            count = count + 1;
        end
    end
    ball_avg_xyz(i,:) = temp / count;
end

ball_avg_xyz(:,3) = ball_avg_xyz(:,3);

figure;
plot3(ball_xyz(:,2),ball_xyz(:,3),ball_xyz(:,4),'x'); axis equal; hold on;
plot3(ball_avg_xyz(:,1),ball_avg_xyz(:,2),ball_avg_xyz(:,3),'o')
grid on;

%% Convert data structures to be ordered by row.
ball_indices = camera_xyrrgb(:,1);
pxl_data = [(1:Npts)' camera_xyrrgb(:,2:4)];
pxl_colr = [(1:Npts)' camera_xyrrgb(:,5:7)];
wrld_data = [(1:Npts)' ball_avg_xyz(ball_indices,:)];
robot_pos = [(1:Npts)' robot_xyz(:,2:4)];

%% Get the points in the C frame and L3 frames respectively using sphere projection size.
Npts = length(pxl_data);
P_C = zeros(Npts,3);
P_L3 = zeros(Npts,3);

for i = 1:Npts
    
    p_c = pxl_data(i,2:3)';  % Get pixel coordinates.
    r_c = pxl_data(i,4);    % Get pixel radius.
    
    P_W = wrld_data(i,2:4)';     % Get the position of the feature in world.
    P_rob = robot_pos(i,2:4)';   % Get the position of the robot.
    
    theta = atan2(P_rob(2),P_rob(1)); % Get the angle of the robot from X.
    R = rotMatZ(-theta);
    P_L3(i,:) = (R*(P_W - P_rob))'; % Get the position of the feature in the link-fixed frame L3.
    
    v = project_line(p_c,M); % Get the direction vector of the feaure in Camera frame.
    d = dist2Sphere(p_c,r_c,M,R_ball); % Get the distance to the ball along the vector.
    P_C(i,:) = (v*d + P_cam_origin); % Get the estimated feature position in Camera frame.
end

%% Calculate transformation from the Camera (C) to Link (L3).

[R_CtoL3, t_CtoL3] = rigid_transform_3D(P_C,P_L3);

T_CtoL3 = [R_CtoL3, t_CtoL3; 0 0 0 1];

%% Fminunc to get optimal transformation that minimizes each points distance to its ray.

pts_L3 = zeros(Npts,3);
for i = 1:Npts
    P_R = robot_pos(i,2:4);
    t_W_L3fromW = P_R';
    theta = atan2(P_R(2),P_R(1)); % Get the angle of the robot from X.
    R_L3toW = rotMatZ(theta);
    T_L3toW = [R_L3toW t_W_L3fromW;0 0 0 1];
    
    temp = [wrld_data(i,2:4) 1]';
    temp = inv(T_L3toW)*temp;
    pts_L3(i,:) = temp(1:3)';
end


totalDistance(pts_L3,pxl_data(:,2:3),0,0,0,0,0,0,M)
f = @(x) totalDistance(pts_L3,pxl_data(:,2:3),x(1),x(2),x(3),x(4),x(5),x(6),M);
lb = [-200,-200,-200,0,0,0];
ub = [200,200,200,2*pi,2*pi,2*pi];
[x,fval] = fmincon(f,zeros(6,1),[],[],[],[],lb,ub);

R_L3toC = rotMatZ(x(6))*rotMatY(x(5))*rotMatZ(x(4));
t_C_L3fromC = x(1:3);

T_L3toC = [R_L3toC, t_C_L3fromC; 0 0 0 1];

%% Plot projected points in world frame using the sphere size projection method.
close all;
figure; hold on; view(3);
projected_pts = zeros(Npts,3);
for i = 1:Npts
    P_R = robot_pos(i,2:4);
    t_W_L3fromW = P_R';
    theta = atan2(P_R(2),P_R(1)); % Get the angle of the robot from X.
    R_L3toW = rotMatZ(theta);
    T_L3toW = [R_L3toW t_W_L3fromW;0 0 0 1];
    
    
    p_c = pxl_data(i,2:3);
    r_c = pxl_data(i,4);
    v = project_line(p_c,M); % Get the direction vector of the feaure in Camera frame.
    d = dist2Sphere(p_c,r_c,M,R_ball); % Get the distance to the ball along the vector.
    P_C = [(v*d + P_cam_origin); 1]; % Get the estimated feature position in Camera frame.
    P_L3 = T_CtoL3*P_C; % Get feature position in L3 frame;
    P_W = T_L3toW*P_L3; % Estimated feature position in World frame
    
    P_W = P_W(1:3);
    
    projected_pts(i,:) = P_W';
    
end
figure; hold on; view(3); grid on; axis equal;
plot3(ball_avg_xyz(:,1),ball_avg_xyz(:,2),ball_avg_xyz(:,3),'o')
plot3(projected_pts(:,1),projected_pts(:,2),projected_pts(:,3),'x')% Position of feature in world frame
xlabel('x'); 
ylabel('y');
zlabel('z');

%% Transform projection vector and position in C to L3 to W, them use plane intersection method.
close all;
surf_height = mean(ball_avg_xyz(:,3));

for i = 1:Npts
    
    P_R = robot_pos(i,2:4);
    t_W_L3fromW = P_R';
    theta = atan2(P_R(2),P_R(1)); % Get the angle of the robot from X.
    R_L3toW = rotMatZ(theta);
    T_L3toW = [R_L3toW t_W_L3fromW;0 0 0 1];
    
    p_c = pxl_data(i,2:3);
    
    
    v_C = project_line(p_c,M); % Get the direction vector of the feaure in Camera frame.
    v_L3 = R_L3toC'*v_C;
    v_W = R_L3toW*v_L3;
    
    a_C = [getCameraOrigin(M); 1];
    a_L3 =  inv(T_L3toC) * a_C;
    a_W = T_L3toW * a_L3;
    a_W = a_W(1:3);
    
    % p_W = a_W + t*v_W;
    t = (surf_height - a_W(3))/v_W(3);
    proj_W(i,:) = (a_W + t*v_W)';
    
end

figure; hold on; view(3); axis equal; grid on;
plot3(proj_W(:,1),proj_W(:,2),proj_W(:,3),'x');
plot3(ball_avg_xyz(:,1),ball_avg_xyz(:,2),ball_avg_xyz(:,3),'o')
xlabel('x');
ylabel('y');
zlabel('z');

%% Transform projection vector and position in C to L3 to W, them use plane intersection method.
close all;
surf_height = mean(ball_avg_xyz(:,3));
P_R = [242.064 -9.39872 110.689];
p_c = [77, 40]; 

t_W_L3fromW = P_R';
theta = atan2(P_R(2),P_R(1)); % Get the angle of the robot from X.
R_L3toW = rotMatZ(theta);
T_L3toW = [R_L3toW t_W_L3fromW;0 0 0 1];

v_C = project_line(p_c,M); % Get the direction vector of the feaure in Camera frame.
v_L3 = R_L3toC'*v_C;
v_W = R_L3toW*v_L3;

a_C = [getCameraOrigin(M); 1];
a_L3 =  inv(T_L3toC) * a_C;
a_W = T_L3toW * a_L3;
a_W = a_W(1:3);

% p_W = a_W + t*v_W;
t = (surf_height - a_W(3))/v_W(3);
result = (a_W + t*v_W)'

%% Calculate error

proj_err = proj_W - wrld_data(:,2:4);
proj_rmse = sqrt(sumsqr(vecnorm(proj_err,2,2))/Npts)

%% Transform world points to camera frame and plot with rays.
close all;
pts_L3 = zeros(Npts,3);
pts_C = zeros(Npts,3);
for i = 1:Npts
    
    P_W = wrld_data(i,2:4)';
    
    P_R = robot_pos(i,2:4);
    t_W_L3fromW = P_R';
    theta = atan2(P_R(2),P_R(1)); % Get the angle of the robot from X.
    R_L3toW = rotMatZ(theta);
    T_L3toW = [R_L3toW t_W_L3fromW;0 0 0 1];
    
    temp = inv(T_L3toW)*[P_W; 1];
    pts_L3(i,:) = temp(1:3)'; % Get the position of the feature in the link-fixed frame L3.
    
    
    temp = T_L3toC*[pts_L3(i,:) 1]';% using the matrix from the ray-projection calibration
%     temp = inv(T_CtoL3)*[pts_L3(i,:) 1]'; %using the matrix from the sphere size calibration
    pts_C(i,:) = temp(1:3)'; % Get the position of the feature in the link-fixed frame L3.
    
end

figure;
plot3(pts_C(:,1),pts_C(:,2),pts_C(:,3),'x','LineWidth',2)
axis equal; grid on; hold on;
xlabel('Camera frame x');
ylabel('Camera frame y');
zlabel('Camera frame z');

for i = 1:Npts
    [v] = project_line(pxl_data(i,2:3),M);
    p1 = P_cam_origin;
    p2 = 350*v + p1;
    line_pts = [p1'; p2'];
    plot3(line_pts(1,1),line_pts(1,2),line_pts(1,3),'.k','MarkerSize',20);
    p = plot3(line_pts(:,1),line_pts(:,2),line_pts(:,3),'r','LineWidth',1);
    if i > 1
       set(get(get(p,'Annotation'),'LegendInformation'),'IconDisplayStyle','off'); 
        
    end
end

legend('Actual Position','Camera Position','Camera Projection Ray','Location','eastoutside')

%% Transform rays to world frame and plot with targets.
close all;

figure; hold on; view(3);
plot3(ball_avg_xyz(:,1),ball_avg_xyz(:,2),ball_avg_xyz(:,3),'o','LineWidth',2)
plot3(proj_W(:,1),proj_W(:,2),proj_W(:,3),'kx','LineWidth',2);
axis equal; grid on; hold on;
xlabel('World frame x');
ylabel('World frame y');
zlabel('World frame z');

for i = 1:Npts
    
    P_R = robot_pos(i,2:4);
    t_W_L3fromW = P_R';
    theta = atan2(P_R(2),P_R(1)); % Get the angle of the robot from X.
    R_L3toW = rotMatZ(theta);
    T_L3toW = [R_L3toW t_W_L3fromW;0 0 0 1];
    
    p_c = pxl_data(i,2:3);
    
    
    v_C = project_line(p_c,M); % Get the direction vector of the feaure in Camera frame.
    v_L3 = R_L3toC'*v_C;
    v_W = R_L3toW*v_L3;
    
    a_C = [getCameraOrigin(M); 1];
    a_L3 =  inv(T_L3toC) * a_C;
    a_W = T_L3toW * a_L3;
    a_W = a_W(1:3);
    
    
    
    % p_W = a_W + t*v_W;
    ray_length = 1.1*norm(a_W - wrld_data(i,2:4)');
    p1 = a_W;
    p2 = ray_length*v_W + a_W;
    line_pts = [p1'; p2'];
    
    plot3(line_pts(1,1),line_pts(1,2),line_pts(1,3),'.k','MarkerSize',20);
    p = plot3(line_pts(:,1),line_pts(:,2),line_pts(:,3),'r','LineWidth',1);
    if i > 1
       set(get(get(p,'Annotation'),'LegendInformation'),'IconDisplayStyle','off'); 
        
    end
    
end

legend('Actual Position','Estimated Position ','Camera Position','Camera Projection Ray','Location','eastoutside')
%% Height vs Radius
close all; figure; hold on;

radii = pxl_data(:,4);
height = robot_pos(:,4);

plot(height,radii,'x');

f=fit(height,radii,'poly2');

height_sort = sort(height);
ub = f.p1*height_sort.^2 + f.p2*height_sort + f.p3 + 5;
lb = f.p1*height_sort.^2 + f.p2*height_sort + f.p3 - 5;

plot(height_sort,ub)
plot(height_sort,lb)

%%
function dist = totalDistance(pts_L3,pts_c,dx,dy,dz,tx,ty,tz,M)
% computes the total distance between all the points and their lines
% assuming the transformation between the L3 and C frames given by the
% translations dx,dy,dz and rotation angles tx,ty,tz.
R = rotMatZ(tz)*rotMatY(ty)*rotMatZ(tx);
d = [dx; dy; dz];

Npts = length(pts_L3);
pts_C = zeros(Npts,3);

% Transform points in L3 frame to C frame assuming the given transformation
for i = 1:Npts
    pts_C(i,:) = (R*(pts_L3(i,:)') + d)';
end

% Get line and compute distance to it.
a = getCameraOrigin(M);
dist = 0;
for i = 1:Npts
    p_c = pts_c(i,:);
    p = pts_C(i,:)';
    n = project_line(p_c,M); % Get the direction vector of the feaure in Camera frame.
    dist = dist + norm((a - p) - (dot(a-p,n)*n));
end
end

function Rz = rotMatZ(t)

Rz = [...
    cos(t) -sin(t) 0;
    sin(t) cos(t) 0;
    0 0 1;
    ];

end

function Ry = rotMatY(t)

Ry = [...
    cos(t) 0 sin(t);
    0 1 0;
    -sin(t) 0 cos(t);
    ];

end

function Rx = rotMatX(t)

Rx = [...
    1 0 0;
    0 cos(t) -sin(t);
    0 sin(t) cos(t);
    ];

end