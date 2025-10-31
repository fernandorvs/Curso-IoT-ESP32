/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 3 - Sensores y Periféricos
    PROYECTO:    Lectura de NTC - Sensor de Temperatura
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Medición de temperatura usando termistor NTC 10kΩ en configuración
    de divisor de tensión. Convierte resistencia a temperatura usando
    ecuación Steinhart-Hart simplificada (parámetro Beta).
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <math.h>

const int PIN_ADC = 1; const int PIN_LED = 2;
const float VREF = 3.3, R_FIXED = 10000, R0 = 10000, CT0 = 298.15, BETA = 3950;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);
}

void loop() {
    int raw = analogRead(PIN_ADC);
    float v = raw * (VREF / 4095.0);
    float R = R_FIXED * v / (VREF - v);
    float Tc = (1 / (1 / CT0 + log(R / R0) / BETA)) - 273.15;
    Serial.printf("T=%.2f °C", Tc);
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
  R_ntc = R_fija × (VCC / V_adc - 1)

--- PROCESO DE CONVERSIÓN ---

1. Leer ADC → valor raw (0-4095)
2. Convertir a voltaje → V = raw × (3.3/4095)
3. Calcular resistencia → R = R_fija × (VCC/V - 1)
4. Aplicar Steinhart-Hart → T(K) = 1/(1/T0 + ln(R/R0)/β)
5. Convertir a Celsius → T(°C) = T(K) - 273.15

--- EJEMPLO PRÁCTICO ---

Valores típicos a diferentes temperaturas:
  15°C → ~15kΩ
  25°C → 10kΩ (R0)
  35°C → ~6.5kΩ
  45°C → ~4.3kΩ

Calibración básica:
  1. Medir con termómetro referencia
  2. Ajustar BETA si difiere >2°C
  3. Rango típico BETA: 3900-4000K

Mejora con filtro:
  float ema = 0;
  ema = 0.1 * Tc + 0.9 * ema;  // Suaviza lecturas

--- SELECCIÓN DE COMPONENTES ---

R_FIXED:
- Idealmente = R0 del NTC
- Maximiza sensibilidad
- 10kΩ común para NTC 10kΩ

Precisión:
- NTC ±5% tolerancia
- R_fixed ±1% recomendado
- ADC ESP32 ±2% error
- Precisión final: ±2-3°C

--- NOTAS IMPORTANTES ---

• Autocalentamiento: Corriente genera calor, alterar medición
• Inercia térmica: Demora en responder a cambios rápidos
• Rango útil: -20°C a +100°C típico
• Derivación temporal en condiciones extremas
• Proteger de corrientes aire para medición estable
• Verificar R_fija con multímetro para mejor precisión

--- TROUBLESHOOTING ---

Temperatura muy alta/baja: Revisar conexiones divisor de tensión
Lecturas inestables: Aplicar filtro EMA (α=0.1)
Error sistemático: Calibrar BETA con temperatura conocida

===============================================================================
*/