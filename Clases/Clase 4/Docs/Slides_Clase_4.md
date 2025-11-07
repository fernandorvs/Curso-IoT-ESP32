# Curso IoT ESP32 - UNSE
## Clase 4: Servidor Web IoT

**Ing. Fernando Raúl Vera Suasnávar**  
Universidad Nacional de Santiago del Estero

---

## 🎯 Objetivos de la Clase

- Comprender el **Modelo TCP/IP** aplicado a IoT
- Conectar el ESP32 a redes WiFi
- Implementar servidores web con API REST
- Crear interfaces web para control remoto
- Visualizar datos de sensores en tiempo real
- Desarrollar un dashboard IoT completo

---

## 📡 Modelo TCP/IP - Vista General

El **Modelo TCP/IP** es el stack de protocolos real de Internet. Tiene 4 capas:

| Capa | Nombre | Función | Protocolos/Tecnologías |
|------|--------|---------|------------------------|
| **4** | Aplicación | Protocolos de usuario | HTTP, MQTT, CoAP, DNS |
| **3** | Transporte | Comunicación extremo a extremo | TCP, UDP |
| **2** | Internet | Enrutamiento de paquetes | IP, ICMP, ARP |
| **1** | Acceso a Red | Hardware y enlace físico | WiFi, Ethernet, PPP |

### ¿Por qué TCP/IP y no OSI?
- **TCP/IP** es el modelo **real** que usa Internet
- **Más simple**: 4 capas vs 7 capas de OSI
- **Práctico**: Enfocado en implementación, no teoría
- **Estándar de facto**: Todo IoT usa TCP/IP

---

## 🔍 Modelo TCP/IP en ESP32 + WiFi

### Nuestro Stack IoT:

```
┌────────────────────────────────────────────────────┐
│  CAPA 4: APLICACIÓN                                │
│  ┌─────────────────────────────────────────────┐   │
│  │ HTTP Server, REST API, JSON, HTML/CSS/JS    │   │
│  │ Ejemplos: GET /estado, POST /led            │   │
│  └─────────────────────────────────────────────┘   │
├────────────────────────────────────────────────────┤
│  CAPA 3: TRANSPORTE                                │
│  ┌─────────────────────────────────────────────┐   │
│  │ TCP Puerto 80                               │   │
│  │ - Control de flujo                          │   │
│  │ - Reenvío de paquetes perdidos              │   │
│  │ - Conexiones confiables                     │   │
│  └─────────────────────────────────────────────┘   │
├────────────────────────────────────────────────────┤
│  CAPA 2: INTERNET                                  │
│  ┌─────────────────────────────────────────────┐   │
│  │ Protocolo IP                                │   │
│  │ - Direcciones IP (192.168.x.x)              │   │
│  │ - Enrutamiento de paquetes                  │   │
│  │ - DHCP para asignación automática           │   │
│  └─────────────────────────────────────────────┘   │
├────────────────────────────────────────────────────┤
│  CAPA 1: ACCESO A RED                              │
│  ┌─────────────────────────────────────────────┐   │
│  │ WiFi 802.11 (2.4 GHz)                       │   │
│  │ - SSID / Password                           │   │
│  │ - MAC Address                               │   │
│  │ - RSSI (intensidad señal)                   │   │
│  │ - Radio físico del ESP32                    │   │
│  └─────────────────────────────────────────────┘   │
└────────────────────────────────────────────────────┘
```

---

## 🌐 Ejemplos: Recorrido por las Capas TCP/IP

Vamos a construir 5 ejemplos que trabajan en diferentes capas:

1. **WiFi Básico** → Capas 1-2 (Acceso a Red + Internet)
2. **Servidor Básico** → Capa 3 (Transporte - TCP)
3. **Control LED Remoto** → Capa 4 (Aplicación - HTTP REST)
4. **Lectura de Sensores** → Capa 4 (Aplicación - JSON API)
5. **Dashboard Completo** → Stack completo integrado

---

# 📡 Ejemplo 4.1: WiFi Básico

## Capas TCP/IP involucradas: 1 y 2

