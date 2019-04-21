% close all;
% xmeas = 100;
% 
% figure; hold on; axis equal;
% for a1 = -90:10:0
%     for a2 = -55:10:35
%         fprintf("a1 = %f, a2 = %f\n",a1,a2);
%         
%        
%         [x1,y1,z1] = transform(0,deg2rad(a1),deg2rad(a2));
%         [x2,y2,z2] = transform(xmeas,deg2rad(a1),deg2rad(a2));
%         
%         plot3([x1 x2],[y1 y2],[z1 z2]);
%     end
% end

data = dlmread('lidar_data1.log',',',0,0);

a1 = deg2rad(data(:,1));
a2 = deg2rad(data(:,2));
d = data(:,3);
v = data(:,4);

[x,y,z] = transform(d,a1,a2);

close all;
scatter3(x,y,z,1,v)
colorbar;
axis equal;


function [x,y,z] = transform(xm, a1, a2)

d35 = deg2rad(35);
x = 3*cos(a1) + 6.5*cos(a1).*cos(d35 - a2) - 2*sin(a1) + 1.5*cos(a1).*sin(d35-a2)+cos(a1).*cos(d35-a2).*xm;
y = 2*cos(a1) + 3*sin(a1)+6.5*cos(d35-a2).*sin(a1) + 1.5*sin(a1).*sin(d35-a2)+cos(d35-a2).*sin(a1).*xm;
z = 4- 1.5*cos(d35-a2)+6.5*sin(d35-a2)+sin(d35-a2).*xm;

end