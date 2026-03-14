/*
   Sistemas Embarcados
   Atividade 2- Automação Residencial
   Diego de Sá Dias- RA:156389

*/

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//          Importando as bibliotecas necessárias
#include <Servo.h>
Servo myservo;

//          Declaração de Constantes (pinos)
const int botao1 = 8;
const int botao2 = 9;
const int botao3 = 12;
const int buzzer = 2;
const int led1 = 5; //PWM
const int led2 = 6; //PWM
const int led3 = 10; //PWM
const int led4 = 11; //PWM

int pinmyservo = 3; //PWM
const int pot = A0; // Pin do Potenciômetro
const int LDR = A1;
const int sensor_temperatura = A2;

//          Declaração de Variáveis
int estado_botao1 = 0;//variavel do estado do botao 1
int estado_botao2 = 0;//variavel do estado do botao 2
int estado_botao3 = 0;//variavel do estado do botao 3
int estado_botao1_anterior = LOW;
int estado_botao2_anterior = LOW;
int estado_botao3_anterior = LOW;
int pressionado1 = false;
int pressionado2 = false;
int pressionado3 = false;
long unsigned lastDebounceTime1 = 0; //ultimo tempo para o botao1
long unsigned lastDebounceTime2 = 0; //ultimo tempo para o botao2
long unsigned lastDebounceTime3 = 0; //ultimo tempo para o botao3
long unsigned debounce_delay = 50; //tempo do debounce
//temperatura
float temperatura[20] = {0};
int tempIndex = 0; //índice da temperatura
unsigned long previousTempMillis = 0;
const unsigned long tempInterval = 1000; // Valor do intervalo de obtenção da temperatura (1 s)
float tempLimit = 30.0; // Valor limite da temperatura
//ldr
float ldrReadings[20] = {0.0};  // Array para armazenar as últimas 20 leituras do LDR
float luminosidade;
int ldrIndex = 0; // Índice do sensor LDR
unsigned long previousLdrMillis = 0;
const unsigned long ldrInterval = 500;  // Intervalo de 500ms para leitura do LDR
//potenciometro
float potReadings[20] = {0};  // Array para armazenar as últimas 20 leituras do potenciômetro
int potIndex = 0; // Índice das medidas do potenciômetro
unsigned long previousPotMillis = 0;
const unsigned long potInterval = 400;  // Intervalo de 400ms para leitura do potenciômetro
//interfaces
int usuario = 0;
bool selecao0 = true;
bool selecao1 = true;
bool selecao2 = true;
bool selecao3 = true;
bool mostrarTempMedia = false;
bool mostrarTempMaxMin = false;
bool mostrarLuminoMedia = false;
bool mostrarLuminoMaxMin = false;
bool mostrarPosicaoMotor = false;
bool estadoBuzzer = false;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  pinMode(botao1, OUTPUT);
  pinMode(botao2, OUTPUT);
  pinMode(botao3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  myservo.attach(pinmyservo);
  pinMode(pot, INPUT);
  pinMode(LDR, INPUT);
  pinMode(sensor_temperatura, INPUT);
  pinMode(botao1, INPUT_PULLUP); //quando o botao for 0, tenho que ligar
  pinMode(botao2, INPUT_PULLUP);
  pinMode(botao3, INPUT_PULLUP);
  myservo.write(0); //inicializando o myservo
  Serial.begin(9600);


}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //          Debounce

  //        Botão 1
  int lendo1 = digitalRead(botao1);
  if (lendo1 != estado_botao1_anterior) {
    lastDebounceTime1 = millis();
    estado_botao1_anterior = !estado_botao1_anterior;
  }

  if ((millis() - lastDebounceTime1) > debounce_delay) {
    if (lendo1 != estado_botao1) {
      estado_botao1 = lendo1; // variável que armazena se o botao foi ou nao apertado

      if (estado_botao1 == LOW) {
        //Aqui eu vou fazer algo quando o botão for apertado
        Serial.println("Botao 1 apertado");
        pressionado1 = true;
        estado_botao1_anterior = botao1;
      }
    }
  }

  //        Botão 2
  int lendo2 = digitalRead(botao2);
  if (lendo2 != estado_botao2_anterior) {
    lastDebounceTime2 = millis();
    estado_botao2_anterior = !estado_botao2_anterior;
  }

  if ((millis() - lastDebounceTime2) > debounce_delay) {
    if (lendo2 != estado_botao2) {
      estado_botao2 = lendo2;
      if (estado_botao2 == LOW) {
        //Aqui eu vou fazer algo quando o botão for apertado
        Serial.println("Botao 2 apertado");
        pressionado2 = true;
        estado_botao2_anterior = botao2;
      }
    }
  }

  //        Botão 3
  int lendo3 = digitalRead(botao3);
  if (lendo3 != estado_botao3_anterior) {
    lastDebounceTime3 = millis();
    estado_botao3_anterior = !estado_botao3_anterior;
  }

  if ((millis() - lastDebounceTime3) > debounce_delay) {
    if (lendo3 != estado_botao3) {
      estado_botao3 = lendo3;
      if (estado_botao3 == LOW) {
        //Aqui eu vou fazer algo quando o botão for apertado
        Serial.println("Botao 3 apertado");
        pressionado3 = true;
        estado_botao3_anterior = botao3;
      }
    }
  }

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //          Medição da temperatura
  unsigned long currentMillis = millis();

  // Leitura e processamento da temperatura
  if (currentMillis - previousTempMillis >= tempInterval) {
    previousTempMillis = currentMillis;
    float temperature = analogRead(sensor_temperatura) * 0.222162978;
    temperatura[tempIndex] = temperature;
    tempIndex = (tempIndex + 1) % 20; //atualizando o índice circularmente
    processTemperature();
  }

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //          Medição da luminosidade
  if (currentMillis - previousLdrMillis >= ldrInterval) {
    previousLdrMillis = currentMillis;
    luminosidade = analogRead(LDR);
    ldrReadings[ldrIndex] = luminosidade;
    ldrIndex = (ldrIndex + 1) % 20; //atualizando o índice circularmente
    // Mapeando a luminosidade média para um valor de 0 a 255 (PWM)

    //Serial.println(intensidadeLed);

    // Acionando os LEDs de acordo com a intensidade da LUZ
    /*
      if (intensidadeLed >= 0 && intensidadeLed <= 64) {
      analogWrite(led1, intensidadeLed);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
      analogWrite(led4, 0);
      }
      if (intensidadeLed > 64 && intensidadeLed <= 128) {
      analogWrite(led1, 255);
      analogWrite(led2, intensidadeLed);
      analogWrite(led3, 0);
      analogWrite(led4, 0);
      }
      if (intensidadeLed > 128 && intensidadeLed <= 192) {
      analogWrite(led1, 255);
      analogWrite(led2, 255);
      analogWrite(led3, intensidadeLed);
      analogWrite(led4, 0);
      }
      if (intensidadeLed > 192 && intensidadeLed <= 255) {
      analogWrite(led1, 255);
      analogWrite(led2, 255);
      analogWrite(led3, 255);
      analogWrite(led4, intensidadeLed);
      }
    */
    processLuminosity();

  }
  int intensidadeLed = map(luminosidade, 0, 1023, 255, 0);
  analogWrite(led1, intensidadeLed);
  analogWrite(led2, intensidadeLed);
  analogWrite(led3, intensidadeLed);
  analogWrite(led4, intensidadeLed);



  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //          Potenciometro (Cortina)
  if (currentMillis - previousPotMillis >= potInterval) {
    previousPotMillis = currentMillis;
    float potValue = analogRead(pot);
    potReadings[potIndex] = potValue;
    potIndex = (potIndex + 1) % 20;
    processPotentiometer();
  }

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //          Interface do usuário
  switch (usuario) {
    case 0:
      //caso de inicialização do usuário
      if (selecao0) {
        selecao0 = false; // para entrar apenas uma vez nesse modo de seleção
        //Imprimir no Monitor Serial as opções da Interface de Seleção
        Serial.println("Escolha a opção desejada:");
        Serial.println("\t- (1) Sensor de Temperatura");
        Serial.println("\t- (2) Sensor de Luminosidade");
        Serial.println("\t- (3) Motor Buzzer\n");
      }

      if (pressionado1) {
        usuario = 1;
        pressionado1 = false;
      }

      if (pressionado2) {
        usuario = 2;
        pressionado2 = false;
      }

      if (pressionado3) {
        usuario = 3;
        pressionado3 = false;
      }
      break;

    case 1:
      //caso para o sensor de temperatura
      if (selecao1) {
        selecao1 = false; // para entrar apenas uma vez nesse modo de seleção
        //Imprimir no Monitor Serial as opções da Interface de Seleção
        Serial.println("Escolha a opção desejada:");
        Serial.println("\t- (1) Temperatura média");
        Serial.println("\t- (2) Temperatura máxima ou mínima");
        Serial.println("\t- (3) Voltar\n");
      }

      if (pressionado1) {
        mostrarTempMedia = true; //diz para mostrar a Temperatura Média
        pressionado1 = false;
      }

      if (pressionado2) {
        mostrarTempMaxMin = true; //diz para mostrar a Temperatura Máxima e Mínima
        pressionado2 = false;
        selecao1 = true;
      }

      if (pressionado3) {
        usuario = 0;
        selecao0 = true;
        pressionado3 = false;
      }
      break;

    case 2:
      //caso para o sensor de Luminosidade
      if (selecao2) {
        selecao2 = false; // para entrar apenas uma vez nesse modo de seleção
        //Imprimir no Monitor Serial as opções da Interface de Seleção
        Serial.println("Escolha a opção desejada:");
        Serial.println("\t- (1) Luminosidade média");
        Serial.println("\t- (2) Luminosidade máxima ou mínima");
        Serial.println("\t- (3) Voltar\n");
      }

      if (pressionado1) {
        mostrarLuminoMedia = true; //diz para mostrar a Luminosidade Média
        pressionado1 = false;
      }

      if (pressionado2) {
        mostrarLuminoMaxMin = true; //diz para mostrar a Luminosidade Máxima e Mínima
        pressionado2 = false;
      }

      if (pressionado3) {
        usuario = 0;
        selecao0 = true;
        pressionado3 = false;
      }


      break;

    case 3:
      //caso para o Motor e o Buzzer
      if (selecao3) {
        selecao3 = false; // para entrar apenas uma vez nesse modo de seleção
        //Imprimir no Monitor Serial as opções da Interface de Seleção
        Serial.println("Escolha a opção desejada:");
        Serial.println("\t- (1) Mostrar posicao do motor");
        Serial.println("\t- (2) Ligar/desligar o buzzer");
        Serial.println("\t- (3) Voltar\n");
      }
      if (pressionado1) {
        mostrarPosicaoMotor = true; //diz para mostrar a posição do motor
        pressionado1 = false;
      }

      if (pressionado2) {
        estadoBuzzer = !estadoBuzzer; //troca o estado do buzzer
        if (estadoBuzzer) {
          tone(buzzer, 1000); // se o estado for verdadeiro, eu ligo o buzzer
        }
        else {
          noTone(buzzer); // se o estado for falso, eu desligo o buzzer
        }
        pressionado2 = false;
        selecao3 = true;
      }

      if (pressionado3) {
        usuario = 0;
        selecao0 = true;
        pressionado3 = false;
      }
      break;
  }

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

