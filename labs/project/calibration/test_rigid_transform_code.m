clearvars all;

Ntests  = 100;
e_R = zeros(Ntests,1) + NaN;
e_t = e_R;
for i = 1:Ntests
    P1 = rand(3,3);
    R_in = rotMatX(deg2rad(rand(1)*2*pi));
    t_in = rand(3,1);
    
    P2 = zeros(size(P1));
    for j = 1:length(P1)
        
        P2(j,:) = (R_in*P1(j,:)' + t_in)';
        
    end
    
    [R_out, t_out] = rigid_transform_3D(P1,P2);
    
    e_R(i) = sumsqr(R_out - R_in);
    e_t(i) = sumsqr(t_out - t_in);
    
    assert(e_R(i) < eps);
    assert(e_t(i) < eps);
    
end

close all;
histogram(e_R,'BinWidth',eps); hold on;
histogram(e_t,'BinWidth',eps);




function Rx = rotMatX(t)

Rx = [...
    1 0 0;
    0 cos(t) -sin(t);
    0 sin(t) cos(t);
    ];

end
function Ry = rotMatY(t)

Ry = [...
    cos(t) 0 sin(t);
    0 1 0;
    -sin(t) 0 cos(t);
    ];

end
function Rz = rotMatZ(t)

Rz = [...
    cos(t) -sin(t) 0;
    sin(t) cos(t) 0;
    0 0 1;
    ];

end