```
┌────────────────────────────────────────────────┐
│  CAPA 2: INTERNET                              │
│  - Obtener dirección IP por DHCP               │
│  - Configurar Gateway (router)                 │
│  - Máscara de subred                           │
│  - DNS servers                                 │
├────────────────────────────────────────────────┤
│  CAPA 1: ACCESO A RED                          │
│  - Conectar a SSID (nombre de red WiFi)        │
│  - Autenticación WPA2                          │
│  - Monitorear RSSI (intensidad señal)          │
│  - MAC Address del ESP32                       │
│  - Radio 2.4GHz activo                         │
└────────────────────────────────────────────────┘
```

### Enfoque de este ejemplo:
Establecer la **conectividad básica de red** - las capas fundamentales para que el ESP32 pueda comunicarse en una red IP.

---

## 📝 Ejemplo 4.1: WiFi Básico - Código Clave

```cpp
#include <WiFi.h>

const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

void setup() {
  Serial.begin(115200);
  
  // CAPA 1: Iniciar hardware WiFi y conectar a red
  WiFi.mode(WIFI_STA);  // Modo Station (cliente)
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // CAPA 2: Información del protocolo IP
  Serial.println("\n- WiFi conectado");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());      // Dirección IP asignada
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());   // Máscara de subred
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());    // Router (puerta de enlace)
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());        // Servidor DNS
  
  // CAPA 1: Información de la capa de acceso a red
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());   // Dirección física
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());           // Intensidad señal WiFi
  Serial.println(" dBm");
}
```

---

## 🔧 Ejemplo 4.1: Características

### Funcionalidades:
- ✅ Conexión WiFi en modo Station (STA)
- ✅ Monitoreo de señal RSSI cada 10s
- ✅ Reconexión automática si se desconecta
- ✅ Muestra IP, MAC, Gateway cada 30s

### Conceptos TCP/IP - Capas 1 y 2:

**Capa 1 - Acceso a Red (WiFi):**
- **SSID**: Nombre de la red WiFi (Service Set Identifier)
- **RSSI**: Intensidad de señal en dBm (-30 excelente, -90 muy débil)
- **MAC Address**: Dirección física única del ESP32
- **WPA2**: Protocolo de seguridad WiFi

**Capa 2 - Internet (IP):**
- **IP Address**: Identificador único en la red (ej: 192.168.1.100)
- **DHCP**: Asignación automática de IP por el router
- **Gateway**: Puerta de enlace (router) para salir a Internet
- **Subnet Mask**: Define el rango de IPs de la red local

---

## 🧪 Ejemplo 4.1: Práctica

### Actividad:
1. Abrir `Código/4.1 WiFi Básico/`
2. Configurar tu SSID y password en `src/main.cpp`
3. Compilar y subir al ESP32
4. Observar en Serial Monitor:
   - Proceso de conexión
   - IP asignada
   - Calidad de señal (RSSI)
   - Reconexión automática (desconecta el router)

### Preguntas:
- ¿Qué IP te asignó el router?
- ¿Cuál es tu RSSI? ¿Es buena la señal?
- ¿Qué pasa si el WiFi se desconecta?

---

# 🌐 Ejemplo 4.2: Servidor Básico

## Capa TCP/IP involucrada: 3 (Transporte)

```
┌─────────────────────────────────────────────────┐
│  CAPA 3: TRANSPORTE                             │
│  - TCP Puerto 80 (HTTP)                         │
│  - Escuchar conexiones entrantes                │
│  - Three-way handshake (SYN, SYN-ACK, ACK)      │
│  - Control de flujo de datos                    │
│  - Garantizar entrega ordenada                  │
│  - Reenvío de paquetes perdidos                 │
├─────────────────────────────────────────────────┤
│  CAPAS 1-2: Ya establecidas                     │
│  (WiFi conectado + IP asignada)                 │
└─────────────────────────────────────────────────┘
```

### Enfoque de este ejemplo:
Implementar la **capa de transporte TCP** que permite conexiones confiables y bidireccionales entre cliente (navegador) y servidor (ESP32).

---

## 📝 Ejemplo 4.2: Servidor Básico - Código Clave

```cpp
#include <WiFi.h>
#include <WebServer.h>

// CAPA 3: Crear servidor TCP en puerto 80
WebServer server(80);

// CAPA 4: Handler de aplicación HTTP
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP32 Web Server</h1>";
  html += "<p>Uptime: " + String(millis()/1000) + "s</p>";
  html += "</body></html>";
  
  // Enviar respuesta HTTP al cliente
  server.send(200, "text/html", html);
}

void setup() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  // CAPA 3: Configurar endpoint y abrir puerto TCP 80
  server.on("/", handleRoot);
  server.begin();  // Socket TCP escuchando en puerto 80
  
  Serial.println("Servidor TCP iniciado en puerto 80");
  Serial.print("Accede desde: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  // CAPA 3: Aceptar y procesar conexiones TCP entrantes
  server.handleClient();
}
```

