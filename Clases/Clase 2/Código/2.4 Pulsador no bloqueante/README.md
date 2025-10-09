# Pulsador No Bloqueante con Antirrebote

## 📖 Descripción

Lectura de pulsador con debouncing no bloqueante usando millis(). No utiliza delay() ni while(), permitiendo ejecutar otras tareas simultáneamente mientras monitorea el estado del botón.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| Pulsador | GPIO 0 | Con pull-up interno |

---

## 💡 Características

- Debouncing no bloqueante con millis()
- Tiempo de antirrebote de 50ms
- Salida por Serial (PRESIONADO/SUELTO)
- No usa delay() ni while()

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Pulsar botón y observar mensajes

---

## 📚 Clase

**Módulo:** Clase 2 - Entradas y Salidas Digitales  
Ver `src/main.cpp` para detalles de implementación
