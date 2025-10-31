# Temperaturas en OLED - Sistema Multi-Sensor

## 📖 Descripción

Sistema integrado que muestra lecturas simultáneas de dos sensores de temperatura (NTC y DS18B20) en pantalla OLED. Combina sensores analógicos, digitales y display en interfaz de usuario completa.

**Integra técnicas avanzadas:** Calibración automática ADC mediante eFuse, modo parásito DS18B20 con control de timing, y biblioteca U8g2 para gráficos optimizados.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| OLED SSD1306 | SDA: GPIO8, SCL: GPIO9 | Display 128x64 I2C (ESP32-C3) |
| NTC 10kΩ | GPIO 1 | ADC - Divisor con R fija 10kΩ |
| DS18B20 | GPIO 3 | 1-Wire modo parásito + pull-up 4.7kΩ |

**Circuito NTC:**
```
3.3V ─── R_fija(10kΩ) ─── [GPIO 1 ADC] ─── NTC(10kΩ) ─── GND
```

**Circuito DS18B20 (modo parásito):**
```
GPIO 3 ─── [Pull-up 4.7kΩ] ─── 3.3V
       │
    DS18B20
       │
      GND
```

---

## 💡 Características

### Sensores
- **NTC 10kΩ**: Analógico, respuesta rápida, ±2-3°C precisión
- **DS18B20**: Digital 1-Wire, alta precisión ±0.5°C
- **Calibración eFuse**: ADC calibrado automáticamente con valores de fábrica
- **Modo parásito**: DS18B20 alimentado desde línea de datos (solo 2 cables)

### Display
- **U8g2**: Biblioteca optimizada con buffer completo
- **Actualización suave**: Sin parpadeo
- **Layout limpio**: Temperaturas + identificación UNSE
- **Fuentes múltiples**: 7x13 para datos, Bold 10 para título

### Integración
- Validación cruzada entre sensores
- Timing optimizado (1.5s/ciclo)
- Control manual de conversión DS18B20
- Arquitectura multi-periférico escalable

---

## 📊 Comparación de Sensores

| Característica | NTC | DS18B20 |
|----------------|-----|---------|
| Precisión | ±2-3°C | ±0.5°C |
| Tiempo respuesta | Inmediato | 1000ms |
| Interfaz | Analógico ADC | Digital 1-Wire |
| Costo | Bajo | Medio |
| Uso | Monitoreo rápido | Medición precisa |

**Ventajas del dual-sensor:**
- ✅ Redundancia ante fallo
- ✅ Validación cruzada de lecturas
- ✅ Características complementarias
- ✅ Mayor confiabilidad del sistema

---

## 🎯 Calibración y Configuración

### ADC (NTC)
Utiliza calibración eFuse automática:
```cpp
#include <esp_adc_cal.h>
esp_adc_cal_characteristics_t adc_chars;
esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, 
                         ADC_WIDTH_BIT_12, 1100, &adc_chars);
```
- Compensa variaciones de referencia interna
- Funciona automáticamente en cualquier ESP32
- Mejora precisión de ±10% a ±2-3%

### DS18B20 (Modo Parásito)
Control manual de timing:
```cpp
ds.setWaitForConversion(false);  // Modo asíncrono
ds.requestTemperatures();         // Iniciar
delay(1000);                      // Esperar conversión
float temp = ds.getTempCByIndex(0);
```
- Requiere pull-up 4.7kΩ externo
- Delay 1000ms para conversión 12-bit
- Solo 2 cables (datos + GND)

---

## 🖥️ Interfaz OLED

**Pantalla mostrada:**
```
┌────────────────────────────┐
│ NTC:     22.5 C            │
│ DS18B20: 23.1 C            │
│                            │
│         UNSE               │
└────────────────────────────┘
```

**Características de diseño:**
- Formato consistente (1 decimal)
- Actualización sin parpadeo
- Fuentes diferenciadas por función
- Centrado automático de título

---

## 🚀 Uso

1. Conectar hardware según tabla de pines
2. Verificar pull-up 4.7kΩ en línea DS18B20
3. Compilar y subir con PlatformIO
4. Observar temperaturas en OLED
5. Esperar ~2 minutos para estabilización térmica

**Salida esperada:**
- Temperaturas actualizadas cada 1.5 segundos
- Diferencia típica de 0.5-2°C entre sensores (normal por inercia térmica)

---

## 🔍 Troubleshooting

| Problema | Causa probable | Solución |
|----------|----------------|----------|
| OLED negro | Dirección I2C incorrecta | Probar 0x3C o 0x3D |
| DS18B20 = -127°C | Sensor desconectado | Verificar conexiones |
| DS18B20 = 85°C | Delay insuficiente | Aumentar a 1000ms |
| NTC errático | Ruido ADC | Añadir filtro EMA |
| Gran diferencia | Inercia térmica | Esperar estabilización |

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  

Ver `src/main.cpp` para:
- Teoría completa de sensores NTC y DS18B20
- Explicación de calibración eFuse
- Documentación de U8g2 y DallasTemperature
- Arquitectura del sistema multi-sensor
- Mejoras opcionales y optimizaciones
