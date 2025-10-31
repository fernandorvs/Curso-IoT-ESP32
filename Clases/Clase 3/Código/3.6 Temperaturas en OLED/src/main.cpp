/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 3 - Sensores y Periféricos
    PROYECTO:    Temperaturas en OLED - Sistema de Monitoreo
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Sistema integrado que muestra lecturas simultáneas de dos sensores
    de temperatura (NTC y DS18B20) en pantalla OLED. Combina sensores
    analógicos, digitales y display en interfaz de usuario completa.
    Utiliza calibración automática eFuse del ADC para lecturas precisas
    del NTC y modo parásito del DS18B20 con control manual de timing.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#include <Wire.h>
#include <U8g2lib.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <esp_adc_cal.h>

#include <math.h>

// Configuración de pines
#define PIN_NTC_ADC   1    // Pin ADC para NTC
#define PIN_DS18B20   3    // Pin para DS18B20

// Constructor para OLED I2C 128x64 (mismo que 3.5 OLED)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

OneWire oneWire(PIN_DS18B20);
DallasTemperature ds(&oneWire);

const float VREF = 3.3, R_FIXED = 10000, R0 = 10000, PT0 = 298.15, BETA = 3950;

// Para calibración eFuse
esp_adc_cal_characteristics_t adc_chars;

void setup() {
    // ESP32-C3 SDA = GPIO8, SCL = GPIO9 (mismo que 3.5 OLED)
    Wire.begin(8, 9);
    u8g2.begin();
    ds.begin();

    // Calibrar ADC con valores eFuse de fábrica (compensa variaciones individuales)
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, &adc_chars);
}

void loop() {
    // Leer NTC
    int raw = analogRead(PIN_NTC_ADC);
    uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
    float v = voltage_mv / 1000.0;
    float R = R_FIXED * v / (VREF - v);
    float Tc = (1 / (1 / PT0 + log(R / R0) / BETA)) - 273.15;

    // Leer DS18B20

    // Control manual del tiempo para modo parásito
    ds.setWaitForConversion(false);
    ds.requestTemperatures();

    // Tiempo necesario para conversión en modo parásito
    delay(1000);

    float Td = ds.getTempCByIndex(0);

    // Mostrar en OLED con U8G2
    u8g2.clearBuffer();
    
    // Configurar fuente
    u8g2.setFont(u8g2_font_7x13_tf);
    
    // Mostrar temperaturas
    u8g2.setCursor(0, 15);
    u8g2.print("NTC:     ");
    u8g2.print(Tc, 1);
    u8g2.print(" C");
    
    u8g2.setCursor(0, 30);
    u8g2.print("DS18B20: ");
    u8g2.print(Td, 1);
    u8g2.print(" C");
    
    // Título UNSE al final
    u8g2.setFont(u8g2_font_helvB10_tf);
    int xUnse = (128 - u8g2.getStrWidth("UNSE")) / 2;
    u8g2.drawStr(xUnse, 60, "UNSE");
    
    u8g2.sendBuffer();
    delay(500);
}