//          Função para processar a temperatura ambiente
void processTemperature() {
  /*
     Este procedimento calcula a media das últimas 20 temperaturas,
     printa qual foi a temperatura máxima e mínima no ambiente e
     faz o buzzer tocar caso a temperatura média ultrapasse a temperatura
     limite
  */
  float soma = 0;
  float maxTemp = temperatura[0];
  float minTemp = temperatura[0];

  for (int i = 0; i < 20; i++) {
    soma += temperatura[i];
    if (temperatura[i] > maxTemp) maxTemp = temperatura[i];
    if (temperatura[i] < minTemp) minTemp = temperatura[i];
  }

  float mediaTemp = soma / 20;

  if (mostrarTempMedia) {
    Serial.print("Média da Temperatura: ");
    Serial.println(mediaTemp);
    selecao1 = true;
    mostrarTempMedia = false;
  }
  if (mostrarTempMaxMin) {
    Serial.print("Temperatura Máxima: ");
    Serial.println(maxTemp);
    Serial.print("Temperatura Mínima: ");
    Serial.println(minTemp);
    selecao1 = true;
    mostrarTempMaxMin = false;
  }

  // Controle do buzzer
  if (maxTemp > tempLimit) {
  Serial.println(maxTemp);
    tone(buzzer, 1000, 1000); // Liga o buzzer por 1 segundo
  } else {
    noTone(buzzer); // Desliga o buzzer
  }
}


