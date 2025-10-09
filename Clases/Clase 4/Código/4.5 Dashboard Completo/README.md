# Dashboard Completo - Sistema IoT Integrado

## 📖 Descripción

Dashboard web profesional con archivos separados (HTML/CSS/JS) servidos desde SPIFFS/LittleFS. Sistema IoT completo con monitoreo de sensores, control de actuadores con PWM, display OLED local e interface responsive moderna.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| OLED SSD1306 | SDA: GPIO20, SCL: GPIO21 | Display 128x64 I2C |
| LED PWM | GPIO 8 (ESP32-C3) / GPIO 2 (otros) | Control brillo 0-100% |

---

## 💡 Características

- Dashboard web con HTML/CSS/JS separados
- Sistema de archivos LittleFS para servir archivos
- Control PWM de LED con brillo ajustable
- Display OLED con temperatura y estado LED
- API REST con endpoints JSON
- Interface responsive moderna

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Subir archivos data/ a LittleFS (PlatformIO: Upload Filesystem Image)
3. Modificar SSID y password en código
4. Compilar y subir con PlatformIO
5. Abrir navegador en http://[IP-ESP32]

---

## 📚 Clase

**Módulo:** Clase 4 - Conectividad y Protocolos  
Ver `src/main.cpp` para detalles de implementación
