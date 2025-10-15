# Lectura de DS18B20 - Sensor Digital de Temperatura

## 📖 Descripción

Lectura de temperatura usando sensor digital DS18B20 con protocolo 1-Wire. Sensor de alta precisión (±0.5°C) con salida digital que elimina necesidad de ADC y conversiones analógicas.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| DS18B20 | GPIO 4 | Protocolo 1-Wire |

---

## 💡 Características

- Sensor digital de temperatura DS18B20
- Protocolo 1-Wire (un solo pin)
- Precisión ±0.5°C
- Librerías OneWire y DallasTemperature

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Observar temperatura cada segundo

---

## 📊 Diagrama de Flujo

![Diagrama DS18B20](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/lectura_ds18b20.pu)

---

## � Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación
