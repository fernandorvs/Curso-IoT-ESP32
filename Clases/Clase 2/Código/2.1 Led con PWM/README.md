# LED con PWM - Control de Brillo

## 📖 Descripción

Control de brillo de LED mediante PWM (Modulación por Ancho de Pulso). Implementa efecto fade in/fade out con corrección gamma para transición visual suave y lineal.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| LED | GPIO 2 | LED integrado |

---

## 💡 Características

- Control PWM a 5kHz con resolución de 8 bits
- Efecto fade in/fade out automático
- Corrección gamma para percepción visual lineal
- Duty cycle variable 0-255 (0-100%)

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Observar LED con efecto de respiración

---

## 📚 Clase

**Módulo:** Clase 2 - Entradas y Salidas Digitales  
Ver `src/main.cpp` para detalles de implementación
