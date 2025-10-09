/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 2 - Entradas y Salidas Digitales
    PROYECTO:    FSM - Máquina de Estados Finitos
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Implementación de Máquina de Estados Finitos (FSM) para controlar
    LED con tres estados: OFF, ON, BLINK. Cada pulsación cambia al
    siguiente estado de forma cíclica. Patrón fundamental para sistemas
    complejos y control de flujo estructurado.
    
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

// Flag de botón pulsado
uint8_t buttonPressed = 0;

// Variable de estado
#define STATE_OFF 0
#define STATE_ON 1
#define STATE_BLINK 2
uint8_t state = STATE_OFF;

void setup() {
	Serial.begin(115200);
	pinMode(PIN_LED, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
	// Pulsador no bloqueante con antirrebote
	int r = digitalRead(BUTTON_PIN);
	if (r != stable && millis() - lastDebounce > 50) {
		lastDebounce = millis();
		stable = r;
		if (stable == LOW) {
			buttonPressed = 1; // Set flag if button is pressed
			Serial.println("PULSADO");
		}
	}

	// FSM
	switch (state) {

		case STATE_OFF:
			digitalWrite(PIN_LED, LOW);
			if (buttonPressed) {
				buttonPressed = 0;
				state = STATE_ON;
			}
			break;

		case STATE_ON:
			digitalWrite(PIN_LED, HIGH);
			if (buttonPressed) {
				buttonPressed = 0;
				state = STATE_BLINK;
			}
			break;

		case STATE_BLINK:
			{
				uint32_t ahora = millis();
				if (ahora - lastBlink >= BLINK_MS) {
					lastBlink = ahora;
					ledState = !ledState;
					digitalWrite(PIN_LED, ledState);
				}
				if (buttonPressed) {
					buttonPressed = 0;
					state = STATE_OFF;
				}
			}
			break;

		default:
			break;
	}
}

/*
===============================================================================
              FSM (FINITE STATE MACHINE) - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

MÁQUINA DE ESTADOS FINITOS:
Modelo computacional con número finito de estados. En cada momento
está en exactamente un estado. Las transiciones entre estados son
disparadas por eventos (como pulsar botón).

COMPONENTES DE UNA FSM:
1. Estados: Condiciones distintas del sistema (OFF, ON, BLINK)
2. Transiciones: Cambios entre estados
3. Eventos: Disparadores de transiciones (buttonPressed)
4. Acciones: Qué hacer en cada estado

VENTAJAS:
- Código estructurado y predecible
- Fácil agregar/modificar comportamientos
- Ideal para protocolos, interfaces, control de dispositivos
- Debugging simplificado

--- IMPLEMENTACIÓN CON SWITCH ---

Patrón usado:
  switch (estado_actual) {
    case ESTADO_1:
      // Acciones del estado 1
      if (evento) {
        estado_actual = ESTADO_2;
      }
      break;
    case ESTADO_2:
      // Acciones del estado 2
      break;
  }

--- EJEMPLO PRÁCTICO ---

Estados de este proyecto:
  OFF → ON → BLINK → OFF (cíclico)

Diagrama de transiciones:
        [botón]         [botón]         [botón]
  OFF ────────→ ON ────────→ BLINK ────────→ OFF

Agregar estado (ejemplo FADE):
  #define STATE_FADE 3
  
  case STATE_BLINK:
    // ... código actual ...
    if (buttonPressed) {
      buttonPressed = 0;
      state = STATE_FADE;  // En vez de STATE_OFF
    }
    break;
  
  case STATE_FADE:
    // Código para fade
    if (buttonPressed) {
      buttonPressed = 0;
      state = STATE_OFF;
    }
    break;

--- USO DE FLAGS ---

buttonPressed como flag:
- Se setea en 1 cuando detecta pulsación
- Cada estado lo lee y resetea a 0
- Evita procesar misma pulsación múltiples veces
- Desacopla detección de acción

--- NOTAS IMPORTANTES ---

• Un solo estado activo a la vez
• Transiciones explícitas y controladas
• Estados deben ser mutuamente excluyentes
• Usar enum o #define para estados legibles
• Flag buttonPressed evita rebotes lógicos entre estados

--- APLICACIONES COMUNES ---

• Menús de navegación
• Protocolos de comunicación
• Control de motores paso a paso
• Gestión de conexiones WiFi/BT
• Interfaces de usuario
• Sistemas de alarma

--- TROUBLESHOOTING ---

Estado no cambia: Verificar que flag se resetea correctamente
Saltos de estado: Asegurar break en cada case
Comportamiento errático: Revisar condiciones de transición

===============================================================================
*/