---

## 🔧 Ejemplo 4.2: Características

### Funcionalidades:
- ✅ Servidor HTTP en puerto 80
- ✅ Página HTML con info del ESP32
- ✅ Contador de visitas
- ✅ Endpoints: `/` (root) y `/about`

### Conceptos TCP/IP - Capa 3 (Transporte):

**TCP (Transmission Control Protocol):**
- **Puerto 80**: Número de puerto estándar para HTTP
- **Socket**: Punto final de comunicación (IP + Puerto)
- **Three-way handshake**: Establecimiento de conexión
  1. Cliente → Servidor: SYN (sincronizar)
  2. Servidor → Cliente: SYN-ACK (reconocimiento)
  3. Cliente → Servidor: ACK (confirmar)
- **Control de flujo**: Regular velocidad de transmisión
- **Confiabilidad**: Reenvío automático de paquetes perdidos
- **Orden**: Los datos llegan en el orden correcto

**¿Por qué TCP y no UDP para HTTP?**
- HTTP necesita **garantía de entrega**
- Navegadores esperan respuestas **completas y ordenadas**
- UDP es más rápido pero **no confiable** (streaming, gaming)

---

## 🧪 Ejemplo 4.2: Práctica

### Actividad:
1. Abrir `Código/4.2 Servidor Básico/`
2. Subir el código al ESP32
3. Anotar la IP del ESP32 del Serial Monitor
4. Desde tu navegador, ir a: `http://[IP_DEL_ESP32]`
5. Probar también: `http://[IP_DEL_ESP32]/about`

### Observar:
- ¿Qué muestra la página principal?
- ¿Cómo cambia el contador de visitas?
- En Serial Monitor: logs de peticiones HTTP

---

# 🎛️ Ejemplo 4.3: Control LED Remoto

## Capa TCP/IP involucrada: 4 (Aplicación - HTTP)

```
┌─────────────────────────────────────────────────┐
│  CAPA 4: APLICACIÓN                             │
│  - Protocolo HTTP (GET, POST)                   │
│  - API REST para control IoT                    │
│  - Endpoints: /estado, /on, /off                │
│  - Códigos de respuesta: 200 OK, 404 Not Found  │
│  - Headers HTTP (Content-Type, etc.)            │
│  - Interface HTML para usuario                  │
├─────────────────────────────────────────────────┤
│  CAPA 3: TRANSPORTE (TCP)                       │
│  Puerto 80 ya establecido                       │
├─────────────────────────────────────────────────┤
│  CAPAS 1-2: INFRAESTRUCTURA                     │
│  WiFi + IP funcionando                          │
└─────────────────────────────────────────────────┘
```

### Enfoque de este ejemplo:
Implementar el **protocolo de aplicación HTTP** con una API REST para control remoto de dispositivos IoT.

---

## 📝 Ejemplo 4.3: Control LED - Código Clave

```cpp
#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN = 8;  // GPIO 8 (ESP32-C3)
bool ledState = false;

WebServer server(80);

// CAPA 4: API REST - Método GET para consultar estado
void handleGetEstado() {
  // Formato JSON para datos estructurados
  String json = "{\"led\":";
  json += ledState ? "true" : "false";
  json += "}";
  
  // Respuesta HTTP con header Content-Type: application/json
  server.send(200, "application/json", json);
}

// CAPA 4: API REST - Método POST para encender LED
void handleLedOn() {
  ledState = true;
  digitalWrite(LED_PIN, HIGH);
  
  // Respuesta HTTP 200 OK
  server.send(200, "text/plain", "LED Encendido");
}

// CAPA 4: API REST - Método POST para apagar LED
void handleLedOff() {
  ledState = false;
  digitalWrite(LED_PIN, LOW);
  
  server.send(200, "text/plain", "LED Apagado");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  
  // Registrar endpoints HTTP
  server.on("/", handleRoot);           // GET: Interface HTML
  server.on("/estado", handleGetEstado);  // GET: Consultar
  server.on("/on", handleLedOn);          // POST: Encender
  server.on("/off", handleLedOff);        // POST: Apagar
  
  server.begin();
}
```

