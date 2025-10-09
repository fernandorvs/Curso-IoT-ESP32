/*
    ─────────────────────────────────────────────────────────────────────
    
    CURSO: Internet de las Cosas con ESP32
    INSTITUCIÓN: Universidad Nacional de Santiago del Estero (UNSE)
    
    ─────────────────────────────────────────────────────────────────────
    
    MÓDULO:      Clase 3 - Sensores y Periféricos
    PROYECTO:    Pantalla OLED - Display Gráfico
    HARDWARE:    Familia ESP32 (todos los modelos)
    FRAMEWORK:   Arduino / PlatformIO
    
    DESCRIPCIÓN:
    Control básico de pantalla OLED SSD1306 128x64 mediante I2C.
    Muestra texto y gráficos usando librería U8G2. Base para crear
    interfaces de usuario e instrumentos de medición.
    
    ─────────────────────────────────────────────────────────────────────
*/

#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>

// Constructor para OLED I2C 128x64
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
    // ESP32-C3 SDA = GPIO20, SCL = GPIO21
    Wire.begin(20, 21);
    u8g2.begin(); 
}

void loop() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB14_tf);
    u8g2.drawStr(35, 35, "UNSE");
    u8g2.sendBuffer();
    delay(500);
}

/*
===============================================================================
                    PANTALLAS OLED - TEORÍA Y REFERENCIA
===============================================================================

--- FUNDAMENTOS DE TECNOLOGÍA OLED ---

¿QUÉ ES OLED?
OLED (Organic Light-Emitting Diode): Tecnología donde cada píxel emite luz 
propia mediante compuestos orgánicos que se iluminan con corriente eléctrica.
No requiere retroiluminación como LCD.

VENTAJAS: Alto contraste, ángulos de visión amplios (160°+), respuesta rápida 
(<1ms), bajo consumo con fondos oscuros, diseño delgado y flexible.

DESVENTAJAS: Degradación orgánica (burn-in), sensible a humedad, costo mayor,
menor brillo máximo que LCD, degradación desigual de colores.

CONTROLADOR SSD1306 (128x64):
- Memoria: 1KB GDDRAM organizada en 8 páginas de 8 filas
- Voltaje: 1.65-3.3V operación, 3.3-5V lógico
- Consumo: ~20mA (todos píxeles ON)
- Interfaces: I2C (0x3C/0x3D, 100-400kHz) o SPI (más rápido)

CONEXIÓN I2C ESP32-C3:
  GPIO20 → SDA  |  GPIO21 → SCL  |  3.3V → VCC  |  GND → GND

--- LIBRERÍA U8G2 ---

MODOS DE MEMORIA:
_F  (Full Buffer): 1KB RAM, gráficos complejos, más memoria
_1  (Page Buffer): 128B RAM, menos memoria, gráficos limitados
U8X8: Solo texto, mínima RAM, sin gráficos

PROCESO DE RENDERIZADO:
1. clearBuffer() → Limpia RAM
2. Funciones dibujo → Modifican buffer
3. sendBuffer() → Transfiere a display

--- API BÁSICA ---

INICIALIZACIÓN:
  u8g2.begin()                    // Iniciar pantalla
  u8g2.enableUTF8Print()          // Caracteres especiales
  u8g2.clearBuffer()              // Limpiar buffer RAM
  u8g2.sendBuffer()               // Enviar a pantalla

TEXTO:
  u8g2.setFont(fuente)            // Cambiar fuente
  u8g2.drawStr(x, y, "texto")     // Dibujar en (x,y)
  u8g2.setCursor(x, y)            // Posicionar cursor
  u8g2.print("texto")             // Imprimir en cursor
  u8g2.getStrWidth("texto")       // Ancho en píxeles

ALINEACIÓN:
  Derecha:  x = 128 - u8g2.getStrWidth("texto");
  Centro:   x = (128 - u8g2.getStrWidth("texto")) / 2;

GRÁFICOS:
  u8g2.drawPixel(x, y)                   // Píxel
  u8g2.drawLine(x1, y1, x2, y2)          // Línea
  u8g2.drawHLine(x, y, w)                // Línea horizontal
  u8g2.drawVLine(x, y, h)                // Línea vertical
  u8g2.drawFrame(x, y, w, h)             // Marco vacío
  u8g2.drawBox(x, y, w, h)               // Caja rellena
  u8g2.drawRFrame(x, y, w, h, r)         // Marco redondeado
  u8g2.drawRBox(x, y, w, h, r)           // Caja redondeada
  u8g2.drawCircle(x, y, r)               // Círculo vacío
  u8g2.drawDisc(x, y, r)                 // Círculo relleno

COLOR (monocromático):
  u8g2.setDrawColor(1)            // Blanco (ON)
  u8g2.setDrawColor(0)            // Negro (OFF)
  u8g2.setDrawColor(2)            // Invertir (XOR)

--- FUENTES ÚTILES ---

PEQUEÑAS (6-8px):
  u8g2_font_4x6_tf    u8g2_font_5x7_tf    u8g2_font_6x10_tf
  u8g2_font_7x13_tf   u8g2_font_helvB08_tf

MEDIANAS (10-12px):
  u8g2_font_8x13_tf   u8g2_font_9x15_tf   u8g2_font_10x20_tf
  u8g2_font_helvB10_tf

NÚMEROS:
  u8g2_font_10x20_mn  (solo números, monospace)
  u8g2_font_10x20_mr  (números + símbolos)

SUFIJOS:
  _tf = Transparente, todos caracteres
  _tr = Transparente, ASCII reducido
  _tn = Transparente, solo números
  _mn = Monospace, solo números

--- EJEMPLOS PRÁCTICOS ---

MENÚ:
  u8g2.setFont(u8g2_font_7x13_tf);
  u8g2.drawStr(0, 15, "> Opcion 1");
  u8g2.drawStr(0, 30, "  Opcion 2");
  u8g2.drawStr(0, 45, "  Opcion 3");

BARRA DE PROGRESO:
  u8g2.drawFrame(10, 20, 100, 10);
  u8g2.drawBox(12, 22, progreso, 6);

SENSOR CON ALINEACIÓN:
  u8g2.setFont(u8g2_font_7x13_tf);
  u8g2.drawStr(0, 15, "Temp:");
  String temp = String(25.3, 1) + " C";
  int x = 128 - u8g2.getStrWidth(temp.c_str());
  u8g2.drawStr(x, 15, temp.c_str());

--- SISTEMA DE COORDENADAS ---

Origen (0,0) = Esquina superior izquierda
X: 0→127 (derecha)  |  Y: 0→63 (abajo)
IMPORTANTE: Y para texto = línea base (no parte superior)
Pantalla 128x64: ~8-10 líneas texto, ~16-20 caracteres/línea

--- MEJORES PRÁCTICAS ---

✓ Usar clearBuffer() al inicio del loop
✓ Usar sendBuffer() al final del loop
✓ Fondos oscuros = menor consumo
✓ Evitar imágenes estáticas (prevenir burn-in)
✓ Minimizar llamadas a sendBuffer() (optimización)
✓ Usar getStrWidth() para alineación precisa
✓ Fuentes _mn para displays numéricos

TROUBLESHOOTING:
- No enciende: Verificar VCC, GND, conexiones I2C
- Texto cortado: Revisar coordenadas y tamaño fuente
- Parpadeo: Verificar clearBuffer()/sendBuffer()
- Lentitud: Optimizar frecuencia I2C o usar SPI

===============================================================================
*/

