# Clase 3 - Sensores Analógicos, Digitales y Visualización OLED

## Sistema integrado: dos sensores de temperatura visualizados en OLED.
- **Hardware**: OLED + NTC + DS18B20 (combinación de proyectos anteriores)

## 🔧 Cómo Usar los Proyectos

### Objetivos

Al finalizar esta clase serás capaz de:
- Leer sensores analógicos usando el ADC del ESP32
- Aplicar filtros digitales para estabilizar lecturas
- Integrar sensores de temperatura (NTC y DS18B20)
- Controlar displays OLED para visualización de datos
- Crear sistemas de monitoreo completos

## 📖 Conceptos Clave

### ADC (Conversor Analógico-Digital)
- **Resolución**: 12 bits (0-4095) → Voltaje: 0-3.3V
- **Conversión**: `voltaje = (valorADC / 4095.0) * 3.3V`
- **Recomendación**: Usar ADC1 (GPIO 32-39) para proyectos IoT

### Filtro EMA (Exponential Moving Average)
- **Fórmula**: `y[n] = α × x[n] + (1-α) × y[n-1]`
- **α bajo (0.1)**: Muy suave, respuesta lenta
- **α alto (0.5)**: Más responsive, menos suavizado

### Sensores de Temperatura

| Sensor | Tipo | Precisión | Protocolo | Costo |
|--------|------|-----------|-----------|-------|
| **NTC** | Analógico | ±2°C | ADC | Bajo |
| **DS18B20** | Digital | ±0.5°C | 1-Wire | Medio |

### Display OLED (SSD1306)
- **Protocolo**: I2C (SDA: GPIO21, SCL: GPIO22)
- **Resolución**: 128x64 píxeles
- **Dirección**: 0x3C (típica)

## 📊 Diagramas de Flujo

### 3.1 - Lectura de ADC
![Diagrama Lectura ADC](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/lectura_adc.pu)

### 3.2 - Filtros Digitales
![Diagrama Filtros Digitales](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/filtros_digitales.pu)

### 3.3 - Lectura de NTC
![Diagrama Lectura NTC](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/lectura_ntc.pu)

### 3.4 - Lectura de DS18B20
![Diagrama DS18B20](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/lectura_ds18b20.pu)

### 3.5 - OLED Básico
![Diagrama OLED](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/oled_basico.pu)

### 3.6 - Temperaturas en OLED
![Diagrama Sistema Integrado](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%203/Diagramas/temperaturas_oled.pu)

##  Proyectos Incluidos

### 3.1 - Lectura de ADC
📁 `Código/3.1 Lectura de ADC/` | 📄 [README](Código/3.1%20Lectura%20de%20ADC/README.md)

Lectura básica del ADC con conversión a voltaje usando potenciómetro.
- **Hardware**: Potenciómetro en GPIO 34

### 3.2 - Filtros Digitales
📁 `Código/3.2 Filtros Digitales/` | 📄 [README](Código/3.2%20Filtros%20Digitales/README.md)

Implementación de filtro EMA para suavizar señales ruidosas. Visualización comparativa en Serial Plotter.
- **Hardware**: Potenciómetro en GPIO 34

### 3.3 - Lectura de NTC
📁 `Código/3.3 Lectura de NTC/` | 📄 [README](Código/3.3%20Lectura%20de%20NTC/README.md)

Sensor de temperatura analógico usando termistor NTC con ecuación Steinhart-Hart.
- **Hardware**: NTC 10kΩ en GPIO 34 + resistencia 10kΩ, LED en GPIO 2

### 3.4 - Lectura de DS18B20
📁 `Código/3.4 Lectura de DS18B20/` | 📄 [README](Código/3.4%20Lectura%20de%20DS18B20/README.md)

Sensor de temperatura digital de alta precisión con protocolo 1-Wire.
- **Hardware**: DS18B20 en GPIO 4 + pull-up 4.7kΩ

### 3.5 - OLED
📁 `Código/3.5 OLED/` | 📄 [README](Código/3.5%20OLED/README.md)

Control básico de display OLED SSD1306 con comunicación I2C.
- **Hardware**: OLED SSD1306 (SDA: GPIO21, SCL: GPIO22)

### 3.6 - Temperaturas en OLED
📁 `Código/3.6 Temperaturas en OLED/` | 📄 [README](Código/3.6%20Temperaturas%20en%20OLED/README.md)

Sistema integrado: dos sensores de temperatura visualizados en OLED.
- **Hardware**: OLED + NTC + DS18B20 (combinación de proyectos anteriores)

## 🔧 Cómo Usar los Proyectos

### Requisitos
- **PlatformIO**: Extensión para VS Code o CLI
- **Hardware**: ESP32 o ESP32-C3 + componentes según proyecto

### Conexiones Típicas
```
GPIO 34 → Potenciómetro/NTC (entrada analógica)
GPIO 4  → DS18B20 + pull-up 4.7kΩ
GPIO 21 → SDA (OLED)
GPIO 22 → SCL (OLED)
GPIO 2  → LED indicador
```

### Pasos
1. Abrir proyecto en VS Code + PlatformIO
2. Seleccionar entorno: `esp32` o `esp32c3`
3. Conectar hardware según diagrama
4. Build & Upload (`→`)
5. Abrir Serial Monitor (115200 baudios)

## 📈 Progresión del Aprendizaje

```
3.1 ADC → 3.2 Filtros → 3.3 NTC → 3.4 DS18B20 → 3.5 OLED → 3.6 Integración
```

Cada proyecto construye sobre el anterior, culminando en un sistema completo de monitoreo.

## 🎯 Ejercicios Propuestos

### Nivel Básico
- Calibrar NTC comparando con termómetro real
- Conectar múltiples DS18B20 en la misma línea 1-Wire
- Agregar íconos y mejoras visuales al OLED

### Nivel Intermedio
- Implementar alarmas visuales/sonoras por temperatura
- Guardar histórico de lecturas en SD card
- Crear gráficos de temperatura en tiempo real

### Nivel Avanzado
- Enviar datos por WiFi a servidor/MQTT
- Dashboard web con visualización en tiempo real
- Sistema multi-sensor (agregar humedad, presión, etc.)

## 📚 Referencias

### Documentación Oficial
- [ESP32 ADC](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)
- [DS18B20 Datasheet](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf)
- [SSD1306 OLED](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)

### Librerías
- [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

---

**Próxima clase**: Conectividad WiFi y comunicación IoT 🌐