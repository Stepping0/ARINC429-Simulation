# ARINC429-Simulation

Bu proje, havacılık sistemlerinde yaygın olarak kullanılan **ARINC 429 iletişim protokolü** için MATLAB/Simulink ortamında geliştirilmiş bir simülasyon ve doğrulama sistemini içermektedir. Proje aynı zamanda, **Deterministik Sonlu Otomat (DFA)** kullanılarak uygulanmış **trend tabanlı bir doğrulama mekanizmasını** da kapsamaktadır.

## 🎯 Amaç

- ARINC 429 veri yapısının modellenmesi
- S-Function tabanlı kod çözücülerin uygulanması
- DFA (Deterministik Sonlu Otomat) ile trend tabanlı veri doğrulama
- Gerçek zamanlı verilerin işlenmesi ve analiz edilmesi

## 🛠️ Kullanılan Dosyalar

| Dosya / Klasör                    | Açıklama |
|----------------------------------|----------|
| `arinc429_decoder.slx`           | Simulink modeli (ARINC kelimesi çözümleme) |
| `arinc_label_sfunction.c`        | Label ayrıştırıcı S-Function (C kodu) |
| `trend_dfa_sfunc_flight.c`       | DFA trend analizi yapan S-Function (uçuş verisi için) |
| `*.mexw64`                        | Windows için derlenmiş S-Function binary dosyaları |
| `arinc429_bcd_to_decimal.c`      | BCD → Decimal dönüşüm fonksiyonu |
| `arinc429_decimal_to_bcd.c`      | Decimal → BCD dönüşüm fonksiyonu |
| `data_original.m`, `datas.m`     | Örnek veri hazırlama scriptleri |
| `filtered_data.csv`              | Filtrelenmiş çıktı verisi (trend sonucu) |
| `flight_simulation_data.mat`     | Simülasyonda kullanılan uçuş verileri |
| `simulation_database_creator.m`  | SQLite tabanlı veri tabanı oluşturucu |
| `arinc_verileridb`               | Oluşturulan SQLite veritabanı |

## 💡 Nasıl Çalıştırılır?

1. MATLAB’i açın ve proje klasörünü çalışma dizini yapın.
2. `arinc429_decoder.slx` dosyasını açın.
3. `Run` tuşuna basarak simülasyonu başlatın.
4. DFA durumlarını ve çıkış verilerini `Scope` bloklarından veya `.csv` ve `.db` dosyalarından inceleyin.

## 📌 Notlar

- `*.mexw64` dosyaları ilgili `.c` dosyalarından MATLAB `mex` komutu ile oluşturulmuştur.
- `.slx.original` dosyası, modelin yedeğidir ve doğrudan kullanılmaz.
- `arinc_verileridb` SQLite veritabanı olarak dışa aktarılmıştır. Veritabanı bağlantısı için MATLAB'de `sqlite()` fonksiyonu kullanılabilir.

## 🔍 DFA Trend Durumları

- `STABLE`: Kararlı veri
- `INCREASING`: Artış gösteren veri
- `DECREASING`: Azalan veri
- `OSCILLATING`: Dalgalı veri
- `ANOMALY`: Anormal değişim
