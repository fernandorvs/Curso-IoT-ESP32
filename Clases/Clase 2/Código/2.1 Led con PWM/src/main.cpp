/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 2 - Entradas y Salidas Digitales
    PROYECTO:    LED con PWM - Control de Brillo
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Control de brillo de LED mediante PWM (Modulación por Ancho de Pulso).
    Implementa efecto fade in/fade out con corrección gamma para transición
    visual suave y lineal.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#define PIN_LED 2

void setup() {

	// Configura el canal 0 en 5 kHz
	// con 8 bits de resolución (0–255).
	ledcSetup(0, 5000, 8);

	// Asocia el pin LED al canal 0 de PWM.
	ledcAttachPin(PIN_LED, 0);

	// Envía un duty cycle del 50% (128/255)
	// → LED a mitad de brillo.
	ledcWrite(0, 128);

}

void loop() {

	// Flashea el LED con un fade in / fade out con corrección de Gamma
	static uint8_t up = true;
	static uint8_t dutyCycle = 0;
	if (up) {
		if (dutyCycle == 255) up = false; else dutyCycle++;
	} else {
		if (dutyCycle == 0) up = true; else dutyCycle--;
	}
	uint8_t dutyGamma = pow(dutyCycle / 255.0, 2.2) * 255.0;
	ledcWrite(0, dutyGamma);
	delay(5);

}

/*
===============================================================================
                    PWM - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

PWM (Pulse Width Modulation):
Técnica para simular señales analógicas mediante pulsos digitales.
Varía el "duty cycle" (tiempo en HIGH vs LOW) para controlar potencia.

DUTY CYCLE:
- 0%: Siempre LOW (LED apagado)
- 50%: Mitad del tiempo HIGH (LED a medio brillo)
- 100%: Siempre HIGH (LED a máximo brillo)

CORRECCIÓN GAMMA:
Los LEDs no son lineales. Una curva gamma (2.2) hace que el fade
se perciba uniforme al ojo humano.

LEDC (LED Control):
Controlador PWM del ESP32 con 16 canales independientes.
Permite control preciso de LEDs, motores, servos, etc.

--- FUNCIONES PRINCIPALES ---

ledcSetup(canal, frecuencia, resolución)
  Configura un canal PWM
  canal: 0-15 (ESP32 tiene 16 canales)
  frecuencia: en Hz (típico 5000 Hz)
  resolución: bits (8 bits = 0-255, 12 bits = 0-4095)
  
ledcAttachPin(pin, canal)
  Asocia un pin GPIO a un canal PWM
  
ledcWrite(canal, valor)
  Escribe duty cycle al canal
  valor: según resolución (0-255 para 8 bits)

--- EJEMPLO PRÁCTICO ---

Brillo fijo al 75%:
  ledcSetup(0, 5000, 8);
  ledcAttachPin(PIN_LED, 0);
  ledcWrite(0, 191);  // 191/255 ≈ 75%

Control con potenciómetro:
  int adc = analogRead(POT_PIN);
  int brillo = map(adc, 0, 4095, 0, 255);
  ledcWrite(0, brillo);

Diferentes frecuencias:
  ledcSetup(0, 1000, 8);   // 1 kHz - puede ser audible
  ledcSetup(0, 5000, 8);   // 5 kHz - estándar
  ledcSetup(0, 25000, 8);  // 25 kHz - ultrasónico

--- NOTAS IMPORTANTES ---

• ESP32 tiene 16 canales PWM independientes
• Frecuencia típica: 5 kHz (imperceptible para humanos)
• Resolución: 8 bits (0-255) es suficiente para LEDs
• Mayor resolución (16 bits) útil para control preciso de motores
• Corrección gamma mejora percepción visual del fade

--- TROUBLESHOOTING ---

LED parpadea: Frecuencia muy baja, usar mínimo 1000 Hz
Brillo no cambia: Verificar que canal y pin coincidan
Fade no suave: Aplicar corrección gamma

===============================================================================
*/
