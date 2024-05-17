#include <IRremote.h>
//declarando todas as variáveis globais
int buzzer_pin = 11;
int RECV_PIN = 10;
int rele1 = 5; //Lâmpada (Lumus Máxima)
int rele2 = 4; //Lâmpada Buzzer
int rele3 = 3; //Lâmpada de errado
int rele4 = 2; //Ventilador

IRrecv irrecv(RECV_PIN);
decode_results res;

//definindo o valor de cada tecla do controle
#define tecla0 16738455
#define teclacelular 551489775
#define tecla1 16724175
#define tecla2 16718055
#define tecla3 1635910171
#define tecla4 16716015
#define tecla_pause 4294967295

//programação da música tema Harry Potter
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

int tempo = 400;

//Definindo os pinos como saída ou entrada
void setup() {
  pinMode(rele1, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode (rele2, OUTPUT);
  pinMode (rele3, OUTPUT);
  pinMode (rele4, OUTPUT);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
  digitalWrite(rele3, HIGH);
  digitalWrite (rele4, HIGH);
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void setTone(int pin, int note, int duration) {
  tone(pin, note, duration);
  delay(duration);
  noTone(pin);
}

void loop() {

  int estadorele1 = digitalRead(rele1);
  int estadorele2 = digitalRead(rele2);
  int estadorele3 = digitalRead(rele3);
  int estadorele4 = digitalRead(rele4);

  if (res.value == tecla3) {
    digitalWrite(rele1, HIGH);
    digitalWrite(rele2, HIGH);
    digitalWrite(rele3, HIGH);
    digitalWrite (rele4, HIGH);
  }
  //Se o valor do sinal for igual, então acontecerá algo
  if (irrecv.decode(&res)) {
    if (res.value == tecla0) {
      //Tocará a musica se a tecla for apertada
      digitalWrite (rele2, HIGH); setTone(buzzer_pin, Music.B, tempo * Length.one);
      digitalWrite (rele2, LOW);
      delay (1);
      setTone(buzzer_pin, Music.E, tempo * Length.one_half);
      digitalWrite (rele2, HIGH);
      delay (1);
      setTone(buzzer_pin, Music.G, tempo * Length.half);
      digitalWrite (rele2, LOW);
      delay (1);
      setTone(buzzer_pin, Music.F, tempo * Length.one);
      digitalWrite (rele2, HIGH);
      delay (1);
      setTone(buzzer_pin, Music.E, tempo * Length.two);
      digitalWrite (rele2, LOW);
      delay (1);
      setTone(buzzer_pin, Music.B2, tempo * Length.one);
      digitalWrite (rele2, HIGH);
      delay (1);
      setTone(buzzer_pin, Music.A2, tempo * Length.two_half);
      digitalWrite (rele2, LOW);
      delay (1);
      setTone(buzzer_pin, Music.Fs, tempo * Length.two_half);
      digitalWrite (rele2, HIGH);
      delay (1);
      setTone(buzzer_pin, Music.E, tempo * Length.one_half);
      digitalWrite (rele2, LOW);
      delay (1);
      setTone(buzzer_pin, Music.G, tempo * Length.half);
      digitalWrite (rele2, HIGH);
      delay (1);
      setTone(buzzer_pin, Music.F, tempo * Length.one);
      digitalWrite (rele2, LOW);
      delay (1);
      setTone(buzzer_pin, Music.Ds, tempo * Length.two);
      digitalWrite (rele2, HIGH);
      delay (1);
      setTone(buzzer_pin, Music.F, tempo * Length.one);
      digitalWrite (rele2, LOW);
      delay (1);
      setTone(buzzer_pin, Music.B, tempo * Length.two_half);
      digitalWrite (rele2, HIGH);
      delay(1);
    }

    //Se apertado o 1, tocara o som de errado (denied)
    if (res.value == tecla1) {
      tone(buzzer_pin, 19);
      digitalWrite (rele3, LOW);
      delay(1300);
      noTone(buzzer_pin);
      delay(10);
      digitalWrite (rele3, HIGH);
      tone(buzzer_pin, 24);
      digitalWrite (rele3, LOW);
      delay(1550);
      noTone(buzzer_pin);
      digitalWrite (rele3, HIGH);
      delay(1);
    }

    //Rele será ativado ao pressionar a tecla correspondente
    //Liga a lâmpada
    if (res.value == tecla2) {
      digitalWrite(rele1, !estadorele1);
    }
    //Liga o ventilador
    if (res.value == tecla4) {
      digitalWrite(rele4, !estadorele4);
    }
    irrecv.resume();
  }
  delay(100);
}