//          Código para processar a luminosidade ambiente
void processLuminosity() {
  /*
     Este procedimento calcula a media das últimas 20 luminosidades, qual
     a luminosidade máxima e mínima e ainda dá um update nas c
  */

  float sum = 0;
  float maxLumino = ldrReadings[0];
  float minLumino = ldrReadings[0];

  for (int i = 0; i < 20; i++) {
    sum += ldrReadings[i];
    if (ldrReadings[i] > maxLumino) maxLumino = ldrReadings[i];
    if (ldrReadings[i] < minLumino) minLumino = ldrReadings[i];
  }
  float mediaLumino = sum / 20;

  if (mostrarLuminoMedia) {
    Serial.print("Média da Luminosidade: ");
    Serial.println(mediaLumino);
    selecao2 = true;
    mostrarLuminoMedia = false;
  }
  if (mostrarLuminoMaxMin) {
    Serial.print("Luminosidade Máxima: ");
    Serial.println(maxLumino);
    Serial.print("Luminosidade Mínima: ");
    Serial.println(minLumino);
    selecao2 = true;
    mostrarLuminoMaxMin = false;
  }

}


//          Código para  função do potenciômetro e do myservomotor (cortina)
void processPotentiometer() {
  float sum = 0;
  float maxPot = potReadings[0];
  float minPot = potReadings[0];

  for (int i = 0; i < 20; i++) {
    sum += potReadings[i];
    if (potReadings[i] > maxPot) maxPot = potReadings[i];
    if (potReadings[i] < minPot) minPot = potReadings[i];
  }

  float mediaPot = sum / 20;

  // Controle do myservomotor com base na média do potenciômetro
  updateServo(mediaPot);
}

//          Função para atualizar a posição do myservomotor com base na média do potenciômetro
void updateServo(float averagePot) {
  // Mapeia a média do potenciômetro (0 a 1023) para a faixa do myservomotor (0 a 180 graus)
  int myservoPosition = map(averagePot, 0, 1023, 0, 180);
  if (mostrarPosicaoMotor) {
    Serial.print("Posição do Motor: ");
    Serial.print(myservoPosition);
    Serial.println("°");
    selecao3 = true;
    mostrarPosicaoMotor = false;
  }
  myservo.write(myservoPosition);
}
