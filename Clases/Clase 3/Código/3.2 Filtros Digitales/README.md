# Filtros Digitales - Suavizado de Señales

## 📖 Descripción

Implementación de filtro EMA (Exponential Moving Average) para suavizar lecturas ruidosas del ADC. Compara señal cruda vs filtrada en Serial Plotter para visualizar mejora en estabilidad.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| Potenciómetro | GPIO 1 | ADC entrada analógica |

---

## 💡 Características

- Filtro EMA con alpha = 0.2
- Comparación señal cruda vs filtrada
- Visualización en Serial Plotter
- Reduce ruido en lecturas ADC

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Abrir Serial Plotter (115200 baudios)
4. Observar diferencia entre señales

---

## 📚 Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación
