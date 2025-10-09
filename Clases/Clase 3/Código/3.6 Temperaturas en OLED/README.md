# Temperaturas en OLED - Sistema de Monitoreo

## 📖 Descripción

Sistema integrado que muestra lecturas simultáneas de dos sensores de temperatura (NTC y DS18B20) en pantalla OLED. Combina sensores analógicos, digitales y display en interfaz de usuario completa.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| OLED SSD1306 | SDA: GPIO20, SCL: GPIO21 | Display 128x64 I2C |
| NTC 10kΩ | GPIO 34 | Con resistencia 10kΩ |
| DS18B20 | GPIO 4 | Protocolo 1-Wire |

---

## 💡 Características

- Doble sensor de temperatura (NTC + DS18B20)
- Visualización en OLED en tiempo real
- Integración completa de múltiples periféricos
- Actualización cada 2 segundos

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Observar temperaturas en pantalla OLED

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación

---

## 🚀 Cómo Usar

1. Conectar el hardware según el diagrama de conexiones
2. Abrir el proyecto en PlatformIO
3. Modificar credenciales WiFi si es necesario (en proyectos de Clase 4)
4. Compilar y subir a la placa ESP32
5. Abrir Serial Monitor a 115200 baudios

---

## 🔍 Salida Esperada

```
Este programa no genera salida por serial.
Observar el comportamiento del hardware (LEDs, etc.)
```

---

## 📚 Referencias

- **Módulo:** Clase 3 - Sensores y Periféricos
- **Documentación ESP32:** [Espressif Documentation](https://docs.espressif.com/)
- **PlatformIO:** [platformio.org](https://platformio.org/)
- Ver comentarios detallados en `src/main.cpp` para información técnica completa
