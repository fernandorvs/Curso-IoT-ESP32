/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 2 - Entradas y Salidas Digitales
    PROYECTO:    Blink No Bloqueante
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Implementación de parpadeo de LED sin usar delay(), permitiendo
    ejecutar otras tareas en paralelo. Usa millis() para timing no
    bloqueante, fundamental para sistemas multitarea.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#define PIN_LED 2
#define BLINK_MS 500

unsigned long lastBlink = 0; 
bool ledState = false; 

void setup() {
	pinMode(PIN_LED, OUTPUT);
}

void loop() {
	unsigned long ahora = millis(); 
	if (ahora - lastBlink >= BLINK_MS) {
		lastBlink = ahora; 
		ledState = !ledState; 
		digitalWrite(PIN_LED, ledState);
	}
	// aquí se pueden leer botones, 
	// UART, sensores, etc.
}

/*
===============================================================================
                    TIMING NO BLOQUEANTE - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

CÓDIGO BLOQUEANTE vs NO BLOQUEANTE:
- Bloqueante: delay() detiene todo el programa
- No bloqueante: el programa sigue ejecutándose mientras espera

millis():
Retorna milisegundos desde que arrancó el ESP32 (uint32_t).
Se desborda cada ~49 días, pero la resta funciona correctamente
por aritmética módulo 2^32.

PATRÓN NO BLOQUEANTE:
1. Guardar timestamp de última acción
2. En cada loop comparar tiempo transcurrido
3. Si cumple intervalo, ejecutar acción y actualizar timestamp

--- FUNCIONES PRINCIPALES ---

millis()
  Retorna milisegundos desde inicio
  Tipo: unsigned long (uint32_t)
  No se detiene, sigue contando siempre
  
micros()
  Similar a millis() pero en microsegundos
  Mayor resolución, desborda más rápido (~70 minutos)

--- EJEMPLO PRÁCTICO ---

Patrón básico usado:
  unsigned long ultimo = 0;
  unsigned long intervalo = 1000;
  
  if (millis() - ultimo >= intervalo) {
    ultimo = millis();
    // Acción cada 1 segundo
  }

Múltiples tareas independientes:
  unsigned long lastBlink = 0;
  unsigned long lastRead = 0;
  
  if (millis() - lastBlink >= 500) {
    lastBlink = millis();
    toggleLED();
  }
  
  if (millis() - lastRead >= 100) {
    lastRead = millis();
    leerSensor();
  }

--- NOTAS IMPORTANTES ---

• millis() nunca usar con delay(), son filosofías opuestas
• La resta (millis() - ultimo) maneja overflow automáticamente
• Cada tarea tiene su propio timestamp independiente
• Ideal para: múltiples LEDs, lecturas periódicas, timeouts
• Crucial para sistemas IoT que deben responder a eventos

--- TROUBLESHOOTING ---

Timing impreciso: Normal, no es tiempo real estricto
Variables no unsigned long: Usar uint32_t o unsigned long
Overflow: No requiere manejo especial, la resta funciona

===============================================================================
*/
