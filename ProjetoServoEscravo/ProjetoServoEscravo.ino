/*
   Sistemas Embarcados
   Atividade 3- Comunicação com entre arduinos- Parte  A
   Diego de Sá Dias
   Larissa Hitomi Morine Cortegana

*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

int usuario = 0;
bool interface = HIGH;
int input1;
float input2 = 0;
char botao;
bool statusBotao = LOW;
bool statusLed = LOW;
bool statusBuzzer = LOW;


//          Declaração de Constantes (pinos)


void setup() {
  // Inicializa a comunicação serial a 9600 bps
  Serial.begin(9600);
  Serial2.begin(9600);

  lcd.init();
  lcd.setBacklight(HIGH);

  // Escreve o estado do LED
  lcd.setCursor(0, 0); // Posição: coluna 0, linha 0
  lcd.print("Led: ");
  lcd.print(statusLed == HIGH ? "HIGH" : "LOW");

  // Escreve o estado do botao
  lcd.setCursor(0, 1); // Posição: coluna 0, linha 1
  lcd.print("Botao: ");
  lcd.print(statusBotao == HIGH ? "HIGH" : "LOW");

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  // Envia uma mensagem para o Arduino Escravo
  //Serial2.println("Olá, Escravo!");

  //caso o B mande 1, o botão está sendo apertado
  if (Serial2.available() == true) {
    botao = Serial2.parseInt();
  }

  if (botao == 1) {
    statusBotao = HIGH;
    Serial.println(statusBotao);
    botao = 0;
    // Move para a próxima linha se necessário (dependerá do tamanho do display)
    // Aqui, por simplicidade, vamos atualizar a mesma linha
    lcd.setCursor(0, 1); // Posição: coluna 0, linha 2 (se houver)
    lcd.print("Botao: ");
    lcd.print(statusBotao == HIGH ? "ON" : "OFF");
  } else {
    statusBotao = LOW;
    // Move para a próxima linha se necessário (dependerá do tamanho do display)
    // Aqui, por simplicidade, vamos atualizar a mesma linha
    lcd.setCursor(0, 1); // Posição: coluna 0, linha 2 (se houver)
    lcd.print("Botao: ");
    lcd.print(statusBotao == HIGH ? "ON" : "OFF");
  }

  // ---------------------------------------------------------------------------------------------------------------------------------------------------------
  //switch para as interfaces
  //Serial.println(usuario);
  switch (usuario) {
    case 0:
      if (interface) {
        Serial.println("Selecione uma opção:");
        Serial.println("(1) Ver Status do Botão");
        Serial.println("(2) Ver Status do LED");
        Serial.println("(3) Ver Status do Buzzer");
        Serial.println("(4) Ver Temperatura");
        Serial.println("(5) Ligar/Desligar LED");
        Serial.println("(6) Ligar/Desligar Buzzer");
        Serial.println("(7) Ligar/Desligar Notificações no Display\n");
        interface = LOW;
      }

      if (Serial.available() > 0) {
        input1 = Serial.parseInt();
        Serial.println(input1);
        usuario = 1; //caso o usuário tenha digitado algo a gente pega esse valor e vai para interface de mostragem
      }

      break;


    case 1:
      switch (input1) {
        case 1:
          Serial.print("O botão está no status: ");
          Serial.println(statusBotao);
          usuario = 0;
          interface = HIGH;
          input1 = 0;
          break;

        case 2:
          Serial.print("O LED está no status: ");
          Serial.println(statusLed);
          usuario = 0;
          interface = HIGH;
          input1 = 0;
          break;

        case 3:
          Serial.print("O Buzzer está no status: ");
          Serial.println(statusBuzzer);
          usuario = 0;
          interface = HIGH;
          input1 = 0;
          break;

        case 4:

          Serial2.print(5);//requisição da temperatura

          if (Serial2.available() == true) {
            Serial.println("Estou aqui");
            input2 = Serial2.parseFloat(); //armazeno a temperatura em uma String
            Serial.print("A temperatura média é de: ");
            Serial.println(input2);
            usuario = 0;
            interface = HIGH;

            //Mostrar a temperatura no Display
            lcd.setCursor(0, 1); // Posição: coluna 0, linha 0]
            lcd.print("Temperatura média: ");
            lcd.print(input2);
            lcd.println(" °C");
            input1 = 0;

          }

          break;

        case 5:
          statusLed = !statusLed;
          if (statusLed == HIGH) {
            Serial2.println(1); //se for high eu mando ligar
            usuario = 0;
            input1 = 0;
            interface = HIGH;
          } else {
            Serial2.println(2); //se for low eu mando desligar
            Serial.println("Entrei aqui para desligar o LED! \n");
            usuario = 0;
            input1 = 0;
            interface = HIGH;
          }

          //atualizar no LCD
          // Escreve o estado do buzzer
          lcd.setCursor(0, 0); // Posição: coluna 0, linha 1
          lcd.print("Led: ");
          lcd.print(statusLed == HIGH ? "ON " : "OFF");


          break;

        case 6:
          statusBuzzer = !statusBuzzer;
          if (statusBuzzer == HIGH) {
            Serial2.println(3); //se for high eu mando ligar
            usuario = 0;
            interface = HIGH;
          } else {
            Serial.println("Estou aqui pedindo para desligar");
            Serial2.println(4); //se for low eu mando desligar
            usuario = 0;
            interface = HIGH;
          }

          //atualizar no LCD
          // Escreve o estado do buzzer
          lcd.setCursor(0, 0); // Posição: coluna 0, linha 1
          lcd.print("Buzzer: ");
          lcd.print(statusBuzzer == HIGH ? "ON " : "OFF");

          break;
      }
      break;
  }

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------

// procedimento para requisitar a média da temperatura e printá-la
void pedirMediaTemperatura() {

}
