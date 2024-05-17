/*
  Disciplina: Sistemas Embarcados
  Título: Código para Batalha Naval
  Aluno: Diego de Sá Dias
  RA: 156389

*/

//--------------------------------------------------------
//        Definições dos pinos
//Foi definido como 1,2,3 da esquerda para direita
const int led1 = 7;
const int led2 = 6;
const int led3 = 5;
const int botao1 = 13;
const int botao2 = 12;
const int botao3 = 11;
//--------------------------------------------------------
//        Definição das variáveis
int usuario = 10; // variável do usuário

//botões
int estado_botao1 = 0;//variavel do estado do botao 1
int estado_botao2 = 0;//variavel do estado do botao 2
int estado_botao3 = 0;//variavel do estado do botao 3
int estado_botao1_anterior = LOW;
int estado_botao2_anterior = LOW;
int estado_botao3_anterior = LOW;
int pressionado1 = false;
int pressionado2 = false;
int pressionado3 = false;
int linha_escolhida; //linha aleatoria em que o arduino irá atacar
int coluna_escolhida; //coluna aleatoria em que o arduino irá atacar
int contador = 0; //variável de controle para ver quantas posições foram abatidas
int contador1 = 0; //variável para auxiliar na contagem do while
int contador2 = 0; //variável para auxiliar na contagem do while
int torpedeiro_abatido = 0; //variável de controle para ver se otorpedeiro foi abatido
int submarino_abatido = 0; //variável de controle para ver se o submarino foi abatido
int estado_led_pisca = HIGH; //variável que irá trocar o estado do LED para fazer ele piscar
long unsigned lastDebounceTime1 = 0; //ultimo tempo para o botao1
long unsigned lastDebounceTime2 = 0; //ultimo tempo para o botao2
long unsigned lastDebounceTime3 = 0; //ultimo tempo para o botao3
long unsigned debounce_delay = 50; //tempo do debounce
long unsigned led_pisca = 0; //variável para armazenar o tempo no qual o LED permanecerá piscando
long unsigned intervalo_pisca = 0; //variável que irá determinar o intervalo entre as piscadas do LED
long int time_waiting = 0; //variável para o tempo de espera para o jogador jogar
long unsigned previousMillis = 0;


//variáveis que irão controlar quando cada opção foi acionada na Interface de Posição dos Navios
bool torpedeiro = false;
bool submarino = false;
bool retornar = false;

//variáveis auxiliares
bool inicial = false; //Variável que irá determinar se está ou não no Menu Inicial
bool selecao = true; //Variável que irá determinar se está ou não no Menu Seleção
bool posicao = false; //Variável que irá determinar se está ou não no Menu Posição Navios
bool arduino_torpedeiro = true; //Variável que irá determinar se o arduino já escolheu ou não a posição do torpedeiro
bool arduino_submarino = true; //Variável que irá determinar se o arduino já escolheu ou não a posição do submarino
bool printar = false; // variável para printar as coisas
bool invalido_submarino = true; //quando o valor do submarino colocado é inválido
bool final_jogada = false; // variável para falar que passou os 30 s da jogada
bool usuario_venceu = false; //Variável de controle se o usuário venceu ou nao
bool arduino_venceu = false; //Variável de controle se o Arduino venceu ou nao
bool selecionando_ataque = false;
bool controle_while = true;

//variaveis das posicoes dos navios do usuário (arrays de 10)
int usuario_posicoes_torpedeiro[4] = {10, 10, 10, 10}; //(2x1)
int usuario_posicoes_submarino[6] = {10, 10, 10, 10, 10, 10}; //(3x1)
int controle_usuario_posicoes_torpedeiro[4] = {10, 10, 10, 10}; //(2x1)
int controle_usuario_posicoes_submarino[6] = {10, 10, 10, 10, 10, 10}; //(3x1)

//variáveis das posicoes dos navios do arduino  (arrays de 10)
int arduino_posicoes_torpedeiro[4] = {10, 10, 10, 10}; //(2x1)
int arduino_posicoes_submarino[6] = {10, 10, 10, 10, 10, 10}; //(3x1)
int controle_arduino_posicoes_torpedeiro[4] = {10, 10, 10, 10}; //(2x1)
int controle_arduino_posicoes_submarino[6] = {10, 10, 10, 10, 10, 10}; //(3x1)

