# Blink No Bloqueante

## 📖 Descripción

Implementación de parpadeo de LED sin usar delay(), permitiendo ejecutar otras tareas en paralelo. Usa millis() para timing no bloqueante, fundamental para sistemas multitarea.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| LED | GPIO 2 | LED integrado |

---

## 💡 Características

- Timing no bloqueante con millis()
- LED parpadea cada 500ms
- Loop libre para ejecutar otras tareas
- Patrón fundamental para multitarea

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Observar LED parpadear sin bloquear ejecución

---

## 📚 Clase

**Módulo:** Clase 2 - Entradas y Salidas Digitales  
Ver `src/main.cpp` para detalles de implementación