---

## 🔧 Ejemplo 4.3: Características

### Funcionalidades:
- ✅ Control de LED por HTTP GET/POST
- ✅ API REST con 4 endpoints
- ✅ Interface web con botones
- ✅ Sincronización estado físico-virtual

### Conceptos TCP/IP - Capa 4 (Aplicación HTTP):

**Protocolo HTTP (HyperText Transfer Protocol):**
- **Métodos HTTP**:
  - `GET`: Obtener recursos (no modifica estado) - **idempotente**
  - `POST`: Enviar datos, ejecutar acciones - **puede modificar**
  - `PUT`: Actualizar recurso completo
  - `DELETE`: Eliminar recurso

**API REST (REpresentational State Transfer):**
- Arquitectura para servicios web sobre HTTP
- **Recursos** identificados por URLs: `/estado`, `/on`, `/off`
- **Sin estado**: Cada petición es independiente
- **Formatos estándar**: JSON, XML, plain text

**Códigos de Respuesta HTTP:**
- `200 OK`: Petición exitosa
- `404 Not Found`: Recurso no existe
- `400 Bad Request`: Petición mal formada
- `500 Internal Server Error`: Error del servidor

**Content-Type Headers:**
- `application/json`: Datos en formato JSON
- `text/html`: Página web HTML
- `text/plain`: Texto simple

---

## 🧪 Ejemplo 4.3: Práctica

### Actividad:
1. Abrir `Código/4.3 Control LED Remoto/`
2. Conectar un LED en GPIO 8 (ESP32-C3)
3. Subir el código
4. Abrir navegador en: `http://[IP_DEL_ESP32]`
5. Probar los botones ON/OFF

### Experimentos:
- Usar la interface web para controlar el LED
- Probar endpoints directamente:
  - `http://[IP]/estado` → Ver estado actual
  - `http://[IP]/on` → Encender
  - `http://[IP]/off` → Apagar
- Observar logs en Serial Monitor

---

# 📊 Ejemplo 4.4: Lectura de Sensores

## Capa TCP/IP: Aplicación con JSON (Capa 4)

```
┌─────────────────────────────────────────────────┐
│  CAPA 4: APLICACIÓN                             │
│  - HTTP GET con respuestas JSON                 │
│  - API REST para lectura de sensores            │
│  - Endpoints: /ntc, /ds18b20                    │
│  - Serialización de datos estructurados         │
│  - Content-Type: application/json               │
├─────────────────────────────────────────────────┤
│  CAPA 3: TRANSPORTE (TCP)                       │
│  Conexiones HTTP sobre TCP puerto 80            │
├─────────────────────────────────────────────────┤
│  CAPAS 1-2: INFRAESTRUCTURA                     │
│  WiFi + IP funcionando                          │
└─────────────────────────────────────────────────┘
```

### Enfoque de este ejemplo:
Uso de **JSON** como formato de intercambio de datos en la capa de aplicación para IoT.

---

## 📝 Ejemplo 4.4: Lectura Sensores - Código Clave

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Hardware
const int NTC_PIN = 0;
const int DS18B20_PIN = 3;
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

WebServer server(80);

// CAPA 4: API REST - Endpoint para sensor NTC
void handleNTC() {
  int adc = analogRead(NTC_PIN);
  float voltage = adc * (3.3 / 4095.0);
  float tempC = calcularTemperaturaNTC(voltage);
  
  // Formato JSON para datos estructurados
  String json = "{";
  json += "\"sensor\":\"NTC\",";
  json += "\"temperatura\":" + String(tempC, 1) + ",";
  json += "\"unidad\":\"C\",";
  json += "\"adc\":" + String(adc);
  json += "}";
  
  // Header HTTP con Content-Type JSON
  server.send(200, "application/json", json);
}

