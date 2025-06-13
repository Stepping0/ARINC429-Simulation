data = readtable('filtered_data.csv');
N = height(data);
stop_time = 0.00036; % Modeldeki Stop Time

time = linspace(0, stop_time, N)';

velocity_input     = [time, data.velocity];
baroaltitude_input = [time, data.baroaltitude];
lat_input          = [time, data.lat];
lon_input          = [time, data.lon];
vertrate_input     = [time, data.vertrate];

save('arinc_verileri.mat');
