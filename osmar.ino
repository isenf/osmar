#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "pitches.h"

/*  Valor do tempo da música */
#define TEMPO1 200
#define TEMPO2 200
#define TEMPO3 200
#define TEMPO4 200

/* BIBLIOTECA DO CABA Q FEZ: https://github.com/robsoncouto/arduino-songs/tree/master */
//          Melodia 1 -> nokia
const int melody[] PROGMEM = {
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};
const int notes = sizeof(melody) / sizeof(melody[0]) / 2;

//          Melodia 2 -> mario game over

const int melody2[] PROGMEM = {
  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2, 
};
const int notes2 = sizeof(melody2) / sizeof(melody2[0]) / 2;

//          Melodia 3 -> subway surfers
const int melody3[] PROGMEM = {
  NOTE_C4,4, REST,8, NOTE_G4,4, REST,8, NOTE_AS4,4, NOTE_C5,8, NOTE_AS4,8, REST,16, NOTE_F4,8, NOTE_DS4,8, REST,16,
  NOTE_C4,4, REST,8, NOTE_G4,4, REST,8, NOTE_AS4,4, NOTE_C5,8, NOTE_AS4,8, REST,16, NOTE_F4,8, NOTE_DS4,8, REST,16,
  NOTE_C4,4, REST,8, NOTE_G4,4, REST,8, NOTE_AS4,4, NOTE_C5,8, NOTE_AS4,8, REST,16, NOTE_F4,8, NOTE_DS4,8, REST,16,

  NOTE_C4,4, REST,8, NOTE_E4,4, REST,8, NOTE_G4,4, NOTE_A4,4, NOTE_AS4,4,
  NOTE_C5,8, REST,16, NOTE_C5,8, REST,16, NOTE_AS4,8, REST,16, NOTE_A4,8, REST,16,
  NOTE_AS4,8, REST,16, NOTE_AS4,8, NOTE_C5,8, REST,16, NOTE_AS4,8, NOTE_A4,8, REST,16,
  REST,4,
  NOTE_C5,8, REST,16, NOTE_AS4,8, REST,16, NOTE_A4,8, REST,16, NOTE_AS4,8, REST,4, NOTE_E5,8,
  REST,4,

  NOTE_C5,8, REST,16, NOTE_C5,8, REST,16, NOTE_AS4,8, REST,16, NOTE_A4,8, REST,16,
  NOTE_AS4,8, REST,16, NOTE_AS4,8, NOTE_C5,8, REST,16, NOTE_AS4,8, NOTE_A4,8, REST,16,
  REST,4,
  NOTE_C5,8, REST,16, NOTE_AS4,8, REST,16, NOTE_A4,8, REST,16, NOTE_AS4,8, REST,4, NOTE_E4,8,
  REST,1
};
const int notes3 = sizeof(melody3) / sizeof(melody3[0]) / 2;

//          Melodia 4 -> despacito
const int melody4[] PROGMEM= {
  NOTE_D5, 2, NOTE_CS5, 2, NOTE_B4, 4, NOTE_FS4, 4,
  NOTE_FS4, 8, NOTE_FS4, 8, NOTE_FS4, 8, NOTE_FS4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_B4, 4, NOTE_G4, 4,
  NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 4, NOTE_A4, 4,
  NOTE_A4, 8, NOTE_A4, 8, NOTE_A4, 8, NOTE_A4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_E4, 8, NOTE_E4, 4, NOTE_CS4, 2,
  0, 8,
};
const int notes4 = sizeof(melody4) / sizeof(melody4[0]) / 2;

#define pinoBuzzer 3

Servo ServoBOCA;
Servo ServoSE;
Servo ServoSD;

const int rgb1[3] = { 42, 40, 44 };
const int rgb2[3] = { 43, 41, 45 };

//qual a utilidade desse pot na A10?
#define potPin A10
#define servoPinB 2 
#define servoPinSE 4
#define servoPinSD 6

const byte pinLinhas[] = { 30, 31, 32, 33 };
const byte pinColunas[] = { 34, 35, 36 };
const char teclas[4][3] = { { '1', '2', '3' },
                            { '4', '5', '6' },
                            { '7', '8', '9' },
                            { '*', '0', '#' } };