// CAPA 4: API REST - Endpoint para sensor DS18B20
void handleDS18B20() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  
  // JSON con datos del sensor
  String json = "{";
  json += "\"sensor\":\"DS18B20\",";
  json += "\"temperatura\":" + String(tempC, 2) + ",";
  json += "\"unidad\":\"C\"";
  json += "}";
  
  server.send(200, "application/json", json);
}
```

---

## 🔧 Ejemplo 4.4: Características

### Funcionalidades:
- ✅ Doble sensor de temperatura (NTC + DS18B20)
- ✅ API REST: `/ntc` y `/ds18b20`
- ✅ Respuestas en formato JSON
- ✅ Página web para visualización

### JSON - Formato de Intercambio en IoT:

**¿Por qué JSON en IoT?**
- **Legible**: Humanos y máquinas pueden entenderlo
- **Ligero**: Menos overhead que XML
- **Estructurado**: Pares clave-valor organizados
- **Estándar**: Soportado por todos los lenguajes
- **Fácil de parsear**: Librerías en C++, JS, Python, etc.

**Ejemplo JSON de Sensor NTC:**
```json
{
  "sensor": "NTC",
  "temperatura": 24.5,
  "unidad": "C",
  "adc": 2048
}
```

**Ejemplo JSON de Sensor DS18B20:**
```json
{
  "sensor": "DS18B20",
  "temperatura": 23.75,
  "unidad": "C"
}
```

**Consumo en JavaScript (navegador):**
```javascript
fetch('/ntc')
  .then(response => response.json())
  .then(data => {
    console.log(data.temperatura);  // 24.5
  });
```

---

## 🧪 Ejemplo 4.4: Práctica

### Actividad:
1. Abrir `Código/4.4 Lectura de Sensores/`
2. Conectar hardware:
   - NTC en GPIO 0 + resistencia 10kΩ
   - DS18B20 en GPIO 3
3. Subir el código
4. Probar endpoints en navegador:
   - `http://[IP]/ntc` → Temperatura NTC
   - `http://[IP]/ds18b20` → Temperatura DS18B20
   - `http://[IP]/` → Interface web

### Observar:
- Formato JSON de las respuestas
- Diferencias entre ambos sensores
- Actualización en tiempo real

---

# 🎨 Ejemplo 4.5: Dashboard Completo

## Stack TCP/IP Completo Integrado

```
┌─────────────────────────────────────────────────┐
│  CAPA 4: APLICACIÓN                             │
│  - HTTP Server completo                         │
│  - REST API (GET /api/estado, POST /api/led)    │
│  - Servir archivos estáticos (HTML/CSS/JS)      │
│  - JSON para datos, HTML para presentación      │
│  - Sistema de archivos LittleFS                 │
├─────────────────────────────────────────────────┤
│  CAPA 3: TRANSPORTE                             │
│  - TCP puerto 80 para HTTP                      │
│  - Múltiples conexiones simultáneas             │
├─────────────────────────────────────────────────┤
│  CAPA 2: INTERNET                               │
│  - IP Address local en red WiFi                 │
│  - Enrutamiento de paquetes                     │
├─────────────────────────────────────────────────┤
│  CAPA 1: ACCESO A RED                           │
│  - WiFi 802.11 conectado                        │
│  - Interface física activa                      │
└─────────────────────────────────────────────────┘
```

### Enfoque de este ejemplo:
Sistema IoT **completo** que usa todas las capas del stack TCP/IP de forma integrada.

---

## 📝 Ejemplo 4.5: Dashboard - Arquitectura

### Estructura del Proyecto:

```
4.5 Dashboard Completo/
├── data/                    ← Sistema de archivos LittleFS
│   ├── index.html          ← Interface HTML5
│   ├── style.css           ← Estilos responsive
│   └── script.js           ← Lógica frontend
├── src/
│   └── main.cpp            ← Backend ESP32
└── platformio.ini          ← Configuración
```

### Separación de Capas:
- **Backend (ESP32)**: API REST, control hardware, sensores
- **Frontend (LittleFS)**: Interface web, visualización, UX
- **Comunicación**: JSON sobre HTTP

---

## 📝 Ejemplo 4.5: Dashboard - Backend

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

WebServer server(80);
Adafruit_SSD1306 display(128, 64, &Wire);

int ledBrightness = 0;  // PWM 0-100%

// CAPA 4 (Aplicación): API REST - Obtener estado del sistema
void handleGetEstado() {
  // JSON con múltiples datos del dispositivo
  String json = "{";
  json += "\"led\":" + String(ledBrightness) + ",";
  json += "\"temperatura\":" + String(leerTemperatura(), 1) + ",";
  json += "\"wifi_rssi\":" + String(WiFi.RSSI());
  json += "}";
  server.send(200, "application/json", json);
}

