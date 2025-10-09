/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 3 - Sensores y Periféricos
    PROYECTO:    Filtros Digitales - Suavizado de Señales
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Implementación de filtro EMA (Exponential Moving Average) para
    suavizar lecturas ruidosas del ADC. Compara señal cruda vs filtrada
    en Serial Plotter para visualizar mejora en estabilidad.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

const int PIN_ADC = 34;
float ema = 0;

void setup() {
    Serial.begin(115200);
}

void loop() {
    int x = analogRead(PIN_ADC);
    ema = 0.2 * x + 0.8 * ema;
    Serial.printf("%d,%.1f", x, ema);
    Serial.println();
    delay(100);
}

/*
===============================================================================
                    FILTROS DIGITALES - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

FILTRO DIGITAL:
Algoritmo que procesa señal para reducir ruido, suavizar variaciones
o extraer información relevante. Alternativa por software a filtros
hardware (resistores, capacitores).

EMA (Exponential Moving Average):
Filtro IIR (Infinite Impulse Response) que da más peso a valores
recientes. Fórmula: y[n] = α·x[n] + (1-α)·y[n-1]

PARÁMETRO α (ALPHA):
- α = 0.1: Muy suave, lento (respuesta en ~10 muestras)
- α = 0.2: Balance recomendado (respuesta en ~5 muestras)
- α = 0.5: Rápido, menos filtrado (respuesta en ~2 muestras)
- α = 1.0: Sin filtro (señal cruda)

--- TIPOS DE FILTROS ---

1. EMA (usado aquí):
   Ventajas: Simple, bajo uso de memoria (1 variable)
   Desventajas: Respuesta exponencial, lag permanente

2. PROMEDIO MÓVIL (Moving Average):
   Ventajas: Suavizado uniforme, fácil entender
   Desventajas: Requiere array, más memoria

3. MEDIANA:
   Ventajas: Excelente contra picos/spikes
   Desventajas: Costoso computacionalmente

--- IMPLEMENTACIÓN EMA ---

Paso a paso:
  float ema = 0;              // Inicializar
  int raw = analogRead(pin);  // Leer sensor
  ema = α*raw + (1-α)*ema;    // Aplicar filtro
  
Ajustar α según necesidad:
  #define ALPHA 0.2
  ema = ALPHA*raw + (1-ALPHA)*ema;

--- EJEMPLO PRÁCTICO ---

EMA con diferentes α:
  // Suave (lento):
  ema = 0.1 * raw + 0.9 * ema;
  
  // Balance:
  ema = 0.2 * raw + 0.8 * ema;
  
  // Rápido:
  ema = 0.5 * raw + 0.5 * ema;

Promedio móvil alternativo:
  #define WINDOW 10
  int buffer[WINDOW];
  int index = 0;
  
  buffer[index] = analogRead(pin);
  index = (index + 1) % WINDOW;
  float avg = sum(buffer) / WINDOW;

--- VISUALIZACIÓN SERIAL PLOTTER ---

Formato correcto (separado por comas):
  Serial.printf("%d,%.1f\n", crudo, filtrado);

Esto crea dos líneas en el plotter:
  - Azul: señal cruda (ruidosa)
  - Roja: señal filtrada (suave)

--- NOTAS IMPORTANTES ---

• Menor α = más suavizado pero mayor retardo (lag)
• Mayor α = respuesta rápida pero menos filtrado
• EMA solo usa 1 variable flotante (eficiente)
• Promedio móvil requiere array (más memoria)
• Inicializar ema en 0 o con primera lectura
• Para temperaturas usar α pequeño (0.1-0.2)

--- TROUBLESHOOTING ---

Filtro muy lento: Aumentar α (probar 0.3-0.5)
Aún muy ruidoso: Reducir α o usar promedio móvil
Offset inicial: Inicializar ema = analogRead(pin)

===============================================================================
*/