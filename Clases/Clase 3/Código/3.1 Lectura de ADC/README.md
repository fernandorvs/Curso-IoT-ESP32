# Lectura de ADC - Conversión Analógica Digital

## 📖 Descripción

Lectura básica del ADC (Conversor Analógico-Digital) del ESP32 con conversión a voltaje. Usa potenciómetro para generar señal analógica variable entre 0-3.3V.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| Potenciómetro | GPIO 1 | ADC entrada analógica |

---

## 💡 Características

- ADC de 12 bits (0-4095)
- Conversión a voltaje (0-3.3V)
- Lectura de potenciómetro
- Salida por Serial cada 200ms

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Girar potenciómetro y observar valores

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación
