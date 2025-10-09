# Blink + Pulsador No Bloqueantes

## 📖 Descripción

Integración de dos tareas independientes ejecutándose simultáneamente: parpadeo de LED y lectura de pulsador, ambos con timing no bloqueante. Demuestra cómo múltiples procesos pueden coexistir sin interferirse.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| LED | GPIO 2 | LED integrado |
| Pulsador | GPIO 0 | Con pull-up interno |

---

## 💡 Características

- Dos tareas simultáneas no bloqueantes
- LED parpadea cada 500ms
- Pulsador con debouncing de 50ms
- Multitarea cooperativa simple

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Observar LED parpadeando mientras pulsador responde

---

## 📚 Clase

**Módulo:** Clase 2 - Entradas y Salidas Digitales  
Ver `src/main.cpp` para detalles de implementación
- **Documentación ESP32:** [Espressif Documentation](https://docs.espressif.com/)
- **PlatformIO:** [platformio.org](https://platformio.org/)
- Ver comentarios detallados en `src/main.cpp` para información técnica completa
