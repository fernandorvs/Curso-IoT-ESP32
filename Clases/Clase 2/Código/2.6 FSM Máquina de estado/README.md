# FSM - Máquina de Estados Finitos

## 📖 Descripción

Implementación de Máquina de Estados Finitos (FSM) para controlar LED con tres estados: OFF, ON, BLINK. Cada pulsación cambia al siguiente estado de forma cíclica. Patrón fundamental para sistemas complejos y control de flujo estructurado.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| LED | GPIO 2 | LED integrado |
| Pulsador | GPIO 0 | Con pull-up interno |

---
 
## 💡 Características

- Máquina de estados con 3 estados (OFF/ON/BLINK)
- Transiciones mediante pulsador
- Debouncing no bloqueante
- Patrón escalable para sistemas complejos

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Pulsar botón para cambiar estados cíclicamente

---

## 📚 Clase

**Módulo:** Clase 2 - Entradas y Salidas Digitales  
Ver `src/main.cpp` para detalles de implementación
- **Documentación ESP32:** [Espressif Documentation](https://docs.espressif.com/)
- **PlatformIO:** [platformio.org](https://platformio.org/)
- Ver comentarios detallados en `src/main.cpp` para información técnica completa