// CAPA 4: API REST - Control PWM del LED
void handleSetLED() {
  if (server.hasArg("brightness")) {
    ledBrightness = server.arg("brightness").toInt();
    ledBrightness = constrain(ledBrightness, 0, 100);
    
    // Convertir 0-100% a PWM 0-255
    int pwmValue = map(ledBrightness, 0, 100, 0, 255);
    analogWrite(LED_PIN, pwmValue);
    
    // Respuesta JSON de confirmación
    server.send(200, "application/json", 
                "{\"success\":true,\"brightness\":" + 
                String(ledBrightness) + "}");
  } else {
    // Error 400: Parámetro faltante
    server.send(400, "application/json", 
                "{\"success\":false,\"error\":\"Missing parameter\"}");
  }
}

void setup() {
  // CAPA 4: Inicializar sistema de archivos LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Error montando LittleFS");
  }
  
  // CAPA 4: Servir archivos HTML/CSS/JS estáticos
  server.serveStatic("/", LittleFS, "/index.html");
  server.serveStatic("/style.css", LittleFS, "/style.css");
  server.serveStatic("/script.js", LittleFS, "/script.js");
  
  // CAPA 4: Registrar endpoints de API REST
  server.on("/api/estado", HTTP_GET, handleGetEstado);
  server.on("/api/led", HTTP_POST, handleSetLED);
  
  // CAPA 3: Iniciar servidor TCP en puerto 80
  server.begin();
}
```

---

## 📝 Ejemplo 4.5: Dashboard - Frontend

**script.js** (Capa 4 de Aplicación - lado cliente)

```javascript
// CAPA 4: Actualización automática cada 2 segundos
setInterval(actualizarDatos, 2000);

async function actualizarDatos() {
  try {
    // CAPA 4: Petición HTTP GET a la API REST
    // Protocolo: HTTP sobre TCP sobre IP sobre WiFi
    const response = await fetch('/api/estado');
    
    // CAPA 4: Parsear respuesta JSON
    const data = await response.json();
    
    // Actualizar interfaz HTML con datos recibidos
    document.getElementById('temperatura').textContent = 
      data.temperatura.toFixed(1) + '°C';
    document.getElementById('wifi-rssi').textContent = 
      data.wifi_rssi + ' dBm';
    document.getElementById('led-estado').textContent = 
      data.led + '%';
  } catch (error) {
    console.error('Error en comunicación HTTP:', error);
  }
}

// CAPA 4: Control de LED con slider (HTTP POST)
async function cambiarLED(brightness) {
  try {
    const response = await fetch('/api/led?brightness=' + brightness, {
      method: 'POST'
    });
    const data = await response.json();
    
    if (data.success) {
      console.log('LED actualizado a ' + data.brightness + '%');
    }
  } catch (error) {
    console.error('Error:', error);
  }
}
```

### Flujo completo de una petición HTTP:

1. **JavaScript** llama `fetch('/api/estado')` → **Capa 4**
2. Navegador construye petición HTTP GET → **Capa 4**
3. HTTP se envía sobre **TCP puerto 80** → **Capa 3**
4. Paquetes TCP se envían con **IP destino del ESP32** → **Capa 2**
5. Datos se transmiten por **WiFi** → **Capa 1**
6. ESP32 recibe por **WiFi** → **Capa 1**
7. IP entrega a **TCP puerto 80** → **Capas 2-3**
8. WebServer procesa **HTTP GET /api/estado** → **Capa 4**
9. Respuesta JSON se envía de vuelta por el mismo camino

---

## 🔧 Ejemplo 4.5: Características

### Funcionalidades Completas:
- ✅ Dashboard web profesional (HTML/CSS/JS separados)
- ✅ Sistema de archivos LittleFS
- ✅ Control PWM de LED (0-100%)
- ✅ Display OLED local con temperatura
- ✅ API REST con endpoints JSON
- ✅ Interface responsive para móviles
- ✅ Actualización automática cada 2s

### Hardware:
- OLED SSD1306 (SDA: GPIO20, SCL: GPIO21)
- LED PWM (GPIO 8 para ESP32-C3)
- Sensor de temperatura (NTC o DS18B20)

---

## 🧪 Ejemplo 4.5: Práctica

### Actividad Completa:

1. **Preparar archivos:**
   ```
   cd "Código/4.5 Dashboard Completo"
   pio run --target uploadfs    # Subir archivos LittleFS
   pio run --target upload       # Subir código
   ```

2. **Probar dashboard:**
   - Abrir navegador: `http://[IP_DEL_ESP32]`
   - Usar slider para controlar brillo del LED
   - Observar actualización automática de temperatura
   - Ver estado en OLED simultáneamente

