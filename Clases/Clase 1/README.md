# Clase 1 - Introducción al IoT y ESP32

## 📚 Contenido de la Clase

Esta es la primera clase del curso **Internet de las Cosas con ESP32** de la Universidad Nacional de Santiago del Estero, impartido por el Ing. Fernando Raúl Vera Suasnávar.

## 🎯 Objetivos de Aprendizaje

Al finalizar esta clase, los estudiantes serán capaces de:
- ✅ Entender qué es el Internet de las Cosas (IoT) y su importancia
- ✅ Conocer la historia y evolución de los microcontroladores
- ✅ Comprender las características especiales del ESP32 para IoT
- ✅ Configurar un entorno de desarrollo (PlatformIO + VS Code)
- ✅ Entender la estructura básica de un programa Arduino (setup/loop)
- ✅ Configurar y controlar pines GPIO digitales
- ✅ Crear su primer programa "Blink" en ESP32
- ✅ Subir código al ESP32 y verificar su funcionamiento

## 📋 Tabla de Contenidos

1. [Teoría Cubierta](#-teoría-cubierta)
2. [Código de Ejemplo](#-código-de-ejemplo)
   - [1.1 - Blink Básico](#11---blink-básico)
3. [Herramientas de Desarrollo](#-herramientas-de-desarrollo)
4. [Cómo Ejecutar el Proyecto](#-cómo-ejecutar-el-proyecto)
5. [Conceptos Clave Aprendidos](#-conceptos-clave-aprendidos)
6. [Ejercicios Propuestos](#-ejercicios-propuestos)
7. [Documentación de Referencia](#-documentación-de-referencia)
8. [Próximos Pasos](#-próximos-pasos)

## 📖 Teoría Cubierta

### 1. Introducción al Internet de las Cosas
- Definición de IoT según Kevin Ashton (1999)
- Componentes de un sistema IoT:
  - Sensores y Actuadores
  - Microcontrolador (MCU)
  - Conectividad
  - Software y Lógica
  - Interfaz de Usuario
- Ejemplos prácticos en diferentes sectores
- Arquitectura en capas del IoT

### 2. Historia de los Microcontroladores
- **1971**: Intel 4004 - Primer microprocesador
- **1974**: TMS1000 - Primer microcontrolador comercial
- **1980**: Intel 8051 - El legendario estándar
- **1996**: Atmel AVR - Base para Arduino futuro
- **2005**: Arduino - Democratización total
- **2016**: ESP32 - Revolución del IoT conectado

### 3. El ESP32: Características Clave
- **Procesador**: Dual-core Xtensa LX6 hasta 240 MHz
- **Conectividad**: WiFi 802.11 b/g/n + Bluetooth 4.2/BLE
- **Memoria**: 520 KB SRAM, 4 MB Flash típica
- **GPIO**: 34 pines configurables
- **Periféricos**: ADC, PWM, UART, SPI, I2C, etc.
- **Consumo**: Desde 5 µA en deep sleep

### 4. Pines GPIO y Consideraciones
- Nivel lógico: 3.3V (⚠️ NO tolera 5V)
- Corriente máxima: 12 mA recomendado por pin
- Pines restringidos y sus limitaciones
- Pines seguros para proyectos

### 5. Entorno de Desarrollo
- **PlatformIO**: Plataforma profesional de desarrollo embebido
- **VS Code**: Editor de código moderno y extensible
- **Arduino Framework**: Framework simplificado para ESP32
- **Gestión de dependencias**: Automática con PlatformIO
- **Debugging**: Capacidades avanzadas integradas

## 💻 Código de Ejemplo

### 1.1 - Blink Básico

**Ubicación**: `Código/1.1 Blink/`  
**📄 [Ver README completo](Código/1.1%20Blink/README.md)**

**Descripción**: El programa "Hello World" de los microcontroladores - hace parpadear el LED integrado del ESP32. Es el proyecto ideal para verificar que todo el entorno de desarrollo funciona correctamente.

**Características**:
- Uso del LED integrado en el pin 2
- Control digital básico (HIGH/LOW)
- Temporización con `delay()` (bloqueante)
- Estructura básica Arduino: `setup()` y `loop()`

**Código principal** (`src/main.cpp`):
```cpp
#include <Arduino.h>

#define PIN_LED 2

void setup()
{
    pinMode(PIN_LED, OUTPUT);
}

void loop()
{
    digitalWrite(PIN_LED, LOW);   // Apagar LED
    delay(1000);                  // Esperar 1 segundo
    digitalWrite(PIN_LED, HIGH);  // Encender LED
    delay(1000);                  // Esperar 1 segundo
}
```

**Configuración** (`platformio.ini`):
```ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

**Aprenderás**:
- ✅ Estructura básica de un programa Arduino
- ✅ Configuración de pines GPIO como salidas
- ✅ Control digital de pines (HIGH/LOW)
- ✅ Temporización básica con `delay()`
- ✅ Diferencia entre `setup()` y `loop()`
- ✅ Uso de constantes con `#define`
- ✅ Compilación y carga de programas al ESP32

**⚠️ Nota importante**: Este código usa `delay()` que es **bloqueante**. En la Clase 2 aprenderás técnicas no bloqueantes más profesionales usando `millis()`.

## 🔧 Herramientas de Desarrollo

### Opción Recomendada: PlatformIO + VS Code
**Ventajas:**
- IDE moderno y potente
- Gestión avanzada de librerías
- Debugging integrado
- Autocompletado inteligente
- Terminal integrada

### Alternativa: Arduino IDE
**Ventajas:**
- Instalación más simple
- Interfaz familiar
- Ideal para principiantes

## 🚀 Cómo Ejecutar el Proyecto

### Requisitos Previos
- **Hardware**: Placa ESP32 + cable USB
- **Software**: VS Code + PlatformIO instalados
- **Drivers**: USB-Serial (CP2102 o CH340, si es necesario)

### Pasos Detallados

1. **Abrir VS Code** con PlatformIO instalado
2. **Abrir el proyecto**: `File → Open Folder → "1.1 Blink"`
3. **Conectar tu ESP32** via USB
4. **Compilar**: Ctrl+Alt+B (o clic en ✓ en la barra inferior)
5. **Subir**: Ctrl+Alt+U (o clic en → en la barra inferior)
6. **Observar**: El LED integrado debe parpadear cada segundo

### Verificación del Funcionamiento
- ✅ El LED debe encenderse por 1 segundo
- ✅ Luego apagarse por 1 segundo
- ✅ El ciclo se repite indefinidamente

### Solución de Problemas Comunes

| Problema | Solución |
|----------|----------|
| **No detecta el ESP32** | Instalar drivers USB-Serial (CP2102 o CH340) |
| **Error al subir** | Mantener BOOT presionado, pulsar RESET, soltar ambos |
| **Puerto COM incorrecto** | Verificar en Administrador de Dispositivos (Windows) |
| **LED no parpadea** | Verificar que el código se subió correctamente |
| **Error de compilación** | Verificar que platformio.ini esté configurado correctamente |

### Comandos de PlatformIO

```bash
# Compilar el proyecto
pio run

# Subir al ESP32
pio run --target upload

# Abrir Serial Monitor
pio device monitor

# Limpiar proyecto
pio run --target clean
```

## 📋 Ejercicios Propuestos

### Ejercicio 1: Cambiar Velocidad de Parpadeo ⭐
**Dificultad**: Básica

Modificar el código para que el LED parpadee más rápido (cada 500ms).

```cpp
delay(500);  // Cambiar de 1000 a 500
```

**Objetivo**: Entender cómo `delay()` controla el tiempo.

---

### Ejercicio 2: Patrón de Parpadeo Personalizado ⭐⭐
**Dificultad**: Intermedia

Crear un patrón donde el LED se encienda por 2 segundos y se apague por 1 segundo.

```cpp
digitalWrite(PIN_LED, HIGH);
delay(2000);  // Encendido 2 segundos
digitalWrite(PIN_LED, LOW);
delay(1000);  // Apagado 1 segundo
```

**Objetivo**: Crear patrones temporales personalizados.

---

### Ejercicio 3: SOS en Código Morse ⭐⭐⭐
**Dificultad**: Intermedia-Avanzada

Implementar el patrón SOS en código Morse:
- **S**: 3 destellos cortos (· · ·)
- **O**: 3 destellos largos (— — —)
- **S**: 3 destellos cortos (· · ·)

```cpp
// Punto: 200ms encendido, 200ms apagado
// Raya: 600ms encendido, 200ms apagado
// Entre letras: 600ms de pausa
```

**Objetivo**: Trabajar con secuencias temporales complejas.

---

### Ejercicio 4: LED Externo ⭐⭐
**Dificultad**: Intermedia

Conectar un LED externo al pin GPIO 25 con una resistencia de 220Ω.

**Hardware necesario**:
- LED (cualquier color)
- Resistencia 220Ω
- Cables dupont
- Protoboard

**Conexión**:
```
ESP32 GPIO 25 → Resistencia 220Ω → LED (ánodo +) → LED (cátodo -) → GND
```

**Código**:
```cpp
#define PIN_LED 25  // Cambiar de 2 a 25
```

**Objetivo**: Aprender a conectar componentes externos.

---

### Ejercicio 5: Comunicación Serial ⭐⭐⭐
**Dificultad**: Avanzada

Agregar mensajes por Serial Monitor que indiquen el estado del LED.

```cpp
void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);
    Serial.println("=== Sistema iniciado ===");
}

void loop() {
    Serial.println("LED: ON");
    digitalWrite(PIN_LED, HIGH);
    delay(1000);
    
    Serial.println("LED: OFF");
    digitalWrite(PIN_LED, LOW);
    delay(1000);
}
```

**Objetivo**: Usar Serial Monitor para debugging.

---

### Ejercicio 6: Múltiples LEDs ⭐⭐⭐⭐
**Dificultad**: Avanzada

Controlar 3 LEDs que parpadean en secuencia (efecto de "ola").

**Hardware**: 3 LEDs + 3 resistencias 220Ω

**Código**:
```cpp
#define LED1 25
#define LED2 26
#define LED3 27

void setup() {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
}

void loop() {
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    
    digitalWrite(LED2, HIGH);
    delay(200);
    digitalWrite(LED2, LOW);
    
    digitalWrite(LED3, HIGH);
    delay(200);
    digitalWrite(LED3, LOW);
}
```

**Objetivo**: Coordinar múltiples salidas digitales.

---

### 🎯 Desafío Extra: Semáforo ⭐⭐⭐⭐⭐

Implementar un semáforo completo con 3 LEDs (rojo, amarillo, verde).

**Secuencia**:
1. Verde: 5 segundos
2. Amarillo: 2 segundos  
3. Rojo: 5 segundos
4. Repetir

**Objetivo**: Crear un sistema con lógica de estados básica.

## 🔍 Conceptos Clave Aprendidos

### Programación Básica de Microcontroladores

| Concepto | Explicación | Ejemplo |
|----------|-------------|---------|
| **GPIO** | General Purpose Input/Output - Pines configurables | Pin 2 del ESP32 |
| **pinMode()** | Configura un pin como entrada o salida | `pinMode(2, OUTPUT)` |
| **digitalWrite()** | Escribe valores digitales (HIGH/LOW) | `digitalWrite(2, HIGH)` |
| **delay()** | Pausa la ejecución por X milisegundos | `delay(1000)` |
| **setup()** | Función que se ejecuta una vez al inicio | Configuración inicial |
| **loop()** | Función que se ejecuta continuamente | Lógica principal |
| **#define** | Define una constante en tiempo de compilación | `#define PIN_LED 2` |

### Estructura de un Programa Arduino

```cpp
#include <Arduino.h>           // Librería principal

#define PIN_LED 2              // Constantes

void setup() {                 // Configuración inicial (1 vez)
    pinMode(PIN_LED, OUTPUT);  // Configurar pines
    Serial.begin(115200);      // Inicializar periféricos
}

void loop() {                  // Bucle principal (infinito)
    // Código que se repite continuamente
}
```

### Niveles Lógicos en ESP32

| Estado | Voltaje | Valor en código |
|--------|---------|-----------------|
| **LOW** | 0V | `LOW` o `0` |
| **HIGH** | 3.3V | `HIGH` o `1` |

⚠️ **Importante**: ESP32 trabaja a 3.3V, NO conectar señales de 5V directamente.

### Funciones de Tiempo

| Función | Tipo | Uso |
|---------|------|-----|
| `delay(ms)` | Bloqueante | Pausa el programa completamente |
| `millis()` | No bloqueante | Devuelve tiempo desde inicio (Clase 2) |

### Pines Importantes del ESP32

| Pin | Función | Notas |
|-----|---------|-------|
| **GPIO 2** | LED integrado | Pin seguro para proyectos |
| **GPIO 0** | Botón BOOT | También usado para programación |
| **GPIO 1/3** | UART TX/RX | Evitar si se usa Serial |
| **GPIO 6-11** | Flash SPI | ⚠️ NO USAR |
| **GPIO 34-39** | Solo entrada | Sin pull-up interno |

## 📚 Documentación de Referencia

### Documentación de la Clase
- **[Manual completo de la Clase 1](Docs/Curso_IoT_ESP32_UNSE_Clase_1.pdf)** - Documento oficial del curso

### README del Proyecto
- **[1.1 Blink - README completo](Código/1.1%20Blink/README.md)** - Guía detallada del proyecto Blink

### Referencias Técnicas
- [Documentación Oficial ESP32](https://docs.espressif.com/) - Referencia completa del hardware
- [PlatformIO Documentation](https://docs.platformio.org/) - Guía de la plataforma de desarrollo
- [Arduino Language Reference](https://www.arduino.cc/reference/) - Referencia del lenguaje Arduino
- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/) - Diagrama de pines

### Recursos Adicionales
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) - Especificaciones técnicas completas
- [Arduino Core for ESP32](https://github.com/espressif/arduino-esp32) - Repositorio oficial en GitHub

## 🎓 Próximos Pasos

### 🚀 En la Clase 2 aprenderás:

**Control Avanzado:**
- ✨ **PWM (Modulación por Ancho de Pulso)**: Control analógico del brillo de LEDs
- 🎨 **Corrección Gamma**: Para transiciones visuales más naturales

**Lectura de Entradas:**
- 🔘 **Pulsadores con Antirrebote**: Lectura confiable de botones
- 📊 **Detección de Flancos**: Identificar cambios de estado

**Programación Profesional:**
- ⏱️ **Código No Bloqueante**: Usar `millis()` en lugar de `delay()`
- 🔄 **Multitarea Cooperativa**: Ejecutar múltiples tareas simultáneamente
- 🤖 **Máquinas de Estado (FSM)**: Arquitectura profesional de software embebido

### Progresión del Curso

```
Clase 1: Fundamentos            ← ESTÁS AQUÍ
    ↓
Clase 2: Control + No Bloqueante
    ↓
Clase 3: Sensores Analógicos
    ↓
Clase 4: WiFi + Servidor Web + IoT
```

### 🎯 Lo que ya dominas:
- ✅ Configuración del entorno de desarrollo
- ✅ Estructura básica de programas Arduino
- ✅ Control de pines GPIO digitales
- ✅ Compilación y carga de código al ESP32
- ✅ Uso de funciones básicas (setup, loop, delay)

### 🔜 Siguientes habilidades:
- Control analógico con PWM
- Programación no bloqueante con millis()
- Lectura confiable de sensores
- Arquitecturas de software escalables

## 🧑‍🏫 Instructor

**Ing. Fernando Raúl Vera Suasnávar**  
Universidad Nacional de Santiago del Estero  
Año 2025

---

## 💡 Consejos para Principiantes

### ✅ Buenas Prácticas desde el Inicio

**Organización del Código:**
- 📝 Usar nombres descriptivos para pines (`PIN_LED` mejor que `LED`)
- 💬 Comentar código complejo
- 📏 Mantener código indentado y legible
- 🔢 Usar `#define` para constantes

**Hardware:**
- ⚡ Siempre usar resistencias con LEDs externos (220Ω típico)
- 🔌 Desconectar USB antes de modificar circuitos
- 📐 Verificar voltajes (ESP32 es 3.3V, NO 5V)
- 🧪 Probar con LED integrado antes de conectar externos

**Debugging:**
- 🖥️ Usar `Serial.println()` para ver qué pasa
- ✔️ Verificar conexiones físicas si algo no funciona
- 🔄 Probar código simple primero, luego agregar complejidad
- 📖 Leer mensajes de error completos

### ⚠️ Errores Comunes a Evitar

| Error | Problema | Solución |
|-------|----------|----------|
| **Olvidar pinMode()** | Pin no configurado | Siempre configurar en setup() |
| **Conectar 5V al ESP32** | ⚡ Puede dañar el chip | ESP32 es 3.3V solamente |
| **LED sin resistencia** | LED se quema | Usar resistencia 220Ω mínimo |
| **Polaridad invertida LED** | LED no enciende | Pata larga (+) hacia pin, corta (-) a GND |
| **Puerto COM incorrecto** | No sube código | Verificar en Device Manager |

### 🎯 Consejos de Aprendizaje

1. **Experimentar**: Modifica el código y observa qué pasa
2. **Documentar**: Anota lo que funciona y lo que no
3. **Preguntar**: No hay preguntas tontas
4. **Practicar**: Hacer todos los ejercicios propuestos
5. **Compartir**: Enseñar a otros refuerza tu conocimiento

---

## 📊 Resumen de la Clase

### Conocimientos Teóricos
- ✅ Conceptos fundamentales de IoT
- ✅ Historia y evolución de microcontroladores
- ✅ Arquitectura del ESP32
- ✅ Características de conectividad WiFi/Bluetooth
- ✅ Especificaciones de pines GPIO

### Habilidades Prácticas
- ✅ Instalación y configuración de PlatformIO
- ✅ Creación de primer proyecto ESP32
- ✅ Compilación y carga de código
- ✅ Control de salidas digitales
- ✅ Uso de funciones de temporización

### Proyecto Completado
- ✅ **1.1 Blink**: LED parpadeante funcionando

### Tiempo Estimado de Clase
- 📖 Teoría: 60 minutos
- 💻 Práctica: 45 minutos
- 🔧 Setup y troubleshooting: 15 minutos
- **Total**: ~2 horas

---

*¡Felicitaciones por completar tu primera clase de IoT con ESP32! El viaje hacia el desarrollo de dispositivos conectados acaba de comenzar. 🚀*