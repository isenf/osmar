#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//alteração -> cada pino do led rgb foi armazenado em vetores chamados rgb1 e rgb2, eles terão o nome mudado
// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .        -=- Banco de notas e Músicas -=-

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

  /*  Valor do tempo da música */
int tempo = 122;
int tempo2 = 122;

  /*  Pino referente ao buzzer */
int pinoBuzzer = 3;

  /* BIBLIOTECA DO CABA Q FEZ: https://github.com/robsoncouto/arduino-songs/tree/master */
  /* Programação das notas, tamanho, quantidade etc: */

  //       @ MELODIA 1 @
const int melody[] PROGMEM = 
{
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2,  
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
  
  //        @ MELODIA 2 @

const int melody2[] PROGMEM = 
{
  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2, 
};

int notes2 = sizeof(melody2) / sizeof(melody2[0]) / 2;
int wholenote2 = (60000 * 4) / tempo2;
int divider2 = 0, noteDuration2 = 0;
// NAO EH O DESPACITO E O NOKIA................ 
// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .         -=- Declaração de variáveis -=-

/*          @ MELODIA 3 @                                                                                                                                                                       */

const int melody3[] PROGMEM= {

  // Pacman
  // Score available at https://musescore.com/user/85429/scores/107109
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
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

const int rgb1[3] = {42, 40, 44};
const int rgb2[3] = {43, 41, 45};

  /* Enderaçamento para as variáveis servos */
#define potPin A10
#define servoPinB 2 /* Antigamente na 1 */
#define servoPinSE 4
#define servoPinSD 6

  /* Enderaçamento/Mapeamento do teclado numérico */
byte pinLinhas[] = {30, 31, 32, 33};
byte pinColunas[] = {34, 35, 36};
char teclas[4][3] = {{'1', '2', '3'},
                    {'4', '5', '6'},
                    {'7', '8', '9'},
                    {'*', '0', '#'}};
char value[50];
char tecla_pressionada;
int numbers[2];
int rand_operator, final_value;
            
  /* Mapeamento do input no display */
Keypad teclado = Keypad( makeKeymap(teclas), pinLinhas, pinColunas, 4, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int operation_math(int num1, int num2, int num_rand)
{
  int value = 0;
}

// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .         -=- Setup de inicializações -=-

void setup() {
  Serial.begin(9600);
  ServoBOCA.attach(servoPinB); 
  ServoSE.attach(servoPinSE); 
  ServoSD.attach(servoPinSD); 

  pinMode(pinoBuzzer, OUTPUT);

  for(int i = 0; i <= 2; i++){
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
  for (int i = 0; i <= 49; i += 1){
    value[i] = {};
  }

    /* Laço para escolher dois valores aleatórios para fazer a operação; */
  for (int i = 0; i <= 1; i += 1){
    numbers[i] = random(2, 10);
  }
  int result_operation = numbers[0] * numbers[1];
    
  lcd.clear();
    
    /* Início de display ao usuario, expressão neutra no O.S.M.A.R e texto inicial na tela */
  lcd.print("Jogo da Tabuada");
  ServoBOCA.write(90);
  ServoSE.write(90);
  ServoSD.write(90);

  digitalWrite(rgb1[2], HIGH);// Liga o led esquerdo em Azul
  digitalWrite(rgb2[2], HIGH);// Liga o led direito em Azul
    
    /* Posicionando os valores de saída no display */
  lcd.setCursor(0,1);
  lcd.print(numbers[0]);
  lcd.print(" x ");
  lcd.print(numbers[1]);
  lcd.print(" = ");

    /*                            ### TEM UM ERRO NO DISPLAY AQUI EM ALGUM LUGAR ###                             */
  while (true){
    tecla_pressionada = teclado.getKey();

    if(tecla_pressionada){
      if (tecla_pressionada == '#'){
        break;
      } else if (tecla_pressionada == '*'){
        for (int i = 0; i <= 19; i += 1){
          value[i] = ' ';
        }
              
        lcd.clear();
        lcd.print("Jogo da Tabuada");
        lcd.setCursor(0,1);
        lcd.print(numbers[0]);
        lcd.print(" x ");
        lcd.print(numbers[1]);
        lcd.print(" = ");

        index = 0; // Mudar para -1 futuramente
            
      } else {
        value[index] = tecla_pressionada; 
      }
      lcd.print(value[index]); // Talvez esse seja o bug
    }
  }
  delay(100);
    
    /*                            -=-                      -=-                   -=-                             */

  lcd.clear();
    
  final_value = atoi(value);

// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .                 -=- Vitória -=-

  if (final_value == result_operation){
    lcd.print("Parabens!!");
    lcd.setCursor(0,1);
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
    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2){

      divider = pgm_read_word_near(melody+thisNote + 1);
      if (divider > 0) {
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; 
      }

      tone(pinoBuzzer, pgm_read_word_near(melody+thisNote), noteDuration * 0.9);
      delay(noteDuration);
      noTone(pinoBuzzer);
    }

      //Desliga os leds, antes verdes
    digitalWrite(rgb1[1], LOW);
    digitalWrite(rgb2[1], LOW);
    
  }

// .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .      .                 -=- Derrota -=-

  else{
    lcd.print("Errooooooouuu :(");
    lcd.setCursor(0,1);
    lcd.print("Resposta: ");
    lcd.print(result_operation);

      //Alterando a expressão do rosto = Bravo
    ServoSE.write(70);
    ServoSD.write(110);
    ServoBOCA.write(180);

      /* Alterando os LEDs respectivos (LOW desliga / HIGH liga) */
    digitalWrite(rgb1[2], LOW);
    digitalWrite(rgb2[2], LOW);
    digitalWrite(rgb1[0], HIGH);
    digitalWrite(rgb2[0], HIGH);

      // .    .    .    .    .    .    .    .    .    .    .    .    .    .    .    Reprodução da música 2 ("melody2")
    for (int thisNote = 0; thisNote < notes2 * 2; thisNote = thisNote + 2){

      divider = pgm_read_word_near(melody2+thisNote + 1);
      if (divider2 > 0) {
        noteDuration2 = (wholenote2) / divider2;
      } else if (divider < 0) {
        noteDuration2 = (wholenote2) / abs(divider2);
        noteDuration2 *= 1.5; 
      }
 
      tone(pinoBuzzer, pgm_read_word_near(melody2+thisNote), noteDuration * 0.9);
      delay(noteDuration);
      noTone(pinoBuzzer);
    }

    delay(600);

      //Desliga os leds 
    digitalWrite(rgb1[0], LOW);
    digitalWrite(rgb2[0], LOW);

  }
}
