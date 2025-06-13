% CSV dosyasını oku (tablo olarak)
data = readtable('filtered_data.csv');

% Her sütunu ayrı değişken olarak ata
velocity     = data.velocity;
baroaltitude = data.baroaltitude;
lat          = data.lat;
lon          = data.lon;
vertrate     = data.vertrate;

% Sonuçları kontrol için ekrana yazdır
disp('velocity:');     disp(velocity);
disp('baroaltitude:'); disp(baroaltitude);
disp('lat:');          disp(lat);
disp('lon:');          disp(lon);
disp('vertrate:');     disp(vertrate);
