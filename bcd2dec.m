function dec = bcd2dec(bits)
% bits: [19x1] logical veya uint8 (vektör)
dec = 0;
n = floor(length(bits)/4); % Kaç BCD digit var
for k = 1:n
    digit = 0;
    for j = 1:4
        idx = (k-1)*4 + j;
        if idx <= length(bits)
            digit = digit * 2 + bits(idx);
        end
    end
    dec = dec * 10 + digit;
end
end
