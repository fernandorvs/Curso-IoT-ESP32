/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 1 - Introducción a ESP32
    PROYECTO:    Blink - Parpadeo de LED
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Programa básico que hace parpadear el LED integrado del ESP32.
    Primer ejemplo para verificar que el entorno de desarrollo funciona
    correctamente y comprender el ciclo básico de Arduino.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#define PIN_LED 8

void setup()
{
	pinMode(PIN_LED, OUTPUT);
}

void loop()
{
	digitalWrite(PIN_LED, LOW);
	delay(1000);
	digitalWrite(PIN_LED, HIGH);
	delay(1000);
}

/*
===============================================================================
                    BLINK - TEORÍA Y REFERENCIA BÁSICA
===============================================================================

--- CONCEPTOS CLAVE ---

ESTRUCTURA DE PROGRAMA ARDUINO:
- setup(): Se ejecuta una sola vez al inicio
- loop(): Se ejecuta continuamente en bucle infinito

GPIO (General Purpose Input/Output):
- Pines configurables como entrada o salida
- Control digital: HIGH (3.3V) y LOW (0V)

LED INTEGRADO:
- La mayoría de placas ESP32 tienen LED en GPIO2
- Algunos modelos usan GPIO8 (verificar según placa)

--- FUNCIONES PRINCIPALES ---

pinMode(pin, modo)
  Configura un pin como entrada o salida
  Modos: INPUT, OUTPUT, INPUT_PULLUP
  
digitalWrite(pin, valor)
  Escribe valor digital en un pin
  Valores: HIGH (encendido) o LOW (apagado)
  
delay(milisegundos)
  Pausa la ejecución por tiempo especificado
  NOTA: Es función bloqueante (detiene todo el programa)

--- EJEMPLO PRÁCTICO ---

Modificar velocidad de parpadeo:
  delay(500);  // Parpadeo rápido (0.5 segundos)
  delay(2000); // Parpadeo lento (2 segundos)

Patrón personalizado:
  digitalWrite(PIN_LED, HIGH);
  delay(100);  // Encendido breve
  digitalWrite(PIN_LED, LOW);
  delay(900);  // Apagado largo

--- NOTAS IMPORTANTES ---

• delay() bloquea todo el programa (no recomendado para proyectos complejos)
• Para múltiples tareas usar técnicas no bloqueantes (ver ejemplos avanzados)
• Verificar voltaje de salida: ESP32 usa 3.3V (no 5V como Arduino UNO)
• Consumo típico de LED: 5-20mA según color y modelo

--- TROUBLESHOOTING ---

LED no enciende: Verificar PIN_LED según modelo de placa
LED muy tenue: Normal en algunos ESP32, es LED de bajo consumo
No compila: Verificar instalación de PlatformIO y drivers USB

===============================================================================
*/
