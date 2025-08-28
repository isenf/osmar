#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "pitches.h"

/*  Valor do tempo da música */
int tempo = 200;
int tempo2 = 200;

/*  Pino referente ao buzzer */
int pinoBuzzer = 3;

/* BIBLIOTECA DO CABA Q FEZ: https://github.com/robsoncouto/arduino-songs/tree/master */
/* Programação das notas, tamanho, quantidade etc: */

//       @ MELODIA 1 @
const int melody[] PROGMEM = {
  NOTE_E5,
  8,
  NOTE_D5,
  8,
  NOTE_FS4,
  4,
  NOTE_GS4,
  4,
  NOTE_CS5,
  8,
  NOTE_B4,
  8,
  NOTE_D4,
  4,
  NOTE_E4,
  4,
  NOTE_B4,
  8,
  NOTE_A4,
  8,
  NOTE_CS4,
  4,
  NOTE_E4,
  4,
  NOTE_A4,
  2,
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

//        @ MELODIA 2 @

const int melody2[] PROGMEM = {
  NOTE_C5,
  -4,
  NOTE_G4,
  -4,
  NOTE_E4,
  4,  // 45
  NOTE_A4,
  -8,
  NOTE_B4,
  -8,
  NOTE_A4,
  -8,
  NOTE_GS4,
  -8,
  NOTE_AS4,
  -8,
  NOTE_GS4,
  -8,
  NOTE_G4,
  8,
  NOTE_D4,
  8,
  NOTE_E4,
  -2,
};

int notes2 = sizeof(melody2) / sizeof(melody2[0]) / 2;
int wholenote2 = (60000 * 4) / tempo2;
int divider2 = 0, noteDuration2 = 0;
// NAO EH O DESPACITO E O NOKIA................
// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .         -=- Declaração de variáveis -=-

/*          @ MELODIA 3 @                                                                                                                                                                       */

const int melody3[] PROGMEM = {

  // Pacman
  // Score available at https://musescore.com/user/85429/scores/107109
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16,  // 1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, NOTE_B5, 32,  // 2
  NOTE_FS5, -16, NOTE_DS5, 8, NOTE_DS5, 32, NOTE_E5, 32, NOTE_F5, 32,
  NOTE_F5, 32, NOTE_FS5, 32, NOTE_G5, 32, NOTE_G5, 32, NOTE_GS5, 32, NOTE_A5, 16, NOTE_B5, 8
};

int notes3 = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote3 = (60000 * 4) / tempo;
int divider3 = 0, noteDuration3 = 0;

/* Declaração de variáveis dos servos */
Servo ServoBOCA;
Servo ServoSE;
Servo ServoSD;

/* Declaração de variáveis dos leds + respectivos endereços no arduíno */
// int verde1 = 40;
// int vermelho1 = 42;
// int verde2 = 41;
// int vermelho2 = 43;
// int azul1 = 44;
// int azul2 = 45;

const int rgb1[3] = { 42, 40, 44 };
const int rgb2[3] = { 43, 41, 45 };

/* Enderaçamento para as variáveis servos */
#define potPin A10
#define servoPinB 2 /* Antigamente na 1 */
#define servoPinSE 4
#define servoPinSD 6

/* Enderaçamento/Mapeamento do teclado numérico */
byte pinLinhas[] = { 30, 31, 32, 33 };
byte pinColunas[] = { 34, 35, 36 };
char teclas[4][3] = { { '1', '2', '3' },
                      { '4', '5', '6' },
                      { '7', '8', '9' },
                      { '*', '0', '#' } };
char value[50];
char tecla_pressionada;
int numbers[2];
int rand_operator, final_value;

/* Mapeamento do input no display */
Keypad teclado = Keypad(makeKeymap(teclas), pinLinhas, pinColunas, 4, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int operation_math(int num1, int num2, int num_rand) {
  int value = 0;
}

// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .         -=- Setup de inicializações -=-

void setup() {
  Serial.begin(9600);

  ServoBOCA.attach(servoPinB);
  ServoSE.attach(servoPinSE);
  ServoSD.attach(servoPinSD);

  pinMode(pinoBuzzer, OUTPUT);

  for (int i = 0; i <= 2; i++) {
    pinMode(rgb1[i], OUTPUT);
    pinMode(rgb2[i], OUTPUT);
  }

  randomSeed(analogRead(A0));

  lcd.init();
  lcd.backlight();
}

// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .            -=- Código Príncipal -=-

void loop() {
  /* Mapeamento de configuração dos servos para as expressões */
  ServoBOCA.write(map(analogRead(potPin), 0, 1023, 10, 170));
  ServoSE.write(map(analogRead(potPin), 0, 1023, 10, 170));
  ServoSD.write(map(analogRead(potPin), 0, 1023, 10, 170));

  /* Feedback interno */
  Serial.print("Angulo: ");
  Serial.println(map(analogRead(potPin), 0, 1023, 10, 170));

  delay(150);

  /* Índice usado para a leitura do valor digitado pelo usuário */
  int index = 0;

  /* Laço para limpar o array  dos valores digitados pelo usuário; */
  for (int i = 0; i <= 49; i += 1) {
    value[i] = {};
  }

  /* Laço para escolher dois valores aleatórios para fazer a operação; */
  for (int i = 0; i <= 1; i += 1) {
    numbers[i] = random(2, 10);
  }
  int result_operation = numbers[0] * numbers[1];

  lcd.clear();

  /* Início de display ao usuario, expressão neutra no O.S.M.A.R e texto inicial na tela */
  lcd.print("Jogo da Tabuada");
  ServoBOCA.write(90);
  ServoSE.write(90);
  ServoSD.write(90);

  digitalWrite(rgb1[2], HIGH);  // Liga o led esquerdo em Azul
  digitalWrite(rgb2[2], HIGH);  // Liga o led direito em Azul

  /* Posicionando os valores de saída no display */
  lcd.setCursor(0, 1);
  lcd.print(numbers[0]);
  lcd.print(" x ");
  lcd.print(numbers[1]);
  lcd.print(" = ");

  /*                            ### TEM UM ERRO NO DISPLAY AQUI EM ALGUM LUGAR ###                             */
  while (true) {
    tecla_pressionada = teclado.getKey();

    if (tecla_pressionada) {
      if (tecla_pressionada == '#') {
        break;
      } else if (tecla_pressionada == '*') {
        for (int i = 0; i <= 19; i += 1) {
          value[i] = ' ';
        }

        lcd.clear();
        lcd.print("Jogo da Tabuada");
        lcd.setCursor(0, 1);
        lcd.print(numbers[0]);
        lcd.print(" x ");
        lcd.print(numbers[1]);
        lcd.print(" = ");

        index = 0;  // Mudar para -1 futuramente
      } else {
        value[index] = tecla_pressionada;
        index++;
        value[index] = '\0';
      }
      lcd.print(tecla_pressionada);  // Talvez esse seja o bug
    }
  }
  delay(100);

  /*                            -=-                      -=-                   -=-                             */

  lcd.clear();

  final_value = atoi(value);

  // .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .                 -=- Vitória -=-

  if (final_value == result_operation) {
    lcd.print("Parabens!!");
    lcd.setCursor(0, 1);
    lcd.print("Acertou Mizeravi :)");

    /* Alterando a expressão do rosto = Feliz */
    ServoSE.write(120);
    ServoSD.write(60);
    ServoBOCA.write(0);

    /* Alterando os LEDs respectivos (LOW desliga / HIGH liga) */
    digitalWrite(rgb1[2], LOW);
    digitalWrite(rgb2[2], LOW);
    digitalWrite(rgb1[1], HIGH);
    digitalWrite(rgb2[1], HIGH);

    // .  .  .  .  . Reprodução da música 1 ("melody") .  .  .  .  .
    Serial.println("TOCANDO MUSICA 1 - VITORIA");

    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
      divider = pgm_read_word_near(melody + thisNote + 1);
      if (divider > 0) {
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5;
      }

      tone(pinoBuzzer, pgm_read_word_near(melody + thisNote), noteDuration * 0.9);
      delay(noteDuration);
      noTone(pinoBuzzer);
    }

    // Desliga os leds, antes verdes
    digitalWrite(rgb1[1], LOW);
    digitalWrite(rgb2[1], LOW);
  }



  // .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .                 -=- Derrota -=-

  else {
    lcd.print("Errooooooouuu :(");
    lcd.setCursor(0, 1);
    lcd.print("Resposta: ");
    lcd.print(result_operation);

    // Alterando a expressão do rosto = Bravo
    ServoSE.write(70);
    ServoSD.write(110);
    ServoBOCA.write(180);

    /* Alterando os LEDs respectivos (LOW desliga / HIGH liga) */
    digitalWrite(rgb1[2], LOW);
    digitalWrite(rgb2[2], LOW);
    digitalWrite(rgb1[0], HIGH);
    digitalWrite(rgb2[0], HIGH);

    // .    .    .    .    .    .    .    .    .    .    .    .    .    .    .    Reprodução da música 2 ("melody2")
    Serial.println("TOCANDO MUSICA 2 - DERROTA");
  
  for (int thisNote = 0; thisNote < notes2 * 2; thisNote = thisNote + 2){
    divider2 = pgm_read_word_near(melody2 + thisNote + 1); // CORRIGIDO: usar divider2
    if (divider2 > 0) {
      noteDuration2 = (wholenote2) / divider2; // CORRIGIDO: usar divider2
    } else if (divider2 < 0) {
      noteDuration2 = (wholenote2) / abs(divider2); // CORRIGIDO: usar divider2
      noteDuration2 *= 1.5; 
    }

    tone(pinoBuzzer, pgm_read_word_near(melody2 + thisNote), noteDuration2 * 0.9); // CORRIGIDO: usar noteDuration2
    delay(noteDuration2); // CORRIGIDO: usar noteDuration2
    noTone(pinoBuzzer);
  }

    delay(600);

    // Desliga os leds
    digitalWrite(rgb1[0], LOW);
    digitalWrite(rgb2[0], LOW);
  }
}
