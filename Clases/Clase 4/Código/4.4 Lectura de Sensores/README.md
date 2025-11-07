# Lectura de Sensores - Monitoreo Web de Temperatura

## 📖 Descripción

Sistema web que muestra lecturas de sensores de temperatura (NTC y DS18B20) a través de peticiones GET. Interface HTML con auto-refresh cada 5 segundos para monitoreo en tiempo real.

**Características técnicas:** Calibración automática ADC mediante eFuse, validación de errores, y endpoints REST para consulta individual o conjunta de sensores.

---

## 🔧 Hardware

| Componente | Pin ESP32-C3 | Pin ESP32 DevKit | Notas |
|------------|--------------|------------------|-------|
| NTC 10kΩ | GPIO 1 (ADC1_CH0) | GPIO 32 (ADC1_CH4) | + Resistencia fija 10kΩ |
| DS18B20 | GPIO 3 | GPIO 4 | + Pull-up 4.7kΩ (1-Wire) |

**Circuito NTC (divisor de tensión):**
```
3.3V ─── R_fija(10kΩ) ─── [GPIO ADC] ─── NTC(10kΩ) ─── GND
```

**Circuito DS18B20 (modo normal):**
```
3.3V ─── [4.7kΩ Pull-up] ─── [GPIO] ─── DS18B20 ─── GND
```

**IMPORTANTE:** Se usan pines ADC1 (compatibles con WiFi activo)

---

## 💡 Características

### Sensores
- **NTC 10kΩ**: Analógico, respuesta rápida, calibración eFuse
- **DS18B20**: Digital 1-Wire, alta precisión (±0.5°C)
- **Validación**: Detección automática de errores y desconexiones
- **Dual-sensor**: Comparación y validación cruzada

### Web Server
- **Método GET únicamente**: Ideal para consultas de solo lectura
- **Auto-refresh**: Página principal se actualiza cada 5 segundos
- **Endpoints múltiples**:
  - `/` - Página HTML con interfaz visual
  - `/temperaturas` - Todas las lecturas (texto plano)
  - `/ntc` - Solo sensor NTC
  - `/ds18b20` - Solo sensor DS18B20
- **Manejo de errores**: Muestra "ERROR" cuando sensor desconectado

### Técnicas Avanzadas
- Calibración ADC con valores eFuse de fábrica
- Ecuación Steinhart-Hart para conversión NTC
- Validación de rangos (voltaje, resistencia, temperatura)
- Control de timing para lecturas periódicas
- Interfaz responsive con CSS inline

---

## 🌐 API REST (Solo GET)

### Página Principal
```
GET http://[IP-ESP32]/
```
Interface HTML con auto-refresh cada 5 segundos

### Todas las Temperaturas
```
GET http://[IP-ESP32]/temperaturas
```
**Respuesta (texto plano):**
```
=== LECTURAS DE TEMPERATURA ===

NTC (Analogico):     23.45 C
DS18B20 (Digital):   23.82 C

Diferencia:          0.37 C
```

### Solo NTC
```
GET http://[IP-ESP32]/ntc
```
**Respuesta:** `Temperatura NTC: 23.45 C`

### Solo DS18B20
```
GET http://[IP-ESP32]/ds18b20
```
**Respuesta:** `Temperatura DS18B20: 23.82 C`

---

## 🎯 Calibración ADC (eFuse)

El sistema utiliza calibración automática del ADC:

```cpp
#include <esp_adc_cal.h>
esp_adc_cal_characteristics_t adc_chars;

// En setup():
esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, 
                         ADC_WIDTH_BIT_12, 1100, &adc_chars);

// En lectura:
uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
float v = voltage_mv / 1000.0;
```

**Ventajas:**
- Compensa variaciones de referencia interna
- Funciona automáticamente en cualquier ESP32
- Mejora precisión de ±10% a ±2-3%
- No requiere calibración manual

---

## 🚀 Uso

### Configuración Inicial
1. **Editar credenciales WiFi** en `main.cpp`:
   ```cpp
   const char *ssid = "TU_RED";
   const char *password = "TU_CONTRASEÑA";
   ```

2. **Conectar sensores** según tabla de hardware

3. **Compilar y subir** con PlatformIO

### Operación
1. Abrir Serial Monitor (115200 baudios)
2. Esperar conexión WiFi
3. Anotar la IP mostrada: `http://192.168.x.x`
4. Abrir navegador en esa IP
5. La página se actualiza automáticamente cada 5 segundos

### Ejemplos de Consulta

**Con navegador:**
- `http://192.168.1.100/` - Interface visual
- `http://192.168.1.100/temperaturas` - Texto plano

**Con curl:**
```bash
# Todas las temperaturas
curl http://192.168.1.100/temperaturas

# Solo NTC
curl http://192.168.1.100/ntc

# Solo DS18B20
curl http://192.168.1.100/ds18b20
```

---

## 🔍 Manejo de Errores

El sistema valida todas las lecturas y detecta:

| Error | Condición | Valor retornado |
|-------|-----------|-----------------|
| NTC voltaje | < 0.1V o > 3.2V | -999.0 |
| NTC resistencia | < 100Ω o > 1MΩ | -999.0 |
| NTC temperatura | < -50°C o > 150°C | -999.0 |
| DS18B20 | -127.0°C o 85.0°C | -999.0 |

**En la interfaz web:** Se muestra "ERROR - Sensor no conectado"

---

## 📊 Salida Serial

```
=== ESP32 Monitor de Temperaturas ===
ADC calibrado con valores eFuse
Sensor NTC configurado en pin 1
  - Lectura ADC inicial: 2048 (1.650V calibrado)
Sensor DS18B20 configurado en pin 3
  - Sensores DS18B20 detectados: 1

--- Primera lectura de sensores ---
✓ NTC: 23.45°C
✓ DS18B20: 23.82°C
-----------------------------------

Conectando a WiFi.....
WiFi conectado!
==================================
Abrir en el navegador: http://192.168.1.100
==================================
Servidor web iniciado

Sensores - NTC: 23.45°C | DS18B20: 23.82°C | Diff: 0.37°C
Status: WiFi OK | http://192.168.1.100 | RSSI: -45 dBm
```

---

## 🔧 Troubleshooting

| Problema | Causa probable | Solución |
|----------|----------------|----------|
| NTC = ERROR | Voltaje fuera de rango | Verificar divisor resistivo |
| NTC valores erráticos | Ruido ADC | Cables cortos, filtro EMA |
| DS18B20 = -127°C | Sensor desconectado | Verificar cableado |
| DS18B20 = 85°C | Sin pull-up | Agregar resistor 4.7kΩ |
| No conecta WiFi | Credenciales incorrectas | Verificar SSID/password |
| Página no carga | IP incorrecta | Revisar Serial Monitor |

---

## 📚 Clase

**Módulo:** Clase 4 - Conectividad y Protocolos  

Ver `src/main.cpp` para:
- Teoría completa de sensores NTC y DS18B20
- Implementación de calibración eFuse
- Estructura del servidor web
- Manejo de errores y validaciones
- Ecuación Steinhart-Hart detallada
