/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 3 - Sensores y Periféricos
    PROYECTO:    Lectura de ADC - Conversión Analógica Digital
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Lectura básica del ADC (Conversor Analógico-Digital) del ESP32
    con conversión a voltaje. Usa potenciómetro para generar señal
    analógica variable entre 0-3.3V.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

const int PIN_ADC = 1;

void setup() {
    Serial.begin(115200);
}

void loop() {
    int raw = analogRead(PIN_ADC);
    float v = raw * (3.3f / 4095.0f);
    Serial.printf("RAW=%04d  V=%.3f", raw, v);
    Serial.println();
    delay(200);
}

/*
===============================================================================
                    ADC ESP32 - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

ADC (Analog-to-Digital Converter):
Convierte señales analógicas continuas (voltaje) en valores digitales
discretos que el microcontrolador puede procesar.

ESPECIFICACIONES ESP32:
- Resolución: 12 bits (0-4095)
- Rango de voltaje: 0V a 3.3V (NO tolerante a 5V)
- ADCs disponibles: ADC1 (8 canales) y ADC2 (10 canales)
- ADC2 no disponible cuando WiFi está activo

ATENUACIÓN (por defecto 11dB):
- 0dB: 0-1.1V (mayor precisión, rango corto)
- 2.5dB: 0-1.5V
- 6dB: 0-2.2V
- 11dB: 0-3.3V (rango completo, predeterminado)

--- FUNCIONES PRINCIPALES ---

analogRead(pin)
  Lee valor ADC del pin especificado
  Retorna: int (0-4095)
  
analogReadResolution(bits)
  Cambia resolución del ADC
  ESP32: 9-12 bits (default 12)
  
analogSetAttenuation(atten)
  Configura atenuación global
  Valores: ADC_0db, ADC_2_5db, ADC_6db, ADC_11db
  
analogSetPinAttenuation(pin, atten)
  Configura atenuación por pin específico

--- CONVERSIÓN A VOLTAJE ---

Fórmula básica:
  voltaje = raw * (3.3 / 4095.0)

Con atenuación específica:
  ADC_0db:   voltaje = raw * (1.1 / 4095.0)
  ADC_2_5db: voltaje = raw * (1.5 / 4095.0)
  ADC_6db:   voltaje = raw * (2.2 / 4095.0)
  ADC_11db:  voltaje = raw * (3.3 / 4095.0)

--- PINES ADC DISPONIBLES ---

ADC1 (recomendado):
  GPIO32, GPIO33, GPIO34, GPIO35, GPIO36, GPIO37, GPIO38, GPIO39

ADC2 (evitar con WiFi):
  GPIO0, GPIO2, GPIO4, GPIO12, GPIO13, GPIO14, GPIO15, GPIO25, GPIO26, GPIO27

--- NOTAS IMPORTANTES ---

• Lecturas fluctúan naturalmente debido al ruido
• ADC ESP32 no es muy preciso (±5% error)
• GPIO34-39 solo entrada (sin pull-up/pull-down)
• Usar filtros digitales para suavizar lecturas
• WiFi interfiere con ADC2, usar siempre ADC1 en proyectos IoT
• Nunca aplicar más de 3.3V al pin ADC

--- TROUBLESHOOTING ---

Valores erráticos: Normal, usar filtros (ver ejemplo 3.2)
Siempre lee 4095: Verificar conexión, puede estar flotante
Siempre lee 0: Revisar conexión a GND del potenciómetro

===============================================================================
*/