3. **Probar desde móvil:**
   - Conectar smartphone a misma red WiFi
   - Abrir navegador móvil con IP del ESP32
   - Verificar diseño responsive

---

## 🎓 Resumen: Capas TCP/IP en cada Ejemplo

| Ejemplo | Capas | Qué Implementamos | Conceptos Clave |
|---------|-------|-------------------|-----------------|
| **4.1 WiFi** | 1-2 | Acceso a Red + Internet | WiFi, SSID, IP, DHCP |
| **4.2 Servidor** | 3 | Transporte TCP | Puerto 80, Socket, Three-way handshake |
| **4.3 Control LED** | 4 | Aplicación HTTP | REST API, GET/POST, Headers |
| **4.4 Sensores** | 4 | Aplicación con JSON | Serialización, Content-Type |
| **4.5 Dashboard** | 1-4 | Stack completo | Todo integrado |

### Progresión Didáctica:
1. **Conectividad básica** (Capas 1-2): WiFi + IP
2. **Protocolo de transporte** (Capa 3): TCP confiable
3. **Aplicación simple** (Capa 4): HTTP REST para control
4. **Datos estructurados** (Capa 4): JSON para sensores
5. **Sistema completo** (Capas 1-4): Dashboard profesional

### Mapeo TCP/IP ↔ Tecnologías IoT:

| Capa TCP/IP | Tecnologías ESP32 | Librerías/APIs |
|-------------|-------------------|----------------|
| **4 - Aplicación** | HTTP, REST, JSON | WebServer.h, ArduinoJson.h |
| **3 - Transporte** | TCP, sockets | WiFiClient.h, AsyncTCP |
| **2 - Internet** | IP, DHCP, ARP | lwIP stack (interno) |
| **1 - Acceso a Red** | WiFi 802.11 | WiFi.h, esp_wifi.h |

---

## 💡 Conceptos Clave de la Clase

### Modelo TCP/IP - 4 Capas:

**CAPA 1: Acceso a Red**
- **WiFi 802.11**: Estándar para redes inalámbricas
- **SSID**: Service Set Identifier (nombre de red)
- **MAC Address**: Dirección física única del hardware
- **RSSI**: Received Signal Strength Indicator (dBm)
- **WPA2**: Seguridad y cifrado WiFi

**CAPA 2: Internet**
- **IP (Internet Protocol)**: Direccionamiento lógico (192.168.x.x)
- **DHCP**: Dynamic Host Configuration Protocol (asignación automática)
- **Gateway**: Router que conecta a otras redes
- **Subnet Mask**: Define rango de IPs locales
- **ARP**: Address Resolution Protocol (IP → MAC)

**CAPA 3: Transporte**
- **TCP**: Transmission Control Protocol (confiable, ordenado)
- **Puerto**: Identificador de aplicación (80 = HTTP)
- **Socket**: IP + Puerto (punto final de comunicación)
- **Three-way handshake**: SYN, SYN-ACK, ACK
- **Control de flujo**: Ventanas deslizantes, ACKs
- **UDP**: Alternativa rápida pero no confiable

**CAPA 4: Aplicación**
- **HTTP**: HyperText Transfer Protocol
- **REST API**: Architectural style para servicios web
- **JSON**: JavaScript Object Notation (intercambio de datos)
- **MIME types**: Identifican tipo de contenido
- **Status codes**: 200 OK, 404 Not Found, 500 Error

---

## 🚀 Mejores Prácticas IoT

### Seguridad:
- ⚠️ No hardcodear credenciales en producción
- ✅ Usar HTTPS en lugar de HTTP (próximas clases)
- ✅ Validar entradas del usuario
- ✅ Implementar timeouts y límites de rate

### Performance:
- ✅ Optimizar tamaño de respuestas JSON
- ✅ Usar polling inteligente (no muy frecuente)
- ✅ Implementar caché cuando sea posible
- ✅ Minimizar archivos HTML/CSS/JS

