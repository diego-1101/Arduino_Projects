/*
   Sistemas Embarcados
   AProjeto Final- Música com músculos
   Diego de Sá Dias
   RA: 156389

*/

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//          Importando as bibliotecas necessárias
#include <Servo.h>
Servo meuServo; // Cria um objeto Servo para controlar o servo motor


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//          Constantes dos pinos
const byte pinBotaoIniciar = 2;
const byte pinBotaoFinalizar = 3;
const int pinLedIniciar = 4;
const int pinLedFinalizar = 5;
const int buzzer_pin = 7;
const int pinMusculo = A0; // o fio vermelho é no meio, o fio amarelo é no final e o verde é a referência
const int pinPot = A1;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//          Variáveis

//musculo
float limiar = 200; // Definir esse limiar

//servo
int pos; // Declara uma variável para controlar a posição do servo motor

//botoes
volatile byte estadoIniciar = LOW;
volatile byte estadoFinalizar = LOW;

//leds
bool estadoLedIniciar = false; // Led Iniciar
bool estadoLedFinalizar = false; // Led finalizar

//interface
int menu = 0; //variável responsável pelos menus do switch case
bool selecao0 = true; //variável que irá controlar a fala de inicialização
bool selecao1 = true; //variável que irá dar o comando de início do jogo (quando iremos começar a contrair)
bool finalizar = true; //variável irá dar o comando para falar uma vez a mensagem de finalizar

//variáveis gerais
int contadorContracoes = 0; //contador para o número de contrações que a pessoa fez
unsigned long tempoAnterior = 0;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//          Structures relacionadas a música do Harry Potter
struct MusicStruct {
  int A = 550;
  int As = 582;
  int B = 617;
  int C = 654;
  int Cs = 693;
  int D = 734;
  int Ds = 777;
  int E = 824;
  int F = 873;
  int Fs = 925;
  int G = 980;
  int Gs = 1003;
  int A2 = 1100;
  int A2s = 1165;
  int B2 = 1234;
  int C3 = 1308;
  int C3s = 1385;
  int D3 = 1555;
} Music;

struct LengthStruct {
  float half = 0.5;
  float one = 1.0;
  float one_half = 1.5;
  float two = 2.0;
  float two_half = 2.5;
} Length;

