function[hpf_y] = second_order_HPF(y,fs, cutoff)
hpf_y = y;

% set coefficients
HPF_C = tan(pi*cutoff/fs);
HPF_a0 = 1/(1+ 0.707*HPF_C + HPF_C * HPF_C);
HPF_a1 = -2*HPF_a0;
HPF_a2 = HPF_a0;
HPF_b1 = 2*HPF_a0*(-1 + HPF_C*HPF_C);
HPF_b2 = HPF_a0*(1-0.707*HPF_C + HPF_C*HPF_C);

% set delays
HPF_xz1 = 0.0;
HPF_xz2 = 0.0;
HPF_yz1 = 0.0;
HPF_yz2 = 0.0;

for i = 1:size(hpf_y)
    
    HPF_xn = y(i,1);
    HPF_xn_1 = HPF_xz1;
    HPF_xn_2 = HPF_xz2;
    HPF_yn_1 = HPF_yz1;
    HPF_yn_2 = HPF_yz2;
    
    HPF_y = HPF_a0*HPF_xn + HPF_a1*HPF_xn_1 + HPF_a2*HPF_xn_2 - HPF_b1*HPF_yn_1 - HPF_b2*HPF_yn_2;
    
    HPF_xz2 = HPF_xz1;
    HPF_xz1 = HPF_xn;
    HPF_yz2 = HPF_yz1;
    HPF_yz1 = HPF_y;
    
    hpf_y(i,1) = HPF_y;
end

end