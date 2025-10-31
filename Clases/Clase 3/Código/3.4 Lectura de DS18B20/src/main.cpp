/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 3 - Sensores y Periféricos
    PROYECTO:    Lectura de DS18B20 - Sensor Digital de Temperatura
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Lectura de temperatura usando sensor digital DS18B20 con protocolo
    1-Wire. Sensor de alta precisión (±0.5°C) con salida digital que
    elimina necesidad de ADC y conversiones analógicas.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// =====================================================================
// CONFIGURACIÓN DE PINES - MODIFICAR SEGÚN TU CONEXIÓN
// =====================================================================
#define PIN_DS18B20  3    // Pin GPIO para el DS18B20 (1-Wire)

OneWire oneWire(PIN_DS18B20);
DallasTemperature ds(&oneWire);

void setup(){ 
    Serial.begin(115200); 
    ds.begin();
    
    Serial.println("\n=== DS18B20 Sensor Digital ===");
    Serial.printf("Pin DS18B20: GPIO%d\n", PIN_DS18B20);
    Serial.printf("Sensores detectados: %d\n", ds.getDeviceCount());
}

void loop(){
  
    // Control manual del tiempo para modo parásito
    ds.setWaitForConversion(false);
    ds.requestTemperatures();
    // Tiempo necesario para conversión en modo parásito
    delay(1000);

    float t = ds.getTempCByIndex(0);
    Serial.printf("DS18B20: %.2f °C", t);
    Serial.println();
    delay(1000);
}

/*
===============================================================================
                    DS18B20 - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

DS18B20:
Sensor digital de temperatura con protocolo 1-Wire. Salida digital
elimina ruido y conversiones ADC. ROM única de 64 bits permite
identificación individual en bus compartido.

ESPECIFICACIONES:
- Rango: -55°C a +125°C
- Precisión: ±0.5°C (-10°C a +85°C)
- Resolución: 9-12 bits programable (0.5°C a 0.0625°C)
- Interfaz: 1-Wire (un solo pin de datos)
- Alimentación: 3.0-5.5V o parasitic power

PROTOCOLO 1-WIRE:
- Un cable bidireccional para datos
- Permite múltiples sensores en mismo pin
- Cada sensor identificable por ROM única
- Master (ESP32) controla, slave (DS18B20) responde

--- CONEXIÓN HARDWARE ---

Configuración típica:
  VDD ─── 3.3V
  DQ  ─┬─ GPIO4
       │
      [R] 4.7kΩ pull-up a VDD
       │
  GND ─── GND

Parasitic power (2 cables):
  VDD ─── GND (conectado a tierra)
  DQ  ─── GPIO4 + pull-up 4.7kΩ
  GND ─── GND

--- RESOLUCIÓN Y TIEMPOS ---

Configuración vs performance:
  9 bits:  0.5°C,     93.75ms conversión
  10 bits: 0.25°C,    187.5ms conversión
  11 bits: 0.125°C,   375ms conversión
  12 bits: 0.0625°C,  750ms conversión (default)

--- FUNCIONES PRINCIPALES ---

Librería DallasTemperature:

ds.begin()
  Inicializa sensores en el bus
  
ds.requestTemperatures()
  Solicita conversión de temperatura
  Bloqueante según resolución (hasta 750ms)
  
ds.getTempCByIndex(index)
  Obtiene temperatura en Celsius
  index: 0 para primer sensor
  Retorna: float con temperatura
  
ds.getDeviceCount()
  Cuenta sensores conectados al bus
  
ds.setResolution(bits)
  Configura resolución (9-12 bits)

--- EJEMPLO PRÁCTICO ---

Múltiples sensores en mismo bus:
  int count = ds.getDeviceCount();
  ds.requestTemperatures();
  for(int i=0; i<count; i++) {
    float t = ds.getTempCByIndex(i);
    Serial.printf("Sensor %d: %.2f°C\n", i, t);
  }

Cambiar resolución:
  ds.setResolution(10);  // 10 bits, más rápido
  
Verificar sensor válido:
  float t = ds.getTempCByIndex(0);
  if(t == DEVICE_DISCONNECTED_C) {
    Serial.println("Sensor desconectado");
  }

--- VENTAJAS vs NTC ---

DS18B20:
✓ Salida digital (sin ruido)
✓ Mayor precisión (±0.5°C)
✓ No requiere calibración
✓ Múltiples sensores en un pin
✓ Identificación única

NTC:
✓ Más económico
✓ Respuesta más rápida
✓ No requiere librerías
✗ Requiere ADC y calibración
✗ Menor precisión

--- NOTAS IMPORTANTES ---

• Pull-up 4.7kΩ obligatorio en línea DQ
• Cables largos (>20m) pueden requerir 2.2kΩ
• Parasitic power limita velocidad conversión
• ROM única permite identificar sensor específico
• Librería DallasTemperature simplifica uso
• Compatible con DS18S20 y DS1822 también

--- TROUBLESHOOTING ---

Temperatura -127°C: Sensor desconectado o pull-up faltante
85°C constante: No se llamó requestTemperatures()
Lecturas erráticas: Verificar pull-up y longitud cable

===============================================================================
*/

