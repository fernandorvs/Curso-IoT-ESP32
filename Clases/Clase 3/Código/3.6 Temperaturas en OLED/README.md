# Temperaturas en OLED - Sistema de Monitoreo

## 📖 Descripción

Sistema integrado que muestra lecturas simultáneas de dos sensores de temperatura (NTC y DS18B20) en pantalla OLED. Combina sensores analógicos, digitales y display en interfaz de usuario completa.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| OLED SSD1306 | SDA: GPIO8, SCL: GPIO9 | Display 128x64 I2C (ESP32-C3) |
| NTC 10kΩ | GPIO 1 | ADC - Divisor con R fija 10kΩ |
| DS18B20 | GPIO 3 | Protocolo 1-Wire, modo parásito |

**Circuito NTC:**
```
3.3V ─── R_fija(10kΩ) ─── [GPIO 1 ADC] ─── NTC(10kΩ) ─── GND
```

---

## 💡 Características

- Doble sensor de temperatura (NTC + DS18B20)
- Fórmula correcta para NTC: `R = R_FIXED * v / (VREF - v)`
- DS18B20 en modo parásito
- Visualización en OLED en tiempo real
- Integración completa de múltiples periféricos
- Actualización cada ~1.5 segundos

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Observar temperaturas en pantalla OLED

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación
