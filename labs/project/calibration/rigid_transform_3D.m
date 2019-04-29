function [R,t] = rigid_transform_3D(P1, P2)
%%% Computes the rigid body transform (R,t) as in p_2 = R*p1 + t
%%% P1 is a vector of xyz coordinates along the row, as is P2.
%%% Adopted from https://nghiaho.com/?page_id=671 or
%%% http://nghiaho.com/uploads/code/rigid_transform_3D.m

assert(all(size(P1) == size(P2)))

centroid_A = mean(P1);
centroid_B = mean(P2);

N = size(P1,1);

H = (P1 - repmat(centroid_A, N, 1))' * (P2 - repmat(centroid_B, N, 1));

[U,~,V] = svd(H);

R = V*U';

% if det(R) < 0
%     R(:,3) = -1 * R(:,3);
% end

if det(R) < 0
    V(:,3) = -1 * V(:,3);
    R = V*U';
end

t = -R*centroid_A' + centroid_B';
end