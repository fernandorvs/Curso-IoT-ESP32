# Lectura de NTC - Sensor de Temperatura

## 📖 Descripción

Medición de temperatura usando termistor NTC 10kΩ en configuración de divisor de tensión. Convierte resistencia a temperatura usando ecuación Steinhart-Hart simplificada (parámetro Beta).

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| NTC 10kΩ | GPIO 34 | Con resistencia 10kΩ |
| LED | GPIO 2 | Indica T > 30°C |

---

## 💡 Características

- Termistor NTC 10kΩ (Beta=3950)
- Ecuación Steinhart-Hart simplificada
- Control LED según temperatura
- Salida en °C cada 500ms

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. LED enciende si T > 30°C

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación
