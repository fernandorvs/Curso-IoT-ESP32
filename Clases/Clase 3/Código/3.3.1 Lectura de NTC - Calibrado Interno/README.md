# Lectura de NTC - Sensor de Temperatura (Calibración eFuse)

## 📖 Descripción

Medición de temperatura usando termistor NTC 10kΩ en configuración de divisor de tensión. Convierte resistencia a temperatura usando ecuación Steinhart-Hart simplificada (parámetro Beta).

**Utiliza calibración automática del ADC mediante valores eFuse** grabados de fábrica en cada chip ESP32, eliminando la necesidad de calibración manual con multímetro. Método recomendado para proyectos escalables.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| NTC 10kΩ | GPIO 1 | ADC - Divisor de tensión con R fija 10kΩ |
| LED | GPIO 2 | Indicador de temperatura >30°C |

**Circuito divisor de tensión:**
```
3.3V ─── R_fija(10kΩ) ─── [GPIO 1 ADC] ─── NTC(10kΩ) ─── GND
```

---

## 💡 Características

- Sensor analógico NTC 10kΩ
- Conversión con ecuación Steinhart-Hart (Beta = 3950K)
- **Calibración automática ADC con valores eFuse de fábrica**
- Compatible con ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6
- Precisión mejorada: ±2-3% (vs ±10% sin calibración)
- Funciona en cualquier placa sin reconfiguración
- Muestra voltaje, resistencia y temperatura
- LED indicador al superar 30°C
- Actualización cada 500ms

---

## 🎯 Calibración Automática eFuse

### ¿Qué es eFuse?

Durante la fabricación, Espressif calibra cada chip ESP32 individualmente y graba valores de corrección en memoria eFuse (One-Time Programmable). Estos valores compensan las variaciones de la referencia interna de voltaje.

### Ventajas

✅ **Automática**: No requiere multímetro ni mediciones  
✅ **Portátil**: Funciona en cualquier ESP32 sin cambios  
✅ **Precisa**: Compensa variaciones individuales de cada chip  
✅ **Profesional**: Mismo método usado en productos comerciales  
✅ **Escalable**: Ideal para producción de múltiples unidades

### Implementación

El código utiliza la librería `esp_adc_cal.h` para acceder a los valores eFuse:

```cpp
#include <esp_adc_cal.h>
esp_adc_cal_characteristics_t adc_chars;

// Cargar calibración
esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, 
                         ADC_WIDTH_BIT_12, 1100, &adc_chars);

// Convertir lectura ADC a voltaje calibrado
uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
```

### Mejora de Precisión

| Método | Error típico | Uso recomendado |
|--------|--------------|-----------------|
| Sin calibración | ±10% | Prototipos rápidos |
| Calibración eFuse | ±2-3% | **Proyectos escalables** ⭐ |
| Calibración manual | ±1% | Máxima precisión en 1 placa |

---

## 🚀 Uso

1. Conectar hardware según tabla y circuito
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Observar mediciones calibradas automáticamente

**Salida típica:**
```
V=1.650V | R=10000Ω | T=25.00°C
V=1.648V | R=10020Ω | T=25.05°C
```

**Sin configuración adicional** - la calibración se aplica automáticamente al arrancar.

---

## 🔄 Comparación con Ejemplo 3.3

| Característica | 3.3 (Manual) | 3.3.1 (eFuse) |
|----------------|--------------|---------------|
| Calibración | Requiere multímetro | Automática |
| Precisión | ±1% (mejor) | ±2-3% (buena) |
| Portabilidad | Solo 1 placa | Todas las placas |
| Setup inicial | 5-10 minutos | Inmediato |
| Uso recomendado | Precisión crítica | Producción/escalable |

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  

Ver `src/main.cpp` para:
- Teoría completa sobre NTC y Steinhart-Hart
- Explicación detallada de calibración eFuse
- Comparación de métodos de calibración
- Troubleshooting y mejoras opcionales
