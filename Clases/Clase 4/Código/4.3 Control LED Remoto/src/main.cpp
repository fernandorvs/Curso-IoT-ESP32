/*
    ─────────────────────────────────────────────────────────────────────

    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)

    ─────────────────────────────────────────────────────────────────────

    MÓDULO:      Clase 4 - Conectividad y Protocolos
    PROYECTO:    Control LED Remoto - Ejemplo Simple y Didáctico
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO

    DESCRIPCIÓN:
    Ejemplo SIMPLE de control remoto de LED vía web usando GET y POST.
    Interface minimalista para entender los conceptos básicos.

    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Configuración WiFi - COMPLETAR CON TUS CREDENCIALES
const char *ssid = "TU_NOMBRE_DE_RED";
const char *password = "TU_CONTRASEÑA";

// Servidor web en el puerto 80
WebServer server(80);

// Configuración del LED según el modelo
#ifdef ESP32C3
#define LED_PIN 8
bool logicaInvertida = false;
#else
#define LED_PIN 2
bool logicaInvertida = true;
#endif

// Variable global para el estado del LED
bool ledState = false;

// Control de tiempo para mensajes de estado periódicos
uint32_t previousStatusMillis = 0;
const uint32_t statusInterval = 3000; // 3 segundos

// Función para servir la página principal
void handleRoot() {
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Control LED ESP32</title>";
    html += "<style>";
    html += "body { font-family: Arial; text-align: center; margin: 50px; }";
    html += "h1 { color: #333; }";
    html += "button { padding: 15px 30px; margin: 10px; font-size: 18px; cursor: pointer; }";
    html += ".on { background-color: #4CAF50; color: white; }";
    html += ".off { background-color: #f44336; color: white; }";
    html += ".status { font-size: 24px; margin: 20px; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>Control LED ESP32</h1>";

    // Mostrar estado actual
    html += "<div class='status'>";
    html += "Estado: <strong>" + String(ledState ? "ENCENDIDO" : "APAGADO") + "</strong>";
    html += "</div>";

    // Botones de control
    html += "<form action='/on' method='POST'>";
    html += "<button type='submit' class='on'>ENCENDER</button>";
    html += "</form>";

    html += "<form action='/off' method='POST'>";
    html += "<button type='submit' class='off'>APAGAR</button>";
    html += "</form>";

    html += "<hr>";
    html += "<p>Rutas disponibles:</p>";
    html += "<p>GET / - Esta pagina<br>";
    html += "GET /estado - Ver estado del LED<br>";
    html += "POST /on - Encender LED<br>";
    html += "POST /off - Apagar LED</p>";

    html += "</body>";
    html += "</html>";

    server.send(200, "text/html", html);
    Serial.println("Pagina principal mostrada");
}

// GET: Consultar estado del LED
void handleEstado() {
    String mensaje = "Estado del LED: ";
    mensaje += ledState ? "ENCENDIDO" : "APAGADO";

    server.send(200, "text/plain", mensaje);
    Serial.println("Estado consultado via GET");
}

// POST: Encender el LED
void handleOn() {
    ledState = true;
    digitalWrite(LED_PIN, logicaInvertida ? HIGH : LOW);
    Serial.println("LED encendido via POST");
    // Redirigir a la página principal
    server.sendHeader("Location", "/");
    server.send(303);
}

// POST: Apagar el LED
void handleOff() {
    ledState = false;
    digitalWrite(LED_PIN, logicaInvertida ? LOW : HIGH);
    Serial.println("LED apagado via POST");
    // Redirigir a la página principal
    server.sendHeader("Location", "/");
    server.send(303);
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
    Serial.println("=== ESP32 Control LED Simple ===");

    // Configurar LED
    pinMode(LED_PIN, OUTPUT);
    // Apagar al inicio
    digitalWrite(LED_PIN, logicaInvertida ? LOW : HIGH);
    ledState = false;

    Serial.printf("LED configurado en pin %d\n", LED_PIN);

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
    } else {
        Serial.println();
        Serial.println("Error: No se pudo conectar a WiFi");
        return;
    }

    // Configurar rutas del servidor
    server.on("/", handleRoot);                  // GET: Página principal
    server.on("/estado", handleEstado);          // GET: Ver estado
    server.on("/on", HTTP_POST, handleOn);       // POST: Encender
    server.on("/off", HTTP_POST, handleOff);     // POST: Apagar
    server.onNotFound(handleNotFound);

    // Iniciar servidor
    server.begin();
    Serial.println("Servidor web iniciado");
    Serial.println();
}

void loop() {
    // Manejar peticiones del servidor web
    server.handleClient();

    // Mostrar estado periódicamente
    uint32_t currentMillis = millis();
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
          CONTROL REMOTO WEB - EJEMPLO SIMPLE Y DIDÁCTICO
===============================================================================

--- ¿QUÉ HACE ESTE PROGRAMA? ---

Este programa convierte tu ESP32 en un servidor web simple que permite
controlar un LED desde cualquier navegador web conectado a la misma red WiFi.

--- CONCEPTOS CLAVE ---

GET vs POST:
  • GET: Se usa para OBTENER información (leer)
    Ejemplo: Consultar el estado del LED

  • POST: Se usa para ENVIAR información (escribir/modificar)
    Ejemplo: Encender o apagar el LED

RUTAS DEL SERVIDOR:
  GET  /         → Muestra la página web principal con botones
  GET  /estado   → Devuelve el estado actual del LED (texto plano)
  POST /on       → Enciende el LED
  POST /off      → Apaga el LED

--- CÓMO FUNCIONA ---

1. El ESP32 se conecta a tu red WiFi
2. Inicia un servidor web en el puerto 80 (puerto HTTP estándar)
3. Cuando accedes desde el navegador, el ESP32 envía HTML
4. Los botones en la página envían peticiones POST al ESP32
5. El ESP32 recibe las peticiones y controla el LED físico
6. Después de cambiar el estado, redirige a la página principal

--- PROBANDO CON NAVEGADOR ---

1. Conecta el ESP32 y anota la IP que muestra en el monitor serial
2. Abre el navegador en tu computadora o celular
3. Escribe la IP en la barra de direcciones (ej: http://192.168.1.100)
4. Verás una página con botones para controlar el LED

--- PROBANDO CON CURL (línea de comandos) ---

# Ver la página principal
curl http://192.168.1.100/

# Consultar estado (GET)
curl http://192.168.1.100/estado

# Encender LED (POST)
curl -X POST http://192.168.1.100/on

# Apagar LED (POST)
curl -X POST http://192.168.1.100/off

--- AMPLIACIONES POSIBLES ---

• Agregar más LEDs con diferentes colores
• Agregar sensor y mostrar lecturas en la página
• Crear una API REST con formato JSON
• Agregar control de brillo con PWM
• Usar AJAX/fetch para actualizar sin recargar página
• Implementar autenticación con usuario/contraseña

===============================================================================
*/