/*
===============================================================================
              INTEGRACIÓN SENSORES + DISPLAY - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

SISTEMA MULTI-SENSOR:
Combina múltiples fuentes de datos (NTC analógico, DS18B20 digital)
con interfaz visual (OLED). Patrón común en instrumentación y IoT
para monitoreo en tiempo real con validación cruzada de mediciones.

COMPARACIÓN DE SENSORES:
┌──────────┬──────────┬───────────┬──────────┬─────────────┐
│ Sensor   │ Precisión│ Respuesta │ Costo    │ Interfaz    │
├──────────┼──────────┼───────────┼──────────┼─────────────┤
│ NTC      │ ±2-3°C   │ Rápida    │ Bajo     │ Analógico   │
│ DS18B20  │ ±0.5°C   │ 750ms     │ Medio    │ Digital 1W  │
└──────────┴──────────┴───────────┴──────────┴─────────────┘

VENTAJAS DE DUAL-SENSOR:
- Validación cruzada: Detectar fallos por comparación
- Redundancia: Continuar operación si un sensor falla
- Características complementarias: Rapidez vs precisión
- Calibración comparativa: Ajustar NTC usando DS18B20

--- ARQUITECTURA DEL SISTEMA ---

Flujo de datos:
  1. Leer NTC (ADC con calibración eFuse)
  2. Convertir voltaje → resistencia → temperatura
  3. Solicitar conversión DS18B20 (modo parásito)
  4. Esperar 1 segundo (conversión 12-bit)
  5. Leer temperatura DS18B20
  6. Actualizar buffer OLED con ambas lecturas
  7. Renderizar en pantalla
  8. Esperar 500ms y repetir

Configuración de pines (ESP32-C3):
  NTC:     GPIO 1  (ADC - Divisor de tensión)
  DS18B20: GPIO 3  (1-Wire con modo parásito)
  OLED:    GPIO 8 (SDA), GPIO 9 (SCL) - I2C

--- CALIBRACIÓN ADC (eFuse) ---

El código utiliza calibración automática para el NTC:

  #include <esp_adc_cal.h>
  esp_adc_cal_characteristics_t adc_chars;
  
  // Cargar valores de fábrica
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, 
                           ADC_WIDTH_BIT_12, 1100, &adc_chars);
  
  // Convertir lectura a voltaje calibrado
  uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);

Esto mejora la precisión del NTC de ±10% a ±2-3% sin calibración manual.

--- MODO PARÁSITO DS18B20 ---

El DS18B20 funciona en modo parásito (alimentado desde línea de datos):

  ds.setWaitForConversion(false);  // Control manual
  ds.requestTemperatures();         // Iniciar conversión
  delay(1000);                      // Esperar conversión
  float temp = ds.getTempCByIndex(0);

VENTAJAS modo parásito:
- Solo 2 cables (datos + GND), sin VCC separado
- Simplifica cableado en instalaciones

CONSIDERACIONES:
- Delay de 1000ms necesario para conversión 12-bit (750ms típico + margen)
- Resistor pull-up 4.7kΩ necesario en línea de datos
- Limitación de distancia (~10m máximo)

--- LIBRERÍAS UTILIZADAS ---

U8g2 (Display OLED):
  u8g2.begin()                    // Inicializar display
  u8g2.clearBuffer()              // Limpiar buffer interno
  u8g2.setFont(font)              // Seleccionar fuente
  u8g2.setCursor(x, y)            // Posicionar cursor
  u8g2.print(value)               // Escribir texto/número
  u8g2.drawStr(x, y, text)        // Dibujar string centrado
  u8g2.sendBuffer()               // Enviar buffer a pantalla

DallasTemperature (DS18B20):
  ds.begin()                      // Inicializar sensor
  ds.setWaitForConversion(false)  // Modo asíncrono
  ds.requestTemperatures()        // Solicitar conversión
  ds.getTempCByIndex(0)           // Leer temperatura

--- DISEÑO DE INTERFAZ ---

Layout actual:
  ┌────────────────────────────┐
  │ NTC:     22.5 C            │  ← Fuente 7x13
  │ DS18B20: 23.1 C            │  ← Fuente 7x13
  │                            │
  │         UNSE               │  ← Fuente Bold 10, centrado
  └────────────────────────────┘

Principios aplicados:
- Información clara y legible
- Formato consistente (1 decimal)
- Actualización sin parpadeo (buffer completo)
- Identificación institucional

--- MEJORAS OPCIONALES ---

Indicador de diferencia entre sensores:
  float diff = abs(Tc - Td);
  u8g2.printf("Diff: %.1f C", diff);

Alarma por discrepancia (>5°C):
  if(diff > 5.0) {
    u8g2.drawBox(0, 0, 128, 10);
    u8g2.setColorIndex(0);
    u8g2.drawStr(20, 8, "CHECK SENSORS");
  }

Filtro EMA para suavizar NTC:
  static float ema = 0;
  if(ema == 0) ema = Tc;
  ema = 0.2 * Tc + 0.8 * ema;

Validación cruzada (detectar fallo):
  if(abs(Tc - Td) > 10.0) {
    if(Td == -127.0 || Td == 85.0) usar_NTC();
    if(Tc < -50 || Tc > 100) usar_DS18B20();
  }

--- TIMING Y PERFORMANCE ---

Ciclo completo: ~1.5 segundos
  • Leer NTC: ~1ms
  • Conversión DS18B20: 1000ms (modo parásito)
  • Actualizar OLED: ~20ms
  • Delay adicional: 500ms

--- NOTAS IMPORTANTES ---

• DS18B20 modo parásito requiere pull-up 4.7kΩ y delay 1000ms
• NTC responde más rápido que DS18B20 por inercia térmica
• OLED consume ~8mA durante actualización
• Direcciones I2C típicas: 0x3C o 0x3D
• U8g2 usa 1KB RAM para buffer 128x64

--- TROUBLESHOOTING ---

OLED no muestra: Verificar dirección I2C y conexiones SDA/SCL
DS18B20 lee -127°C: Sensor desconectado o sin pull-up 4.7kΩ
DS18B20 lee 85°C: Conversión no completada, aumentar delay
NTC erráticos: Añadir filtro EMA o verificar conexiones
Gran diferencia: Normal por inercia térmica, esperar 2 minutos

===============================================================================
*/
