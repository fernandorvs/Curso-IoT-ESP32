/*
    ─────────────────────────────────────────────────────────────────────

    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)

    ─────────────────────────────────────────────────────────────────────

    MÓDULO:      Clase 4 - Conectividad y Protocolos
    PROYECTO:    Servidor Web Básico - HTTP Server
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO

    DESCRIPCIÓN:
    Servidor HTTP básico que sirve página web desde ESP32. Muestra
    información del dispositivo y contador de visitas. Fundamento
    para crear interfaces web e APIs REST.

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

// Contador de visitas
int visitCounter = 0;

// Control de tiempo para mensajes de estado periódicos
uint32_t previousStatusMillis = 0;
const uint32_t statusInterval = 3000; // 3 segundos

// Función para servir la página principal
void handleRoot() {
    visitCounter++;

    // Crear página HTML simple
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head><title>ESP32 Servidor</title></head>";
    html += "<body>";
    html += "<h1>ESP32 Servidor Web</h1>";
    html += "<p>IP: " + WiFi.localIP().toString() + "</p>";
    html += "<p>Visitas: " + String(visitCounter) + "</p>";
    html += "<p>Tiempo: " + String(millis() / 1000) + " segundos</p>";
    html += "</body>";
    html += "</html>";

    server.send(200, "text/html", html);
    Serial.println("Página servida - Visita #" + String(visitCounter));
}

void setup() {
    Serial.begin(115200);
    Serial.println("Iniciando ESP32...");

    // Conectar WiFi
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // Configurar servidor
    server.on("/", handleRoot);
    server.begin();
    Serial.println("Servidor iniciado");
}

void loop() {
    server.handleClient();

    // Mostrar estado periódicamente
    uint32_t currentMillis = millis();
    if (currentMillis - previousStatusMillis >= statusInterval) {
        previousStatusMillis = currentMillis;

        // Status compacto en una línea
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Status: WiFi OK | IP: " + WiFi.localIP().toString() +
                " | RSSI: " + String(WiFi.RSSI()) + " dBm");
        } else {
            Serial.println("Status: WiFi DESCONECTADO");
        }
    }
}

/*
===============================================================================
                SERVIDOR WEB ESP32 - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

SERVIDOR HTTP:
ESP32 actúa como servidor web, responde a peticiones HTTP de clientes
(navegadores, apps). Permite crear interfaces web para control y
monitoreo del dispositivo.

ARQUITECTURA CLIENTE-SERVIDOR:
1. Cliente (navegador) hace petición HTTP GET/POST
2. ESP32 recibe petición en puerto 80
3. Handler procesa petición y genera respuesta
4. ESP32 envía respuesta HTML/JSON/texto
5. Cliente renderiza contenido

CÓDIGOS HTTP:
- 200 OK: Petición exitosa
- 404 Not Found: Recurso no existe
- 500 Internal Server Error: Error del servidor

--- LIBRERÍA WEBSERVER ---

Funciones principales:

server.begin()
  Inicia servidor en puerto especificado

server.on(ruta, handler)
  Asocia ruta URL a función handler
  Ejemplo: server.on("/temp", handleTemp)

server.handleClient()
  Procesa peticiones entrantes
  Llamar en loop() constantemente

server.send(código, tipo, contenido)
  Envía respuesta HTTP al cliente
  código: 200, 404, etc.
  tipo: "text/html", "application/json", etc.

server.arg(nombre)
  Obtiene parámetro de URL
  Ejemplo: /led?state=on → server.arg("state")

--- MONITOREO DE CONEXIÓN WIFI ---

WiFi.status()
  Retorna el estado actual de la conexión WiFi
  WL_CONNECTED: Conectado exitosamente
  WL_DISCONNECTED: Desconectado
  WL_CONNECTION_LOST: Conexión perdida
  
WiFi.localIP()
  Retorna la IP local asignada al ESP32
  Usar .toString() para convertir a String

WiFi.RSSI()
  Retorna la intensidad de señal en dBm
  Valores típicos: -30 (excelente) a -90 (débil)

--- TEMPORIZACIÓN NO BLOQUEANTE ---

Técnica para ejecutar tareas periódicas sin delay():

  uint32_t previousMillis = 0;
  const uint32_t interval = 10000;  // 10 segundos
  
  void loop() {
    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // Código a ejecutar periódicamente
    }
  }

Ventajas:
• No bloquea la ejecución del programa
• Permite múltiples temporizadores independientes
• El servidor sigue respondiendo durante los intervalos

--- TIPOS DE DATOS EN SISTEMAS EMBEBIDOS ---

uint32_t vs unsigned long:
• uint32_t: Especifica exactamente 32 bits sin signo
• Más portable entre diferentes arquitecturas
• Recomendado para temporizadores con millis()
• millis() retorna uint32_t (desborda en ~49 días)

--- EJEMPLO PRÁCTICO ---

Múltiples rutas:
  server.on("/", handleRoot);
  server.on("/info", handleInfo);
  server.on("/reset", handleReset);

Respuesta JSON:
  void handleAPI() {
    String json = "{\"temp\":25.3,\"hum\":60}";
    server.send(200, "application/json", json);
  }

Manejo parámetros GET:
  // URL: /led?state=on
  void handleLED() {
    String state = server.arg("state");
    if(state == "on") {
      digitalWrite(LED, HIGH);
      server.send(200, "text/plain", "LED ON");
    }
  }

Página 404 personalizada:
  void handleNotFound() {
    server.send(404, "text/plain", "Página no encontrada");
  }
  server.onNotFound(handleNotFound);

--- HTML DINÁMICO ---

Construcción de HTML:
  String html = "<html><body>";
  html += "<h1>Temperatura: " + String(temp) + "°C</h1>";
  html += "<button onclick='location.reload()'>Actualizar</button>";
  html += "</body></html>";

CSS inline:
  String css = "<style>";
  css += "body{font-family:Arial;background:#f0f0f0;}";
  css += "h1{color:#333;text-align:center;}";
  css += "</style>";

--- CONCATENACIÓN DE STRINGS ---

Método usado en este proyecto:
  Serial.println("IP: " + WiFi.localIP().toString() + 
                 " | RSSI: " + String(WiFi.RSSI()) + " dBm");

Conversiones útiles:
• IPAddress.toString() → String con formato IP
• String(valor) → Convierte int, float, etc. a String
• Operador + concatena múltiples valores en una línea

--- NOTAS IMPORTANTES ---

• handleClient() debe llamarse frecuentemente en loop()
• No usar delay() largos, afecta respuesta del servidor
• HTML generado en String consume RAM
• Para HTML grande usar PROGMEM o SPIFFS
• Puerto 80 es estándar HTTP (no requiere especificar en URL)
• ESP32 puede manejar múltiples clientes simultáneos (limitado)
• Monitorear WiFi.status() ayuda a detectar desconexiones
• RSSI permite evaluar calidad de señal WiFi

--- TROUBLESHOOTING ---

Servidor no responde: 
  Verificar IP correcta y WiFi conectado
  Comprobar WiFi.status() == WL_CONNECTED

Timeout: 
  Reducir delays en loop(), handleClient() bloqueado
  Usar temporizadores no bloqueantes

Memoria insuficiente: 
  HTML muy grande, usar archivos externos
  Reducir uso de String, considerar PROGMEM

Conexión intermitente:
  Verificar RSSI (menor a -80 dBm es débil)
  Implementar reconexión automática si es necesario

===============================================================================
*/