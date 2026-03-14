/*
   Sistemas Embarcados
   Atividade 3- Comunicação com entre Arduino e PIC
   Diego de Sá Dias
   Larissa Hitomi Morine Cortegana

*/


const int botao1 = 3;
const int botao2 = 4;
const int pinPot = A0;

//          Declaração de Variáveis
int estado_botao1 = 0;//variavel do estado do botao 1
int estado_botao2 = 0;//variavel do estado do botao 2
bool pressionado1 = false;
bool pressionado2 = false;  
int estado_botao1_anterior = LOW;
int estado_botao2_anterior = LOW;
long unsigned lastDebounceTime1 = 0; //ultimo tempo para o botao1
long unsigned lastDebounceTime2 = 0; //ultimo tempo para o botao2
long unsigned debounce_delay = 50; //tempo do debounce
int contador = 0; //contador
float leitura_pot;
float leitura_anterior_pot;

void setup() {

  pinMode(pinPot), INPUT);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);

  Serial.begin(9600);
  Serial2.begin(9600);

}

void loop() {
  // --------Debounce -------------------------------------------------------------------------------------------------------------------------------------------
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
        //Serial.println("Botao 1 apertado");
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
        //Serial.println("Botao 2 apertado");
        pressionado2 = true;
        estado_botao2_anterior = botao2;
      }
    }
  }

  // ---------------------------------------------------------------------------------------------------------------------------------------------------

  // Contador dos botoes
  if (pressionado1) {
    contador = contador + 1;
    Serial2.print(contador);
    pressionado1 = false;
  }

  if (pressionado2) {
    contador = contador - 1;
    Serial2.print(contador);
    pressionado2 = false;
  }

  //Potenciometro 
  int leitura = analogRead(pinPot);
  leitura_pot = 0.0048828125 * leitura;

  //Se mudou o valor do potenciômetro, eu  armazeno esse valor e printo para o PIC
  if (leitura_anterior_pot != leitura_pot){
    leitura_anterior_pot = leitura_pot;
    Serial2.print(leitura_pot);
  }
  
  
}
