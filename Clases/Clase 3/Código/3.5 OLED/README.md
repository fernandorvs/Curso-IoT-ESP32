# Pantalla OLED - Display Gráfico

## 📖 Descripción

Control básico de pantalla OLED SSD1306 128x64 mediante I2C. Muestra texto y gráficos usando librería U8G2. Base para crear interfaces de usuario e instrumentos de medición.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| OLED SSD1306 | SDA: GPIO20, SCL: GPIO21 | Display 128x64 I2C |

---

## 💡 Características

- Display OLED 128x64 píxeles
- Comunicación I2C
- Librería U8G2 para gráficos
- Muestra texto "UNSE" centrado

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Compilar y subir con PlatformIO
3. Observar texto en pantalla OLED

---

## 📊 Diagrama de Flujo

![Diagrama OLED](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/oled_basico.pu)

---

## � Clase

**Módulo:** Clase 3 - Sensores y Periféricos  
Ver `src/main.cpp` para detalles de implementación
