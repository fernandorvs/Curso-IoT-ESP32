/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 2 - Entradas y Salidas Digitales
    PROYECTO:    Pulsador con Antirrebote (Debouncing)
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Lectura de pulsador con técnica de debouncing por software.
    Elimina rebotes mecánicos del pulsador para detectar pulsaciones
    limpias y confiables. Cada pulsación invierte el estado del LED.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#define BUTTON_PIN 0
#define PIN_LED 2

void setup() {
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(PIN_LED, OUTPUT);
}

void loop() {
	if (digitalRead(BUTTON_PIN) == LOW) {
		delay(50);
		// espera establecimiento
		if (digitalRead(BUTTON_PIN) == LOW) {
			// Invierte el estado del LED con cada pulsación
			digitalWrite(PIN_LED, !digitalRead(PIN_LED));
			// espera soltar
			while (digitalRead(BUTTON_PIN) == LOW);
		}
	}
}

/*
===============================================================================
                    DEBOUNCING - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

REBOTE MECÁNICO (BOUNCING):
Los pulsadores mecánicos no cierran/abren limpiamente. Al presionar,
los contactos rebotan múltiples veces en ~5-50ms, generando falsos
cambios de estado.

DEBOUNCING:
Técnica para filtrar estos rebotes y obtener una lectura limpia.
Métodos: por software (delays, timers) o hardware (capacitores).

INPUT_PULLUP:
Resistencia interna que mantiene el pin en HIGH cuando el botón
está abierto. Al presionar, el pin va a LOW (tierra).

--- FUNCIONES PRINCIPALES ---

digitalRead(pin)
  Lee estado digital de un pin
  Retorna: HIGH (1) o LOW (0)
  
pinMode(pin, INPUT_PULLUP)
  Configura pin como entrada con resistencia pull-up interna
  Botón no presionado = HIGH
  Botón presionado = LOW (conectado a GND)

digitalWrite(pin, !digitalRead(pin))
  Lee estado actual y escribe el opuesto
  Útil para toggle (invertir estado)

--- EJEMPLO PRÁCTICO ---

Técnica usada en este código:
1. Detectar flanco (botón LOW)
2. Esperar 50ms (delay de estabilización)
3. Verificar que sigue LOW
4. Ejecutar acción
5. Esperar a que se suelte (while)

Alternativa no bloqueante (ver ejemplo 2.4):
  uint32_t lastDebounce = 0;
  if (reading != lastState && millis() - lastDebounce > 50) {
    lastDebounce = millis();
    // Acción
  }

--- NOTAS IMPORTANTES ---

• Rebotes típicos: 5-50ms (usamos 50ms para seguridad)
• INPUT_PULLUP invierte lógica: presionado = LOW
• while() espera bloquea programa hasta soltar botón
• Para múltiples botones usar técnica no bloqueante
• Conexión: Un terminal a GPIO, otro a GND (sin resistencia externa)

--- TROUBLESHOOTING ---

Múltiples toggles con una pulsación: Aumentar delay de debounce
LED no cambia: Verificar conexión a GND y INPUT_PULLUP
Respuesta lenta: Delay de 50ms es necesario, optimizar con millis()

===============================================================================
*/
