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


# ARINC429-Simulation

This project involves a simulation and verification system built in MATLAB/Simulink for the **ARINC 429 communication protocol**, which is widely used in avionics systems. The project also includes a **trend-based validation** mechanism implemented using a **Deterministic Finite Automaton (DFA)**.

## 🎯 Purpose

- To model ARINC 429 word structure
- To decode data using custom S-Functions
- To detect anomalies using DFA-based trend analysis
- To process and analyze flight-related data in real-time

## 🛠️ Key Files

| File / Folder                   | Description |
|--------------------------------|-------------|
| `arinc429_decoder.slx`         | Simulink model (for decoding ARINC words) |
| `arinc_label_sfunction.c`      | S-Function for extracting label (written in C) |
| `trend_dfa_sfunc_flight.c`     | DFA trend analysis S-Function (for flight data) |
| `*.mexw64`                      | Precompiled S-Function binaries for Windows |
| `arinc429_bcd_to_decimal.c`    | Converts BCD to Decimal |
| `arinc429_decimal_to_bcd.c`    | Converts Decimal to BCD |
| `data_original.m`, `datas.m`   | MATLAB scripts for data preparation |
| `filtered_data.csv`            | Output results (filtered trend data) |
| `flight_simulation_data.mat`   | Input flight data file |
| `simulation_database_creator.m`| Script for creating an SQLite database |
| `arinc_verileridb`             | Exported SQLite database file |

## 💡 How to Run

1. Open MATLAB and set the project directory as your working folder.
2. Open the Simulink model: `arinc429_decoder.slx`
3. Click `Run` to start the simulation.
4. Analyze trend states and output data through Scope blocks or via the `.csv` and `.db` files.

## 📌 Notes

- `*.mexw64` files are compiled from corresponding `.c` files using MATLAB's `mex` function.
- `.slx.original` is a backup and not required for execution.
- `arinc_verileridb` can be accessed in MATLAB using the `sqlite()` function.

## 🔍 DFA Trend States

- `STABLE`: Steady data
- `INCREASING`: Ascending trend
- `DECREASING`: Descending trend
- `OSCILLATING`: Fluctuating pattern
- `ANOMALY`: Abnormal values or sudden change
