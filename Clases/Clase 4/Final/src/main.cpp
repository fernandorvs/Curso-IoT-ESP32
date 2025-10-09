#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

// Configuración WiFi
const char *ssid = "VERA AP 5";
const char *password = "32055136";

/*
const char* ssid = "welectronica";
const char* password = "electro@23";
*/

// Servidor
WebServer server(80);

#define PWM_CHANNEL 0

// Variable para manejar lógica invertida del PWM
#ifdef ESP32C3
#define LED_PIN 8
bool logicaInvertida = true;   // ESP32C3 usa lógica invertida
#else
#define LED_PIN 2
bool logicaInvertida = false;  // Otros ESP32 usan lógica normal
#endif

// Variables del sistema
float temperature = 25.0;
bool ledState = false;
int ledBrightness = 50;

// Función para convertir brillo lineal (0-100) a PWM con corrección gamma (0-255)
// Esto hace que el slider sea más lineal para la percepción humana
int brightnessToGammaPWM(int brightness) {
    if (brightness == 0) return 0;
    // Convertir brillo (0-100) a duty cycle (0-255)
    float dutyCycle = (brightness / 100.0) * 255.0;
    // Aplicar corrección gamma 2.2
    uint8_t dutyGamma = pow(dutyCycle / 255.0, 2.2) * 255.0;
    return dutyGamma;
}

// Timing para lecturas de sensores
uint32_t lastSensorRead = 0;
const uint32_t sensorInterval = 2000; // 2 segundos

// Timing para mostrar IP
uint32_t lastIpShow = 0;
const uint32_t ipShowInterval = 30000; // 30 segundos

// Timing para verificar WiFi
uint32_t lastWiFiCheck = 0;
const uint32_t wifiCheckInterval = 10000; // 10 segundos

// Función para verificar y mantener conexión WiFi
void checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi desconectado, intentando reconectar...");
        WiFi.reconnect();

        // Esperar un poco para la reconexión
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

void readSensors() {
    // Leer temperatura interna del ESP32
    temperature = temperatureRead(); // Función incorporada del ESP32

    // Solo reportar el estado, sin intentar reconectar
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("Temp: %.1f°C | IP: %s", temperature, WiFi.localIP().toString().c_str());
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
                uint8_t raw = logicaInvertida ? (255 - pwmValue) : pwmValue;
                ledcWrite(PWM_CHANNEL, raw);
            } else {
                uint8_t raw = logicaInvertida ? 255 : 0;  // Apagado
                ledcWrite(PWM_CHANNEL, raw);
            }

            stateChanged = true;
            Serial.printf("LED %s", ledState ? "ON" : "OFF");
            Serial.println();
        } else if (action == "brightness" && server.hasArg("value")) {
            ledBrightness = server.arg("value").toInt();
            if (ledBrightness < 0) ledBrightness = 0;
            if (ledBrightness > 100) ledBrightness = 100;

            // Si el brillo es 0, apagar el LED
            if (ledBrightness == 0) {
                ledState = false;
                uint8_t raw = logicaInvertida ? 255 : 0;  // Apagado según lógica
                ledcWrite(PWM_CHANNEL, raw);
            } else {
                // Si se ajusta el brillo y es mayor a 0, encender el LED
                ledState = true;
                int pwmValue = brightnessToGammaPWM(ledBrightness);
                uint8_t raw = logicaInvertida ? (255 - pwmValue) : pwmValue;
                ledcWrite(PWM_CHANNEL, raw);
            }

            stateChanged = true;
            int pwmValue = brightnessToGammaPWM(ledBrightness);
            uint8_t finalPwm = logicaInvertida ? (255 - pwmValue) : pwmValue;
            Serial.printf("Brillo: %d%% (PWM: %d, Estado: %s, Lógica: %s)",
                ledBrightness,
                finalPwm,
                ledState ? "ON" : "OFF",
                logicaInvertida ? "Invertida" : "Normal");
            Serial.println();
        }
    }

    if (stateChanged) {
        server.send(200, "text/plain", "OK");
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

void setup() {
    Serial.begin(115200);
    delay(1000);  // Dar tiempo al Serial para inicializar

    Serial.println();
    Serial.println("=== Sistema IoT ESP32 (WebServer + LittleFS) ===");

    // Configurar pines ANTES de WiFi
    pinMode(LED_PIN, OUTPUT);

    // Configurar PWM para control de brillo
    ledcSetup(PWM_CHANNEL, 5000, 8);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);

    // Inicializar LED apagado usando lógica apropiada
    uint8_t offValue = logicaInvertida ? 255 : 0;  // Valor para LED apagado
    ledcWrite(PWM_CHANNEL, offValue);
    ledState = false;

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
        // Continuar sin LittleFS
    } else {
        Serial.println("LittleFS montado correctamente");
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
    Serial.println("SERVIDOR WEB INICIADO");
    Serial.println(String('=', 50));
    Serial.println("IP Local: " + WiFi.localIP().toString());
    Serial.println("URL: http://" + WiFi.localIP().toString());
    Serial.println("Puerto: 80");
    Serial.println("SSID: " + String(ssid));
    Serial.println("Sistema de archivos: LittleFS");
    Serial.println(String('=', 50));
    Serial.println();

    // Lectura inicial de sensores
    readSensors();
}

void loop() {
    // Manejar clientes del servidor web
    server.handleClient();

    // Leer sensores periódicamente
    if (millis() - lastSensorRead > sensorInterval) {
        lastSensorRead = millis();
        readSensors();
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
            Serial.println("IP del servidor: http://" + WiFi.localIP().toString());
        } else {
            Serial.println();
            Serial.println("WiFi desconectado - servidor no disponible");
        }
    }

    // Pequeña pausa para estabilidad
    delay(10);
}
