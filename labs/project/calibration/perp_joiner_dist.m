function t = perp_joiner_dist(v1,v2,R)
%%% For a pair of lines in the directions v1 and v2 projected from the
%%% origin p_origin, this finds the distance along v1 to the point where a
%%% line perpindicular to v1 can go between v1 and v2 with length R. 
%%% -----------------------------------------------------------------------
%%% In other words:
%%% this finds this solution to norm(p1-p2) == R and (p1-p2).v1 == 0 for
%%% when p1 = v1 * t + p_origin and p2 = v2 * t2 + p_origin.
%%% -----------------------------------------------------------------------
%%% NOTE!!!: v1 and v2 should be in the same units or better yet, unitless.
v2_v1 = dot(v2,v1);
if (abs(v2_v1) < 10*eps)
   error('v2 and v1 should not be nearly perpindicular');
end

k = dot(v1,v1)/v2_v1;

t = sqrt((v1(1) - k*v2(1))^2 + (v1(2) - k*v2(2))^2 + (v1(3) - k*v2(3))^2 );
if (t < 10*eps)
    error('v1 and v2 should not be nearly colinear');
end
t = R / t;

end