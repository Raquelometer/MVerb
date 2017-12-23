function[y_scaled] = rescale_sound(y, dim)

y_max_L = max(y(:,1));
y_min_L = min(y(:,1));

if dim == 2
    y_max_R = max(y(:,2));
    y_min_R = min(y(:,2));
end

% formula to rescale x[n] to range [a, b]
% x_scaled[n] = (b-a) * (x[n]-xmin)/(xmax-xmin) + a

y_scaled_L = 2 * (y(:,1) - y_min_L)/(y_max_L-y_min_L) - 1;

if dim == 2
    y_scaled_R = 2 * (y(:,2) - y_min_R)/(y_max_R-y_min_R) - 1;
    y_scaled = [y_scaled_L y_scaled_R];
else 
    y_scaled = y_scaled_L;
end



end