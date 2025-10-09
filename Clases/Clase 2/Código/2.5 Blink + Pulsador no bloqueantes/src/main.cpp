/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 2 - Entradas y Salidas Digitales
    PROYECTO:    Blink + Pulsador No Bloqueantes
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Integración de dos tareas independientes ejecutándose simultáneamente:
    parpadeo de LED y lectura de pulsador, ambos con timing no bloqueante.
    Demuestra cómo múltiples procesos pueden coexistir sin interferirse.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

// Blink
#define PIN_LED 2
#define BLINK_MS 500
unsigned long lastBlink = 0; 
bool ledState = false; 

// Pulsador
#define BUTTON_PIN 0
uint32_t lastDebounce = 0;
uint8_t stable = HIGH;

void setup() {
	Serial.begin(115200); 
	pinMode(PIN_LED, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP); 
}

void loop() {

	// Blink no bloqueante
	unsigned long ahora = millis(); 
	if (ahora - lastBlink >= BLINK_MS) {
		lastBlink = ahora; 
		ledState = !ledState; 
		digitalWrite(PIN_LED, ledState);
	}

	// Pulsador no bloqueante con antirrebote
	int r = digitalRead(BUTTON_PIN);
	if (r != stable && millis() - lastDebounce > 50) {
		lastDebounce = millis();
		stable = r;
		Serial.println(
			stable == LOW ?
			"PRESIONADO" : 
			"SUELTO"
		);
	}
	
}

/*
===============================================================================
              MULTITAREA NO BLOQUEANTE - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

MULTITAREA COOPERATIVA:
Múltiples tareas independientes comparten el mismo loop() sin
bloquearse mutuamente. Cada tarea verifica su propio timing.

INDEPENDENCIA DE TAREAS:
Cada tarea mantiene:
- Su propio timestamp (lastBlink, lastDebounce)
- Sus propias variables de estado
- Su propio intervalo de actualización

ESCALABILIDAD:
Este patrón permite agregar tantas tareas como necesites:
LEDs, botones, sensores, comunicaciones, etc.

--- PATRÓN DE IMPLEMENTACIÓN ---

Estructura básica por tarea:
  unsigned long lastTarea = 0;
  const unsigned long intervaloTarea = 1000;
  
  if (millis() - lastTarea >= intervaloTarea) {
    lastTarea = millis();
    // Ejecutar tarea
  }

--- EJEMPLO PRÁCTICO ---

Agregar tercera tarea (sensor cada 100ms):
  unsigned long lastSensor = 0;
  
  if (millis() - lastSensor >= 100) {
    lastSensor = millis();
    int valor = analogRead(SENSOR_PIN);
    // Procesar valor
  }

Diferentes frecuencias:
  Blink: 500ms (2 Hz)
  Botón: Tiempo real (sin intervalo fijo)
  Sensor: 100ms (10 Hz)
  Serial: 1000ms (1 Hz)

--- VENTAJAS DEL ENFOQUE ---

✓ Responsive: Responde inmediatamente a eventos
✓ Eficiente: No desperdicia tiempo en delays
✓ Modular: Fácil agregar/quitar tareas
✓ Escalable: Soporta decenas de tareas simultáneas
✓ Mantenible: Cada tarea es independiente

--- NOTAS IMPORTANTES ---

• Ninguna tarea debe bloquear (no delay, no while infinito)
• Cada tarea verifica su timing en cada iteración del loop
• El loop() debe ejecutarse lo más rápido posible
• Evitar operaciones pesadas que demoren el loop
• Variables de timing deben ser unsigned long

--- TROUBLESHOOTING ---

Timing impreciso: Normal, no es RTOS. Usar FreeRTOS para timing crítico
Una tarea "salta": Verificar que variables sean unsigned long
Loop lento: Eliminar Serial.print excesivos o delays ocultos

===============================================================================
*/
