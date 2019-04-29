function d = dist2Sphere2(pc,rc,M,R_sphere,tol,maxIts)
R = R_sphere;
 
D = dist2Disk(pc,rc,M,R);
 
Dlast = D;
for i = 1:maxIts
    r = R*sqrt(1-(R/D)^2);
 
    d = dist2Disk(pc,rc,M,r);
 
    D = 1/2*(d + sqrt(d^2 + 4*R^2));
    if(abs(Dlast - D) < tol)
        break;
    end
    Dlast = D;
end
 
d = D - R;
 
end

function d = dist2Disk(pc,rc,m,r)
 
v1 = project_line(pc,m); % the ray from the pinhole to the center of sphere
v2 = project_line(pc + [rc 0],m); % the ray from the pinhole through one edge of the circle
d = perp_joiner_dist(v1,v2,r);% the distance to the plane normal to v1 which bisects the sphere
 
end