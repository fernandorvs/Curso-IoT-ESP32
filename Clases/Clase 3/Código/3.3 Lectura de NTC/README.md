# Lectura de NTC - Sensor de Temperatura (Calibración Manual)

## 📖 Descripción

Medición de temperatura usando termistor NTC 10kΩ en configuración de divisor de tensión. Convierte resistencia a temperatura usando ecuación Steinhart-Hart simplificada (parámetro Beta).

**Incluye calibración manual del ADC** mediante corrección por voltaje medido con multímetro para compensar errores de la referencia interna del ESP32.

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
- **Calibración manual del ADC** con factor de corrección
- Configuración ADC optimizada:
  - Atenuación ADC_11db (rango 0-3.3V)
  - Resolución 12 bits (0-4095)
  - Pin en alta impedancia (sin pull-up/pull-down)
- Muestra voltaje, resistencia y temperatura
- LED indicador al superar 30°C
- Actualización cada 500ms

---

## 🎯 Calibración del ADC

El ESP32 tiene una referencia interna que puede variar ±10% entre chips, causando errores en la medición.

**Procedimiento de calibración:**

1. Subir el código al ESP32
2. Medir el voltaje REAL en GPIO 1 con un multímetro
3. Observar el voltaje MEDIDO en el Serial Monitor
4. Calcular factor: `ADC_CORRECTION = V_real / V_medido`
5. Actualizar la constante en el código:
   ```cpp
   const float ADC_CORRECTION = 1.73 / 1.91;  // Ejemplo
   ```
6. Recompilar y subir

**Ejemplo:**
- Multímetro: 1.73V
- Serial: 1.91V  
- Factor: 1.73 / 1.91 = 0.905

Esto corrige el error sistemático y proporciona lecturas precisas de temperatura.

---

## 🚀 Uso

1. Conectar hardware según tabla y circuito
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Realizar calibración (ver sección anterior)
5. Observar: Voltaje | Resistencia | Temperatura

**Salida típica:**
```
V=1.730V | R=10000Ω | T=25.00°C
```

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación y teoría completa sobre:
- Ecuación Steinhart-Hart
- Divisores de tensión
- Calibración del ADC
- Solución de problemas
