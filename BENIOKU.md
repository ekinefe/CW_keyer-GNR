# Ultimate CW Keyer

[Click for English version (README.md)](README.md)

Arduino tabanlı, birleştirilmiş ve zengin özellikli CW (Mors Kodu) Anahtarlayıcı (Keyer), Çözücü (Decoder) ve Kodlayıcı (Encoder). Bu proje, LCD üzerinden görsel geri bildirim ve Komut Satırı Arayüzü (CLI) ile kontrol imkanı sunarak Mors kodu pratiği yapmak ve göndermek için kapsamlı bir çözüm sağlar.

## Özellikler

- **Çift Modlu Çalışma**: Hem CW Anahtarlayıcı (gönderme) hem de Çözücü (paddle girişini alma/yorumlama) olarak çalışır.
- **Metinden Morsa**: Seri Monitör üzerinden metin göndererek kodlanmasını ve Mors kodu olarak çalınmasını sağlayın.
- **Gerçek Zamanlı Çözme**: Paddle girişini metne çözer ve 16x2 I2C LCD üzerinde görüntüler.
- **Komut Satırı Arayüzü (CLI)**: Anahtarlayıcı ayarlarını (WPM, Ton, Mod) doğrudan Seri Monitör'den kontrol edin.
- **Donanım Kontrolleri**:
  - WPM (Hız), Ton Frekansı ve Ses/Analog için Potansiyometreler.
  - Mod değiştirme ve Tampon (Buffer) temizleme için Düğmeler.
- **Çoklu Ekran Modları**:
  - `CHAR`: Çözülen karakterleri görüntüler.
  - `MRS`: Mors nokta/çizgi dizilerini görüntüler.
  - `BOTH`: Hem karakteri hem de Mors dizisini görüntüler.

## Donanım Kurulumu

Proje, aşağıdaki pin konfigürasyonuna sahip bir Arduino kartı (örn. Uno, Nano) için tasarlanmıştır (`PinSettings.h` dosyasında tanımlanmıştır):

| Bileşen | Pin | Açıklama |
|-----------|-----|-------------|
| **Paddle (Anahtar)** | | |
| Nokta (Dot) | D4 | Nokta girişi |
| Çizgi (Dash) | D5 | Çizgi girişi |
| **Kontroller** | | |
| WPM Pot | A0 | Dakikadaki Kelime Sayısı (hız) kontrolü |
| Vol Pot | A1 | Ses Kontrolü (Analog çıkış ölçekleme) |
| Tone Pot | A2 | Ton Frekansı Kontrolü |
| Fonksiyon Düğmeleri | A3 | Çoklu düğmeler için analog merdiven |
| Mod Düğmesi | D3 | Çözme modları arasında geçiş |
| Temizle Düğmesi | D2 | Metin/mors tamponlarını temizle |
| **Çıkışlar** | | |
| Buzzer | D9 | Yan ton (sidetone) için ses çıkışı |
| Kulaklık Çıkışı | D7 | Amplifikatör için çıkış |
| Radyo Çıkışı | D6 | Radyo arayüzü için çıkış |
| Güç LED'i | D10 | Güç durumunu gösterir (Kırmızı) |
| Mors LED'i | D8 | Anahtarlama durumunu gösterir (Yeşil) |
| **Ekran** | | |
| LCD SDA | A4 | I2C Veri |
| LCD SCL | A5 | I2C Saat |

*Not: LCD adresi `0x27` olarak ayarlanmıştır.*

![Devre Şeması](Schmeatic.png)

[📄 Şematik (PDF)](Schmeatic.pdf)


## Kurulum

1. **Bağımlılıklar**: Arduino IDE'nizde `LiquidCrystal_I2C` kütüphanesinin kurulu olduğundan emin olun.
2. **Yükleme**: Arduino IDE'de `Ultimate_CW_Keyer/Ultimate_CW_Keyer.ino` dosyasını açın ve kartınıza yükleyin.
3. **Bağlantı**: Seri Monitörü **9600 baud** hızında açın.

## Kullanım

### Komut Satırı Arayüzü (CLI)

Mevcut komutları görmek için Seri Monitör'de `help` yazın.

- `wpm <5-50>`: Dakikadaki Kelime (WPM) hızını ayarlar.
- `tone <400-1200>`: Yan ton frekansını Hz cinsinden ayarlar.
- `mode <0|1|2>`: Ekran modunu ayarlar (0=CHAR, 1=MRS, 2=BOTH).
- `silent <0|1>`: Sessiz modu Etkinleştirir/Devre Dışı Bırakır.
- `clear`: LCD'yi ve dahili tamponları temizler.
- `status`: Mevcut WPM, Ton, Mod ve Tampon durumunu gösterir.
- **Metin Girişi**: Mors kodu olarak çalınması için herhangi bir cümle yazın.

### Paddle İşlemi

- Mors kodu girmek için paddle'ı kullanın.
- LCD, seçilen moda bağlı olarak çözülen metni veya nokta/çizgileri görüntüleyecektir.
- Ekran modları arasında geçiş yapmak için **Mod Düğmesi**ni kullanın.
- Ekranı sıfırlamak için **Temizle Düğmesi**ni kullanın.

## Proje Yapısı

- `Ultimate_CW_Keyer.ino`: Ana uygulama mantığı ve döngüsü.
- `PinSettings.h`: Pin tanımları ve donanım yapılandırması.
- `CLI_Controller.h`: Seri komutları ve metin girişlerini işler.
- `Decoder.h`: Mors dizilerini karakterlere dönüştürme mantığı.
- `Encoder.h`: Metni Mors dizilerine dönüştürme mantığı.
- `LCD_Controller.h`: Belirli LCD güncellemelerini yönetir.
- `Buzzer_LED_Controller.h`: Ses ve görseller için düşük seviyeli kontrol.
- `Potentiometer_Controller.h`: Ayarlar için analog girişleri okur.
- `Button_Controller.h`: Düğme basışlarını (debounce) ve işlemeyi yönetir.
