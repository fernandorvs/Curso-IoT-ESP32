/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 2 - Entradas y Salidas Digitales
    PROYECTO:    Pulsador No Bloqueante con Antirrebote
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Lectura de pulsador con debouncing no bloqueante usando millis().
    No utiliza delay() ni while(), permitiendo ejecutar otras tareas
    simultáneamente mientras monitorea el estado del botón.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#define BUTTON_PIN 0
#define PIN_LED 2

// Tiempo en milisegundos del último cambio válido
uint32_t lastDebounce = 0;

// Estado estable actual del pulsador (HIGH = no presionado)
uint8_t stable = HIGH;

void setup() {
	Serial.begin(115200); 
	pinMode(BUTTON_PIN, INPUT_PULLUP); 
}

void loop() {
	// Lee el estado actual del pulsador
	int r = digitalRead(BUTTON_PIN);

	// Verifica si hay un cambio de estado Y si han pasado al menos 50ms desde el último cambio
	if (r != stable && millis() - lastDebounce > 50) {
		lastDebounce = millis(); // Actualiza el tiempo del último cambio válido
		stable = r; // Actualiza el estado estable con el nuevo valor
		// Imprime el estado del pulsador en el monitor serie
		Serial.println(
			stable == LOW ? // Si stable es LOW (pulsador presionado)
			"PRESIONADO" : // Imprime "PRESIONADO"
			"SUELTO" // Si no, imprime "SUELTO"
		);
	}
}

/*
===============================================================================
              DEBOUNCING NO BLOQUEANTE - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

DEBOUNCING NO BLOQUEANTE:
Combina técnica antirrebote con timing basado en millis().
No usa delay() ni while(), permitiendo código 100% responsive.

DETECCIÓN DE FLANCOS:
En lugar de esperar con delay(), detectamos cambios de estado
y validamos que persistan más de 50ms antes de aceptarlos.

VENTAJAS vs VERSIÓN BLOQUEANTE:
- No detiene otras tareas
- Permite múltiples botones simultáneos
- Respuesta inmediata a otros eventos
- Escalable a sistemas complejos

--- LÓGICA DEL ALGORITMO ---

1. Leer estado actual del botón (r)
2. Comparar con estado estable (stable)
3. Si son diferentes Y han pasado >50ms:
   - Actualizar timestamp
   - Aceptar nuevo estado como estable
   - Ejecutar acción correspondiente

--- EJEMPLO PRÁCTICO ---

Patrón usado:
  uint32_t lastDebounce = 0;
  uint8_t stable = HIGH;
  
  int reading = digitalRead(BUTTON);
  if (reading != stable && millis() - lastDebounce > 50) {
    lastDebounce = millis();
    stable = reading;
    if (stable == LOW) {
      // Acción al presionar
    }
  }

Detección de flancos específicos:
  // Solo flanco de bajada (presionar)
  if (reading == LOW && stable == HIGH && ...) { }
  
  // Solo flanco de subida (soltar)
  if (reading == HIGH && stable == LOW && ...) { }

Múltiples botones:
  // Cada botón tiene su propio lastDebounce y stable
  uint32_t debounce1 = 0, debounce2 = 0;
  uint8_t stable1 = HIGH, stable2 = HIGH;

--- NOTAS IMPORTANTES ---

• No bloquea el programa en ningún momento
• Detecta tanto presionar como soltar
• Tiempo de debounce: 50ms típico
• Compatible con otras tareas no bloqueantes
• Ideal para sistemas con múltiples entradas

--- TROUBLESHOOTING ---

Dobles detecciones: Aumentar tiempo de debounce (>50ms)
No detecta pulsación: Verificar INPUT_PULLUP y conexión a GND
Estado inicial incorrecto: Inicializar stable = HIGH

===============================================================================
*/
