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
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#include <Wire.h>
#include <U8g2lib.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <math.h>

// Constructor para OLED I2C 128x64 (mismo que 3.5 OLED)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

OneWire oneWire(4);
DallasTemperature ds(&oneWire);

const float VREF = 3.3, R_FIXED = 10000, R0 = 10000, PT0 = 298.15, BETA = 3950;

void setup() {
    // ESP32-C3 SDA = GPIO20, SCL = GPIO21 (mismo que 3.5 OLED)
    Wire.begin(20, 21);
    u8g2.begin();
    ds.begin();
}

void loop() {
    // Leer NTC
    int raw = analogRead(34);
    float v = raw * (VREF / 4095.0);
    float R = R_FIXED * (VREF / v - 1);
    float Tc = (1 / (1 / PT0 + log(R / R0) / BETA)) - 273.15;

    // Leer DS18B20
    ds.requestTemperatures();
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
para monitoreo en tiempo real.

COMPARACIÓN DE SENSORES:
NTC: Respuesta rápida, menor precisión, económico
DS18B20: Mayor precisión (±0.5°C), salida digital, más costoso

DISEÑO DE INTERFAZ:
- Información clara y legible
- Actualización periódica sin parpadeo
- Formato consistente de datos
- Feedback visual de estado

--- ARQUITECTURA DEL SISTEMA ---

Flujo de datos:
1. Leer sensores (analógico y digital)
2. Procesar/convertir lecturas
3. Actualizar buffer de display
4. Renderizar en pantalla
5. Esperar intervalo y repetir

Pines típicos:
  NTC:     GPIO34 (ADC)
  DS18B20: GPIO4 (1-Wire)
  OLED:    GPIO8(SDA), GPIO9(SCL) - I2C

--- LIBRERÍAS UTILIZADAS ---

Adafruit_SSD1306:
  display.begin(mode, address)
  display.clearDisplay()
  display.setCursor(x, y)
  display.setTextSize(size)
  display.printf(format, ...)
  display.display()

DallasTemperature:
  ds.begin()
  ds.requestTemperatures()
  ds.getTempCByIndex(index)

--- EJEMPLO PRÁCTICO ---

Display mejorado con formato:
  display.clearDisplay();
  display.setTextSize(1);
  
  display.setCursor(0, 0);
  display.println("TEMPERATURAS");
  display.drawLine(0, 10, 128, 10, WHITE);
  
  display.setCursor(0, 15);
  display.printf("NTC:     %.1f C", ntc);
  display.setCursor(0, 30);
  display.printf("DS18B20: %.1f C", ds);
  
  display.display();

Alarma visual:
  if(temp > 30) {
    display.setTextColor(BLACK, WHITE); // Invertir
    display.println(" ALERTA! ");
    display.setTextColor(WHITE); // Restaurar
  }

Uptime en pantalla:
  unsigned long secs = millis() / 1000;
  display.printf("Uptime: %02lu:%02lu", secs/60, secs%60);

--- OPTIMIZACIONES ---

Reducir parpadeo:
- Usar clearDisplay() solo cuando necesario
- Actualizar solo áreas cambiadas
- Mantener timing consistente

Mejorar legibilidad:
- Usar fuentes apropiadas al tamaño
- Alto contraste texto/fondo
- Espaciado consistente
- Alinear valores numéricamente

Eficiencia energética:
- Reducir frecuencia actualización OLED
- Usar fondos oscuros (apaga píxeles)
- Sleep entre lecturas si no crítico

--- NOTAS IMPORTANTES ---

• DS18B20 toma ~750ms para conversión 12-bit
• Actualización OLED ~10-50ms dependiendo contenido
• I2C puede compartir bus con otros dispositivos
• Verificar direcciones I2C únicas por dispositivo
• NTC más rápido pero menos preciso que DS18B20
• Combinar ambos permite validación cruzada

--- TROUBLESHOOTING ---

OLED no muestra nada: Verificar dirección I2C (0x3C/0x3D)
DS18B20 lee -127°C: Sensor desconectado o sin pull-up
NTC valores erráticos: Añadir filtro EMA
Display parpadea: Mejorar fuente alimentación

===============================================================================
*/