### Usabilidad:
- ✅ Feedback visual de acciones
- ✅ Indicadores de conexión/desconexión
- ✅ Diseño responsive para móviles
- ✅ Manejo graceful de errores

---

## 🔧 Herramientas de Debugging

### Serial Monitor:
```
WiFi conectado - IP: 192.168.1.100
Servidor HTTP iniciado
[HTTP] GET / - 200 OK
[HTTP] POST /api/led?brightness=75 - 200 OK
```

### Browser DevTools (F12):
- **Console**: Ver errores JavaScript
- **Network**: Inspeccionar peticiones HTTP
- **Elements**: Inspeccionar DOM y CSS

### Postman / curl:
```bash
# Probar API directamente
curl http://192.168.1.100/api/estado
curl -X POST http://192.168.1.100/api/led?brightness=50
```

---

## 📚 Recursos y Referencias

### Documentación Oficial:
- [ESP32 WiFi Library](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html)
- [WebServer Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer)
- [LittleFS Guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/littlefs.html)

### Tutoriales:
- [Random Nerd Tutorials - ESP32](https://randomnerdtutorials.com/projects-esp32/)
- [REST API Best Practices](https://restfulapi.net/)
- [MDN Web Docs - JavaScript](https://developer.mozilla.org/en-US/docs/Web/JavaScript)

---

## ✅ Checklist de la Clase

Al finalizar, debes poder:

- [ ] Explicar las 4 capas del modelo TCP/IP
- [ ] Diferenciar TCP/IP de OSI y por qué usamos TCP/IP
- [ ] Conectar ESP32 a WiFi (Capas 1-2)
- [ ] Obtener IP por DHCP y entender Gateway
- [ ] Crear un servidor TCP en puerto 80 (Capa 3)
- [ ] Implementar API REST con HTTP (Capa 4)
- [ ] Usar JSON para intercambio de datos
- [ ] Servir archivos HTML/CSS/JS desde LittleFS
- [ ] Controlar LEDs remotamente con PWM
- [ ] Leer sensores y retornar JSON
- [ ] Crear interfaces web responsive
- [ ] Integrar todo en un dashboard completo
- [ ] Identificar qué capa TCP/IP usa cada tecnología

---

## 🎯 Desafío Final

### Proyecto Integrador:

**Crear un sistema IoT completo que:**
1. Conecte a WiFi automáticamente
2. Sirva un dashboard web profesional
3. Controle 2 LEDs con PWM (colores diferentes)
4. Lea 2 sensores (temperatura + luminosidad)
5. Muestre datos en OLED local
6. Actualice dashboard web cada 3 segundos
7. Tenga diseño responsive para móvil

**Tiempo**: 30-40 minutos  
**Entregable**: Código funcionando + demostración

---

## 🚀 Próxima Clase

### Clase 5: Protocolos IoT Avanzados

- **MQTT**: Protocolo pub/sub para IoT
- **WebSockets**: Comunicación bidireccional en tiempo real
- **Cloud Integration**: ThingSpeak, Blynk, AWS IoT
- **OTA Updates**: Actualización remota de firmware
- **Low Power**: Optimización de consumo energético

---

## 📞 Contacto y Soporte

**Instructor**: Ing. Fernando Raúl Vera Suasnávar  
**Institución**: Universidad Nacional de Santiago del Estero  
**Curso**: Internet de las Cosas con ESP32

**GitHub**: [fernandorvs/Curso-IoT-ESP32](https://github.com/fernandorvs/Curso-IoT-ESP32)

---

# ¡Gracias!

## ¿Preguntas?

**Recuerden:**
- El **modelo TCP/IP** es el stack real de Internet y de IoT
- **4 capas simples** vs 7 capas de OSI (más práctico)
- Cada ejemplo construye sobre el anterior agregando capas
- La práctica es fundamental para dominar IoT
- Experimenten y no tengan miedo de romper cosas

### Analogía del Modelo TCP/IP:
Enviar un paquete físico es como enviar datos:

1. **Acceso a Red (WiFi)**: El camión que transporta
2. **Internet (IP)**: La dirección en el paquete
3. **Transporte (TCP)**: El servicio de correo que garantiza entrega
4. **Aplicación (HTTP)**: El contenido del paquete (carta, regalo)

**¡Manos a la obra con los ejemplos!** 🚀