/*
 * ============================================================================
 * RESUMEN TEÓRICO: SENSOR DE TEMPERATURA DIGITAL DS18B20
 * ============================================================================
 *
 * 1. ¿QUÉ ES EL DS18B20?
 *    El DS18B20 es un sensor de temperatura digital programable fabricado por
 *    Maxim Integrated (ahora Analog Devices). Es uno de los sensores de temperatura
 *    más populares en proyectos de electrónica y IoT debido a su facilidad de uso
 *    y comunicación digital.
 *
 * 2. CARACTERÍSTICAS PRINCIPALES:
 *    - Rango de temperatura: -55°C a +125°C
 *    - Precisión: ±0.5°C (de -10°C a +85°C)
 *    - Resolución programable: 9, 10, 11 o 12 bits
 *    - Tiempo de conversión: 93.75ms a 750ms (según resolución)
 *    - Alimentación: 3.0V a 5.5V (modo normal) o parasitic power
 *    - Interfaz: 1-Wire (un solo cable de datos)
 *    - ROM único de 64 bits para identificación
 *
 * 3. PROTOCOLO 1-WIRE:
 *    El DS18B20 utiliza el protocolo 1-Wire de Dallas Semiconductor:
 *    - Un solo cable para datos bidireccionales
 *    - Alimentación por el bus de datos (parasitic power) o externa
 *    - Múltiples dispositivos en el mismo bus
 *    - Cada dispositivo tiene una dirección ROM única de 64 bits
 *    - Master-slave: el microcontrolador es master, DS18B20 es slave
 *
 * 4. ESTRUCTURA DEL CÓDIGO ROM (64 bits):
 *    - 8 bits: Código de familia (0x28 para DS18B20)
 *    - 48 bits: Número de serie único
 *    - 8 bits: CRC para verificación de errores
 *
 * 5. CONEXIÓN HARDWARE:
 *    Configuración típica:
 *    - VDD: 3.3V o 5V (alimentación)
 *    - GND: Ground
 *    - DQ: Línea de datos 1-Wire + resistencia pull-up 4.7kΩ
 *    
 * 6. MODOS DE ALIMENTACIÓN:
 *    a) ALIMENTACIÓN NORMAL:
 *       - VDD conectado a fuente de alimentación
 *       - Más confiable y rápido
 *       - Recomendado para aplicaciones críticas
 *    
 *    b) PARASITIC POWER:
 *       - VDD conectado a GND
 *       - Energía tomada del bus de datos
 *       - Útil cuando se quiere minimizar cables
 *       - Limitaciones en conversiones rápidas
 *
 * 7. RESOLUCIÓN Y PRECISIÓN:
 *    - 9 bits:  0.5°C resolución, 93.75ms conversión
 *    - 10 bits: 0.25°C resolución, 187.5ms conversión
 *    - 11 bits: 0.125°C resolución, 375ms conversión
 *    - 12 bits: 0.0625°C resolución, 750ms conversión (default)
 *
 * 8. COMANDOS PRINCIPALES:
 *    - SKIP ROM (0xCC): Dirigirse a todos los dispositivos
 *    - MATCH ROM (0x55): Dirigirse a un dispositivo específico
 *    - SEARCH ROM (0xF0): Enumerar dispositivos en el bus
 *    - CONVERT T (0x44): Iniciar conversión de temperatura
 *    - READ SCRATCHPAD (0xBE): Leer resultado de temperatura
 *    - WRITE SCRATCHPAD (0x4E): Configurar alarmas y resolución
 *
 * 9. VENTAJAS DEL DS18B20:
 *    - Salida digital (no necesita ADC)
 *    - Alta precisión
 *    - Múltiples sensores en un bus
 *    - Identificación única de cada sensor
 *    - Amplio rango de temperatura
 *    - Bajo consumo de energía
 *    - Inmune al ruido eléctrico
 *    - No requiere calibración
 *
 * 10. DESVENTAJAS:
 *     - Protocolo 1-Wire más complejo que interfaces simples
 *     - Tiempo de conversión relativamente lento
 *     - Requiere resistencia pull-up externa
 *     - Sensible a la calidad de las conexiones
 *     - Costo mayor que termistores simples
 *
 * 11. LIBRERÍAS UTILIZADAS:
 *     a) OneWire.h:
 *        - Implementa el protocolo 1-Wire de bajo nivel
 *        - Maneja la comunicación con el bus
 *        - Funciones para reset, lectura/escritura de bits/bytes
 *     
 *     b) DallasTemperature.h:
 *        - Capa de abstracción para sensores Dallas/Maxim
 *        - Simplifica el uso del DS18B20
 *        - Funciones de alto nivel para temperatura
 *
 * 12. FUNCIONES PRINCIPALES DE LA LIBRERÍA:
 *     - ds.begin(): Inicializa el bus y busca sensores
 *     - ds.getDeviceCount(): Cuenta dispositivos conectados
 *     - ds.requestTemperatures(): Inicia conversión en todos los sensores
 *     - ds.getTempCByIndex(index): Lee temperatura por índice
 *     - ds.getTempC(address): Lee temperatura por dirección específica
 *     - ds.setResolution(resolution): Configura resolución
 *
 * 13. MÚLTIPLES SENSORES:
 *     Cuando hay varios DS18B20 en el mismo bus:
 *     - Cada uno tiene dirección ROM única
 *     - Se pueden leer todos juntos o individualmente
 *     - Útil para monitoreo distribuido de temperatura
 *     - Ejemplo: monitoreo de temperatura en múltiples habitaciones
 *
 * 14. APLICACIONES TÍPICAS:
 *     - Termómetros digitales precisos
 *     - Control de temperatura en acuarios
 *     - Monitoreo ambiental en invernaderos
 *     - Sistemas HVAC
 *     - Dataloggers de temperatura
 *     - Control de procesos industriales
 *     - Monitoreo de temperatura de CPU/equipos
 *
 * 15. SOLUCIÓN DE PROBLEMAS:
 *     - Si no se detecta el sensor: verificar conexiones y pull-up
 *     - Si lecturas erráticas: revisar calidad de conexiones
 *     - Si temperatura = -127°C: error de comunicación o sensor desconectado
 *     - Si demora en lecturas: considerar reducir resolución
 *     - Para largas distancias: usar cables de buena calidad y pull-up más fuerte
 *
 * 16. COMPARACIÓN CON OTROS SENSORES:
 *     vs NTC/Termistores:
 *     + Mayor precisión y linealidad
 *     + Salida digital, inmune a ruido
 *     + No requiere calibración
 *     - Mayor costo y complejidad
 *     
 *     vs DHT22:
 *     + Mayor precisión en temperatura
 *     + Protocolo más robusto
 *     - Solo temperatura (DHT22 incluye humedad)
 *     - Mayor tiempo de conversión
 *
 * ============================================================================
 */