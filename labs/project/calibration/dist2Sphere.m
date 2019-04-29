function d = dist2Sphere(pc,rc,M,R)
%%% Computes the distance to the center of a sphere as detected on the
%%% camera at x,y position pc, with radius rc, and known sphere radius R
%%% and known camera calibration matrix m.

v2 = project_line(pc,M); % the ray from the pinhole to the center of sphere
v1 = project_line(pc + [rc 0],M); % the ray from one edge of the circle to the tangent of the sphere.
d = perp_joiner_dist(v1,v2,R);% the distance to the plane normal to v1 which bisects the sphere
d = sqrt(d^2 + R^2);
end

function d = dist2Disk(pc,rc,M,r)

v1 = project_line(pc,M); % the ray from the pinhole to the center of sphere
v2 = project_line(pc + [rc 0],M); % the ray from the pinhole through one edge of the circle
d = perp_joiner_dist(v1,v2,r);% the distance to the plane normal to v1 which bisects the sphere

end