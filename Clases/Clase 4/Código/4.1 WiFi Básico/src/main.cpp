/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 4 - Conectividad y Protocolos
    PROYECTO:    WiFi Básico - Conexión a Red Inalámbrica
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Conexión básica a red WiFi en modo estación (STA). Muestra información
    de red, monitorea calidad de señal y reconecta automáticamente si se
    pierde conexión. Base para todos los proyectos IoT con ESP32.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>
#include <WiFi.h>

// Configuración WiFi - COMPLETAR CON TUS CREDENCIALES
const char *ssid = "TU_NOMBRE_DE_RED";
const char *password = "TU_CONTRASEÑA";

// Timing para mostrar IP periódicamente
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

void setup() {
    Serial.begin(115200);
    delay(1000);  // Dar tiempo al Serial para inicializar

    Serial.println();
    Serial.println("=== ESP32 WiFi Básico ===");

    // Configurar WiFi en modo estación (cliente)
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");

    // Intentar conectar por hasta 30 segundos
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 30) {
        delay(1000);
        Serial.print(".");
        wifiAttempts++;
    }

    // Verificar resultado de la conexión
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi conectado exitosamente!");
        Serial.println("===========================================");
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("IP del dispositivo: ");
        Serial.println(WiFi.localIP());
        Serial.print("Máscara de red: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("Gateway: ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("DNS: ");
        Serial.println(WiFi.dnsIP());
        Serial.print("Intensidad de señal (RSSI): ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
        Serial.println("===========================================");
    } else {
        Serial.println();
        Serial.println("Error: No se pudo conectar a WiFi");
        Serial.println("Verifique las credenciales y la red");
        Serial.println("El programa continuará sin conexión...");
    }
}

void loop() {
    // Verificar conexión WiFi periódicamente
    if (millis() - lastWiFiCheck > wifiCheckInterval) {
        lastWiFiCheck = millis();
        checkWiFiConnection();
    }

    // Mostrar IP periódicamente si está conectado
    if (millis() - lastIpShow > ipShowInterval) {
        lastIpShow = millis();
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println();
            Serial.println("Estado: Conectado a WiFi");
            Serial.println("IP del dispositivo: " + WiFi.localIP().toString());
            Serial.print("Intensidad de señal: ");
            Serial.print(WiFi.RSSI());
            Serial.println(" dBm");
        } else {
            Serial.println();
            Serial.println("Estado: WiFi desconectado");
        }
    }

    // Pequeña pausa para estabilidad
    delay(100);
}

/*
===============================================================================
                    WIFI ESP32 - TEORÍA Y REFERENCIA
===============================================================================

--- CONCEPTOS CLAVE ---

MODOS WiFi ESP32:
- STA (Station): Cliente WiFi, se conecta a router
- AP (Access Point): Crea red WiFi propia
- AP+STA: Ambos modos simultáneamente

PROCESO DE CONEXIÓN:
1. WiFi.mode(WIFI_STA) - Configurar modo
2. WiFi.begin(ssid, password) - Iniciar conexión
3. Esperar WL_CONNECTED
4. Obtener IP por DHCP automáticamente

RSSI (Received Signal Strength Indicator):
Indica calidad de señal en dBm:
  -30 a -50 dBm: Excelente
  -50 a -60 dBm: Buena
  -60 a -70 dBm: Aceptable
  -70 a -80 dBm: Débil
  < -80 dBm: Muy débil/inestable

--- FUNCIONES PRINCIPALES ---

WiFi.mode(mode)
  Configura modo WiFi
  WIFI_STA, WIFI_AP, WIFI_AP_STA
  
WiFi.begin(ssid, password)
  Inicia conexión a red WiFi
  Retorna inmediatamente, conexión asíncrona
  
WiFi.status()
  Estado actual de conexión
  WL_CONNECTED, WL_DISCONNECTED, WL_NO_SSID_AVAIL, etc.
  
WiFi.localIP()
  IP asignada al ESP32
  IPAddress, convertir a String con .toString()
  
WiFi.RSSI()
  Intensidad de señal en dBm
  Valores negativos, más cerca de 0 = mejor
  
WiFi.reconnect()
  Reintenta conexión con últimas credenciales

WiFi.disconnect()
  Desconecta de red actual

--- EJEMPLO PRÁCTICO ---

Conexión bloqueante simple:
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

Timeout de conexión:
  WiFi.begin(ssid, pass);
  int timeout = 20; // segundos
  while(WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(1000);
    timeout--;
  }

Manejo de reconexión automática:
  void loop() {
    if(WiFi.status() != WL_CONNECTED) {
      WiFi.reconnect();
    }
  }

--- INFORMACIÓN DE RED ---

Datos disponibles una vez conectado:
  WiFi.localIP()      // IP del ESP32
  WiFi.subnetMask()   // Máscara de subred
  WiFi.gatewayIP()    // IP del router
  WiFi.dnsIP()        // Servidor DNS
  WiFi.macAddress()   // MAC address del ESP32
  WiFi.SSID()         // Nombre de red conectada

--- SEGURIDAD WiFi ---

Tipos de encriptación soportados:
- WPA2-PSK (recomendado)
- WPA-PSK
- WEP (inseguro, obsoleto)
- Open (sin seguridad)

Mejores prácticas:
• Usar WPA2 siempre que sea posible
• No hardcodear credenciales (usar config file)
• Implementar timeout de conexión
• Monitorear y reconectar automáticamente

--- NOTAS IMPORTANTES ---

• WiFi consume ~80-170mA cuando activo
• ADC2 no funciona con WiFi activo (usar ADC1)
• Conexión toma ~2-5 segundos típicamente
• WiFi.begin() no bloquea, usar WiFi.status() para verificar
• IP asignada por DHCP puede cambiar
• RSSI varía con distancia y obstáculos
• Auto-reconnect habilitado por defecto

--- TROUBLESHOOTING ---

No conecta: Verificar SSID y contraseña correctos
Desconexión frecuente: Mover ESP32 más cerca del router, RSSI < -75dBm
IP 0.0.0.0: Aún no conectado o DHCP falló

===============================================================================
*/
