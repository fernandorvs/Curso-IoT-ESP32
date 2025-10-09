# Clase 2 - Control Avanzado y Programación No Bloqueante

## 📚 Contenido de la Clase

Esta es la segunda clase del curso **Internet de las Cosas con ESP32** de la Universidad Nacional de Santiago del Estero. Se enfoca en técnicas fundamentales para crear sistemas embebidos más robustos y profesionales.

## 🎯 Objetivos de Aprendizaje

Al finalizar esta clase, los estudiantes serán capaces de:
- ✅ Controlar la intensidad de LEDs usando PWM (Modulación por Ancho de Pulso)
- ✅ Aplicar corrección gamma para transiciones visuales naturales
- ✅ Leer pulsadores con técnicas de antirrebote (debounce) confiables
- ✅ Implementar programación no bloqueante con `millis()`
- ✅ Combinar múltiples tareas en ejecución simultánea
- ✅ Diseñar máquinas de estado finito (FSM) para sistemas multitarea
- ✅ Escribir código escalable y profesional para sistemas embebidos

## 📋 Tabla de Contenidos

1. [Teoría Cubierta](#-teoría-cubierta)
2. [Proyectos Incluidos](#-proyectos-incluidos)
   - [2.1 - LED con PWM](#21---led-con-pwm)
   - [2.2 - Pulsador con Antirrebote](#22---pulsador-con-antirrebote)
   - [2.3 - Blink No Bloqueante](#23---blink-no-bloqueante)
   - [2.4 - Pulsador No Bloqueante](#24---pulsador-no-bloqueante)
   - [2.5 - Blink + Pulsador No Bloqueantes](#25---blink--pulsador-no-bloqueantes)
   - [2.6 - FSM Máquina de Estado](#26---fsm-máquina-de-estado)
3. [Conceptos Técnicos Importantes](#-conceptos-técnicos-importantes)
5. [Cómo Ejecutar los Proyectos](#-cómo-ejecutar-los-proyectos)
6. [Progresión del Aprendizaje](#-progresión-del-aprendizaje)
7. [Ejercicios Propuestos](#-ejercicios-propuestos)
8. [Documentación de Referencia](#-documentación-de-referencia)

## 📖 Teoría Cubierta

### 1. Control de LED con PWM
- **Concepto de PWM**: Simulación de señales analógicas con pulsos digitales
- **Duty Cycle**: Porcentaje de tiempo en HIGH (0-100%)
- **Frecuencia**: Ciclos por segundo para control suave
- **Hardware ESP32**: 16 canales PWM independientes, hasta 20 bits de resolución
- **Corrección Gamma**: Para transiciones visuales más naturales

### 2. Lectura de Pulsadores con Antirrebote
- **Problema del rebote**: Múltiples lecturas por una sola pulsación
- **Antirrebote por software**: Técnica de temporización para filtrar ruido
- **INPUT_PULLUP**: Resistencias pull-up internas del ESP32
- **Detección de flancos**: Identificar cambios de estado confiables

### 3. Programación No Bloqueante
- **Problema de delay()**: Bloquea toda la ejecución del programa
- **función millis()**: Contador de tiempo desde el arranque
- **Técnica de temporización**: Comparar tiempos para ejecutar acciones
- **Multitarea cooperativa**: Múltiples procesos en un solo hilo

### 4. Máquinas de Estado Finito (FSM)
- **Concepto de estado**: Situación actual del sistema
- **Transiciones**: Cambios entre estados por eventos
- **Estructura switch-case**: Implementación práctica en C++
- **Flags de eventos**: Comunicación entre procesos

## 💻 Proyectos Incluidos

### 2.1 - LED con PWM
**Ubicación**: `Código/2.1 Led con PWM/`  
**📄 [Ver README completo](Código/2.1%20Led%20con%20PWM/README.md)**

**Descripción**: Control de intensidad de LED con efecto fade in/out suave usando corrección gamma.

**Características**:
- PWM a 5 kHz con resolución de 8 bits
- Efecto de respiración automático
- Corrección gamma para transiciones naturales
- Explicación detallada de duty cycle y frecuencia

```cpp
// Configuración PWM
ledcSetup(0, 5000, 8);        // Canal 0, 5kHz, 8 bits
ledcAttachPin(PIN_LED, 0);    // GPIO2 al canal 0
ledcWrite(0, dutyCycle);      // Escribir valor PWM
```

**Aprenderás**: Control analógico, PWM, corrección gamma, hardware LEDC del ESP32

---

### 2.2 - Pulsador con Antirrebote
**Ubicación**: `Código/2.2 Pulsador con Antirrebote/`  
**📄 [Ver README completo](Código/2.2%20Pulsador%20con%20Antirrebote/README.md)**

**Descripción**: Lectura confiable de botón que invierte el estado del LED con cada pulsación usando antirrebote **bloqueante**.

**Características**:
- Antirrebote por delay de 50ms
- Detección de flanco de bajada
- Espera activa hasta soltar el botón
- INPUT_PULLUP para botón BOOT integrado

```cpp
// Antirrebote básico bloqueante
if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50);                           // Espera estabilización
    if (digitalRead(BUTTON_PIN) == LOW) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        while (digitalRead(BUTTON_PIN) == LOW); // Espera soltar
    }
}
```

**Aprenderás**: Rebote mecánico, pull-up interno, antirrebote por software, limitaciones del código bloqueante

---

### 2.3 - Blink No Bloqueante
**Ubicación**: `Código/2.3 Blink no bloqueante/`  
**📄 [Ver README completo](Código/2.3%20Blink%20no%20bloqueante/README.md)**

**Descripción**: LED parpadeante que no bloquea el programa principal usando `millis()`.

**Características**:
- Usa `millis()` en lugar de `delay()`
- Permite ejecutar otras tareas simultáneamente
- Control de tiempo preciso sin bloqueos
- Manejo correcto de overflow de millis()

```cpp
// Técnica no bloqueante
unsigned long ahora = millis();
if (ahora - lastBlink >= BLINK_MS) {
    lastBlink = ahora;
    ledState = !ledState;
    digitalWrite(PIN_LED, ledState);
}
// Continúa inmediatamente sin esperar
```

**Aprenderás**: Programación no bloqueante, función millis(), marcas de tiempo, intervalos temporales

---

### 2.4 - Pulsador No Bloqueante
**Ubicación**: `Código/2.4 Pulsador no bloqueante/`  
**📄 [Ver README completo](Código/2.4%20Pulsador%20no%20bloqueante/README.md)**

**Descripción**: Lectura de botón con antirrebote **no bloqueante** usando `millis()`.

**Características**:
- Antirrebote temporal con `millis()`
- No bloquea otras funciones
- Detección confiable de cambios de estado
- Reporta "PRESIONADO" y "SUELTO" por Serial

```cpp
// Antirrebote no bloqueante
int r = digitalRead(BUTTON_PIN);
if (r != stable && millis() - lastDebounce > 50) {
    lastDebounce = millis();
    stable = r;
    Serial.println(stable == LOW ? "PRESIONADO" : "SUELTO");
}
```

**Aprenderás**: Antirrebote no bloqueante, detección de cambios de estado, operador ternario, patrón reutilizable

---

### 2.5 - Blink + Pulsador No Bloqueantes
**Ubicación**: `Código/2.5 Blink + Pulsador no bloqueantes/`  
**📄 [Ver README completo](Código/2.5%20Blink%20+%20Pulsador%20no%20bloqueantes/README.md)**

**Descripción**: Combinación de LED parpadeante y lectura de botón ejecutándose **simultáneamente e independientemente**.

**Características**:
- Dos tareas no bloqueantes en paralelo
- LED parpadea cada 500ms
- Botón detecta pulsaciones en tiempo real
- Base para sistemas multitarea cooperativos

```cpp
void loop() {
    // Tarea 1: Blink no bloqueante
    if (millis() - lastBlink >= BLINK_MS) {
        lastBlink = millis();
        ledState = !ledState;
        digitalWrite(PIN_LED, ledState);
    }
    
    // Tarea 2: Pulsador no bloqueante
    int r = digitalRead(BUTTON_PIN);
    if (r != stable && millis() - lastDebounce > 50) {
        lastDebounce = millis();
        stable = r;
        Serial.println(stable == LOW ? "PRESIONADO" : "SUELTO");
    }
}
```

**Aprenderás**: Multitarea cooperativa, independencia de tareas, escalabilidad, diseño modular

---

### 2.6 - FSM Máquina de Estado
**Ubicación**: `Código/2.6 FSM Máquina de estado/`  
**📄 [Ver README completo](Código/2.6%20FSM%20Máquina%20de%20estado/README.md)**

**Descripción**: Sistema de control de LED con tres estados diferentes controlados por botón usando **Máquina de Estados Finitos (FSM)**.

**Estados del sistema**:
1. **STATE_OFF**: LED apagado
2. **STATE_ON**: LED encendido fijo
3. **STATE_BLINK**: LED parpadeando

**Transiciones**:
- OFF → ON → BLINK → OFF (cíclico con cada pulsación del botón)

```cpp
// Estructura FSM
switch (state) {
    case STATE_OFF:
        digitalWrite(PIN_LED, LOW);
        if (buttonPressed) {
            buttonPressed = 0;
            state = STATE_ON;
        }
        break;
    
    case STATE_ON:
        digitalWrite(PIN_LED, HIGH);
        if (buttonPressed) {
            buttonPressed = 0;
            state = STATE_BLINK;
        }
        break;
    
    case STATE_BLINK:
        // Lógica de parpadeo no bloqueante
        if (millis() - lastBlink >= BLINK_MS) {
            lastBlink = millis();
            ledState = !ledState;
            digitalWrite(PIN_LED, ledState);
        }
        if (buttonPressed) {
            buttonPressed = 0;
            state = STATE_OFF;
        }
        break;
}
```

**Aprenderás**: FSM, estados y transiciones, patrón flag + FSM, arquitectura profesional de software embebido

## 🔧 Conceptos Técnicos Importantes

### PWM en ESP32
- **Hardware LEDC**: Controlador dedicado para PWM
- **16 canales disponibles**: Permite controlar múltiples LEDs/motores
- **Frecuencias configurables**: 1Hz hasta >40MHz
- **Resolución variable**: 1-20 bits según frecuencia

### Función millis()
- **Contador de 32 bits**: Cuenta milisegundos desde el arranque
- **Desbordamiento**: Regresa a 0 después de ~49 días
- **Resolución**: 1 milisegundo
- **No bloqueante**: Continúa contando en background

### Técnicas de Temporización
```cpp
// Patrón básico no bloqueante
unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 1000;

if (millis() - tiempoAnterior >= intervalo) {
    tiempoAnterior = millis();
    // Ejecutar acción cada 1 segundo
}
```

## 🚀 Cómo Ejecutar los Proyectos

### Requisitos de Hardware
- Placa ESP32
- LED externo + resistencia 220Ω (opcional, usa el integrado)
- Botón pulsador + resistencia 10kΩ pull-up (opcional, usa GPIO0/BOOT)
- Protoboard y cables

### Configuración Básica
```cpp
// Configuración común en todos los proyectos
#define PIN_LED 2      // LED integrado
#define BUTTON_PIN 0   // Botón BOOT integrado
```

### Pasos para cada proyecto
1. **Abrir proyecto** en PlatformIO
2. **Conectar ESP32** via USB
3. **Compilar y subir** (Ctrl+Alt+U)
4. **Abrir Serial Monitor** para mensajes de debug
5. **Probar funcionalidad** según descripción del proyecto

## 🎓 Progresión del Aprendizaje

### Nivel Básico → Intermedio → Avanzado

1. **2.1 PWM**: Control analógico básico con corrección gamma
2. **2.2 Antirrebote**: Lectura confiable de entradas (bloqueante)
3. **2.3 Blink no bloqueante**: Introducción a programación sin delay()
4. **2.4 Pulsador no bloqueante**: Antirrebote sin bloquear el programa
5. **2.5 Multitarea**: Dos tareas independientes simultáneas
6. **2.6 FSM**: Arquitectura profesional con máquina de estados

### Conceptos que se construyen
- **Clase 1**: Básicos digitales (HIGH/LOW, delay())
- **Clase 2**: Control analógico (PWM) + programación no bloqueante + FSM
- **Próximas clases**: Sensores + comunicación + IoT

### Ruta de Aprendizaje Recomendada
```
Clase 1: Blink básico
    ↓
2.1: Controlar brillo (PWM)
    ↓
2.2: Leer botón confiable (antirrebote bloqueante)
    ↓
2.3: Parpadear sin bloquear (millis())
    ↓
2.4: Botón sin bloquear (antirrebote no bloqueante)
    ↓
2.5: Combinar tareas (multitarea cooperativa)
    ↓
2.6: Controlar con estados (FSM)
    ↓
Clase 3: Sensores analógicos
```

## 🔍 Ejercicios Propuestos

### Ejercicio 1: PWM Variable con Potenciómetro
Modificar el proyecto 2.1 para controlar la velocidad del fade con un potenciómetro conectado a un pin ADC. El potenciómetro determinará el valor de `delay()` entre incrementos.

### Ejercicio 2: Múltiples LEDs No Bloqueantes
Crear un programa con 3 LEDs que parpadeen a diferentes frecuencias (200ms, 500ms, 1000ms) usando el patrón no bloqueante del proyecto 2.3.

### Ejercicio 3: FSM Extendida
Expandir la FSM del proyecto 2.6 para incluir:
- Un cuarto estado: **STATE_FAST_BLINK** (parpadeo rápido a 100ms)
- Secuencia: OFF → ON → BLINK → FAST_BLINK → OFF

### Ejercicio 4: Control por Serial
Modificar el proyecto 2.6 para recibir comandos por Serial Monitor:
- '0' = STATE_OFF
- '1' = STATE_ON  
- '2' = STATE_BLINK

### Ejercicio 5: Botón con Pulsación Larga
Expandir el proyecto 2.4 para detectar:
- **Pulsación corta** (<1 segundo): imprime "CORTA"
- **Pulsación larga** (≥1 segundo): imprime "LARGA"

### Ejercicio 6: Sistema de Alarma Simple
Combinar conceptos de 2.5 y 2.6 para crear:
- Estado DESARMADO (LED apagado)
- Estado ARMADO (LED parpadea lento)
- Estado ALARMA (LED parpadea rápido + buzzer)
- Transiciones con botón

## 📚 Documentación de Referencia

### Documentación de la Clase
- **[Manual completo de la Clase 2](Docs/Curso_IoT_ESP32_UNSE_Clase_2.pdf)** - Documento oficial del curso

### README por Proyecto
- **[2.1 Led con PWM](Código/2.1%20Led%20con%20PWM/README.md)** - Control de brillo y corrección gamma
- **[2.2 Pulsador con Antirrebote](Código/2.2%20Pulsador%20con%20Antirrebote/README.md)** - Antirrebote bloqueante
- **[2.3 Blink no bloqueante](Código/2.3%20Blink%20no%20bloqueante/README.md)** - Introducción a millis()
- **[2.4 Pulsador no bloqueante](Código/2.4%20Pulsador%20no%20bloqueante/README.md)** - Antirrebote no bloqueante
- **[2.5 Blink + Pulsador no bloqueantes](Código/2.5%20Blink%20+%20Pulsador%20no%20bloqueantes/README.md)** - Multitarea
- **[2.6 FSM Máquina de estado](Código/2.6%20FSM%20Máquina%20de%20estado/README.md)** - Estados finitos

### Referencias Técnicas
- [ESP32 LEDC Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html) - Documentación oficial de PWM
- [Arduino millis() Reference](https://www.arduino.cc/reference/en/language/functions/time/millis/) - Función de temporización
- [Finite State Machines in Embedded Systems](https://barrgroup.com/embedded-systems/how-to/state-machines-event-driven-systems) - Teoría de FSM

## 🎯 Puntos Clave para Recordar

### ✅ Buenas Prácticas

**Programación No Bloqueante:**
- **Evitar delay()** en programas multitarea
- **Usar millis()** para medir tiempo sin bloquear
- **Implementar flags** para comunicación entre procesos
- **Cada tarea verifica su propio tiempo** independientemente

**PWM y Control Analógico:**
- **Usar const para frecuencias** y resoluciones
- **Aplicar corrección gamma** para transiciones visuales naturales
- **Documentar el canal PWM** utilizado

**Máquinas de Estado:**
- **Estructurar FSMs** con switch-case claros
- **Comentar transiciones** de estado y eventos
- **Definir estados con #define** o enum para legibilidad
- **Cada estado hace una cosa** específica

**Antirrebote:**
- **50ms es típico** para antirrebote de botones
- **Verificar dos veces** el estado del botón
- **Usar variables de estado estable** para comparación

### ⚠️ Errores Comunes

**Temporización:**
- ❌ Olvidar actualizar `lastTime = millis()` después de verificar el intervalo
- ❌ Usar `delay()` mezclado con código no bloqueante
- ❌ No manejar el overflow de millis() (aunque la resta funciona bien)
- ❌ Comparar `millis() == valor` en lugar de `millis() >= valor`

**Máquinas de Estado:**
- ❌ Olvidar resetear flags de eventos después de usarlos
- ❌ Estados sin salida en FSMs (loops infinitos sin transición)
- ❌ No incluir `break` al final de cada case
- ❌ Olvidar case `default` para estados no definidos

**Pulsadores:**
- ❌ Leer botones sin antirrebote (lecturas múltiples)
- ❌ No configurar pull-up/pull-down (lecturas flotantes)
- ❌ Procesar pulsación en cada loop (multiplicar eventos)

**PWM:**
- ❌ Usar el mismo canal PWM para múltiples pines
- ❌ Frecuencias muy altas con resolución muy alta (limitación hardware)
- ❌ No apagar PWM antes de reasignar el canal

## 🔄 Conexión con Otras Clases

### 📖 Desde Clase 1
Conceptos previos necesarios:
- ✅ Configuración de GPIO con `pinMode()`
- ✅ Estructura básica `setup()` / `loop()`
- ✅ Control digital con `digitalWrite()` y `digitalRead()`
- ✅ Uso básico de `delay()` (para luego evitarlo)
- ✅ Constantes con `#define`
- ✅ Serial Monitor básico

### 📖 Hacia Clase 3
Fundamentos que preparan para:
- 🔜 **Lectura de sensores analógicos** (ADC) de forma no bloqueante
- 🔜 **Filtros digitales** aplicados con temporización precisa
- 🔜 **Múltiples sensores** leyéndose simultáneamente
- 🔜 **Comunicación de datos** por Serial de forma eficiente
- 🔜 **Displays OLED** actualizándose sin bloquear sensores

### 📖 Hacia Clase 4
Conceptos que escalan a IoT:
- 🔜 **Servidor web** respondiendo peticiones sin bloquear lecturas de sensores
- 🔜 **WiFi conectado** mientras se mantiene control local
- 🔜 **APIs REST** con FSM para control de dispositivos
- 🔜 **Dashboard en tiempo real** con actualizaciones no bloqueantes

### 🎯 Habilidades Adquiridas
Después de esta clase podrás:
- ✅ Crear sistemas que hacen múltiples cosas al mismo tiempo
- ✅ Controlar actuadores con precisión (PWM)
- ✅ Leer entradas digitales de forma confiable (antirrebote)
- ✅ Diseñar arquitecturas de software escalables (FSM)
- ✅ Escribir código profesional no bloqueante

## 🧑‍🏫 Instructor

**Ing. Fernando Raúl Vera Suasnávar**  
Universidad Nacional de Santiago del Estero  
Año 2025

---

## 📊 Comparación Visual de Conceptos

### Bloqueante vs No Bloqueante

| Aspecto | Código Bloqueante | Código No Bloqueante |
|---------|-------------------|----------------------|
| **Función principal** | `delay()` | `millis()` |
| **Ejecución** | Secuencial (una cosa a la vez) | Cooperativa (múltiples tareas) |
| **CPU durante espera** | ❌ Bloqueado, no hace nada | ✅ Libre, puede hacer otras cosas |
| **Complejidad** | ⭐ Simple | ⭐⭐⭐ Más complejo |
| **Escalabilidad** | ❌ Difícil agregar tareas | ✅ Fácil agregar tareas |
| **Respuesta a eventos** | ❌ Retrasada | ✅ Inmediata |
| **Ejemplo** | Proyecto 2.2 | Proyectos 2.3 - 2.6 |

### Progresión de Complejidad

```
Complejidad Baja  ─────────────────────────────────────→  Complejidad Alta
    │                │              │             │             │
2.1 PWM          2.2 Botón      2.3 Blink     2.5 Multi    2.6 FSM
(Control         (Antirrebote   no-block      tarea        (Estados
analógico)       bloqueante)    (millis)      (Paralelo)   secuenciales)
                                                │
                                            2.4 Botón
                                            no-block
```

### Casos de Uso Recomendados

| Proyecto | Mejor para | No recomendado para |
|----------|-----------|---------------------|
| **2.1 PWM** | Control suave de brillo, motores, servos | Señales digitales simples ON/OFF |
| **2.2 Antirrebote bloqueante** | Prototipado rápido, proyectos simples | Sistemas que requieren multitarea |
| **2.3 Blink no bloqueante** | Base para cualquier proyecto con temporizadores | Proyectos donde delay() es aceptable |
| **2.4 Pulsador no bloqueante** | Interfaces de usuario, sistemas reactivos | - |
| **2.5 Multitarea** | Sistemas con sensores + actuadores + UI | Proyectos muy simples |
| **2.6 FSM** | Control de procesos, menús, protocolos | Lógica muy simple sin estados claros |

---

*¡Has dominado las técnicas fundamentales para sistemas embebidos profesionales! Ahora puedes crear dispositivos que hacen múltiples tareas simultáneamente de manera confiable.*