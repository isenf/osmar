#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "pitches.h"

/*  Valor do tempo da música */
#define TEMPO1 200
#define TEMPO2 200
#define TEMPO3 105

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

//          Melodia 3 -> pacman
const int melody3[] PROGMEM = {
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16,  // 1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,
};
const int notes3 = sizeof(melody3) / sizeof(melody3[0]) / 2;

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
    value[i] = {};
  }

  //Laço para escolher dois valores aleatórios para fazer a operação
  for (int i = 0; i <= 1; i ++) {
    numbers[i] = random(2, 10);
  }
  int result_operation = numbers[0] * numbers[1];

  lcd.clear();

  //Início de display ao usuario, expressão neutra no O.S.M.A.R e texto inicial na tela
  lcd.print("Jogo da Tabuada");
  ServoBOCA.write(90);
  ServoSE.write(90);
  ServoSD.write(90);

  estadoLeds(false, false, true);

  lcd.setCursor(0, 1);
  lcd.print(numbers[0]);
  lcd.print(" x ");
  lcd.print(numbers[1]);
  lcd.print(" = ");

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
        lcd.print("Jogo da Tabuada");
        lcd.setCursor(0, 1);
        lcd.print(numbers[0]);
        lcd.print(" x ");
        lcd.print(numbers[1]);
        lcd.print(" = ");

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
    lcd.print("Parabens!!");
    lcd.setCursor(0, 1);
    lcd.print("Acertou Mizeravi :)");

    /* Alterando a expressão do rosto = Feliz */
    ServoSE.write(120);
    ServoSD.write(60);
    ServoBOCA.write(0);

    estadoLeds(false, true, false);

    //Serial.println("TOCANDO MUSICA 1 - VITORIA");
    tocarMusica(melody, notes, TEMPO1);

    estadoLeds(false, false, false);
  }



  //         Derrota
  else {
    lcd.print("Errooooooouuu :(");
    lcd.setCursor(0, 1);
    lcd.print("Resposta: ");
    lcd.print(result_operation);

    // Alterando a expressão do rosto = Bravo
    ServoSE.write(70);
    ServoSD.write(110);
    ServoBOCA.write(180);

    estadoLeds(true, false, false);

    //Serial.println("TOCANDO MUSICA 2 - DERROTA");
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
