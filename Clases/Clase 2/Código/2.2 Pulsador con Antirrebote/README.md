# Pulsador con Antirrebote (Debouncing)

## 📖 Descripción

Lectura de pulsador con técnica de debouncing por software. Elimina rebotes mecánicos del pulsador para detectar pulsaciones limpias y confiables. Cada pulsación invierte el estado del LED.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| Pulsador | GPIO 0 | Con pull-up interno |
| LED | GPIO 2 | LED integrado |

---

## 💡 Características

- Debouncing por software con delay de 50ms
- INPUT_PULLUP para resistencias internas
- Toggle de LED con cada pulsación válida
- Espera a soltar botón con while

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Pulsar botón para cambiar estado del LED

---

## 📚 Clase

**Módulo:** Clase 2 - Entradas y Salidas Digitales  
Ver `src/main.cpp` para detalles de implementación
