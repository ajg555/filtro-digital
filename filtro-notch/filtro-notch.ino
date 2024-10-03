/*
 * 
 * CAT332 - Exercício Final 04
 * Alex Júnior Guimarães - 17.2.1471
 * Arduino IDE 1.8.19
 *
 */


// ========== HABILITANDO MODO DEBUG ========

#define DEBUG_FILTER


// ========== Macros e constantes ==========

#define TAPS        3                                                 // qtde. de taps do filtro
#define FREQ_AMOST  1E3                                               // freq. de amost. do filtro
                                                                      // coef b dos polos do filtro
#define IIR_B_COEF  {0.00024132, 0.00048264, 0.00024132}              
                                                                      // coef b dos zeros do filtro
#define IIR_A_COEF  {1.95558189, -0.95654717}

// ========== INCLUSAO DAS BIBLIOTECAS ========

// ========== Mapeamento de portas =========


// ========== redefinicao de tipo ==========
typedef unsigned char u_int8;                                         // var. int. de  8 bits nao sinalizada
typedef unsigned char u_int16;                                        // var. int. de 16 bits nao sinalizada
typedef unsigned long u_int32;                                        // var. int. de 32 bits nao sinalizada

// ========== Variaveis globais ============

u_int32 runtime;                                                      // contador de intervalos de tempo
float x[TAPS] = {0, 0, 0},                                            // vetor de entradas
      y[TAPS] = {0, 0, 0};                                            // vetor de saidas

// ========== Prototipos das Funcoes ========

// ========== Configuracoes iniciais ========
void setup() {
  Serial.begin(115200);                                               // inicializando Serial/UART

}

// ========== Codigo principal ==============
void loop() {

  // filtragem do sinal
  

}

// ========== Desenvolv. das funcoes ========



/*

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue);
}
*/
