# Servidor Básico - HTTP Server

## 📖 Descripción

Servidor HTTP básico que sirve página web desde ESP32. Muestra información del dispositivo y contador de visitas. Fundamento para crear interfaces web e APIs REST.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| - | - | Solo WiFi interno |

---

## 💡 Características

- Servidor web HTTP en puerto 80
- Página HTML dinámica con datos del ESP32
- Contador de visitas
- Endpoints: / (root) y /about

---

## 🚀 Uso

1. Modificar SSID y password en código
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor para ver IP asignada
4. Abrir navegador en http://[IP-ESP32]

---

## � Diagrama de Flujo

![Diagrama Servidor Básico](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%204/Diagramas/servidor_basico.pu)

---

## �📚 Clase

**Módulo:** Clase 4 - Conectividad y Protocolos  
Ver `src/main.cpp` para detalles de implementación
