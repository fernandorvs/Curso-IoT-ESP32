/*
   ESP32 IoT Dashboard
   Taller de Sistemas Microprogramables - UNSE
*/

// Variables para evitar parpadeos cuando el usuario usa los controles
let userInteracting = false;
let lastUserAction = 0;
let brightnessTimeout;

// Función para actualizar los datos desde el ESP32
function update() {
    
    // Obtener temperatura del ESP32
    fetch('/api/sensors').then(r => r.json()).then(d => {
        document.getElementById('temp').innerHTML = d.temperature.toFixed(1) + '°C';
        document.getElementById('time').innerHTML = new Date().toLocaleTimeString();
    }).catch(() => {
        document.getElementById('temp').innerHTML = '--°C';
    });
    
    // Solo actualizar LED si el usuario no lo está tocando (evita parpadeos)
    const timeSinceLastAction = Date.now() - lastUserAction;
    if (!userInteracting && timeSinceLastAction > 1000) {
        fetch('/api/led').then(r => r.json()).then(d => {
            const toggle = document.getElementById('toggleLed');
            const status = document.getElementById('ledStatus');
            const ledVisual = document.getElementById('ledVisual');
            
            // Actualizar switch solo si cambió
            if (toggle.checked !== d.state) {
                toggle.checked = d.state;
            }
            
            // Actualizar texto del estado
            status.innerHTML = d.state ? 'LED Encendido' : 'LED Apagado';
            
            // Actualizar la bolita LED visual
            if (d.state) {
                ledVisual.classList.add('on');
                const opacity = d.brightness / 100;
                ledVisual.style.opacity = 0.3 + (opacity * 0.7);
                
                const glowIntensity = d.brightness / 100;
                ledVisual.style.boxShadow = `
                    0 0 ${10 + glowIntensity * 10}px rgba(255,235,59,${0.3 + glowIntensity * 0.3}), 
                    0 0 ${20 + glowIntensity * 20}px rgba(255,235,59,${0.2 + glowIntensity * 0.2}),
                    0 0 ${30 + glowIntensity * 30}px rgba(255,235,59,${0.1 + glowIntensity * 0.1})
                `;
            } else {
                ledVisual.classList.remove('on');
                ledVisual.style.opacity = 1;
                ledVisual.style.boxShadow = '0 0 10px rgba(0,0,0,0.3)';
            }
            
            // Actualizar slider solo si cambió bastante
            const currentSliderValue = parseInt(document.getElementById('brightness').value);
            if (Math.abs(currentSliderValue - d.brightness) > 2) {
                document.getElementById('brightValue').innerHTML = d.brightness;
                document.getElementById('brightness').value = d.brightness;
            }
        }).catch(() => {
            document.getElementById('ledStatus').innerHTML = 'Error de conexión';
        });
    }
}

// Función para enviar comandos al ESP32
function sendCmd(action, value = null) {
    const fd = new FormData();
    fd.append('action', action);
    if (value !== null) fd.append('value', value);
    fetch('/api/led', {method: 'POST', body: fd})
        .then(() => setTimeout(update, 100))
        .catch(console.error);
}

// Función para actualizar la bolita LED cuando el usuario mueve el slider
function updateLedVisual(brightness) {
    const ledVisual = document.getElementById('ledVisual');
    const toggle = document.getElementById('toggleLed');
    
    if (toggle.checked) {
        const opacity = brightness / 100;
        ledVisual.style.opacity = 0.3 + (opacity * 0.7);
        
        const glowIntensity = brightness / 100;
        ledVisual.style.boxShadow = `
            0 0 ${10 + glowIntensity * 10}px rgba(255,235,59,${0.3 + glowIntensity * 0.3}), 
            0 0 ${20 + glowIntensity * 20}px rgba(255,235,59,${0.2 + glowIntensity * 0.2}),
            0 0 ${30 + glowIntensity * 30}px rgba(255,235,59,${0.1 + glowIntensity * 0.1})
        `;
    }
}

// Configurar el switch del LED
function setupToggleEvents() {
    document.getElementById('toggleLed').onchange = () => {
        userInteracting = true;
        lastUserAction = Date.now();
        sendCmd('toggle');
        setTimeout(() => { userInteracting = false; }, 500);
    };
}

// Configurar el slider de brillo
function setupBrightnessEvents() {
    const brightnessSlider = document.getElementById('brightness');
    
    // Cuando el usuario empieza a arrastrar el slider
    brightnessSlider.onmousedown = () => {
        userInteracting = true;
        lastUserAction = Date.now();
    };
    
    // Cuando suelta el mouse
    brightnessSlider.onmouseup = () => {
        setTimeout(() => { userInteracting = false; }, 500);
    };
    
    // Para celulares y tablets
    brightnessSlider.ontouchstart = () => {
        userInteracting = true;
        lastUserAction = Date.now();
    };
    
    brightnessSlider.ontouchend = () => {
        setTimeout(() => { userInteracting = false; }, 500);
    };
    
    // Cuando el usuario mueve el slider
    brightnessSlider.oninput = (e) => {
        const brightness = e.target.value;
        lastUserAction = Date.now();
        document.getElementById('brightValue').innerHTML = brightness;
        
        // Actualizar la bolita LED inmediatamente
        updateLedVisual(brightness);
        
        // Esperar 300ms después de que pare de mover para enviar al ESP32
        clearTimeout(brightnessTimeout);
        brightnessTimeout = setTimeout(() => {
            sendCmd('brightness', brightness);
        }, 300);
    };
}

// Función principal que se ejecuta cuando carga la página
function initDashboard() {
    // Mostrar la IP del ESP32
    document.getElementById('ip').innerHTML = window.location.hostname;
    
    // Configurar botones y sliders
    setupToggleEvents();
    setupBrightnessEvents();
    
    // Primera actualización
    update();
    
    // Actualizar cada 3 segundos
    setInterval(update, 3000);
}

// Iniciar todo cuando la página esté lista
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initDashboard);
} else {
    initDashboard();
}
