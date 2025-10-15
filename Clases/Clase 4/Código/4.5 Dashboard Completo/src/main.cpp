/*
    ─────────────────────────────────────────────────────────────────────

    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)

    ─────────────────────────────────────────────────────────────────────

    MÓDULO:      Clase 4 - Conectividad y Protocolos
    PROYECTO:    Dashboard Completo - Sistema IoT Integrado
    HARDWARE:    Familia ESP32 (todos los modelos) + Display OLED SSD1306
    FRAMEWORK:   Arduino / PlatformIO

    DESCRIPCIÓN:
    Dashboard web profesional con archivos separados (HTML/CSS/JS) servidos
    desde SPIFFS/LittleFS. Sistema IoT completo con monitoreo de sensores,
    control de actuadores, display OLED local e interface responsive moderna.
    Integración de visualización local en OLED mostrando temperatura y estado
    del LED en tiempo real.

    CONEXIONES HARDWARE (ESP32-C3):
    - Display OLED SSD1306 128x64 (I2C):
      * SDA: GPIO20
      * SCL: GPIO21
      * VCC: 3.3V
      * GND: GND

    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <U8g2lib.h>
#include <Wire.h>

// Configuración WiFi - COMPLETAR CON TUS CREDENCIALES
const char *ssid = "TU_NOMBRE_DE_RED";
const char *password = "TU_CONTRASEÑA";

// Servidor web
WebServer server(80);

// Constructor para OLED I2C 128x64
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define PWM_CHANNEL 0

// Variable para manejar lógica invertida del PWM
#ifdef ESP32C3
#define LED_PIN 8
bool logicaInvertida = false;   // ESP32C3 usa lógica invertida
#else
#define LED_PIN 2
bool logicaInvertida = true;  // Otros ESP32 usan lógica normal
#endif

// Variables del sistema
float temperature = 25.0;
bool ledState = false;
uint8_t ledBrightness = 50;  // 0-100%

// Función para convertir brillo lineal (0-100) a PWM con corrección gamma
uint8_t brightnessToGammaPWM(uint8_t brightness) {
    if (brightness == 0) return 0;
    float dutyCycle = (brightness / 100.0) * 255.0;
    uint8_t dutyGamma = pow(dutyCycle / 255.0, 2.2) * 255.0;
    return dutyGamma;
}

// Timing para diferentes procesos
uint32_t lastSensorRead = 0;
const uint32_t sensorInterval = 2000; // 2 segundos

uint32_t lastOledUpdate = 0;
const uint32_t oledUpdateInterval = 500; // 0.5 segundos

uint32_t lastIpShow = 0;
const uint32_t ipShowInterval = 30000; // 30 segundos

uint32_t lastWiFiCheck = 0;
const uint32_t wifiCheckInterval = 10000; // 10 segundos

// Función para verificar y mantener conexión WiFi
void checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi desconectado, intentando reconectar...");
        WiFi.reconnect();

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10) {
            delay(500);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println();
            Serial.println("WiFi reconectado!");
            Serial.println("IP: " + WiFi.localIP().toString());
        } else {
            Serial.println();
            Serial.println("No se pudo reconectar WiFi");
        }
    }
}

// Función para obtener el tipo MIME de un archivo
String getContentType(String filename) {
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".jpg")) return "image/jpeg";
    else if (filename.endsWith(".gif")) return "image/gif";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    return "text/plain";
}

// Función para servir archivos estáticos desde LittleFS
bool handleFileRead(String path) {
    Serial.println("Solicitado: " + path);

    if (path.endsWith("/")) path += "index.html";

    String contentType = getContentType(path);

    if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        server.streamFile(file, contentType);
        file.close();
        Serial.println("Archivo servido: " + path);
        return true;
    }

    Serial.println("Archivo no encontrado: " + path);
    return false;
}

// Función para leer sensores
void readSensors() {
    // Leer temperatura interna del ESP32
    temperature = temperatureRead(); // Función incorporada del ESP32

    // Log básico sin intentar reconectar
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("Temp: %.1f°C | http:// %s", temperature, WiFi.localIP().toString().c_str());
        Serial.println();
    } else {
        Serial.printf("Temp: %.1f°C | WiFi: DESCONECTADO", temperature);
        Serial.println();
    }
}

// Función para servir la página principal
void handleRoot() {
    if (!handleFileRead("/index.html")) {
        // Fallback: servir página básica si LittleFS no está disponible
        String basicPage = "<!DOCTYPE html><html><head><title>ESP32 IoT</title></head><body>";
        basicPage += "<h1>ESP32 IoT Dashboard</h1>";
        basicPage += "<p>Temperatura: <span id='temp'>--</span>°C</p>";
        basicPage += "<p>Sistema funcionando sin archivos LittleFS</p>";
        basicPage += "<p><a href='/upload'>Cargar archivos LittleFS</a></p>";
        basicPage += "<script>setInterval(()=>{fetch('/api/sensors').then(r=>r.json()).then(d=>{document.getElementById('temp').innerHTML=d.temperature.toFixed(1)})},3000)</script>";
        basicPage += "</body></html>";
        server.send(200, "text/html", basicPage);
        Serial.println("Sirviendo página básica (fallback)");
    }
}

// API REST para sensores
void handleApiSensors() {
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["timestamp"] = millis();
    doc["uptime"] = millis() / 1000;
    doc["free_heap"] = ESP.getFreeHeap();
    doc["wifi_rssi"] = WiFi.RSSI();

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
    Serial.println("API sensores consultada");
}

// API para estado del LED (GET)
void handleApiLedGet() {
    StaticJsonDocument<150> doc;
    doc["state"] = ledState;
    doc["brightness"] = ledBrightness;

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
    Serial.println("Estado LED consultado");
}

// API para control del LED (POST)
void handleApiLedPost() {
    bool stateChanged = false;
    if (server.hasArg("action")) {
        String action = server.arg("action");
        if (action == "toggle") {
            ledState = !ledState;

            // Aplicar PWM según el estado y brillo usando lógica invertida
            if (ledState) {
                int pwmValue = brightnessToGammaPWM(ledBrightness);
                uint8_t raw = logicaInvertida ? pwmValue : (255 - pwmValue);
                ledcWrite(PWM_CHANNEL, raw);
            } else {
                uint8_t raw = logicaInvertida ? 0 : 255;  // Apagado
                ledcWrite(PWM_CHANNEL, raw);
            }

            stateChanged = true;
            Serial.printf("LED %s", ledState ? "ON" : "OFF");
            Serial.println();
        } else if (action == "brightness" && server.hasArg("value")) {
            int tempBrightness = server.arg("value").toInt();
            // Validar rango y convertir a uint8_t
            if (tempBrightness < 0) tempBrightness = 0;
            if (tempBrightness > 100) tempBrightness = 100;
            ledBrightness = (uint8_t)tempBrightness;

            // Si el brillo es 0, apagar el LED
            if (ledBrightness == 0) {
                ledState = false;
                uint8_t raw = logicaInvertida ? 0 : 255;  // Apagado según lógica
                ledcWrite(PWM_CHANNEL, raw);
            } else {
                // Si se ajusta el brillo y es mayor a 0, encender el LED
                ledState = true;
                int pwmValue = brightnessToGammaPWM(ledBrightness);
                uint8_t raw = logicaInvertida ? pwmValue : (255 - pwmValue);
                ledcWrite(PWM_CHANNEL, raw);
            }

            stateChanged = true;
            Serial.printf("Brillo: %d%% (Estado: %s)", ledBrightness, ledState ? "ON" : "OFF");
            Serial.println();
        }
    }

    if (stateChanged) {
        StaticJsonDocument<200> doc;
        doc["success"] = true;
        doc["state"] = ledState;
        doc["brightness"] = ledBrightness;

        String response;
        serializeJson(doc, response);
        server.send(200, "application/json", response);
    } else {
        server.send(400, "text/plain", "Invalid parameters");
    }
}

// Manejar 404 y archivos estáticos
void handleNotFound() {
    if (!handleFileRead(server.uri())) {
        server.send(404, "text/plain", "Archivo no encontrado");
    }
}

// Función para actualizar display OLED
void updateOLED() {
    u8g2.clearBuffer();

    // Fuente mediana para labels y valores
    u8g2.setFont(u8g2_font_7x13_tf);

    // Mostrar temperatura
    u8g2.drawStr(0, 15, "Temp:");
    String tempStr = String(temperature, 1) + " C";
    int xTemp = 128 - u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xTemp, 15, tempStr.c_str());

    // Mostrar brillo del LED
    u8g2.drawStr(0, 30, "Brillo LED:");
    String brightnessStr = String(ledBrightness) + " %";
    int xBright = 128 - u8g2.getStrWidth(brightnessStr.c_str());
    u8g2.drawStr(xBright, 30, brightnessStr.c_str());

    // Mostrar estado del LED
    u8g2.drawStr(0, 45, "Estado:");
    String stateStr = ledState ? "ON" : "OFF";
    int xState = 128 - u8g2.getStrWidth(stateStr.c_str());
    u8g2.drawStr(xState, 45, stateStr.c_str());

    // Logo UNSE centrado al final (fuente más pequeña)
    u8g2.setFont(u8g2_font_helvB08_tf);
    String unse = "UNSE IoT";
    int xUnse = (128 - u8g2.getStrWidth(unse.c_str())) / 2;
    u8g2.drawStr(xUnse, 62, unse.c_str());

    u8g2.sendBuffer();
}

void setup() {
    Serial.begin(115200);
    delay(1000);  // Dar tiempo al Serial para inicializar

    Serial.println();
    Serial.println("=== ESP32 IoT Dashboard Completo (LittleFS) ===");

    // Configurar pines ANTES de WiFi
    pinMode(LED_PIN, OUTPUT);

    // Configurar PWM para control de brillo
    ledcSetup(PWM_CHANNEL, 5000, 8);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);

    // Inicializar LED apagado usando lógica apropiada
    uint8_t offValue = logicaInvertida ? 0 : 255;  // Valor para LED apagado
    ledcWrite(PWM_CHANNEL, offValue);
    ledState = false;

    Serial.printf("LED configurado en pin %d (Lógica: %s)", LED_PIN, logicaInvertida ? "Invertida" : "Normal");
    Serial.println();

    // Inicializar OLED
    Wire.begin(20, 21); // ESP32-C3 SDA = GPIO20, SCL = GPIO21
    u8g2.begin();
    Serial.println("OLED inicializado correctamente");

    // Mostrar mensaje de inicio en OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB10_tf);
    String initMsg = "Iniciando...";
    int xInit = (128 - u8g2.getStrWidth(initMsg.c_str())) / 2;
    u8g2.drawStr(xInit, 32, initMsg.c_str());
    u8g2.sendBuffer();

    // Conectar WiFi PRIMERO
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");

    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 30) {
        delay(1000);
        Serial.print(".");
        wifiAttempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi conectado!");
        Serial.println("===========================================");
        Serial.print("IP del dispositivo: ");
        Serial.println(WiFi.localIP());
        Serial.println("===========================================");
        Serial.println("Abrir en el navegador:");
        Serial.print("   http://");
        Serial.println(WiFi.localIP());
        Serial.println("===========================================");
    } else {
        Serial.println();
        Serial.println("Error: No se pudo conectar a WiFi");
        Serial.println("Continuando sin conexión...");
        return;  // Salir si no hay WiFi
    }

    // Inicializar LittleFS DESPUÉS de WiFi
    if (!LittleFS.begin(true)) {  // true = formatear si es necesario
        Serial.println("Error montando LittleFS, usando modo básico");
        Serial.println("El dashboard funcionará con página básica");
        // Continuar sin LittleFS
    } else {
        Serial.println("LittleFS montado correctamente");
        Serial.println("Archivos disponibles:");

        // Listar archivos en LittleFS para debug
        File root = LittleFS.open("/");
        File file = root.openNextFile();
        while (file) {
            Serial.println("  - " + String(file.name()) + " (" + String(file.size()) + " bytes)");
            file = root.openNextFile();
        }
    };

    // Configurar rutas del servidor
    server.on("/", handleRoot);
    server.on("/api/sensors", HTTP_GET, handleApiSensors);
    server.on("/api/led", HTTP_GET, handleApiLedGet);
    server.on("/api/led", HTTP_POST, handleApiLedPost);
    server.onNotFound(handleNotFound);

    // Iniciar servidor
    server.begin();
    Serial.println("Servidor web iniciado en puerto 80");

    // Mostrar información de conexión de manera prominente
    Serial.println();
    Serial.println(String('=', 50));
    Serial.println("DASHBOARD COMPLETO INICIADO");
    Serial.println(String('=', 50));
    Serial.println("IP Local: " + WiFi.localIP().toString());
    Serial.println("URL: http://" + WiFi.localIP().toString());
    Serial.println("Puerto: 80");
    Serial.println("SSID: " + String(ssid));
    Serial.println("Sistema de archivos: LittleFS " + String(LittleFS.totalBytes()) + " bytes");
    Serial.println("Funcionalidades:");
    Serial.println("  Archivos estáticos desde LittleFS");
    Serial.println("  Sensor temperatura ESP32");
    Serial.println("  Control LED con PWM");
    Serial.println("  Display OLED SSD1306");
    Serial.println("  API REST completa");
    Serial.println("  Dashboard interactivo");
    Serial.println(String('=', 50));
    Serial.println();

    // Lectura inicial de sensores
    readSensors();

    // Actualización inicial del OLED
    updateOLED();
}

void loop() {
    // Manejar clientes del servidor web
    server.handleClient();

    // Leer sensores periódicamente
    if (millis() - lastSensorRead > sensorInterval) {
        lastSensorRead = millis();
        readSensors();
    }

    // Actualizar OLED periódicamente
    if (millis() - lastOledUpdate > oledUpdateInterval) {
        lastOledUpdate = millis();
        updateOLED();
    }

    // Verificar conexión WiFi periódicamente
    if (millis() - lastWiFiCheck > wifiCheckInterval) {
        lastWiFiCheck = millis();
        checkWiFiConnection();
    }

    // Mostrar IP periódicamente
    if (millis() - lastIpShow > ipShowInterval) {
        lastIpShow = millis();
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println();
            Serial.println("Dashboard: http://" + WiFi.localIP().toString() + " | Temp UC: " + String(temperature, 1) + "°C | LED: " + String(ledState ? "ON" : "OFF"));
        } else {
            Serial.println();
            Serial.println("WiFi desconectado - dashboard no disponible");
        }
    }

    // Pequeña pausa para estabilidad
    delay(10);
}

/*
===============================================================================
          DASHBOARD COMPLETO IoT - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

SISTEMA IoT INTEGRADO:
Este proyecto combina múltiples tecnologías IoT en un solo sistema:
- Dashboard web remoto (HTML/CSS/JS)
- Display OLED local para visualización inmediata
- Sensores integrados (temperatura interna)
- Control de actuadores (LED con PWM)
- API REST para comunicación
- Sistema de archivos para contenido web

ARQUITECTURA COMPLETA:
- Backend (ESP32): API REST + servidor archivos + control hardware
- Frontend Web (Browser): HTML/CSS/JS separados, responsive
- Frontend Local (OLED): Visualización en tiempo real sin conectividad
- Comunicación: JSON sobre HTTP (web) + I2C (OLED)
- Actualización: Fetch API asíncrono (web) + polling periódico (OLED)

VENTAJAS DEL SISTEMA:
✓ Código organizado y mantenible
✓ Archivos web editables sin recompilar
✓ Desarrollo frontend/backend independiente
✓ Visualización local sin depender de WiFi
✓ Interfaz dual: remota (web) y local (OLED)
✓ Reutilización de componentes
✓ Debugging simplificado con múltiples interfaces

--- DIFERENCIA CON EJEMPLO 4.3 (SIMPLE) ---

El ejemplo 4.3 es DIDÁCTICO y usa:
  • GET y POST básicos con HTML forms
  • Solo ON/OFF (sin brillo)
  • HTML generado en C++ (String concatenation)
  • Sin JavaScript fetch
  • Sin JSON
  • Sin archivos separados

Este ejemplo 4.5 es PROFESIONAL y agrega:
  • API REST con JSON
  • Control de brillo con PWM
  • Archivos separados (HTML/CSS/JS)
  • Fetch API asíncrono
  • Sistema de archivos (LittleFS)
  • Display OLED local
  • Corrección gamma
  • Diseño responsive

RECOMENDACIÓN: Aprender primero el 4.3, luego avanzar a este 4.5

--- SISTEMA DE ARCHIVOS ---

SPIFFS/LittleFS:
Sistema de archivos en flash para almacenar HTML/CSS/JS separados
del código C++. Permite desarrollo web independiente y actualizaciones
sin recompilar firmware.

LittleFS.begin()
  Monta sistema de archivos flash

server.serveStatic("/", LittleFS, "/")
  Sirve archivos estáticos desde raíz

File file = LittleFS.open(path, "r")
  Abre archivo para lectura

LittleFS vs SPIFFS:
- LittleFS: Más nuevo, rápido, confiable (recomendado)
- SPIFFS: Legacy, compatible con código antiguo

--- DISPLAY OLED ---

TECNOLOGÍA OLED SSD1306:
Display monocromático 128x64 píxeles con controlador I2C.
Cada píxel emite luz propia (sin retroiluminación).

LIBRERÍA U8G2:
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

  Wire.begin(SDA_PIN, SCL_PIN)  // Inicializar I2C
  u8g2.begin()                   // Inicializar display
  u8g2.clearBuffer()             // Limpiar buffer RAM
  u8g2.setFont(fuente)           // Seleccionar fuente
  u8g2.drawStr(x, y, "texto")    // Dibujar texto
  u8g2.getStrWidth("texto")      // Obtener ancho
  u8g2.sendBuffer()              // Enviar a display

PINES I2C ESP32-C3:
  SDA: GPIO20  |  SCL: GPIO21  |  VCC: 3.3V  |  GND: GND

PATRÓN DE ACTUALIZACIÓN:
  1. clearBuffer()  → Limpiar
  2. Dibujar contenido
  3. sendBuffer()   → Mostrar

TÉCNICAS DE ALINEACIÓN:
  Izquierda:  x = 0
  Derecha:    x = 128 - u8g2.getStrWidth("texto")
  Centro:     x = (128 - u8g2.getStrWidth("texto")) / 2

--- ESTRUCTURA PROYECTO ---

/data/                  # Carpeta archivos web
  index.html           # Página principal
  style.css            # Estilos
  script.js            # Lógica frontend

/src/
  main.cpp             # Backend ESP32

platformio.ini:
  board_build.filesystem = littlefs
  lib_deps =
    bblanchon/ArduinoJson@^6.21.3
    olikraus/U8g2@^2.34.22
  build_flags = -D ESP32C3

--- TIMING NO BLOQUEANTE ---

MÚLTIPLES PROCESOS CONCURRENTES:
  uint32_t lastSensorRead = 0;
  const uint32_t sensorInterval = 2000;    // Sensores cada 2s

  uint32_t lastOledUpdate = 0;
  const uint32_t oledUpdateInterval = 500; // OLED cada 0.5s

  uint32_t lastWiFiCheck = 0;
  const uint32_t wifiCheckInterval = 10000; // WiFi cada 10s

PATRÓN EN LOOP:
  if (millis() - lastX > intervalX) {
    lastX = millis();
    // Ejecutar tarea X
  }

VENTAJA: Todas las tareas se ejecutan de forma independiente sin bloqueos.

--- EJEMPLO PRÁCTICO ---

Cargar archivos a ESP32:
  pio run --target uploadfs

Servidor archivos + API:
  server.serveStatic("/", LittleFS, "/");
  server.on("/api/data", handleAPI);
  server.begin();

JavaScript frontend:
  async function updateData() {
    const res = await fetch('/api/sensors');
    const data = await res.json();
    document.getElementById('temp').innerText = data.temp;
  }
  setInterval(updateData, 2000);

Actualizar OLED:
  void updateOLED() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13_tf);
    u8g2.drawStr(0, 15, "Temp:");
    String temp = String(temperature, 1) + " C";
    u8g2.drawStr(128 - u8g2.getStrWidth(temp.c_str()), 15, temp.c_str());
    u8g2.sendBuffer();
  }

--- API REST vs GET/POST SIMPLE ---

EJEMPLO 4.3 (Simple):
  POST /on         → Enciende LED, redirige con código 303
  POST /off        → Apaga LED, redirige con código 303
  GET /estado      → Retorna texto plano: "Estado del LED: ENCENDIDO"
  
  Ventaja: Fácil de entender, funciona con HTML forms estándar
  Desventaja: Requiere recargar página, no hay control fino

ESTE EJEMPLO 4.5 (Avanzado):
  POST /api/led    → JSON request/response, sin recargar página
                     {action: "toggle"} o {action: "brightness", value: 75}
  GET /api/led     → Retorna JSON: {"state": true, "brightness": 75}
  GET /api/sensors → Retorna JSON: {"temperature": 25.5, "uptime": 12345}
  
  Ventaja: Actualización asíncrona, control fino, datos estructurados
  Desventaja: Más complejo, requiere JavaScript

CUANDO USAR CADA UNO:
  • Proyectos simples/educativos → GET/POST con forms (4.3)
  • Dashboards profesionales → API REST con JSON (4.5)
  • Interfaces móviles → API REST permite apps nativas
  • Integración con otros sistemas → API REST es estándar

--- CONTROL LED CON PWM ---

CONFIGURACIÓN PWM:
  ledcSetup(PWM_CHANNEL, 5000, 8);     // Canal, frecuencia, resolución
  ledcAttachPin(LED_PIN, PWM_CHANNEL);  // Asociar pin
  ledcWrite(PWM_CHANNEL, dutyCycle);    // Escribir valor 0-255

CORRECCIÓN GAMMA:
  Convierte brillo lineal (0-100%) a PWM con corrección gamma 2.2
  para percepción visual más natural.

LÓGICA INVERTIDA (ESP32-C3):
  Algunos ESP32 requieren lógica invertida en el LED:
  raw = logicaInvertida ? (255 - pwmValue) : pwmValue;

--- RESPONSIVE DESIGN ---

CSS Media Queries:
  @media (max-width: 768px) {
    .dashboard { flex-direction: column; }
  }

Viewport meta tag:
  <meta name="viewport" content="width=device-width, initial-scale=1">

--- OPTIMIZACIONES ---

Minificar archivos:
- HTML/CSS/JS minificados reducen tamaño
- Herramientas: UglifyJS, CSSNano

Caché browser:
  server.sendHeader("Cache-Control", "max-age=86400");

Compresión gzip:
  Archivos .gz servidos automáticamente si existen

Actualización OLED eficiente:
- Intervalo de 500ms balanceo entre suavidad y CPU
- Buffer completo para evitar parpadeos
- Fuentes optimizadas para tamaño

--- NOTAS IMPORTANTES ---

• Separar archivos web facilita mantenimiento
• Actualizar frontend sin recompilar firmware
• LittleFS reemplaza SPIFFS (más confiable)
• OLED provee feedback inmediato sin depender de WiFi
• Favicon 404 es normal si no se incluye
• CORS headers necesarios para APIs externas
• Limitar tamaño archivos (<500KB recomendado)
• Display OLED: evitar imágenes estáticas (burn-in)
• Timing no bloqueante permite múltiples tareas concurrentes

PROGRESIÓN DE APRENDIZAJE:
  4.1 WiFi Básico      → Conexión WiFi simple
  4.2 Servidor Básico  → Primer servidor web
  4.3 Control LED      → GET/POST básicos (APRENDER PRIMERO)
  4.4 Sensores         → Lectura y envío de datos
  4.5 Dashboard        → Sistema completo (ESTE EJEMPLO)

--- TROUBLESHOOTING ---

404 en archivos: Verificar uploadfs ejecutado correctamente
SPIFFS mount failed: Cambiar a LittleFS en platformio.ini
Dashboard no carga: Verificar rutas y servidor serveStatic
OLED no enciende: Verificar conexiones I2C y voltaje (3.3V)
OLED texto cortado: Revisar coordenadas y ancho de fuente
LED no responde: Verificar lógica invertida según modelo ESP32
WiFi desconectado: Sistema sigue funcionando con OLED local
JSON parse error: Verificar formato con JSONLint.com
Fetch no funciona: Revisar consola browser (F12) para errores

--- FUNCIONALIDADES INTEGRADAS ---

1. SERVIDOR WEB:
   - Archivos estáticos desde LittleFS
   - Dashboard HTML/CSS/JS profesional
   - Responsive design para móviles

2. API REST:
   - GET /api/sensors → Estado de sensores
   - GET /api/led → Estado LED
   - POST /api/led → Control LED (toggle, brightness)

3. DISPLAY OLED LOCAL:
   - Temperatura en tiempo real
   - Estado LED (ON/OFF)
   - Brillo LED (0-100%)
   - Logo institucional

4. SENSORES:
   - Temperatura interna ESP32 (temperatureRead())
   - Extensible a sensores externos

5. ACTUADORES:
   - LED con control PWM
   - Corrección gamma para brillo perceptual
   - Soporte lógica invertida

6. COMUNICACIÓN:
   - WiFi STA mode
   - Auto-reconexión WiFi
   - Monitoreo periódico de conexión

===============================================================================
*/