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
unsigned int contadorA;
unsigned int contadorB;


// Ciclo de trabajo de la forma de onda PWM
unsigned int cicloTrabajoA;
unsigned int cicloTrabajoB;
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
  Serial.begin(9600);
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

  if(valorB == HIGH){
    contadorB += 1;
  }

  if(valorA == HIGH){
    contadorA += 1;
  }

  switch (edoLed) {
    case LEDA_ON_LEDB_OFF:
      if(valorB == HIGH) Serial.println("Intensidad máxima del ledA");
      if (valorA ==  HIGH) disminuirAAumentarB(PIN_LEDA, PIN_LEDB);
      break;
    case LEDA_ON_LEDB_ON:
      if (valorB ==  HIGH && cicloTrabajoB != 0) disminuirBAumentarA(PIN_LEDA, PIN_LEDB);
      if (valorB ==  HIGH && cicloTrabajoB == 0) apagarBEncenderA(PIN_LEDA, PIN_LEDB);
      if (valorA ==  HIGH && cicloTrabajoA != 0) disminuirAAumentarB(PIN_LEDA, PIN_LEDB);
      if (valorA ==  HIGH && cicloTrabajoA == 0) encenderBApagarA(PIN_LEDA, PIN_LEDB);
      break;
    case LEDA_OFF_LEDB_ON:
      if(valorA == HIGH) Serial.println("Intensidad máxima del ledB");
      if (valorB ==  HIGH) disminuirBAumentarA(PIN_LEDA, PIN_LEDB);
  }
}

/**
  Esta funcion enciende el LEDA y apaga el LEDB
  También actualiza la variable edoLed al valor LEDA_ON_LEDB_OFF
*/
void apagarBEncenderA(int pinA, int pinB) {
  cicloTrabajoA = CT_MAX;
  cicloTrabajoB = CT_MIN;
  contadorA = 0;
  contadorB = 0;

  // Enciende el LEDA
  analogWrite(pinA, cicloTrabajoA);

  // Apaga el LEDB
  analogWrite(pinB, cicloTrabajoB);

  Serial.println(cicloTrabajoB);
  Serial.println(cicloTrabajoA);
  Serial.println("en el apagarBEncenderA ");
  // Actualiza la variable que guarda el estado del LED
  edoLed = LEDA_ON_LEDB_OFF;
}

void disminuirAAumentarB(int pinA, int pinB) {
  if(cicloTrabajoA>23){
    cicloTrabajoA = cicloTrabajoA - (contadorB*1);
     // disminuye intensidad del LEDA
    analogWrite(pinA, cicloTrabajoA);
  }
  else{
    cicloTrabajoA = CT_MIN;
    analogWrite(pinA, CT_MIN);
  }

  if(cicloTrabajoB<230){
    cicloTrabajoB += (contadorB*1);
     // Aumenta intensidad del LEDB
    analogWrite(pinB, cicloTrabajoB);
  }
  else{
    cicloTrabajoB = CT_MAX;
    analogWrite(pinB, CT_MAX);
  }
  Serial.println(cicloTrabajoB);
  Serial.println(cicloTrabajoA);
  Serial.println("en el disminuirAAumentarB ");

  edoLed = LEDA_ON_LEDB_ON;
}

/**
  Esta funcion enciende el LEDB y apaga el LEDA
  También actualiza la variable edoLed al valor LEDA_OFF_LEDB_ON
*/
void encenderBApagarA(int pinA, int pinB) {
  cicloTrabajoB = CT_MAX;
  cicloTrabajoA = CT_MIN;
  contadorB = 0;
  contadorA = 0;
  
  // Enciende el LEDB
  analogWrite(pinB, cicloTrabajoB);
  // Apaga el LEDA
  analogWrite(pinA, cicloTrabajoA);
  Serial.println("en el encenderBApagarA ");
  // Actualiza la variable que guarda el estado del LED
  edoLed = LEDA_OFF_LEDB_ON;
  
}

void disminuirBAumentarA(int pinA, int pinB) {
  if(cicloTrabajoB>23){
    cicloTrabajoB = cicloTrabajoB - (contadorA*1);
     // Apaga el LEDA
    analogWrite(pinB, cicloTrabajoB);
  }
  else{
    cicloTrabajoB = CT_MIN;
    analogWrite(pinB, CT_MIN);
  }

  if(cicloTrabajoA<230){
    cicloTrabajoA += (contadorA*1);
     // Apaga el LEDB
    analogWrite(pinA, cicloTrabajoA);
  }
  else{
    cicloTrabajoA = CT_MAX;
    analogWrite(pinA, CT_MAX);
  }
  Serial.println(cicloTrabajoB);
  Serial.println(cicloTrabajoA);
  Serial.println("en el disminuirBAumentarA ");

  edoLed = LEDA_ON_LEDB_ON;
}