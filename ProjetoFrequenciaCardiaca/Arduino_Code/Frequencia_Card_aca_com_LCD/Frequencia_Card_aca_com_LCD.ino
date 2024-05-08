// ----------------------------------------  VARIÁVEIS GLOBAIS E BIBLIOTECAS  ---------------------------------------- 
#include "MAX30100_PulseOximeter.h" //biblioteca sensor oximetro
#include <LiquidCrystal.h> //biblioteca LCD
LiquidCrystal lcd(3,4,5,10,11,12,13); //definindo os pinos do LCD em sequência
#define REPORTING_PERIOD_MS     1000 //tempo de 1 segundo para mostrar o batimento cardíaco 
PulseOximeter pox;

uint32_t tsLastReport = 0;
int botao1 = 6; //botão da esquerda 
int botao2 = 7; //botão da direita
int contador = 0; //variavél auxiliar tipo inteiro
String excel; //criando a String excel para armazenar os dados vindos do excel

// ----------------------------------------  FUNÇÃO DETECTOR DE BATIDAS  ---------------------------------------- 
void onBeatDetected()
{
    //Serial.println("Beat!");
}


// ----------------------------------------  FUNÇÃO SETUP  ---------------------------------------- 
void setup() {
  
   //definindo os pinos dos botões como entradas pull up
 pinMode(botao1, INPUT_PULLUP); 
 pinMode(botao2, INPUT_PULLUP); 
//-------------------------------------------------------------------------------------------------    
    //Configurações do Sensor de Frequência cardíaca
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
   lcd.begin(16,2);
   lcd.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        lcd.print("FAILED");
        for(;;);
    } else {
        lcd.print("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
//-------------------------------------------------------------------------------------------------     
    Serial.begin(115200); // iniciando o serial monitor com velocidade de 115200 (necessária para o funcionamento do sensor frenq. card.)
    bonjour(); //chamando a função bonjour 

}


// ----------------------------------------  FUNÇÃO DE LEITURA DO SENSOR  ---------------------------------------- 
void leitura(){
   pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) { //teste do sensor de acordo com a biblioteca
        
        lcd.clear(); // limpa o lcd
        
        //Escrita no LCD da leitura dos batimentos cardíacos e da oxigenação
          lcd.print("BPM:");
          lcd.print(pox.getHeartRate());
          lcd.print("bpm");
          lcd.setCursor(0,2);
          lcd.print("O2:");
          lcd.print(pox.getSpO2());
          lcd.print("%");

        //Escrita no monitor Serial o batimento e o nível de oxigenação (sepaados por vígula para o tratamento de dados no Excel)    
          Serial.print(pox.getHeartRate());
          Serial.print(",");
          Serial.print(pox.getSpO2());
          Serial.print(",");
          Serial.print("\n");          
        
        
        tsLastReport = millis();
    }
}


// ----------------------------------------  FUNÇÃO DE BOAS VINDAS  ---------------------------------------- 
void bonjour(){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Esquerdo para");
  lcd.setCursor(5,2);
  lcd.print("Medir");
}


// ----------------------------------------  FUNÇÃO DE BOAS VINDAS 2  ---------------------------------------- 
void bonjour2(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Pronto!");
  delay(800);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Direito para");
  lcd.setCursor(5,2);
  lcd.print("Enviar");
}


// ----------------------------------------  FUNÇÃO RESET ARDUINO ---------------------------------------- 
void (*funcReset) () = 0;


// ----------------------------------------  FUNÇÃO ADEUS ---------------------------------------- 
void adeus(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enviado!");
  lcd.setCursor(0,2);
  lcd.print("Adeus...");
}


// ----------------------------------------  FUNÇÃO LOOP ---------------------------------------- 
void loop(){       

  pox.update();

 
   if(digitalRead(botao1) == LOW){
        contador = 1;
   }
   if(digitalRead(botao2) == LOW){
        contador = 2;
   }
   
    //Se o contador for igual a 1 (apertou o botao1) mandar para o excel os batimentos através da função leitura que printa do monitor serial 
      if(contador == 1){
        leitura();
        
  // (Comunicação Excel --> Arduino) Se estiver escrito na Serial "e," e nova linha (modo em que o excel manda os dados para a serial) executa a função bonjour2           
    if(Serial.available() > 0){
       excel = Serial.readString();
       if(excel == "e,,\n"){
          bonjour2();
         contador = 0;
        }
       }

    //Enviar da planilha para o e-mail e reiniciar (Se o contador for igual a 2 (apertou o botao2) manda para o excel a letra "i" e reinicia o código  
        }else if(contador == 2){ 
          Serial.print(" , ,i \n"); //printa "i" na terceira coluna para que o excel execute o envio dos dados 
          adeus();
          delay(1500);
          funcReset();          
       }
                     
  }
 