float tempo = 400;//tempo em que cada nota será tocada

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  //Setting up ports
  pinMode(pinBotaoIniciar, INPUT_PULLUP);
  pinMode(pinBotaoFinalizar, INPUT_PULLUP);
  pinMode(pinMusculo, INPUT);
  pinMode(pinPot, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(pinLedIniciar, OUTPUT);
  pinMode(pinLedFinalizar, OUTPUT);

  digitalWrite(pinLedIniciar, LOW);
  digitalWrite(pinLedFinalizar, LOW);

  meuServo.attach(6); // Associa o servo motor ao pino digital 6 do Arduino
  meuServo.write(8); // Define a posição inicial do servo motor para 0 graus

  //parte da interrupção
  attachInterrupt(digitalPinToInterrupt(pinBotaoIniciar), blink1, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBotaoFinalizar), blink2, FALLING);


}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void loop() {

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //          Parte do potenciômetro
  //O potenciômetro irá controlar a velocidade da contração e a ventuinha
  int pot = map(analogRead(pinPot), 0, 1023, 0, 255);

  if (millis() >= tempoAnterior) {
    tempoAnterior = millis() + 100;
    Serial2.write(pot); // escrevendo o dutty cicle da ventuínha
  }



  // alterando a velocidade do pedido de contração
  if (pot >= 0 && pot <= 64) {
    tempo = tempo * 1;
  } else {
    if (pot >= 65 && pot <= 128) {
      tempo = tempo * 0.75;
    } else {
      if (pot >= 129 && pot <= 192) {
        tempo = tempo * 0.6;
      } else {
        if (pot >= 192 && pot <= 255) {
          tempo = tempo * 0.5;
        }
      }
    }
  }

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //          Parte da Interface com o usuário

  switch (menu) {
    case 0:
      //Início
      if (selecao0) {
        Serial.println("Ola, este é o menu iniciar!");
        Serial.println("\nCaso voce queira iniciar, aperte o botao 1");
        selecao0 = false;
      }

      if (estadoIniciar) {
        //se o botao 1 for apertado e pedir para iniciar, começamos o game
        menu = 1;
        digitalWrite(pinLedIniciar, HIGH);
        estadoIniciar = LOW;
      }

      break; //break caso 0

    case 1:
      if (selecao1) {
        Serial.println("Vamos comecar!");
        Serial.println("Caso queira finalizar, aperte o botao 2");
        selecao1 = false;
      }


      if (estadoFinalizar == LOW) {
        // mandar para o PIC que iniciou
        Serial2.write(1); //PIC irá ligar o LCD e escrever bem-vindo enquuanto pisca um LED 3 vezes (com delay de 50 ms) Depois de piscar o LED, escreve no LCD "Músuculos Sonoros" e o Buzzer do PIC apita 2 vezes

        //          Parte do sensor muscular
        int leituraMusculo = (5000 / 1023) * analogRead(pinMusculo) ; // !!!!!!!!!!!!!!!!!!!!!!! Acertar o valor do sensor

        // Testar se o valor está acima do limiar
        meuServo.write(8);
        if (leituraMusculo >= limiar) {
          Serial.println(leituraMusculo);
          //troco o caso do menu para garantir que só incremente 1 no contador
          menu = 2;
        }

      } else {
        //caso tenha apertado o botao 2, finaliza todo o esquema automaticamente
        if (finalizar) {
          Serial2.write(3); //aviso para o PIC que foi finalizado
          digitalWrite(pinLedFinalizar, HIGH);
          digitalWrite(estadoLedIniciar, LOW);
          Serial.println("Voce finalizou o seu treino pelo botao!");
          digitalWrite(pinLedFinalizar, HIGH);
          Serial.println("Se quiser reiniciar, aperte o botao vermelho do arduino");
          finalizar = false;
        }


      }

      break; //break caso 1

    case 2:
      // começar a tocar a música
      contadorContracoes++; // incrementa o contador de contrações
      meuServo.write(90); // coloca o servo na posição 90° simbolizando que houve uma contração
      Serial2.write(2); // o PIC vai piscar o LED rapidamente

      //Serial.print(contadorContracoes); //visualizar se ta dando certo !!!!!!

      menu = 3; //troco para o caso 3, pois é nele que vou chamar a função para tocar a música
      break; //break caso 2

    case 3:
      //switch case para as notas do Harry Potter
      switch (contadorContracoes) {
        case 0:
          Serial.println("Nao atingiu o limiar nenhuma vez, tente novamente");

        case 1:
          setTone(buzzer_pin, Music.B, tempo * Length.one);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 1 do contador

        case 2:
          setTone(buzzer_pin, Music.E, tempo * Length.one_half);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 2 do contador

        case 3:
          setTone(buzzer_pin, Music.G, tempo * Length.half);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 4 do contador

        case 4:
          setTone(buzzer_pin, Music.F, tempo * Length.one);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 4 do contador

        case 5:
          setTone(buzzer_pin, Music.E, tempo * Length.two);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 5 do contador

        case 6:
          setTone(buzzer_pin, Music.B2, tempo * Length.one);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 6 do contador

        case 7:
          setTone(buzzer_pin, Music.A2, tempo * Length.two_half);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 7 do contador

        case 8:
          setTone(buzzer_pin, Music.Fs, tempo * Length.two_half);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 8 do contador

        case 9:
          setTone(buzzer_pin, Music.E, tempo * Length.one_half);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 9 do contador

        case 10:
          setTone(buzzer_pin, Music.G, tempo * Length.half);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 10 do contador

        case 11:
          setTone(buzzer_pin, Music.F, tempo * Length.one);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 11 do contador

        case 12:
          setTone(buzzer_pin, Music.Ds, tempo * Length.two);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 12 do contador

        case 13:
          setTone(buzzer_pin, Music.F, tempo * Length.one);
          menu = 1;
          Serial.println("Contraia novamente!");
          break; //break caso 13 do contador

        case 14:
          setTone(buzzer_pin, Music.B, tempo * Length.two_half);
          menu = 0;
          selecao0 = true;
          contadorContracoes = 0;
          digitalWrite(pinLedIniciar, LOW);
          Serial.println("Parabens! Voce Fez todas as repeticoes\n");
          break; //break caso 14 do contador

      } // fim do switch do contador de contrações


      break; //break caso 3
  }


  //-----------------------------------------------------------------------------------------------------------------------------------------------------------
  //


}

void blink1() {
  estadoIniciar = HIGH; //troco o estado do botao de iniciar
}

void blink2() {
  estadoFinalizar = HIGH; //troco o estado do botao de finalizar
}

//Procedimento para tocar a nota musical
void setTone(int pin, int note, int duration) {
  tone(pin, note, duration);
  delay(duration);
  noTone(pin);
}
