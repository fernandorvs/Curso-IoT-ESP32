# Lectura de Sensores - Monitoreo Web de Temperatura

## 📖 Descripción

Sistema web que muestra lecturas de sensores de temperatura (NTC y DS18B20) a través de peticiones GET. Interface HTML simple para consultar datos en tiempo real desde navegador.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| LED | GPIO 8 (ESP32-C3) / GPIO 2 (otros) | LED integrado |
| NTC 10kΩ | GPIO 0 (ESP32-C3) / GPIO 34 (otros) | Con resistencia 10kΩ |
| DS18B20 | GPIO 3 (ESP32-C3) / GPIO 4 (otros) | Protocolo 1-Wire |

---

## 💡 Características

- Doble sensor de temperatura (NTC + DS18B20)
- API REST con endpoints /ntc y /ds18b20
- Página web para visualizar datos
- Control de LED remoto incluido

---

## 🚀 Uso

1. Conectar hardware según tabla
2. Modificar SSID y password en código
3. Compilar y subir con PlatformIO
4. Abrir navegador en http://[IP-ESP32]
5. Consultar temperaturas en tiempo real

---

## 📚 Clase

**Módulo:** Clase 4 - Conectividad y Protocolos  
Ver `src/main.cpp` para detalles de implementación
