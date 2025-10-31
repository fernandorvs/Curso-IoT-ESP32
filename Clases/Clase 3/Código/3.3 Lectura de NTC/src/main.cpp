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
    Incluye calibración manual del ADC mediante corrección por voltaje
    medido con multímetro para compensar errores de referencia interna.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <math.h>

const int PIN_ADC = 1; 
const int PIN_LED = 2;
const float VREF = 3.3, R_FIXED = 10000, R0 = 10000, CT0 = 298.15, BETA = 3950;

// Factor de calibración (V_real / V_medido)
const float ADC_CORRECTION = 1.73 / 1.91;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);    
    pinMode(PIN_ADC, INPUT);
    gpio_pullup_dis((gpio_num_t)PIN_ADC);
    gpio_pulldown_dis((gpio_num_t)PIN_ADC);
    analogSetAttenuation(ADC_11db);
    analogReadResolution(12);
}

void loop() {
    int raw = analogRead(PIN_ADC);
    float v = raw * (VREF / 4095.0) * ADC_CORRECTION;
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
  R_ntc = R_fija × (V_adc / (VCC - V_adc))

--- PROCESO DE CONVERSIÓN ---

1. Leer ADC → valor raw (0-4095)
2. Convertir a voltaje → V = raw × (3.3/4095) × ADC_CORRECTION
3. Calcular resistencia → R = R_fija × V / (VCC - V)
4. Aplicar Steinhart-Hart → T(K) = 1/(1/T0 + ln(R/R0)/β)
5. Convertir a Celsius → T(°C) = T(K) - 273.15

--- CALIBRACIÓN DEL ADC ---

PROBLEMA: 
El ADC del ESP32 tiene una referencia interna de ~3.3V que puede variar
±10% entre chips. Esto causa errores sistemáticos en la medición.

SOLUCIÓN - CALIBRACIÓN MANUAL:
1. Conectar divisor resistivo al pin ADC
2. Medir voltaje REAL con multímetro en el pin
3. Observar voltaje MEDIDO en el Monitor Serial
4. Calcular factor: ADC_CORRECTION = V_real / V_medido
5. Aplicar corrección en código

EJEMPLO:
- Multímetro muestra: 1.73V
- Serial muestra: 1.91V
- Factor: 1.73 / 1.91 = 0.905
- Resultado: Voltajes corregidos y temperatura precisa

CONFIGURACIÓN ADC:
- analogSetAttenuation(ADC_11db): Rango 0-3.3V completo
- analogReadResolution(12): 12 bits (0-4095)
- gpio_pullup_dis/gpio_pulldown_dis: Alta impedancia real
  (elimina resistencias parásitas que afectan el divisor)

--- VALORES TÍPICOS ---

Temperaturas y resistencias:
  15°C → ~15kΩ
  25°C → 10kΩ (R0)
  35°C → ~6.5kΩ
  45°C → ~4.3kΩ

--- SELECCIÓN DE COMPONENTES ---

R_FIXED:
- Idealmente = R0 del NTC (10kΩ)
- Maximiza sensibilidad en rango de interés
- Usar resistencia ±1% para mejor precisión

Precisión del sistema:
- NTC: ±5% tolerancia típica
- R_fixed: ±1% recomendado
- ADC sin calibrar: ±10% error
- ADC calibrado: ±2-3% error
- Precisión final con calibración: ±2°C

--- MEJORAS OPCIONALES ---

Filtro EMA para suavizar lecturas:
  static float ema = 0;
  ema = 0.1 * Tc + 0.9 * ema;

Ajuste fino de BETA:
  1. Medir con termómetro de referencia
  2. Si error >2°C, ajustar BETA
  3. Rango típico: 3900-4000K

--- TROUBLESHOOTING ---

Error sistemático (temperatura muy alta/baja):
  → Verificar calibración ADC_CORRECTION
  → Revisar conexiones del divisor de tensión
  → Medir R_fija real con multímetro

Lecturas inestables:
  → Aplicar filtro EMA
  → Proteger sensor de corrientes de aire
  → Verificar conexiones (cables cortos)

Temperatura no responde a cambios:
  → Verificar que el NTC esté bien conectado
  → Probar con otro NTC (puede estar dañado)
  → Verificar valor de BETA del fabricante

===============================================================================
*/