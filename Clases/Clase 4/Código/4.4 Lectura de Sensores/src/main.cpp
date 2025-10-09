/*
    ─────────────────────────────────────────────────────────────────────

    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)

    ─────────────────────────────────────────────────────────────────────

    MÓDULO:      Clase 4 - Conectividad y Protocolos
    PROYECTO:    Lectura de Sensores - Monitoreo Web de Temperatura
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO

    DESCRIPCIÓN:
    Sistema web que muestra lecturas de sensores de temperatura (NTC y DS18B20)
    a través de peticiones GET. Interface simple para consultar datos en tiempo real.

    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

// Configuración WiFi - COMPLETAR CON TUS CREDENCIALES
const char *ssid = "TU_NOMBRE_DE_RED";
const char *password = "TU_CONTRASEÑA";

// Servidor web en el puerto 80
WebServer server(80);

// Configuración de sensores de temperatura
// NTC - Sensor analógico
const float VREF = 3.3;
const float R_FIXED = 10000;  // Resistencia fija en divisor de voltaje
const float R0 = 10000;       // Resistencia NTC a 25°C
const float PT0 = 298.15;     // 25°C en Kelvin
const float BETA = 3950;      // Coeficiente Beta del NTC

// Configuración según el modelo
#ifdef ESP32C3
#define LED_PIN 8
const int NTC_PIN = 0;        // ESP32-C3: GPIO0 (ADC1_CH0) - Compatible con WiFi
const int DS18B20_PIN = 3;    // ESP32-C3: GPIO3
bool logicaInvertida = false;
#else
#define LED_PIN 2
const int NTC_PIN = 32;       // ESP32 DevKit: GPIO32 (ADC1_CH4) - Compatible con WiFi
const int DS18B20_PIN = 4;    // ESP32 DevKit: GPIO4
bool logicaInvertida = true;
#endif

// DS18B20 - Sensor digital 1-Wire
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensorDS(&oneWire);

// Variables para almacenar temperaturas
float temperaturaNTC = 0.0;
float temperaturaDS18B20 = 0.0;

// Variable global para el estado del LED
bool ledState = false;

// Control de tiempo para mensajes de estado periódicos
uint32_t previousStatusMillis = 0;
const uint32_t statusInterval = 3000; // 3 segundos

// Control de tiempo para lectura de sensores
uint32_t previousSensorMillis = 0;
const uint32_t sensorInterval = 2000; // 2 segundos

// Función para leer temperatura del NTC
float leerNTC() {
    int rawADC = analogRead(NTC_PIN);
    float voltaje = rawADC * (VREF / 4095.0);
    
    // Validar lectura
    if (voltaje < 0.1 || voltaje > (VREF - 0.1)) {
        Serial.printf("Advertencia NTC: voltaje fuera de rango (%.3fV, ADC=%d)\n", voltaje, rawADC);
        return -999.0; // Valor de error
    }
    
    // Calcular resistencia del NTC
    float resistenciaNTC = R_FIXED * (VREF / voltaje - 1);
    
    // Validar resistencia calculada
    if (resistenciaNTC < 100 || resistenciaNTC > 1000000) {
        Serial.printf("Advertencia NTC: resistencia fuera de rango (%.2f ohms)\n", resistenciaNTC);
        return -999.0; // Valor de error
    }
    
    // Ecuación de Steinhart-Hart simplificada (Beta)
    float temperaturaKelvin = 1.0 / (1.0 / PT0 + log(resistenciaNTC / R0) / BETA);
    float temperaturaCelsius = temperaturaKelvin - 273.15;
    
    // Validar temperatura resultante
    if (temperaturaCelsius < -50 || temperaturaCelsius > 150) {
        Serial.printf("Advertencia NTC: temperatura fuera de rango (%.2f°C)\n", temperaturaCelsius);
        return -999.0; // Valor de error
    }
    
    return temperaturaCelsius;
}

// Función para leer temperatura del DS18B20
float leerDS18B20() {
    sensorDS.requestTemperatures();
    float temp = sensorDS.getTempCByIndex(0);
    
    // Validar lectura (-127°C indica error del sensor)
    if (temp == -127.0 || temp == 85.0) {
        Serial.println("Advertencia DS18B20: sensor no detectado o error de lectura");
        return -999.0; // Valor de error
    }
    
    return temp;
}

// Función para servir la página principal
void handleRoot() {
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta http-equiv='refresh' content='5'>";
    html += "<title>Lectura Sensores ESP32</title>";
    html += "<style>";
    html += "body { font-family: Arial; text-align: center; margin: 50px; }";
    html += "h1 { color: #333; }";
    html += ".sensor { font-size: 24px; margin: 20px; padding: 15px; background-color: #f0f0f0; border-radius: 5px; }";
    html += ".temp { font-size: 36px; font-weight: bold; color: #2196F3; }";
    html += ".error { color: #f44336; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>Lectura de Sensores ESP32</h1>";

    // Mostrar NTC
    html += "<div class='sensor'>";
    html += "Sensor NTC (Analogico):<br>";
    if (temperaturaNTC > -900) {
        html += "<span class='temp'>" + String(temperaturaNTC, 1) + " &deg;C</span>";
    } else {
        html += "<span class='error'>ERROR - Sensor no conectado</span>";
    }
    html += "</div>";

    // Mostrar DS18B20
    html += "<div class='sensor'>";
    html += "Sensor DS18B20 (Digital):<br>";
    if (temperaturaDS18B20 > -900) {
        html += "<span class='temp'>" + String(temperaturaDS18B20, 1) + " &deg;C</span>";
    } else {
        html += "<span class='error'>ERROR - Sensor no conectado</span>";
    }
    html += "</div>";

    html += "<hr>";
    html += "<p>Rutas disponibles:</p>";
    html += "<p>GET <a href='/'>/ - Esta pagina</a><br>";
    html += "GET <a href='/temperaturas'>/temperaturas - Ver todas las temperaturas</a><br>";
    html += "GET <a href='/ntc'>/ntc - Solo temperatura NTC</a><br>";
    html += "GET <a href='/ds18b20'>/ds18b20 - Solo temperatura DS18B20</a></p>";

    html += "</body>";
    html += "</html>";

    server.send(200, "text/html", html);
    Serial.println("Página principal mostrada");
}

// GET: Consultar todas las temperaturas (texto plano)
void handleTemperaturas() {
    String mensaje = "=== LECTURAS DE TEMPERATURA ===\n\n";
    
    if (temperaturaNTC > -900) {
        mensaje += "NTC (Analogico):     " + String(temperaturaNTC, 2) + " C\n";
    } else {
        mensaje += "NTC (Analogico):     ERROR - Sensor no conectado\n";
    }
    
    if (temperaturaDS18B20 > -900) {
        mensaje += "DS18B20 (Digital):   " + String(temperaturaDS18B20, 2) + " C\n";
    } else {
        mensaje += "DS18B20 (Digital):   ERROR - Sensor no conectado\n";
    }
    
    if (temperaturaNTC > -900 && temperaturaDS18B20 > -900) {
        mensaje += "\nDiferencia:          " + String(abs(temperaturaNTC - temperaturaDS18B20), 2) + " C\n";
    }

    server.send(200, "text/plain", mensaje);
    Serial.println("Temperaturas consultadas via GET");
}

// GET: Consultar solo temperatura NTC
void handleNTC() {
    String mensaje;
    if (temperaturaNTC > -900) {
        mensaje = "Temperatura NTC: " + String(temperaturaNTC, 2) + " C";
    } else {
        mensaje = "Temperatura NTC: ERROR - Sensor no conectado o fuera de rango";
    }
    server.send(200, "text/plain", mensaje);
    Serial.println("Temperatura NTC consultada via GET");
}

// GET: Consultar solo temperatura DS18B20
void handleDS18B20() {
    String mensaje;
    if (temperaturaDS18B20 > -900) {
        mensaje = "Temperatura DS18B20: " + String(temperaturaDS18B20, 2) + " C";
    } else {
        mensaje = "Temperatura DS18B20: ERROR - Sensor no conectado o fuera de rango";
    }
    server.send(200, "text/plain", mensaje);
    Serial.println("Temperatura DS18B20 consultada via GET");
}


// Función para páginas no encontradas (404)
void handleNotFound() {
    String message = "Pagina no encontrada\n\n";
    message += "URI: " + server.uri() + "\n";
    message += "Metodo: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";

    server.send(404, "text/plain", message);
    Serial.println("404 - Ruta no encontrada: " + server.uri());
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("=== ESP32 Monitor de Temperaturas ===");

    // Configurar LED de indicación
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, logicaInvertida ? LOW : HIGH);
    ledState = false;
    Serial.printf("LED indicador en pin %d\n", LED_PIN);

    // Configurar pin NTC
    pinMode(NTC_PIN, INPUT);
    Serial.printf("Sensor NTC configurado en pin %d\n", NTC_PIN);
    
    // Diagnóstico inicial NTC
    int rawTest = analogRead(NTC_PIN);
    float voltTest = rawTest * (VREF / 4095.0);
    Serial.printf("  - Lectura ADC inicial: %d (%.3fV)\n", rawTest, voltTest);

    // Inicializar sensor DS18B20
    sensorDS.begin();
    Serial.printf("Sensor DS18B20 configurado en pin %d\n", DS18B20_PIN);
    
    // Verificar si hay sensores DS18B20 conectados
    int numSensores = sensorDS.getDeviceCount();
    Serial.printf("  - Sensores DS18B20 detectados: %d\n", numSensores);

    // Realizar primera lectura de sensores
    Serial.println("\n--- Primera lectura de sensores ---");
    temperaturaNTC = leerNTC();
    temperaturaDS18B20 = leerDS18B20();
    
    if (temperaturaNTC > -900) {
        Serial.printf("✓ NTC: %.2f°C\n", temperaturaNTC);
    } else {
        Serial.println("✗ NTC: ERROR - Verificar conexión del sensor");
    }
    
    if (temperaturaDS18B20 > -900) {
        Serial.printf("✓ DS18B20: %.2f°C\n", temperaturaDS18B20);
    } else {
        Serial.println("✗ DS18B20: ERROR - Verificar conexión del sensor");
    }
    Serial.println("-----------------------------------\n");

    // Conectar WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");

    int intentos = 0;
    while (WiFi.status() != WL_CONNECTED && intentos < 20) {
        delay(500);
        Serial.print(".");
        intentos++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi conectado!");
        Serial.println("==================================");
        Serial.print("Abrir en el navegador: http://");
        Serial.println(WiFi.localIP());
        Serial.println("==================================");
        
        // Encender LED cuando está conectado
        digitalWrite(LED_PIN, logicaInvertida ? HIGH : LOW);
        ledState = true;
    } else {
        Serial.println();
        Serial.println("Error: No se pudo conectar a WiFi");
        return;
    }

    // Configurar rutas del servidor (solo GET)
    server.on("/", handleRoot);                      // GET: Página principal
    server.on("/temperaturas", handleTemperaturas);  // GET: Todas las temperaturas
    server.on("/ntc", handleNTC);                    // GET: Solo NTC
    server.on("/ds18b20", handleDS18B20);            // GET: Solo DS18B20
    server.onNotFound(handleNotFound);

    // Iniciar servidor
    server.begin();
    Serial.println("Servidor web iniciado");
    Serial.println("Rutas disponibles:");
    Serial.println("Todas las temperaturas");
    Serial.println("Solo temperatura NTC");
    Serial.println("Solo temperatura DS18B20");
    Serial.println();
}

void loop() {
    // Manejar peticiones del servidor web
    server.handleClient();

    // Leer sensores periódicamente
    uint32_t currentMillis = millis();
    if (currentMillis - previousSensorMillis >= sensorInterval) {
        previousSensorMillis = currentMillis;
        
        // Actualizar temperaturas
        temperaturaNTC = leerNTC();
        temperaturaDS18B20 = leerDS18B20();
        
        // Mostrar en serial
        Serial.print("Sensores - ");
        if (temperaturaNTC > -900) {
            Serial.printf("NTC: %.2f°C", temperaturaNTC);
        } else {
            Serial.print("NTC: ERROR");
        }
        Serial.print(" | ");
        if (temperaturaDS18B20 > -900) {
            Serial.printf("DS18B20: %.2f°C", temperaturaDS18B20);
        } else {
            Serial.print("DS18B20: ERROR");
        }
        if (temperaturaNTC > -900 && temperaturaDS18B20 > -900) {
            Serial.printf(" | Diff: %.2f°C", abs(temperaturaNTC - temperaturaDS18B20));
        }
        Serial.println();
    }

    // Mostrar estado periódicamente
    if (currentMillis - previousStatusMillis >= statusInterval) {
        previousStatusMillis = currentMillis;

        // Status compacto en una línea
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Status: WiFi OK | http://" + WiFi.localIP().toString() +
                " | RSSI: " + String(WiFi.RSSI()) + " dBm");
        } else {
            Serial.println("Status: WiFi DESCONECTADO");
        }
    }
}

/*
===============================================================================
          LECTURA DE SENSORES VIA WEB - SISTEMA DE MONITOREO
===============================================================================

--- ¿QUÉ HACE ESTE PROGRAMA? ---

Este programa convierte tu ESP32 en un servidor web que muestra las lecturas
de dos sensores de temperatura (NTC analógico y DS18B20 digital) en tiempo real.
Todas las consultas se hacen mediante peticiones GET únicamente.

--- CONCEPTOS CLAVE ---

SENSORES DE TEMPERATURA:
  • NTC (Thermistor): Sensor analógico económico, lectura vía ADC
    - Respuesta rápida pero menor precisión
    - Requiere cálculo con ecuación de Steinhart-Hart
    - Validación de rango y detección de errores
    
  • DS18B20: Sensor digital de precisión, protocolo 1-Wire
    - Alta precisión (±0.5°C)
    - Comunicación digital directa
    - Detección automática de sensores

MÉTODOS HTTP GET:
  • GET solo consulta información (lectura)
  • No modifica el estado del sistema
  • Ideal para lecturas de sensores y monitoreo
  • Todas las rutas son solo GET (sin POST/PUT/DELETE)

AUTO-REFRESH:
  • La página principal se actualiza automáticamente cada 5 segundos
  • Muestra temperaturas en tiempo real sin necesidad de recargar manualmente

--- RUTAS DEL SERVIDOR ---

  GET  /              → Página web con interfaz visual (auto-refresh cada 5s)
  GET  /temperaturas  → Todas las temperaturas (texto plano)
  GET  /ntc           → Solo temperatura del NTC (texto plano)
  GET  /ds18b20       → Solo temperatura del DS18B20 (texto plano)

--- CÓMO FUNCIONA ---

1. El ESP32 lee los sensores cada 2 segundos en el loop()
2. Almacena las últimas lecturas en variables globales
3. Valida cada lectura y detecta errores (retorna -999.0 si hay error)
4. El servidor web sirve estas lecturas cuando se solicitan
5. La página HTML muestra los valores de forma visual
6. La página se auto-actualiza cada 5 segundos
7. Los endpoints de texto plano permiten consultas directas con links

--- HARDWARE Y CONEXIONES ---

ESP32 DevKit (clásico):
  • NTC:     GPIO32 (ADC1_CH4) + Resistencia 10kΩ
  • DS18B20: GPIO4 (1-Wire) + Resistencia pull-up 4.7kΩ
  • LED:     GPIO2 (indicador de WiFi)

ESP32-C3 SuperMini:
  • NTC:     GPIO0 (ADC1_CH0) + Resistencia 10kΩ
  • DS18B20: GPIO3 (1-Wire) + Resistencia pull-up 4.7kΩ
  • LED:     GPIO8 (indicador de WiFi)

IMPORTANTE: Todos los pines ADC utilizados son de ADC1, compatibles con WiFi.

--- USO BÁSICO ---

1. Configurar WiFi en las credenciales del código
2. Conectar los sensores según el esquema de tu placa
3. Compilar y subir al ESP32
4. Abrir el navegador en la IP mostrada
5. La página se actualizará automáticamente cada 5 segundos

--- PROBANDO CON NAVEGADOR ---

Abrir en el navegador:
  http://192.168.1.100/              (página principal, auto-refresh)
  http://192.168.1.100/temperaturas  (todas las temperaturas)
  http://192.168.1.100/ntc           (solo NTC)
  http://192.168.1.100/ds18b20       (solo DS18B20)

--- PROBANDO CON CURL ---

# Ver interfaz web (HTML)
curl http://192.168.1.100/

# Consultar todas las temperaturas (texto plano)
curl http://192.168.1.100/temperaturas

# Solo NTC (texto plano)
curl http://192.168.1.100/ntc

# Solo DS18B20 (texto plano)
curl http://192.168.1.100/ds18b20

--- VALIDACIÓN Y MANEJO DE ERRORES ---

El programa valida las lecturas y detecta:
  • NTC: Voltaje fuera de rango (0.1V - 3.2V)
  • NTC: Resistencia fuera de rango (100Ω - 1MΩ)
  • NTC: Temperatura fuera de rango (-50°C - 150°C)
  • DS18B20: Sensor no detectado (-127°C o 85°C)

Cuando hay error:
  • Retorna -999.0 como valor de error
  • Muestra "ERROR" en la interfaz web
  • Imprime mensaje de advertencia en Serial

--- VENTAJAS DE USAR SOLO GET ---

✓ Simplicidad: No hay que manejar POST/PUT/DELETE
✓ Cacheable: Los navegadores pueden cachear respuestas GET
✓ Seguro: GET no modifica datos (idempotente)
✓ Fácil de probar: Se puede usar directamente en el navegador
✓ Ideal para monitoreo: Lecturas son operaciones de solo lectura
✓ RESTful: Sigue principios REST para consultas

--- CÁLCULO DE TEMPERATURA NTC ---

Ecuación de Steinhart-Hart simplificada (parámetro Beta):

  1/T = 1/T₀ + (1/β) * ln(R/R₀)

Donde:
  T  = Temperatura en Kelvin
  T₀ = Temperatura de referencia (298.15K = 25°C)
  β  = Coeficiente Beta (típicamente 3950K)
  R  = Resistencia actual del NTC
  R₀ = Resistencia a 25°C (10kΩ)

La resistencia se calcula del divisor de voltaje:
  R_NTC = R_FIXED × (VREF / V_medido - 1)

--- PROGRESIÓN DEL CURSO ---

Este proyecto sigue la línea didáctica:
  4.1 WiFi Básico        → Conectividad
  4.2 Servidor Básico    → Conceptos HTTP
  4.3 Control LED        → GET + POST simple
  4.4 Lectura Sensores   → GET con datos de sensores (este proyecto)
  4.5 Dashboard Completo → Interface avanzada con JSON y gráficos

--- PRÓXIMOS PASOS SUGERIDOS ---

• Agregar formato JSON para APIs REST
• Implementar gráficos en tiempo real con Chart.js
• Guardar histórico de lecturas en SPIFFS/LittleFS
• Agregar alertas cuando la temperatura supere umbrales
• Implementar WebSocket para actualizaciones más eficientes
• Agregar más sensores (humedad, presión, etc.)

===============================================================================
*/
