# ARINC 429 Simülasyon ve Trend Tabanlı DFA Doğrulama

Bu proje, havacılıkta yaygın olarak kullanılan **ARINC 429 veri iletişim protokolü** üzerine kurulmuş, MATLAB/Simulink ortamında gerçekleştirilmiş bir simülasyon ve analiz uygulamasıdır. Ayrıca, iletilen verilerin doğruluğunu kontrol etmek amacıyla **Deterministik Sonlu Otomat (DFA)** tabanlı trend analizi uygulanmıştır.

## 🔧 Kullanılan Teknolojiler

- MATLAB R2023a
- Simulink
- S-Function (C dili ile)
- SQLite (veri saklama ve analiz için)
- MATLAB Function blokları

## 📌 Proje Bileşenleri

### 1. ARINC 429 Simülasyon Bloğu
- ARINC 429 protokolüne uygun olarak veri kelimeleri oluşturur.
- Label, SDI, Data, SSM ve Parity bitlerini içerir.

### 2. DFA Tabanlı Doğrulama Sistemi
- Sinyal trendlerini izleyerek aşağıdaki beş durumu belirler:
  - **STABLE**
  - **INCREASING**
  - **DECREASING**
  - **OSCILLATING**
  - **ANOMALY**
- Her bir durum için eşik değerleri S-Function içinde C kodu olarak tanımlanmıştır.

### 3. Veri Tabanı Entegrasyonu (Opsiyonel)
- Simülasyon sonuçları SQLite veritabanına kaydedilebilir.
- Veriler: hız, irtifa, konum, dikey hız, analiz sonuçları vb.
  

## ▶️ Nasıl Çalıştırılır?

1. MATLAB'i açın ve proje klasörünü çalışma dizini olarak ayarlayın.
2. `arinc_decoder.slx` dosyasını açın.
3. Simülasyonu başlatın (`Run` tuşu).
4. DFA sonucu ve çıkış verilerini `Scope` bloklarından veya veritabanından kontrol edin.

## 📈 Örnek Çıktılar

- Trend DFA Durumları: STABLE → INCREASING → ANOMALY
- ARINC kelimesi örneği: `Label: 203, SDI: 01, Data: 12345, Parity: OK`

## 📚 Kaynaklar

- ARINC 429 Standard Documentation
- MATLAB & Simulink Documentation
- DFA (Deterministic Finite Automaton) Teorisi

## 📌 Notlar

- `examples`, `slprj`, `arinc429_grt_rtw` gibi derleme klasörleri `.gitignore` içinde tutulmalıdır.
- Kod ve model sadece eğitim amaçlıdır.