//variáveis das posições de ataque do arduino e do usuário
int usuario_posicoes_ataque[2] = {10, 10};
int arduino_posicoes_ataque[2] = {10, 10};


//Matriz do tabuleiro do arduino
char tabuleiro_arduino[5][5] = {
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'}
};

//Matriz do tabuleiro do usuário
char tabuleiro_usuario[5][5] = {
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'},
  {'?', '?', '?', '?', '?'}
};


//Matriz das posições já escolhidas pelo arduino
bool posicoesEscolhidas[5][5] = {
  {false, false, false, false, false},
  {false, false, false, false, false},
  {false, false, false, false, false},
  {false, false, false, false, false},
}; // Matriz para manter o registro das posições já escolhidas

//--------------------------------------------------------



//--------------------------------------------------------
void setup() {
  //        Definindo os tipos dos pinos
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  Serial.begin(9600);
  //--------------------------------------------------------
  //        Começando na Interface Inicial
  interfaceInicial();
  inicial = true;
}

void loop() {
  //                                Debounce
  //        Botão 1
  int lendo1 = digitalRead(botao1);
  if (lendo1 != estado_botao1_anterior) {
    lastDebounceTime1 = millis();
    estado_botao1_anterior = !estado_botao1_anterior;
  }

  if ((millis() - lastDebounceTime1) > debounce_delay) {
    if (lendo1 != estado_botao1) {
      estado_botao1 = lendo1; // variável que armazena se o botao foi ou nao apertado

      if (estado_botao1 == HIGH) {
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
      if (estado_botao2 == HIGH) {
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
      if (estado_botao3 == HIGH) {
        //Aqui eu vou fazer algo quando o botão for apertado
        Serial.println("Botao 3 apertado");
        pressionado3 = true;
        estado_botao3_anterior = botao3;
      }
    }
  }

  //Se estiver no menu inicial e o botão 1 for pressionado eu tenho que ir para a seleção do usuário
  if (inicial && pressionado1) {
    usuario = 0;
    inicial = false;
    pressionado1 = false;
  }
  //-----------------------------------------------------------------------------------------------------------------------------
  //                                casos do switch para as opções do usuário (irá mudar as interfaces)
  switch (usuario) {
    case 0:
      //caso de inicialização do usuário

      //      Interface de Seleção

      //Todos os LED's acesos
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);

      //Se eu estiver querendo selecionar, então eu printo a mensagem de selecao
      if (selecao) {
        //Imprimir no Monitor Serial as opções da Interface de Seleção
        Serial.println("Pressione o botao (1) para escolher as posicoes dos navios");
        Serial.println("Pressione o botao (2) para iniciar o jogo\n");
        selecao = false;// selecao se torna true para eu conseguir testar o botao 1
      }

      if (pressionado1) {
        usuario = 1; // vou para interface de seleção dos navios
        posicao = true;
        pressionado1 = false;
      }

      //caso o botao dois seja pressionado, temos que ir para o caso de inicialização do jogo
      if (pressionado2) {
        usuario = 2;
        pressionado2 = false;
      }

      break;

    case 1:
      //      Interface de Posição de Navios

      //Todos os LED's acesos
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      if (posicao) {
        Serial.println("Pressione o botao (1) para  escolher a posicao do torpedeiro");
        Serial.println("Pressione o botao (2) para  escolher a posicao do submarino");
        Serial.println("Pressione o botao (3) para  Retornar\n");
        posicao = false;
      }

      if (pressionado1) {
        //Escolher o navio torpedeiro
        Serial.println("Voce selecionou o navio torpedeiro (tamanho 2x1), escolha as duas posicoes (inicial e final) em um tabuleiro 5X5.");
        torpedeiro = true;//variável dizendo que o torpedeiro foi ativo
        pressionado1 = false;
        usuario_posicoes_torpedeiro[3] = 10;
      } else if (pressionado2) {
        Serial.println("Voce selecionou o submarino (tamanho 3x1), escolha as duas posicoes (inicial e final) em um tabuleiro 5X5.");
        submarino = true;
        pressionado2 = false;
        usuario_posicoes_submarino[3] = 10;
      } else if (pressionado3) {
        retornar = true;
        pressionado3 = false;
      }


      //caso torpedeiro tenha sido selecionado
      if (torpedeiro) {
        //pegando os valores da posição dos navios
        /*
          while(Serial.available() > 0){//flush
          Serial.read();
          }
        */
        if (Serial.available() == true) {
          usuario_posicoes_torpedeiro[0] = Serial.parseInt();
          usuario_posicoes_torpedeiro[1] = Serial.parseInt();
          usuario_posicoes_torpedeiro[2] = Serial.parseInt();
          usuario_posicoes_torpedeiro[3] = Serial.parseInt();
        }
        if (usuario_posicoes_torpedeiro[3] != 10 && torpedeiro) {
          Serial.println("\nVoce escolheu as seguintes posicoes do torpedeiro:");
          Serial.println("Posicao Inicial: ");
          Serial.print(usuario_posicoes_torpedeiro[0]);
          Serial.print(" x ");
          Serial.print(usuario_posicoes_torpedeiro[1]);
          Serial.println("\nPosicao Final: ");
          Serial.print(usuario_posicoes_torpedeiro[2]);
          Serial.print(" x ");
          Serial.print(usuario_posicoes_torpedeiro[3]);
          Serial.println("\n");
          //armazenando a posição em um vetor de controle para que eu consiga plotar as posições
          for (int i = 0; i < 4; i++) {
            controle_usuario_posicoes_torpedeiro[i] = usuario_posicoes_torpedeiro[i];
          }
          torpedeiro = false;
          //Printando a Interface de Posição dos Navios
          posicao = true;
        }
      }

      //caso submarino tenha sido selecionado
      if (submarino) {
        /*
          while(Serial.available() > 0){
          Serial.read();
          }
        */
        if (Serial.available() == true) {
          usuario_posicoes_submarino[0] = Serial.parseInt();
          usuario_posicoes_submarino[1] = Serial.parseInt();
          usuario_posicoes_submarino[4] = Serial.parseInt();
          usuario_posicoes_submarino[5] = Serial.parseInt();
        }
        if (usuario_posicoes_submarino[0] == usuario_posicoes_submarino[4]) {
          //se a linha for igual, eu decremento na coluna
          usuario_posicoes_submarino[3] = usuario_posicoes_submarino[5] - 1;
        } else if (usuario_posicoes_submarino[1] == usuario_posicoes_submarino[5]) {
          //se a coluna for igual, eu decremento na linha
          usuario_posicoes_submarino[2] = usuario_posicoes_submarino[4] - 1;
        } else {
          //se nem a linha nem a coluna forem iguais, tem erro!
          Serial.println("DIGITE UM VALOR VÁLIDO!");
          invalido_submarino = false;
        }
      }
      //testando para ver se o usuário ja terminou de digitar tudo
      if (usuario_posicoes_submarino[5] != 10 && submarino && invalido_submarino) {
        Serial.println("\nVoce escolheu as seguintes posicoes do submarino:");
        Serial.println("Posicao Inicial: ");
        Serial.print(usuario_posicoes_submarino[0]);
        Serial.print(" x ");
        Serial.print(usuario_posicoes_submarino[1]);
        Serial.println("\nPosicao Final: ");
        Serial.print(usuario_posicoes_submarino[4]);
        Serial.print(" x ");
        Serial.print(usuario_posicoes_submarino[5]);
        Serial.println("\n");
        //armazenando a posição em um vetor de controle para que eu consiga plotar as posições
        for (int i = 0; i < 6; i++) {
          controle_usuario_posicoes_submarino[i] = usuario_posicoes_submarino[i];
        }
        submarino = false;
        //Printando a Interface de Posição dos Navios
        posicao = true;
      }

      //caso retornar seja selecionado
      if (retornar) {
        //teremos que voltar para a Interface de Seleção.
        usuario = 0;
        selecao = true; //volto o seleção para true para poder aparecer a mensagem
      }

      break;

    case 2:
      //caso em que o jogo foi iniciado

      //      Interface de Jogo Iniciado

      //Arduino escolhe as posicoes do torpedeiro e do submarino dele

      //Torpedeiro
      if (arduino_torpedeiro) {
        //Gerar a primeira posicao do torpedeiro
        arduino_posicoes_torpedeiro[0] = random(0, 5);
        arduino_posicoes_torpedeiro[1] = random(0, 5);
        int lin_col = random(1, 7);
        //Segunda posição do torpedeiro
        /*para gerar a segunda posição, preciso ver se vou incrementar na linha ou na coluna,
           por isso vou gerar um numero aleatório entre 1 e 6 e se o número for ímpar eu vou
           incrementar na linha, se for par incrementarei na coluna
        */
        if (lin_col % 2 == 0) {
          //o número aleatório é par então eu incremento na coluna
          arduino_posicoes_torpedeiro[3] = arduino_posicoes_torpedeiro[1] + 1;
          arduino_posicoes_torpedeiro[2] = arduino_posicoes_torpedeiro[0];
          //se o valor for maior que 4, eu tenho que decrementar
          if (arduino_posicoes_torpedeiro[3] > 4) {
            arduino_posicoes_torpedeiro[3] = arduino_posicoes_torpedeiro[1] - 1;
          }
        } else {
          //o número aleatório é impar então eu incremento na linha
          arduino_posicoes_torpedeiro[2] = arduino_posicoes_torpedeiro[0] + 1;
          arduino_posicoes_torpedeiro[3] = arduino_posicoes_torpedeiro[1];
          //se o valor for maior que 4, eu tenho que decrementar
          if (arduino_posicoes_torpedeiro[2] > 4) {
            arduino_posicoes_torpedeiro[2] = arduino_posicoes_torpedeiro[0] - 1;
          }
        }
        //caso a última posição seja diferente de 10, então eu irei parar de sortear as posições do torpedeiro
        if (arduino_posicoes_torpedeiro[3] != 10) {
          arduino_torpedeiro = false;
        }
      }

      //submarino
      if (arduino_submarino) {
        //Gerar a primeira posicao do submarino
        arduino_posicoes_submarino[0] = random(0, 5);
        arduino_posicoes_submarino[1] = random(0, 5);
        //Segunda posição do submarino
        /*para gerar a segunda posição, preciso ver se vou incrementar na linha ou na coluna,
           por isso vou gerar um numero aleatório entre 1 e 6 e se o número for ímpar eu vou
           incrementar na linha, se for par incrementarei na coluna
        */
        int lin_col = random(1, 7);
        if (lin_col % 2 == 0) {
          //o número aleatório é par então eu incremento na coluna
          arduino_posicoes_submarino[5] = arduino_posicoes_submarino[1] + 2;
          arduino_posicoes_submarino[4] = arduino_posicoes_submarino[0];
          arduino_posicoes_submarino[2] = arduino_posicoes_submarino[0];
          arduino_posicoes_submarino[3] = arduino_posicoes_submarino[5] + 1;
          //se o valor for maior que 4, eu tenho que decrementar
          if (arduino_posicoes_submarino[5] > 4) {
            arduino_posicoes_submarino[5] = arduino_posicoes_submarino[1] - 2;
            arduino_posicoes_submarino[3] = arduino_posicoes_submarino[5] + 1;
          }
        } else {
          //o número aleatório é impar então eu incremento na linha
          arduino_posicoes_submarino[4] = arduino_posicoes_submarino[0] + 2;
          arduino_posicoes_submarino[5] = arduino_posicoes_submarino[1];
          arduino_posicoes_submarino[2] = arduino_posicoes_submarino[4] + 1;
          arduino_posicoes_submarino[3] = arduino_posicoes_submarino[1];
          //se o valor for maior que 4, eu tenho que decrementar
          if (arduino_posicoes_submarino[4] > 4) {
            arduino_posicoes_submarino[4] = arduino_posicoes_submarino[0] - 2;
            arduino_posicoes_submarino[2] = arduino_posicoes_submarino[4] + 1;
          }
        }

        //caso a última posição seja diferente de 10, então eu irei parar de sortear as posições do submarino
        if (arduino_posicoes_submarino[5] != 10) {
          arduino_submarino = false;
          printar = true;
        }
      }

      if (printar) {
        Serial.println("\nPOSICOES DO ARDUINO:");
        Serial.println("Posicoes torpedeiro:");
        Serial.println("Posicao Inicial: ");
        Serial.print(arduino_posicoes_torpedeiro[0]);
        Serial.print(" x ");
        Serial.print(arduino_posicoes_torpedeiro[1]);
        Serial.println("\nPosicao Final: ");
        Serial.print(arduino_posicoes_torpedeiro[2]);
        Serial.print(" x ");
        Serial.print(arduino_posicoes_torpedeiro[3]);
        Serial.println("\n");

        Serial.println("Posicoes submarino:");
        Serial.println("Posicao Inicial: ");
        Serial.print(arduino_posicoes_submarino[0]);
        Serial.print(" x ");
        Serial.print(arduino_posicoes_submarino[1]);
        Serial.println("\n");
        Serial.println("Posicao do meio: ");
        Serial.print(arduino_posicoes_submarino[2]);
        Serial.print(" x ");
        Serial.print(arduino_posicoes_submarino[3]);
        Serial.println("\nPosicao Final: ");
        Serial.print(arduino_posicoes_submarino[4]);
        Serial.print(" x ");
        Serial.print(arduino_posicoes_submarino[5]);
        Serial.println("\n");

        //armazenando a posição em um vetor de controle para que eu consiga plotar as posições
        for (int i = 0; i < 4; i++) {
          controle_arduino_posicoes_torpedeiro[i] = arduino_posicoes_torpedeiro[i];
        }
        for (int i = 0; i < 6; i++) {
          controle_arduino_posicoes_submarino[i] = arduino_posicoes_submarino[i];
        }
        //não printo mais a posição do arduino
        printar = false;
      }


      //vez do jogador
      //acender o LED 1 e digitar no monitor serial "Digite a coordenada alvo"
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      Serial.println("O jogo comecou!");
      Serial.println("Digite a coordenada alvo");
      time_waiting = millis() + 30000; //O tempo de espera para a jogada tem que ser o tempo atual mais 30.000 ms
      usuario = 3;

      break;

    case 3:
      //      Caso em que o jogador irá jogar

      //caso pressione o botao 3 acabar o jogo
      if (pressionado3) {
        //encerra o jogo
        final_jogada = true;
        usuario = 6;
        led_pisca = millis() + 3000; // intervalo em que o LED irá permanecer piscando
        intervalo_pisca  = millis() + 500; // intervalo em que o LED irá trocar de estado (ligado/desligado)
        Serial.println("\nJogo finalizado pelo usuário!");
        pressionado3 = false;
      }
      //pressiona o botao 1 mostra o tabuleiro
      if (pressionado1) {

        //se o botao1 for pressionado no meio da jogada, devo mostrar as posições de ataque do usuário e do Arduino

        //printando o tabuleiro de ataque (defesa do arduino)
        Serial.println("\nTabuleiro de ataque (tabuleiro do arduino)");
        for (int i = 0; i <= 4; i++) {
          for (int j = 0; j <= 4; j++) {
            Serial.print(tabuleiro_arduino[i][j]);
            Serial.print(" ");
          }
          Serial.println();
        }
        //printando o tabuleiro de defesa (ataque do arduino)
        Serial.println("\nTabuleiro de defesa (tabuleiro do usuário)");
        for (int i = 0; i <= 4; i++) {
          for (int j = 0; j <= 4; j++) {
            Serial.print(tabuleiro_usuario[i][j]);
            Serial.print(" ");
          }
          Serial.println();
        }
        pressionado1 = false;
      }

      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);

      if (millis() >= time_waiting) {
        //final_jogada = true;
        Serial.println("Perdeu a vez!\n");
        usuario_posicoes_ataque[0] = 10;
        usuario_posicoes_ataque[1] = 10;
        usuario = 4;
      }
      //pegando os valores que o usuário digitou
      if (!final_jogada) {
        selecionando_ataque = true;
        //input jogador
        /*
          while(Serial.available() > 0){
          Serial.read();
          }
        */
        if (Serial.available() == true) {
          usuario_posicoes_ataque[0] = Serial.parseInt();
          usuario_posicoes_ataque[1] = Serial.parseInt();
          Serial.println("Voce escolheu:");
          Serial.println(usuario_posicoes_ataque[0]);
          Serial.println(usuario_posicoes_ataque[1]);
        }
        if (usuario_posicoes_ataque[1] != 10) {
          selecionando_ataque = false;
        }
        //Ver se o jogador acertou ou nao o alvo
        //Caso o jogador tenha acertado as duas posições, ele acertou um alvo
        if (!selecionando_ataque) { // quando o usuário terminar de selecionar o ataque, irei fazer a verificação se acertou ou não
          if (usuario_posicoes_ataque[0] == arduino_posicoes_torpedeiro[0] && usuario_posicoes_ataque[1] == arduino_posicoes_torpedeiro[1]) {
            tabuleiro_arduino[usuario_posicoes_ataque[0]][usuario_posicoes_ataque[1]] = 'X';//atualizando a matriz falando que aquele lugar que ele atacou tem alvo
            arduino_posicoes_torpedeiro[0] = -1;
            arduino_posicoes_torpedeiro[1] = -1;
            Serial.println("Alvo Localizado! (Usuario)");
            //resetando para 10 o valor da variável de ataque
            usuario_posicoes_ataque[0] = 10;
            usuario_posicoes_ataque[1] = 10;
            usuario = 4;
          } else {
            if (usuario_posicoes_ataque[0] == arduino_posicoes_torpedeiro[2] && usuario_posicoes_ataque[1] == arduino_posicoes_torpedeiro[3]) {
              tabuleiro_arduino[usuario_posicoes_ataque[0]][usuario_posicoes_ataque[1]] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
              arduino_posicoes_torpedeiro[2] = -1;
              arduino_posicoes_torpedeiro[3] = -1;
              Serial.println("Alvo Localizado! (Usuario)");
              //resetando para 10 o valor da variável de ataque
              usuario_posicoes_ataque[0] = 10;
              usuario_posicoes_ataque[1] = 10;
              usuario = 4;
            } else {
              //O usuário errou o torpedeiro iremos testar o submarino
              if (usuario_posicoes_ataque[0] == arduino_posicoes_submarino[0] && usuario_posicoes_ataque[1] == arduino_posicoes_submarino[1]) {
                tabuleiro_arduino[usuario_posicoes_ataque[0]][usuario_posicoes_ataque[1]] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
                arduino_posicoes_submarino[0] = -1;
                arduino_posicoes_submarino[1] = -1;
                Serial.println("Alvo Localizado! (Usuario)");
                //resetando para 10 o valor da variável de ataque
                usuario_posicoes_ataque[0] = 10;
                usuario_posicoes_ataque[1] = 10;
                usuario = 4;
              } else {
                if (usuario_posicoes_ataque[0] == arduino_posicoes_submarino[2] && usuario_posicoes_ataque[1] == arduino_posicoes_submarino[3]) {
                  tabuleiro_arduino[usuario_posicoes_ataque[0]][usuario_posicoes_ataque[1]] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
                  arduino_posicoes_submarino[2] = -1;
                  arduino_posicoes_submarino[3] = -1;
                  Serial.println("Alvo Localizado! (Usuario)");
                  //resetando para 10 o valor da variável de ataque
                  usuario_posicoes_ataque[0] = 10;
                  usuario_posicoes_ataque[1] = 10;
                  usuario = 4;
                } else {
                  if (usuario_posicoes_ataque[0] == arduino_posicoes_submarino[4] && usuario_posicoes_ataque[1] == arduino_posicoes_submarino[5]) {
                    tabuleiro_arduino[usuario_posicoes_ataque[0]][usuario_posicoes_ataque[1]] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
                    arduino_posicoes_submarino[4] = -1;
                    arduino_posicoes_submarino[5] = -1;
                    Serial.println("Alvo Localizado! (Usuario)");
                    //resetando para 10 o valor da variável de ataque
                    usuario_posicoes_ataque[0] = 10;
                    usuario_posicoes_ataque[1] = 10;
                    usuario = 4;
                  } else {
                    //Nenhum alvo foi localizado, passando para a vez do arduino
                    Serial.println("Alvo Perdido! (Usuario)");
                    if ( tabuleiro_arduino[usuario_posicoes_ataque[0]][usuario_posicoes_ataque[1]] != 'X') {
                      tabuleiro_arduino[usuario_posicoes_ataque[0]][usuario_posicoes_ataque[1]] = 'O'; //atualizando a matriz falando que aquele lugar que ele atacou não tem nada ("O" simboliza sem alvo)
                    }
                    //resetando para 10 o valor da variável de ataque
                    usuario_posicoes_ataque[0] = 10;
                    usuario_posicoes_ataque[1] = 10;
                    usuario = 4;
                  }
                }
              }
            }
          }

          //teste para ver se o usuário ganhou
          //verificando se o usuário derrubou o torpedeiro
          contador = 0;
          torpedeiro_abatido = 0;
          for (int i = 0; i <= 3; i++) {
            if (arduino_posicoes_torpedeiro[i] == -1) {
              contador++;
            }
          }
          if (contador == 4) {
            Serial.println("Torpedeiro abatido!");
            torpedeiro_abatido = 1;
          }

          //verificando se o usuário derrubou o submarino
          contador = 0;
          submarino_abatido = 0;
          for (int i = 0; i <= 5; i++) {
            if (arduino_posicoes_submarino[i] == -1) {
              contador++;
            }
          }
          if (contador == 6) {
            Serial.println("Submarino abatido!");
            submarino_abatido = 1;
          }

          if (torpedeiro_abatido == 1 && submarino_abatido == 1) {
            usuario_venceu = true;
            usuario = 5;
          }
        }

      }

      break;

    case 4:
      //jogada do arduino

      //escolhendo aleatoriamente uma linha e uma coluna
      linha_escolhida = random(5);
      coluna_escolhida = random(5);
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      final_jogada = false;
      while (posicoesEscolhidas[linha_escolhida][coluna_escolhida]) {
        // se a posição já foi escolhida, gere novos números aleatórios
        linha_escolhida = random(5);
        coluna_escolhida = random(5);
      }

      //armazenando que aquela posição já foi
      posicoesEscolhidas[linha_escolhida][coluna_escolhida] = true;

      //fazendo o teste para ver se acertou algum alvo
      if ((linha_escolhida == usuario_posicoes_torpedeiro[0]  && coluna_escolhida == usuario_posicoes_torpedeiro[1])) {
        Serial.println("Alvo Localizado! (Arduino)\n");
        tabuleiro_usuario[linha_escolhida][coluna_escolhida] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
        usuario_posicoes_torpedeiro[0] = -1;
        usuario_posicoes_torpedeiro[1] = -1;
        usuario = 3;
        Serial.println("Digite a coordenada alvo");
        time_waiting = millis() + 30000; //O tempo de espera para a jogada tem que ser o tempo atual mais 30.000 ms
      } else {
        if ((linha_escolhida == usuario_posicoes_torpedeiro[2] && coluna_escolhida == usuario_posicoes_torpedeiro[3])) {
          Serial.println("Alvo Localizado! (Arduino)\n");
          tabuleiro_usuario[linha_escolhida][coluna_escolhida] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
          usuario_posicoes_torpedeiro[2] = -1;
          usuario_posicoes_torpedeiro[3] = -1;
          usuario = 3;
          Serial.println("Digite a coordenada alvo");
          time_waiting = millis() + 30000; //O tempo de espera para a jogada tem que ser o tempo atual mais 30.000 ms
        } else {
          if ((linha_escolhida == usuario_posicoes_submarino[0]  && coluna_escolhida == usuario_posicoes_submarino[1])) {
            Serial.println("Alvo Localizado! (Arduino)\n");
            tabuleiro_usuario[linha_escolhida][coluna_escolhida] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
            usuario_posicoes_submarino[0] = -1;
            usuario_posicoes_submarino[1] = -1;
            usuario = 3;
            Serial.println("Digite a coordenada alvo");
            time_waiting = millis() + 30000; //O tempo de espera para a jogada tem que ser o tempo atual mais 30.000 ms
          } else {
            if ((linha_escolhida == usuario_posicoes_submarino[2] && coluna_escolhida == usuario_posicoes_submarino[3])) {
              Serial.println("Alvo Localizado! (Arduino)\n");
              tabuleiro_usuario[linha_escolhida][coluna_escolhida] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
              usuario_posicoes_submarino[2] = -1;
              usuario_posicoes_submarino[3] = -1;
              usuario = 3;
              Serial.println("Digite a coordenada alvo");
              time_waiting = millis() + 30000; //O tempo de espera para a jogada tem que ser o tempo atual mais 30.000 ms
            }  else {
              if ((linha_escolhida == usuario_posicoes_submarino[4] && coluna_escolhida == usuario_posicoes_submarino[5])) {
                Serial.println("Alvo Localizado! (Arduino)\n");
                tabuleiro_usuario[linha_escolhida][coluna_escolhida] = 'X'; //atualizando a matriz falando que aquele lugar que ele atacou tem alvo
                usuario_posicoes_submarino[4] = -1;
                usuario_posicoes_submarino[5] = -1;
                usuario = 3;
                Serial.println("Digite a coordenada alvo\n");
                time_waiting = millis() + 30000; //O tempo de espera para a jogada tem que ser o tempo atual mais 30.000 ms
              } else {
                Serial.println("Alvo Perdido! (Arduino)\n");
                tabuleiro_usuario[linha_escolhida][coluna_escolhida] = 'O'; //atualizando a matriz falando que aquele lugar que ele atacou não tem nada ("O" simboliza sem alvo)
                usuario = 3;
                Serial.println("Digite a coordenada alvo\n");
                time_waiting = millis() + 30000; //O tempo de espera para a jogada tem que ser o tempo atual mais 30.000 ms
              }
            }
          }
        }
      }

      //teste para ver se o usuário ganhou
      //verificando se o usuário derrubou o torpedeiro
      contador = 0;
      torpedeiro_abatido = 0;
      for (int i = 0; i <= 3; i++) {
        if (usuario_posicoes_torpedeiro[i] == -1) {
          contador++;
        }
      }
      if (contador == 4) {
        Serial.println("Torpedeiro abatido!");
        torpedeiro_abatido = 1;
      }

      //verificando se o usuário derrubou o submarino
      contador = 0;
      submarino_abatido = 0;
      for (int i = 0; i <= 5; i++) {
        if (usuario_posicoes_submarino[i] == -1) {
          contador++;
        }
      }
      if (contador == 6) {
        Serial.println("Submarino abatido!");
        submarino_abatido = 1;
      }

      if (torpedeiro_abatido == 1 && submarino_abatido == 1) {
        arduino_venceu = true;
        usuario = 5;
      }
      delay(500); //Esse delay é apenas para jogada nao ser tão rapida
      break;

    case 5:
      //o jogo acabou e alguém venceu
      if (usuario_venceu) {
        Serial.println("\nParabéns!! Você acaba de vencer uma máquina e provar que elas \n ainda estão atrás na revolução das máquinas!");
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        //Printando o tabuleiro final
        Serial.println("\nTabuleiro de ataque (tabuleiro do arduino)");
        for (int i = 0; i <= 4; i++) {
          for (int j = 0; j <= 4; j++) {
            Serial.print(tabuleiro_arduino[i][j]);
            Serial.print(" ");
          }
          Serial.println();
        }
        //printando o tabuleiro de defesa (ataque do arduino)
        Serial.println("\nTabuleiro de defesa (tabuleiro do usuário)");
        for (int i = 0; i <= 4; i++) {
          for (int j = 0; j <= 4; j++) {
            Serial.print(tabuleiro_usuario[i][j]);
            Serial.print(" ");
          }
          Serial.println();
        }
        usuario_venceu = false;
      }
      if (arduino_venceu) {
        Serial.println("\nPô cara que decepção! Perdeu para o Arduíno! \n Se depender de você as máquinas vão dominar os seres humanos mesmo...");
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        //Printando o tabuleiro final
        Serial.println("\nTabuleiro de ataque (tabuleiro do arduino)");
        for (int i = 0; i <= 4; i++) {
          for (int j = 0; j <= 4; j++) {
            Serial.print(tabuleiro_arduino[i][j]);
            Serial.print(" ");
          }
          Serial.println();
        }
        //printando o tabuleiro de defesa (ataque do arduino)
        Serial.println("\nTabuleiro de defesa (tabuleiro do usuário)");
        for (int i = 0; i <= 4; i++) {
          for (int j = 0; j <= 4; j++) {
            Serial.print(tabuleiro_usuario[i][j]);
            Serial.print(" ");
          }
          Serial.println();
        }
        arduino_venceu = false;
      }
      break;

    case 6:
      //O jogo acaba por interrupção
      //Piscar os leds
      if (millis() <= led_pisca) {
        if (millis() <= intervalo_pisca) {
          digitalWrite(led1, estado_led_pisca);
          digitalWrite(led2, estado_led_pisca);
          digitalWrite(led3, estado_led_pisca);
        } else {
          //se o tempo de piscada passar de 500 ms, eu reinicio o intervalo e troco o estado do LED
          intervalo_pisca = millis() + 500;
          estado_led_pisca = !estado_led_pisca;
        }
      }


      break;
  }

}

//função de interface inicial
int interfaceInicial() {
  //        Interface inicial
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  Serial.println("Pressione o botao (1) para iniciar\n");
}
