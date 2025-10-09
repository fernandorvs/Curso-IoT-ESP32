# 🚀 Curso IoT ESP32 - UNSE - 2025

**Universidad Nacional de Santiago del Estero**  
**Instructor:** Ing. Fernando Raúl Vera Suasnávar | **Año:** 2025

Curso completo de desarrollo IoT con ESP32: desde fundamentos hasta servidores web y control remoto.

---

## 📚 Contenido del Curso

### 📘 [Clase 1 - Introducción al IoT y ESP32](Clases/Clase%201/)
**Tema:** Fundamentos y configuración inicial  
**Proyecto:** `1.1 Blink` - LED parpadeante básico  
**Conceptos:** GPIO, pinMode, digitalWrite, delay, estructura setup/loop

### 📗 [Clase 2 - Control Avanzado y Programación No Bloqueante](Clases/Clase%202/)
**Temas:** PWM, antirrebote, millis(), FSM  
**Proyectos:**
- `2.1 Led con PWM` - Control de intensidad con corrección gamma
- `2.2 Pulsador con Antirrebote` - Debounce bloqueante
- `2.3 Blink no bloqueante` - Temporización con millis()
- `2.4 Pulsador no bloqueante` - Detección de eventos sin bloqueo
- `2.5 Blink + Pulsador no bloqueantes` - Multitarea básica
- `2.6 FSM Máquina de estado` - Estados y transiciones

### 📙 [Clase 3 - Sensores y Visualización OLED](Clases/Clase%203/)
**Temas:** ADC, filtros digitales, sensores temperatura, I2C  
**Proyectos:**
- `3.1 Lectura de ADC` - Convertidor analógico-digital 12 bits
- `3.2 Filtros Digitales` - EMA para estabilizar señales
- `3.3 Lectura de NTC` - Termistor con modelo Beta
- `3.4 Lectura de DS18B20` - Sensor digital 1-Wire
- `3.5 OLED` - Display SSD1306 128x64
- `3.6 Temperaturas en OLED` - Sistema monitoreo completo

### 📕 [Clase 4 - Servidor Web IoT](Clases/Clase%204/)
**Temas:** WiFi, HTTP, API REST, HTML/CSS/JS, LittleFS  
**Proyectos:**
- `4.1 WiFi Básico` - Conexión y reconexión automática
- `4.2 Servidor Básico` - WebServer con rutas HTTP
- `4.3 Control LED Remoto` - Actuación vía web
- `4.4 Lectura de Sensores` - API REST JSON
- `4.5 Dashboard Completo` - Interfaz web responsive

---

## 🎯 Objetivos de Aprendizaje

✅ IoT y arquitectura de sistemas embebidos  
✅ Programación ESP32 con PlatformIO + Arduino Framework  
✅ Control digital/analógico (GPIO, PWM, ADC)  
✅ Programación no bloqueante y FSM  
✅ Sensores analógicos (NTC) y digitales (DS18B20)  
✅ Displays OLED I2C  
✅ Conectividad WiFi y servidores web  
✅ APIs REST y dashboards HTML/JS

---

## 🚀 Cómo Empezar

### Requisitos
- **Hardware:** ESP32 DevKit, cable USB, LEDs, resistencias, sensores
- **Software:** VS Code + PlatformIO IDE

### Instalación
```bash
# 1. Instalar VS Code + extensión PlatformIO
# 2. Clonar repositorio
git clone https://github.com/fernandorvs/Taller-ESP32.git

# 3. Abrir proyecto en VS Code
# 4. Compilar y subir (PlatformIO: Upload)
```

### Primer Proyecto
1. Abrir `Clases/Clase 1/Código/1.1 Blink/`
2. Compilar con PlatformIO (✓)
3. Subir al ESP32 (→)
4. ¡LED integrado parpadeando! 🎉

---

## 🛠️ Tecnologías

**Hardware:** ESP32 DevKit, sensores (NTC, DS18B20), OLED SSD1306  
**Software:** VS Code, PlatformIO, Arduino Framework  
**Librerías:** WiFi, WebServer, OneWire, DallasTemperature, Adafruit_SSD1306

---

## 📖 Recursos

- [Documentación ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [PlatformIO](https://platformio.org/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

---

## 📂 Estructura del Repositorio

```
📦 Curso-IoT-ESP32
├── 📁 Clases/
│   ├── 📁 Clase 1/ → Introducción y Blink (1 proyecto)
│   ├── 📁 Clase 2/ → PWM, No bloqueante, FSM (6 proyectos)
│   ├── 📁 Clase 3/ → Sensores y OLED (6 proyectos)
│   └── 📁 Clase 4/ → WiFi y Servidor Web (5 proyectos)
└── 📄 README.md → Este archivo
```

---

## 📝 Licencia

Material educativo de la Universidad Nacional de Santiago del Estero (UNSE)

---

**¿Listo para comenzar?** Empieza con la [Clase 1](Clases/Clase%201/) y sumérgete en el mundo del IoT con ESP32! 🎓✨ 