char value[50];
char tecla_pressionada;
int numbers[2];
int rand_operator, final_value;

Keypad teclado = Keypad(makeKeymap(teclas), pinLinhas, pinColunas, 4, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pontuacao = 0;

int operacao_atual = 0;
const char* operacoes_simb[] = { " + ", " - ", " x ", " / " };
int nivel_dificuldade = 1;
int fase_atual = 1; // (1 a 5)
#define NUM_OPERACOES 4
#define MUD_FASE 5

//          Cria struct para dificuldade das operações e define cada nível de dificuldade (pode ser mudado dps)
struct DificuldadeOperacao{
  int val_min;
  int val_max;
};

DificuldadeOperacao dif_adi_sub[5] = {
  {1, 20},
  {10, 40},
  {20, 60},
  {30, 80},
  {50, 100}
};

DificuldadeOperacao dif_mult_div[5] = {
  {2, 5},
  {2, 8},
  {2, 11},
  {2, 15},
  {2, 20}
};

void setup() {
  ServoBOCA.attach(servoPinB);
  ServoSE.attach(servoPinSE);
  ServoSD.attach(servoPinSD);

  pinMode(pinoBuzzer, OUTPUT);

  for (int i = 0; i <= 2; i++) {
    pinMode(rgb1[i], OUTPUT);
    pinMode(rgb2[i], OUTPUT);
  }

  randomSeed(analogRead(A0));

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  operacao_atual = random(0, NUM_OPERACOES);
}


void loop() {
  /* Mapeamento de configuração dos servos para as expressões */
  ServoBOCA.write(map(analogRead(potPin), 0, 1023, 10, 170));
  ServoSE.write(map(analogRead(potPin), 0, 1023, 10, 170));
  ServoSD.write(map(analogRead(potPin), 0, 1023, 10, 170));

  Serial.print("Angulo: ");
  Serial.println(map(analogRead(potPin), 0, 1023, 10, 170));

  delay(150);

  int index = 0;

  //Laço para limpar o array  dos valores digitados pelo usuário
  for (int i = 0; i <= 49; i ++) {
    value[i] = '\0';
  }

  int result_operation = 0;
  bool eh_op_inv = false; //é operação inversa
  int temp_num1, temp_num2;

  switch(operacao_atual){
    case 0: //adicao
      numbers[0] = gerarNumAletorio(dif_adi_sub, fase_atual);
      numbers[1] = gerarNumAletorio(dif_adi_sub, fase_atual);

      result_operation = numbers[0] + numbers[1];
      break;
    case 1: //subtração
      temp_num1 = gerarNumAletorio(dif_adi_sub, fase_atual);
      temp_num2 = gerarNumAletorio(dif_adi_sub, fase_atual);

      result_operation = temp_num2;
      numbers[0] = temp_num1 + temp_num2;
      numbers[1] = temp_num1;

      break;
    
    case 2:
      numbers[0] = gerarNumAletorio(dif_mult_div, fase_atual);
      numbers[1] = gerarNumAletorio(dif_mult_div, fase_atual);

      result_operation = numbers[0] * numbers[1];
      break;

    case 3:
      temp_num1 = gerarNumAletorio(dif_mult_div, fase_atual);
      temp_num2 = gerarNumAletorio(dif_mult_div, fase_atual);

      result_operation = temp_num2;
      numbers[0] = temp_num1 * temp_num2;
      numbers[1] = temp_num1;
      break;

  }  

  lcd.clear();

  //        Expressao neutra no osmar
  ServoBOCA.write(90);
  ServoSE.write(90);
  ServoSD.write(90);

  estadoLeds(false, false, true);
  imprimeOperacao(fase_atual, nivel_dificuldade, numbers, operacoes_simb[operacao_atual]);

  while (true) {
    tecla_pressionada = teclado.getKey();

    if (tecla_pressionada) {
      if (tecla_pressionada == '#') {
        break;
      } else if (tecla_pressionada == '*') {
        for (int i = 0; i <= 19; i += 1) {
          value[i] = {};
        }

        lcd.clear();

        imprimeOperacao(fase_atual, nivel_dificuldade, numbers, operacoes_simb[operacao_atual]);

        index = 0;  
      } else {
        value[index] = tecla_pressionada;
        index++;
        value[index] = '\0';
        lcd.print(tecla_pressionada);  
      }
    }
  }
  delay(100);


  lcd.clear();
  final_value = atoi(value);

  //        Vitotória
  if (final_value == result_operation) {
    pontuacao++;
    nivel_dificuldade++;

    // Alterando a expressão do rosto = Feliz
    ServoSE.write(120);
    ServoSD.write(60);
    ServoBOCA.write(0);

    estadoLeds(false, true, false);

    if(nivel_dificuldade == (MUD_FASE * 5) + MUD_FASE){
      lcd.print("VENCEU O OSMAR");
      lcd.setCursor(0, 1);
      lcd.print("PARABENS!! :D");

      tocarMusica(melody3, notes3, TEMPO3);
      
    }
    else if(nivel_dificuldade % MUD_FASE == 0 && fase_atual < 5){ 
      fase_atual++;

      lcd.print("FASE ");
      lcd.print(fase_atual);
      lcd.print(" DESBLOQ.");
      lcd.setCursor(0, 1);
      lcd.print("Parabens :D");

      tocarMusica(melody4, notes4, TEMPO1);
    } else{

      lcd.print("Acertou Mizeravi");
      lcd.setCursor(0, 1);
      lcd.print("Pontos: ");
      lcd.print(pontuacao);
    
      tocarMusica(melody, notes, TEMPO1);
    }
    estadoLeds(false, false, false);
  }



  //         Derrota
  else {
    pontuacao = 0;
    nivel_dificuldade = 1;
    fase_atual = 1;

    lcd.print("Errooooooouuu :(");
    lcd.setCursor(0, 1);
    lcd.print("Resposta: ");
    lcd.print(result_operation);

    // Alterando a expressão do rosto = Bravo
    ServoSE.write(70);
    ServoSD.write(110);
    ServoBOCA.write(180);

    estadoLeds(true, false, false);

    tocarMusica(melody2, notes2, TEMPO2);

    delay(600);

    estadoLeds(false, false, false);
  }
}

//          Função responsável por tocar músicas
void tocarMusica(const int melodia[], int numNotas, int tempo){
  int wholenote = (60000 * 4) / tempo;

  for(int thisNote = 0; thisNote < numNotas * 2; thisNote += 2){
    int divisor = pgm_read_word_near(melodia + thisNote + 1);
    int duracaoNota;

    if(divisor > 0){
      duracaoNota = wholenote / divisor;
    } else if(divisor < 0){
      duracaoNota = wholenote / abs(divisor);
      duracaoNota *= 1.5;
    }

    tone(pinoBuzzer, pgm_read_word_near(melodia + thisNote), duracaoNota * 0.9);
    delay(duracaoNota);
    noTone(pinoBuzzer);

  }

  operacao_atual = random(0, NUM_OPERACOES);

}

// Controla o status dos leds (nao ta ruim, mas da pra melhorar em breve)
void estadoLeds(bool statusRed, bool statusGreen, bool statusBlue){
  digitalWrite(rgb1[0], statusRed);
  digitalWrite(rgb1[1], statusGreen);
  digitalWrite(rgb1[2], statusBlue);

  digitalWrite(rgb2[0], statusRed);
  digitalWrite(rgb2[1], statusGreen);
  digitalWrite(rgb2[2], statusBlue);
}

int gerarNumAletorio(const DificuldadeOperacao* dificuldade, int fase){
  return random(dificuldade[fase - 1].val_min, dificuldade[fase - 1].val_max + 1);
}

void imprimeOperacao(int fase, int nlv_dif, int nums[], char* operacao){
  lcd.print("Fase ");
  lcd.print(fase_atual);
  lcd.print(" - Nvl ");
  lcd.print(nivel_dificuldade);

  lcd.setCursor(0, 1);
  lcd.print(nums[0]);
  lcd.print(operacao);
  lcd.print(nums[1]);
  lcd.print(" = ");
}
