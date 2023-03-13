#include <Bounce2.h>
// Pin a la que esta conectado el interruptor, GPIO14
const unsigned int PIN_BOTONB = 14;
// Pin a la que esta conectado el LED, GPIO12
const unsigned int PIN_LEDB = 13;

// Pin a la que esta conectado el interruptor, GPIO4
const unsigned int PIN_BOTONA = 4;
// Pin a la que esta conectado el LED, GPIO2
const unsigned int PIN_LEDA = 2;

// Valor minimo del ciclo de trabajo
const unsigned int CT_MIN = 0;
// Valor maximo del ciclo de trabajo. 2**RESOLUCION - 1
const unsigned int CT_MAX = 255;

// Ciclo de trabajo de la forma de onda PWM
unsigned int cicloTrabajo;
// Incremento/decremento en el ciclo de trabajo
unsigned int paso;

// Define los posibles estados del LED
typedef enum {
  LEDA_ON_LEDB_OFF,
  LEDA_ON_LEDB_ON,
  LEDA_OFF_LEDB_ON
} estadoLed;

// Variable que guarda el estado del LED
estadoLed edoLed;

// Crea una instancia de la clase Bounce
Bounce debouncerB = Bounce();
Bounce debouncerA = Bounce();

void disminuirAAumentarB();
void apagarBEncenderA();
void encenderBApagarA();
void disminuirBAumentarA();

void setup() {
  // Establece el pin PIN_LED (GPIO2) como de salida.
  pinMode(PIN_LEDB, OUTPUT);
  // Establece el pin PIN_BOTON (GPIO4) como de entrada.
  pinMode(PIN_BOTONB, INPUT);

  // Establece el pin PIN_LED (GPIO2) como de salida.
  pinMode(PIN_LEDA, OUTPUT);
  // Establece el pin PIN_BOTON (GPIO4) como de entrada.
  pinMode(PIN_BOTONA, INPUT);

  // Establece el interruptor de boton al que se le
  // eliminara el ruido
  debouncerB.attach(PIN_BOTONB);
  debouncerA.attach(PIN_BOTONA);

  // Establece el intervalo (en ms) de espera para eliminar
  // el ruido
  debouncerB.interval(5);
  debouncerA.interval(5);

  // Establece el estado inicial del LED
  apagarBEncenderA(PIN_LEDA, PIN_LEDB);
}

void loop() {
  // Actualiza el valor de la instancia del objeto Bounce.
  // Debe hacerse frecuentemente. Una vez en cada iteracion de
  // la funcion loop()
  debouncerB.update();
  debouncerA.update();

  // Obten el estado del interruptor (HIGH o LOW)
  int valorB = debouncerB.read();
  int valorA = debouncerA.read();

  switch (edoLed) {
    case LEDA_ON_LEDB_OFF:
      if (valorB ==  HIGH) disminuirAAumentarB(PIN_LEDA, PIN_LEDB);
      break;
    case LEDA_ON_LEDB_ON:
      if (valorB ==  HIGH) encenderBApagarA(PIN_LEDA, PIN_LEDB);
      if (valorA ==  HIGH) apagarBEncenderA(PIN_LEDA, PIN_LEDB);
      break;
    case LEDA_OFF_LEDB_ON:
      if (valorB == HIGH) disminuirBAumentarA(PIN_LEDA, PIN_LEDB);
  }
}

/**
  Esta funcion enciende el LEDA y apaga el LEDB
  También actualiza la variable edoLed al valor LEDA_ON_LEDB_OFF
*/
void apagarBEncenderA(int pinA, int pinB) {
  // Enciende el LEDA
  digitalWrite(pinA, HIGH);
  // Apaga el LEDB
  digitalWrite(pinB, LOW);
  // Actualiza la variable que guarda el estado del LED
  edoLed = LEDA_ON_LEDB_OFF;
}

void disminuirAAumentarB(int pinA, int pinB) {
  // Enciende el LEDB con un ciclo de trabajo del 50%
  analogWrite(pinB, 127);
  // Apaga el LEDA
  digitalWrite(pinA, 127);
  edoLed = LEDA_ON_LEDB_ON;
}

/**
  Esta funcion enciende el LEDB y apaga el LEDA
  También actualiza la variable edoLed al valor LEDA_OFF_LEDB_ON
*/
void encenderBApagarA(int pinA, int pinB) {
  // Enciende el LEDB
  digitalWrite(pinB, HIGH);
  // Apaga el LEDA
  digitalWrite(pinA, LOW);
  // Actualiza la variable que guarda el estado del LED
  edoLed = LEDA_OFF_LEDB_ON;
}

void disminuirBAumentarA(int pinA, int pinB) {
  // Enciende el LEDA con un ciclo de trabajo del 50%
  analogWrite(pinA, 127);
  // Apaga el LEDB
  digitalWrite(pinB, 127);
  edoLed = LEDA_ON_LEDB_OFF;
}