# Lectura de NTC - Sensor de Temperatura

## 📖 Descripción

Medición de temperatura usando termistor NTC 10kΩ en configuración de divisor de tensión. Convierte resistencia a temperatura usando ecuación Steinhart-Hart simplificada (parámetro Beta).

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
- Fórmula correcta para divisor: `R = R_FIXED * v / (VREF - v)`
- LED indicador al superar 30°C
- Actualización cada 500ms

---

## 🚀 Uso

1. Conectar hardware según tabla y circuito
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Observar temperatura cada medio segundo

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación
