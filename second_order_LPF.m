function[lpf_y] = second_order_LPF(y, fs, cutoff)
lpf_y = y;

% set coefficients
LPF_Cutoff_Value = cutoff;
LPF_C = 1/tan(pi*LPF_Cutoff_Value/fs);
LPF_a0 = 1/(1+ 0.707*LPF_C + LPF_C*LPF_C);
LPF_a1 = 2*LPF_a0;
LPF_a2 = LPF_a0;
LPF_b1 = 2*LPF_a0*(1 - LPF_C*LPF_C);
LPF_b2 = LPF_a0*(1-0.707*LPF_C + LPF_C*LPF_C);

% set delays
LPF_xz1 = 0.0;
LPF_xz2 = 0.0;
LPF_yz1 = 0.0;
LPF_yz2 = 0.0;

for i = 1:size(lpf_y)
    
    LPF_xn = y(i,1);
    LPF_xn_1 = LPF_xz1;
    LPF_xn_2 = LPF_xz2;
    LPF_yn_1 = LPF_yz1;
    LPF_yn_2 = LPF_yz2;
    
    LPF_y = LPF_a0*LPF_xn + LPF_a1*LPF_xn_1 + LPF_a2*LPF_xn_2 - LPF_b1*LPF_yn_1 - LPF_b2*LPF_yn_2;
    
    LPF_xz2 = LPF_xz1;
    LPF_xz1 = LPF_xn;
    LPF_yz2 = LPF_yz1;
    LPF_yz1 = LPF_y;
    
    lpf_y(i,1) = LPF_y;
end

end