/*

// ===============================================================================
// EJEMPLO COMPLETO

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Constructor para OLED I2C 128x64
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
    Wire.begin(20, 21); // ESP32-C3 SDA = GPIO20, SCL = GPIO21
    u8g2.begin();
    u8g2.enableUTF8Print(); // Soporte para caracteres especiales
}

void loop() {
    u8g2.clearBuffer();

    // Fuente mediana (8 píxeles altura) - IDEAL para tu caso
    u8g2.setFont(u8g2_font_7x13_tf);

    // Labels alineadas a la izquierda
    u8g2.drawStr(0, 15, "NTC:");
    u8g2.drawStr(0, 30, "DS18B20:");

    // Temperaturas alineadas a la derecha
    String temp1 = "25.3 C";
    String temp2 = "25.1 C";

    // Calcular posición X para alineación a la derecha
    int x1 = 128 - u8g2.getStrWidth(temp1.c_str());
    int x2 = 128 - u8g2.getStrWidth(temp2.c_str());

    u8g2.drawStr(x1, 15, temp1.c_str());
    u8g2.drawStr(x2, 30, temp2.c_str());

    // String centrada "UNSE" al final
    u8g2.setFont(u8g2_font_10x20_te);
    String unse = "UNSE";
    int xUnse = (128 - u8g2.getStrWidth(unse.c_str())) / 2;
    u8g2.drawStr(xUnse, 64, unse.c_str());

    u8g2.sendBuffer();
    delay(500);
}

*/