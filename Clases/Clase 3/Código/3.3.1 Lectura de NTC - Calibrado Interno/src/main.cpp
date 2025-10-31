/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 3 - Sensores y Periféricos
    PROYECTO:    Lectura de NTC - Sensor de Temperatura - Calibrado Interno
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Medición de temperatura usando termistor NTC 10kΩ en configuración
    de divisor de tensión. Convierte resistencia a temperatura usando
    ecuación Steinhart-Hart simplificada (parámetro Beta).
    Utiliza calibración automática del ADC mediante valores eFuse 
    grabados de fábrica en cada chip ESP32 para mayor precisión sin
    necesidad de calibración manual con multímetro.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <math.h>
#include <esp_adc_cal.h>

const int PIN_ADC = 1; 
const int PIN_LED = 2;
const float VREF = 3.3, R_FIXED = 10000, R0 = 10000, CT0 = 298.15, BETA = 3950;

// Para calibración eFuse
esp_adc_cal_characteristics_t adc_chars;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);
       
    // Calibrar ADC con valores eFuse de fábrica (compensa variaciones individuales)
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, &adc_chars);
}

void loop() {

    int raw = analogRead(PIN_ADC);
    
    uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
    float v = voltage_mv / 1000.0;
    
    float R = R_FIXED * v / (VREF - v);
    float Tc = (1 / (1 / CT0 + log(R / R0) / BETA)) - 273.15;
    
    Serial.printf("V=%.3fV | R=%.0fΩ | T=%.2f°C", v, R, Tc);
    Serial.println();    
    digitalWrite(PIN_LED, Tc > 30 ? HIGH : LOW);
    delay(500);
}

/*
===============================================================================
                    SENSOR NTC - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

NTC (Negative Temperature Coefficient):
Termistor semiconductor cuya resistencia disminuye exponencialmente
al aumentar temperatura. Hecho de óxidos metálicos sinterizados.

CARACTERÍSTICAS:
- A 25°C: Resistencia nominal (R0 = 10kΩ típico)
- Alta sensibilidad: ~4-5% cambio por °C
- No lineal: Requiere ecuación matemática para conversión
- Rápido: Tiempo de respuesta térmico <10 segundos

ECUACIÓN STEINHART-HART (SIMPLIFICADA):
  1/T = 1/T0 + (1/β)·ln(R/R0)
  
  T: Temperatura absoluta (Kelvin)
  T0: 298.15K (25°C referencia)
  R: Resistencia actual del NTC
  R0: Resistencia nominal (10kΩ)
  β: Constante del material (3950K típico)

--- CIRCUITO DIVISOR DE TENSIÓN ---

Configuración:
  VCC(3.3V) ─┬─ R_fija(10kΩ) ─┬─ NTC(10kΩ) ─┬─ GND
                               │
                            ADC_Pin

Cálculo de resistencia NTC:
  V_adc = VCC × (R_ntc / (R_fija + R_ntc))
  R_ntc = R_fija × V_adc / (VCC - V_adc)

--- CALIBRACIÓN AUTOMÁTICA ADC (eFuse) ---

PROBLEMA:
El ADC del ESP32 tiene una referencia interna de ~3.3V que varía
entre chips (±10% típico). Cada ESP32 tiene características únicas.

SOLUCIÓN eFuse:
Durante la fabricación, Espressif calibra cada chip y graba valores
de corrección en memoria eFuse (One-Time Programmable).

VENTAJAS:
- Automática: No requiere multímetro ni mediciones manuales
- Portátil: Funciona en cualquier ESP32 sin reconfigurar
- Precisa: Compensa variaciones individuales de cada chip
- Profesional: Mismo método usado en productos comerciales

IMPLEMENTACIÓN:
  #include <esp_adc_cal.h>
  esp_adc_cal_characteristics_t adc_chars;
  
  // En setup():
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, 
                           ADC_WIDTH_BIT_12, 1100, &adc_chars);
  
  // En loop():
  uint32_t voltage_mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
  float v = voltage_mv / 1000.0;

PRECISIÓN OBTENIDA:
- Sin calibración: ±10% error típico
- Con eFuse: ±2-3% error típico
- Mejora: 3-5x más preciso

COMPATIBILIDAD:
✓ ESP32 (original)
✓ ESP32-S2
✓ ESP32-S3  
✓ ESP32-C3
✓ ESP32-C6

--- PROCESO DE CONVERSIÓN ---

1. Leer ADC → valor raw (0-4095)
2. Aplicar calibración eFuse → voltage_mv
3. Convertir a voltios → V = voltage_mv / 1000.0
4. Calcular resistencia → R = R_fija × V / (VCC - V)
5. Aplicar Steinhart-Hart → T(K) = 1/(1/T0 + ln(R/R0)/β)
6. Convertir a Celsius → T(°C) = T(K) - 273.15

--- VALORES TÍPICOS ---

Temperaturas y resistencias:
  15°C → ~15kΩ
  20°C → ~12kΩ
  25°C → 10kΩ (R0)
  30°C → ~8kΩ
  35°C → ~6.5kΩ

--- SELECCIÓN DE COMPONENTES ---

R_FIXED:
- Idealmente = R0 del NTC (10kΩ)
- Maximiza sensibilidad en el rango de interés
- Usar resistencia ±1% para mejor precisión

Precisión del sistema:
- NTC: ±5% tolerancia típica
- R_fixed: ±1% recomendado
- ADC con eFuse: ±2-3% error
- Precisión final: ±2°C típico

--- MEJORAS OPCIONALES ---

Filtro EMA para lecturas más estables:
  static float ema = 0;
  if (ema == 0) ema = Tc;  // Inicializar
  ema = 0.1 * Tc + 0.9 * ema;  // Suavizar

Ajuste fino de BETA:
- Si error >2°C con termómetro de referencia
- Probar valores entre 3900-4000K
- Consultar datasheet del fabricante del NTC

--- COMPARACIÓN DE MÉTODOS ---

MÉTODO 1: Sin calibración
  Ventaja: Simple, no requiere nada adicional
  Desventaja: Error ±10%, varía entre chips
  Uso: Prototipos rápidos, precisión no crítica

MÉTODO 2: Calibración manual (ver ejemplo 3.3)
  Ventaja: Máxima precisión en una placa específica
  Desventaja: Requiere multímetro, recalibrar cada chip
  Uso: Producción unitaria, aplicaciones críticas

MÉTODO 3: Calibración eFuse (ESTE EJEMPLO)
  Ventaja: Automática, funciona en todas las placas
  Desventaja: Requiere librería esp_adc_cal.h
  Uso: RECOMENDADO para proyectos escalables

--- TROUBLESHOOTING ---

Error sistemático en todas las lecturas:
  → Verificar R_fija real con multímetro
  → Revisar conexiones del divisor de tensión
  → Ajustar valor de BETA según datasheet

Lecturas inestables/con ruido:
  → Aplicar filtro EMA (código arriba)
  → Usar cables cortos entre NTC y ESP32
  → Proteger sensor de corrientes de aire

Temperatura no responde:
  → Verificar NTC conectado correctamente
  → Probar con otro NTC (puede estar dañado)
  → Verificar que GPIO 1 sea pin ADC válido

Diferencia con otro sensor:
  → Normal: NTCs tienen ±5% tolerancia
  → Verificar BETA de ambos sensores
  → Inercia térmica: NTC tarda ~10s en estabilizar

===============================================================================
*/