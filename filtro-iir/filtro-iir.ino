/*
 * 
 * Algoritmo para aplicação de filtros digitais em ambiente Arduino
 * Exemplo utilizando um filtro notch de fc = 60Hz
 * Alex Júnior Guimarães
 * Arduino IDE 1.8.19
 *
 */


// ========== HABILITANDO MODO DEBUG ========

#define DEBUG_FILTER                                                  // printa log de eq. de dif. na serial
#undef DEBUG_FILTER


// ========== Macros e constantes ==========

#define FREQ_AMOST  1E3                                               // freq. de amost. do filtro

/*
Transfer function 'H' from input 'u1' to output ...

      0.9329 z^4 - 3.47 z^3 + 5.092 z^2 - 3.47 z + 0.9329
 y1:  ---------------------------------------------------
        z^4 - 3.697 z^3 + 5.394 z^2 - 3.654 z + 0.9766
y[n] = 0.9329x[n] + −3.47x[n−1] + 5.092x[n−2] + −3.47x[n−3] + 0.9329x[n−4] + 3.697y[n−1] + −5.394y[n−2] + 3.654y[n−3] + −0.9766y[n−4]
*/

                                                                      // coef b das entradas filtro
#define IIR_B_COEF  {0.9329,  -3.4700,   5.0924,  -3.4700,   0.9329}             
#define IIR_B_SIZE  5                                                 // qtde de coef das entradas    (num)

                                                                      // coef a das saidas do filtro  (den)
#define IIR_A_COEF  {1.0000,  -3.6973,   5.3940,  -3.6539,   0.9766}
#define IIR_A_SIZE  5                                                 // qtde de coef das saidas

// ========== INCLUSAO DAS BIBLIOTECAS ========

// ========== Mapeamento de portas =========

#define FUNCTION_GENERATOR A0                                         // entrada analógica para gerador de funcoes

// ========== redefinicao de tipo ==========
typedef unsigned char u_int8;                                         // var. int. de  8 bits nao sinalizada
typedef unsigned char u_int16;                                        // var. int. de 16 bits nao sinalizada
typedef unsigned long u_int32;                                        // var. int. de 32 bits nao sinalizada

// ========== Variaveis globais ============

u_int32 sample_time = 0,                                              // controle de timer para freq. de amost.
        ctrl_serial = 0;                                              // controle print da saida na serial
float x[IIR_B_SIZE],                                                  // vetor de entradas  (num)
      y[IIR_A_SIZE],                                                  // vetor de saidas    (den)
      b[] = IIR_B_COEF,                                               // coef b das saidas do filtro
      a[] = IIR_A_COEF;                                               // coef a das entradas do filtro

// ========== Prototipos das Funcoes ========
void zeros(float *v, u_int16 vsize);                                  // inicializa vetores com 0
void filterResponse();
void inputRead();
void signalFilter(float *a, float *b, u_int16 sizeA, u_int16 sizeB);

// ========== Configuracoes iniciais ========
void setup() {
  Serial.begin(115200);                                               // inicializando Serial/UART
                                                                      // inicializando entradas/saidas com 0, passando o vetor e o seu tamanho
  zeros(x, sizeof(x)/sizeof(x[0]));
  zeros(y, sizeof(y)/sizeof(y[0])); 
}

// ========== Codigo principal ==============
void loop() {

                                                                      // ciclo de amostragem e filtragem do sinal
  if(micros() - sample_time > (int) (1/FREQ_AMOST)*1E6){
    sample_time = micros();
    
                                                                      // captura do sinal
    x[0] = analogRead(FUNCTION_GENERATOR) * (5.0/1024);               // ADC de 10 bits de resolução - 0 ~ 5V
                                                                      // filtragem do sinal
    signalFilter(y, x, a, b, sizeof(a)/sizeof(a[0]), sizeof(b)/sizeof(b[0]));

                                                                       // printando sinal filtrado
    ctrl_serial++;
    //if(ctrl_serial % 3 == 0){                                        // limita amostragem de print do sinal na serial - opcional
      Serial.print(x[0]);
      Serial.print(",");
      Serial.println(y[0]);
    //}
  }
}

// ========== Desenvolv. das funcoes ========
                                                                     // filtragem de sinal
void signalFilter(float *output, float *input, float *a, float *b, u_int16 sizeA, u_int16 sizeB){
  
  u_int16 posDelay = 0;
  
  output[0] = 0;

  #ifdef DEBUG_FILTER
    Serial.print("y[n] = ");
  #endif
  
                                                                      // aplicando filtro no sinal
  for(posDelay = 0; posDelay < sizeB; posDelay++){              
    output[0] += b[posDelay]*input[posDelay];
    #ifdef DEBUG_FILTER
      Serial.print(b[posDelay]);
      Serial.print(" * ");
      Serial.print(x[posDelay]);
      Serial.print(" + ");
    #endif
  }

  for(posDelay = 1; posDelay < sizeA; posDelay++){
    output[0] -= a[posDelay]*output[posDelay];
    #ifdef DEBUG_FILTER
      Serial.print(a[posDelay]);
      Serial.print(" * ");
      Serial.print(y[posDelay]);
      Serial.print(" + ");
    #endif
  }

  #ifdef DEBUG_FILTER
    Serial.print(" = ");
    Serial.println(output[0]);
  #endif

  /*
    Transfer function 'H' from input 'u1' to output ...
    
          0.9329 z^4 - 3.47 z^3 + 5.092 z^2 - 3.47 z + 0.9329
     y1:  ---------------------------------------------------
            z^4 - 3.697 z^3 + 5.394 z^2 - 3.654 z + 0.9766
    y[n] = 0.9329x[n] + −3.47x[n−1] + 5.092x[n−2] + −3.47x[n−3] + 0.9329x[n−4] + 3.697y[n−1] + −5.394y[n−2] + 3.654y[n−3] + −0.9766y[n−4]
  */
  
  for(posDelay = sizeA - 1; posDelay > 0; posDelay--){                // shift de amostras no vetor de entradas - primeira posicao é a nova amostra do ADC e é pulada no laço
    input[posDelay] = input[posDelay - 1];
  }

  for(posDelay = sizeB - 1; posDelay > 0; posDelay--){                // shift de amostras no vetor de saidas - primeira posicao é o calculo atual e é pulada no laço
    output[posDelay] = output[posDelay - 1];
  }
}
void filterResponse();

void zeros(float *v, u_int16 vsize){
  u_int16 pos = 0;                                                    // pos. do vetor
  for(pos = 0; pos < vsize; pos++){                                   // inicializa vetor com zeros
    v[pos] = 0;
  }
}

/* 
*  FONTES
*  https://github.com/curiores/ArduinoTutorials/blob/main/BasicFilters/ArduinoImplementations/LowPass/SimpleExamples/butterworth2.ino
*
*/
