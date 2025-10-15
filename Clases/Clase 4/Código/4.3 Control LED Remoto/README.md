# Control LED Remoto - Ejemplo Simple y Didáctico

## 📖 Descripción

Ejemplo SIMPLE de control remoto de LED vía web usando GET y POST. Interface minimalista con botones para encender/apagar LED y consultar su estado desde navegador.

---

## 🔧 Hardware

| Componente | Pin | Notas |
|------------|-----|-------|
| LED | GPIO 8 (ESP32-C3) / GPIO 2 (otros) | LED integrado |

---

## 💡 Características

- Control de LED por HTTP (GET/POST)
- Endpoints: /, /estado, /on, /off
- Interface web con botones
- Respuestas en texto plano y HTML

---

## 🚀 Uso

1. Modificar SSID y password en código
2. Compilar y subir con PlatformIO
3. Abrir Serial Monitor para ver IP asignada
4. Abrir navegador en http://[IP-ESP32]
5. Usar botones para controlar LED

---

## � Diagrama de Flujo

![Diagrama Control LED](https://www.plantuml.com/plantuml/proxy?src=https://raw.githubusercontent.com/fernandorvs/Curso-IoT-ESP32/main/Clases/Clase%204/Diagramas/control_led_remoto.pu)

---

## �📚 Clase

**Módulo:** Clase 4 - Conectividad y Protocolos  
Ver `src/main.cpp` para detalles de implementación

- **Módulo:** Clase 4 - Conectividad y Protocolos
- **Documentación ESP32:** [Espressif Documentation](https://docs.espressif.com/)
- **PlatformIO:** [platformio.org](https://platformio.org/)
- Ver comentarios detallados en `src/main.cpp` para información técnica completa
