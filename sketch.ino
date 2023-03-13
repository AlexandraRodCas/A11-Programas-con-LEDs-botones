#include <Bounce2.h>
// Pin a la que esta conectado el interruptor, GPIO14
const unsigned int PIN_BOTON = 14;
// Pin a la que esta conectado el LED, GPIO12
const unsigned int PIN_LED = 12;
// Pin a la que esta conectado el interruptor, GPIO4
const unsigned int PIN_BOTONA = 4;
// Pin a la que esta conectado el LED, GPIO2
const unsigned int PIN_LEDA = 2;
// Define los posibles estados del LED
typedef enum {
  LED_APAGADO,
  LED_ENCENDIDO
} estadoLed;
// Variable que guarda el estado del LED
estadoLed edoLed;
// Crea una instancia de la clase Bounce
Bounce debouncer = Bounce();
Bounce debouncerA = Bounce();
void apagaLED();
void enciendeLED();
void setup() {
  // Establece el pin PIN_LED (GPIO2) como de salida.
  pinMode(PIN_LED, OUTPUT);
  // Establece el pin PIN_BOTON (GPIO4) como de entrada.
  pinMode(PIN_BOTON, INPUT);
  // Establece el pin PIN_LED (GPIO2) como de salida.
  pinMode(PIN_LEDA, OUTPUT);
  // Establece el pin PIN_BOTON (GPIO4) como de entrada.
  pinMode(PIN_BOTONA, INPUT);
  // Establece el interruptor de boton al que se le
  // eliminara el ruido
  debouncer.attach(PIN_BOTON);
  debouncerA.attach(PIN_BOTONA);
  // Establece el intervalo (en ms) de espera para eliminar
  // el ruido
  debouncer.interval(5);
  debouncerA.interval(5);
  // Establece el estado inicial del LED
  enciendeLED(PIN_LEDA);
}
void loop() {
  // Actualiza el valor de la instancia del objeto Bounce.
  // Debe hacerse frecuentemente. Una vez en cada iteracion de
  // la funcion loop()
  debouncer.update();
  // Obten el estado del interruptor (HIGH o LOW)
  int valor = debouncer.read();
  // Prender/apagar el LED
  switch (edoLed) {
    case LED_APAGADO:
      if (valor == HIGH) enciendeLED(PIN_LED);
      break;
    case LED_ENCENDIDO:
      if (valor == LOW) apagaLED(PIN_LED);
  }

  debouncerA.update();
  // Obten el estado del interruptor (HIGH o LOW)
  int valorA = debouncerA.read();
  switch (edoLed) {
    case LED_APAGADO:
      if (valorA == HIGH) enciendeLED(PIN_LEDA);
      break;
    case LED_ENCENDIDO:
      if (valorA == LOW) apagaLED(PIN_LEDA);
  }
}
/**
  Esta funcion apaga el LED de status del microcontrolador
  ESP32 DEVKIT DOIT de 30 pines o un LED conectado al pin 2
  También actualiza la variable edoLed al valor LED_APAGADO
*/
void apagaLED(int pin) {
  // Apaga el LED
  digitalWrite(pin, LOW);
  // Actualiza la variable que guarda el estado del LED
  edoLed = LED_APAGADO;
}
/**
  Esta funcion enciende el LED de status del microcontrolador
  ESP32 DEVKIT DOIT de 30 pines o un LED conectado al pin 2
  También actualiza la variable edoLed al valor ENCENDIDO
*/
void enciendeLED(int pin) {
  // Enciende el LED
  digitalWrite(pin, HIGH);
  // Actualiza la variable que guarda el estado del LED
  edoLed = LED_ENCENDIDO;
}