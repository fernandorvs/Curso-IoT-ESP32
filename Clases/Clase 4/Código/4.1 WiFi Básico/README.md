# WiFi Básico - Conexión a Red Inalámbrica

## 📖 Descripción

Conexión básica a red WiFi en modo estación (STA). Muestra información de red, monitorea calidad de señal (RSSI) y reconecta automáticamente si se pierde conexión. Base para todos los proyectos IoT con ESP32.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| - | - | Solo WiFi interno |

---

## 💡 Características

- Conexión WiFi en modo estación (STA)
- Monitoreo de señal RSSI cada 10s
- Reconexión automática si se desconecta
- Muestra IP, MAC, Gateway cada 30s

---

## 🚀 Uso

1. Modificar SSID y password en código
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor (115200 baudios)
4. Observar conexión y datos de red

---

## � Diagrama de Flujo

![Diagrama WiFi Básico](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%204/Diagramas/wifi_basico.pu)

---

## �📚 Clase

**Módulo:** Clase 4 - Conectividad y Protocolos  
Ver `src/main.cpp` para detalles de implementación
