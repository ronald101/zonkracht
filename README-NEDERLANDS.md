# ☀️ ESP32 Zonlicht Monitor

Een real-time zonlicht intensiteit monitoring systeem met ESP32 en BH1750 sensor, inclusief mobiele webinterface.

![ESP32 Zonlicht Monitor](https://img.shields.io/badge/ESP32-Zonlicht%20Monitor-blue?style=for-the-badge&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-IDE-teal?style=for-the-badge&logo=arduino)
![Licentie](https://img.shields.io/badge/Licentie-MIT-green?style=for-the-badge)

## 📋 Inhoudsopgave

- [Overzicht](#-overzicht)
- [Functies](#-functies)
- [Benodigde Componenten](#-benodigde-componenten)
- [Bedrading](#-bedrading)
- [Installatie](#-installatie)
- [Gebruik](#-gebruik)
- [Webinterface](#-webinterface)
- [Probleemoplossing](#-probleemoplossing)
- [Bijdragen](#-bijdragen)
- [Licentie](#-licentie)

## 🎯 Overzicht

Dit project maakt gebruik van een ESP32 microcontroller en een BH1750 lichtsensor om real-time zonlicht intensiteit te meten. Het systeem biedt een moderne, mobiel-geoptimaliseerde webinterface die toegankelijk is vanaf elke smartphone of computer op hetzelfde WiFi netwerk.

Perfect voor:
- 🌱 Tuiniers die optimale lichtcondities willen monitoren
- 🏠 Smart home automatisering
- 📊 Weerstation uitbreidingen
- 🔬 Educatieve projecten
- ☀️ Zonnepaneel efficiency monitoring

## ✨ Functies

- 📊 **Real-time monitoring** - Updates elke 2 seconden
- 📱 **Mobiel geoptimaliseerd** - Perfect voor iPhone en Android
- 📈 **Historische data** - Toont laatste 24 metingen met grafiek
- 📋 **Dagstatistieken** - Min, max en gemiddelde waarden
- 🎨 **Moderne interface** - Prachtige, intuïtieve webpagina
- 🔄 **Automatische updates** - Pagina ververst elke 3 seconden
- 🌐 **Geen internet vereist** - Werkt volledig lokaal
- ⚡ **Laag stroomverbruik** - Efficiënt ontwerp

## 🛠️ Benodigde Componenten

| Component | Beschrijving | Geschatte Kosten |
|-----------|--------------|------------------|
| **ESP32 Development Board** | Hoofd microcontroller met WiFi | €8-15 |
| **BH1750 Lichtsensor** | Digitale lichtintensiteit sensor (I2C) | €3-8 |
| **Jumper Draden** | Male-naar-female jumper draden (4 stuks) | €2-5 |
| **Breadboard** *(Optioneel)* | Voor prototyping en testen | €3-8 |
| **Behuizing** *(Optioneel)* | Weerbestendige behuizing voor buiten | €5-15 |

### Waarom BH1750?

De BH1750 sensor is specifiek ontworpen voor lichtmeting en biedt:
- 🎯 Breed meetbereik: 1-65535 lux
- ⚡ Hoge nauwkeurigheid en resolutie
- 🔌 I2C interface (slechts 4 draden nodig)
- 🔋 Laag stroomverbruik
- 💰 Uitstekende prijs-kwaliteitverhouding

## 🔌 Bedrading

```
BH1750 Sensor    →    ESP32 Pin
────────────────────────────────
VCC (3.3V)      →    3.3V
GND             →    GND
SDA             →    GPIO 21 (SDA)
SCL             →    GPIO 22 (SCL)
```

### ⚠️ Belangrijke Bedradingsnotities

- **Spanning:** Gebruik 3.3V, NIET 5V! De BH1750 is een 3.3V apparaat
- **I2C Pinnen:** GPIO 21 (SDA) en GPIO 22 (SCL) zijn de standaard I2C pinnen
- **Pull-up Weerstanden:** Meeste BH1750 modules hebben ingebouwde pull-up weerstanden

## 🚀 Installatie

### Stap 1: Arduino Libraries Installeren

Installeer deze libraries via de Arduino IDE Library Manager:

```
1. BH1750 door Christopher Laws
2. ArduinoJson door Benoit Blanchon (versie 6.x)
```

### Stap 2: WiFi Configuratie

Update deze regels in de code met jouw WiFi netwerk informatie:

```cpp
const char* ssid = "JOUW_WIFI_SSID";
const char* password = "JOUW_WIFI_WACHTWOORD";
```

### Stap 3: Hardware Aansluiten

1. Sluit de BH1750 sensor aan op de ESP32 volgens het bedradingsschema
2. Controleer alle verbindingen nogmaals
3. Zorg voor stevige verbindingen

### Stap 4: Code Uploaden

1. Open Arduino IDE
2. Selecteer jouw ESP32 board onder **Tools → Board**
3. Kopieer de meegeleverde code
4. Compileer en upload naar ESP32

### Stap 5: Testen

1. Open Serial Monitor (115200 baud)
2. Wacht op WiFi verbinding
3. Noteer het IP-adres dat wordt weergegeven
4. Ga naar de webinterface vanaf je telefoon

## 📖 Gebruik

### Lichtniveau Interpretatie

| Lux Waarde | Conditie | Voorbeelden |
|------------|----------|-------------|
| 100.000+ | Direct Zonlicht | Middag op heldere dag |
| 10.000-25.000 | Vol Daglicht | Bewolkte dag, open schaduw |
| 1.000-5.000 | Helder Binnen | Goed verlicht kantoor |
| 100-500 | Typisch Binnen | Huisverlichting, gangen |
| 10-100 | Schemerig | Schemering, dageraad |
| <10 | Zeer Donker | Nacht, diepe schaduw |

### Webinterface Functies

- **Huidige Meting:** Grote weergave toont real-time lux waarde en status
- **Statistieken:** Piek, minimum en gemiddelde waarden voor de sessie
- **Geschiedenis Grafiek:** Visuele weergave van recente metingen
- **Ververs Knop:** Handmatig data bijwerken
- **Reset Knop:** Alle statistieken en geschiedenis wissen

## 📱 Webinterface

De webinterface biedt:

- 🎨 **Modern Design** - Prachtige gradiënten en animaties
- 📊 **Real-time Data** - Live updates van lichtintensiteit
- 📈 **Visuele Grafieken** - Historische data weergave
- 📱 **Responsive Layout** - Optimaal op alle schermgroottes
- 🔄 **Auto-refresh** - Automatische updates elke 3 seconden

## 🔧 Probleemoplossing

### ESP32 verbindt niet met WiFi
**Oplossingen:** Controleer SSID/wachtwoord spelling, zorg voor 2.4GHz netwerk, controleer signaalsterkte

### Sensor niet gedetecteerd / metingen tonen 0 of -1
**Oplossingen:** Controleer bedradingsverbindingen, verifieer 3.3V voeding, controleer I2C pinnen (GPIO 21/22)

### Webpagina laadt niet
**Oplossingen:** Verifieer IP-adres van Serial Monitor, zorg dat telefoon op hetzelfde WiFi netwerk zit, probeer pagina te verversen

### Metingen lijken onnauwkeurig
**Oplossingen:** Maak sensorlens schoon, zorg dat sensor niet geblokkeerd is, kalibreer tegen bekende lichtbron

## 🎯 Technische Specificaties

### ESP32
- **CPU:** Dual-core 240MHz
- **RAM:** 520KB SRAM
- **Flash:** 4MB (typisch)
- **WiFi:** 802.11 b/g/n 2.4GHz

### BH1750
- **Meetbereik:** 1-65535 lux
- **Resolutie:** 1 lux
- **Nauwkeurigheid:** ±20%
- **Interface:** I2C
- **Stroomverbruik:** 0.12mA

## 🔮 Toekomstige Uitbreidingen

- 💾 Data logging naar SD kaart of cloud opslag
- 📧 Email/SMS waarschuwingen voor extreme lichtcondities
- 🔢 Ondersteuning voor meerdere sensoren
- 🏠 Integratie met home automation systemen
- ☀️ Zonnepaneel efficiency monitoring
- 🌱 Plant groei optimalisatie waarschuwingen

## 🤝 Bijdragen

Bijdragen zijn welkom! Voel je vrij om:

1. 🍴 Fork dit project
2. 🌟 Maak een feature branch (`git checkout -b feature/AmazingFeature`)
3. 💾 Commit je wijzigingen (`git commit -m 'Add some AmazingFeature'`)
4. 📤 Push naar de branch (`git push origin feature/AmazingFeature`)
5. 🔄 Open een Pull Request

## 📄 Licentie

Dit project is gelicenseerd onder de MIT Licentie - zie het [LICENSE](LICENSE) bestand voor details.

## 🙏 Acknowledgments

- ESP32 community voor uitstekende documentatie
- Christopher Laws voor de BH1750 library
- Benoit Blanchon voor de ArduinoJson library

## 📞 Contact

Heb je vragen of suggesties? Open een issue op GitHub!

---

⭐ **Vond je dit project nuttig? Geef het een ster!** ⭐

