// Notas musicais 
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Incluindo bibliotecas necessárias
#include <LiquidCrystal.h>
#include <ArduinoJson.h>

// Definindo os pinos do LCD
LiquidCrystal lcd (13, 12,14, 27, 26, 25);

// Pinos para LEDs, botões e buzzer
int pinoLEDVermelho =19 ;    // Alternativa A
int pinoLEDVerde =21 ;       // Alternativa B
int pinoLEDAzul = 22;         // Alternativa C
int pinoLEDAmarelo = 23;      // Alternativa D

int pinoBotaoVermelho = 2;
int pinoBotaoVerde = 4;
int pinoBotaoAzul = 5;
int pinoBotaoAmarelo =18 ;

int pinoBuzzer = 33;

// Start do programa
bool interacaoExibida = false;

// Variáveis de controle de brilho
int brilhoVermelho = 255; 
int brilhoVerde = 255;
int brilhoAzul = 255;
int brilhoAmarelo = 255;

// Variaveis com os dados do quiz (perguntas/respostas/pontuação)
String topQuestions[] = {"Questao 1", "Questao 2", "Questao 3", "Questao 4", "Questao 5"};
String bottomQuestions = "A - B - C - D";
int maxNumQuestions = 5;
int NumQuestions = 0;
// A = 0 (vermelho)
// B = 1 (Verde) 
// C = 2 (Azul)
// D = 3 (Amarelo)
int answers[] = {0, 2, 3, 2, 1}; 
int score = 0;

void setup() {

  lcd.begin(16,2);

  pinMode(pinoLEDVermelho, OUTPUT);
  pinMode(pinoLEDVerde, OUTPUT);
  pinMode(pinoLEDAzul, OUTPUT);
  pinMode(pinoLEDAmarelo, OUTPUT);

  pinMode(pinoBotaoVermelho, INPUT_PULLUP);
  pinMode(pinoBotaoVerde, INPUT_PULLUP);
  pinMode(pinoBotaoAzul, INPUT_PULLUP);
  pinMode(pinoBotaoAmarelo, INPUT_PULLUP);

  pinMode(pinoBuzzer, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Exibe a interação inicial apenas uma vez
  if (!interacaoExibida) {
    interacaoInicial(); 
    interacaoExibida = true;
  }

  delay(500); 
  responderQuiz();
}

// Abertura do QUIZ
void interacaoInicial() {

  lcd.setCursor(0,0);
  lcd.print("QUIZ:");
  lcd.setCursor(0,1);
  lcd.print("Exame Hemograma");

  delay(1500); 

  // Rolagem da direita para a esquerda
  for (int i = 0; i < 16; i++) {
    lcd.scrollDisplayRight();
    delay(200);
  }

  int notasInicio[] = {
    NOTE_E5, NOTE_E5, 0, NOTE_E5, 0, NOTE_C5, NOTE_E5, 0,
    NOTE_G5, 0, 0, 0, NOTE_G4, 0, 0, 0,
    NOTE_C5, 0, 0, NOTE_G4, 0, 0, NOTE_E4, 0,
    0, NOTE_A4, 0, NOTE_B4, 0, NOTE_AS4, NOTE_A4, 0
  };

  int duracao = 30;

  // Musica de início
  for (int i = 0; i < 32; i++) {
    acendeLED(i % 4);
    tocaMelodiaNota(notasInicio[i], duracao);
    delay(25);
    apagaLEDs();
    delay(10);
  }

  lcd.clear();
}

// Função que valida a opção escolhida pelo usuário
void responderQuiz() {

  while (NumQuestions != maxNumQuestions){
    lcd.setCursor(0, 0);                        
    // Questões
    lcd.print(topQuestions[NumQuestions]);
    lcd.setCursor(0, 1);                        
    // Alternativas
    lcd.print(bottomQuestions);

    if (botaoPressionado()) {
      // Acendendo o led correspondente ao botão clicado
      int corPressionada = leBotaoPressionado();
      acendeLED(corPressionada);
      tocaMelodia(corPressionada);
      delay(15);
      apagaLEDs();

      // Caso a resposta esteja correta
      if (corPressionada == (answers[NumQuestions])){
        // Pisca o LED correspondente ao botão correto 3 vezes rapidamente
        delay(50);
        piscaLED(answers[NumQuestions], 3); 
        NumQuestions++;
        score++;
        lcd.clear();
      }

      // Caso a resposta esteja errada
      else{
        // Pisca o LED correspondente ao botão correto 5 vezes rapidamente
        delay(50);
        tocaMelodiaErro();
        piscaLED(answers[NumQuestions], 5); 
        NumQuestions++;
        lcd.clear();
      }
    }
    // Depois de responder as questões mostrar a pontuação
    if (NumQuestions == maxNumQuestions){
        showScore();
    }
  }
}

// Função que envia pro servidor e apresenta o resultado do quiz
void showScore() {

    // Enviando os dados para o node-red
    // Criar um objeto JSON
    StaticJsonDocument<200> doc;
    // Adicionar a variável SCORE ao JSON
    doc["pontuacao"] = score; 

    // Serializar o objeto JSON para enviar pela porta serial
    serializeJson(doc, Serial);
    Serial.println(); // Adicionar uma quebra de linha para separar os dados

    lcd.clear();
    
    // O jogador acertou mais de 3 questões, toca a melodia de acerto
    if (score > 3)                    
    { 
      lcd.setCursor(0, 0);
      lcd.print("Parabens :D");
      lcd.setCursor(0, 1);
      lcd.print("Sua nota foi: ");
      lcd.setCursor(15, 1);
      lcd.print(score);
      tocaMelodiaAcerto();
      delay(90);
      // Reseta o jogo
      interacaoExibida = false;  
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Tente denovo :-(");
      lcd.setCursor(0, 1);
      lcd.print("Sua nota foi: ");
      lcd.setCursor(15, 1);
      lcd.print(score);
      exibeErro();
      delay(90);
      // Reseta o jogo
      interacaoExibida = false;  
    }
    
    delay(30000);         
}

bool botaoPressionado() {
  return digitalRead(pinoBotaoVermelho) == LOW ||
         digitalRead(pinoBotaoVerde) == LOW ||
         digitalRead(pinoBotaoAzul) == LOW ||
         digitalRead(pinoBotaoAmarelo) == LOW;
}

int leBotaoPressionado() {
  if (digitalRead(pinoBotaoVermelho) == LOW) return 0;
  if (digitalRead(pinoBotaoVerde) == LOW) return 1;
  if (digitalRead(pinoBotaoAzul) == LOW) return 2;
  if (digitalRead(pinoBotaoAmarelo) == LOW) return 3;

  return -1;  // Nenhum botão pressionado
}

void acendeLED(int cor) {
  switch (cor) {
    case 0:  // Vermelho
      analogWrite(pinoLEDVermelho, brilhoVermelho);
      break;
    case 1:  // Verde
      analogWrite(pinoLEDVerde, brilhoVerde);
      break;
    case 2:  // Azul
      analogWrite(pinoLEDAzul, brilhoAzul);
      break;
    case 3:  // Amarelo
      analogWrite(pinoLEDAmarelo, brilhoAmarelo);
      break;
    default:
      break;
  }
}

void apagaLEDs() {
  analogWrite(pinoLEDVermelho, 0);
  analogWrite(pinoLEDVerde, 0);
  analogWrite(pinoLEDAzul, 0);
  analogWrite(pinoLEDAmarelo, 0);
}

// Som dos botões ao clicar
void tocaMelodia(int cor) {
  int duracao = 300;

  switch (cor) {
    case 0:  // Vermelho
      tocaMelodiaNota(NOTE_D4, duracao);
      break;
    case 1:  // Verde
      tocaMelodiaNota(NOTE_G4, duracao);
      break;
    case 2:  // Azul
      tocaMelodiaNota(NOTE_B4, duracao);
      break;
    case 3:  // Amarelo
      tocaMelodiaNota(NOTE_E4, duracao);
      break;
    default:
      delay(duracao);
      break;
  }
}

void tocaMelodiaAcerto() {
  // Define a melodia de acerto
  int melodiaAcerto[] = {
    NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6
  };
  int duracaoNotas[] = {
    200, 200, 200, 200
  };

  // Toca a melodia de acerto
  for (int i = 0; i < 4; i++) {
    acendeLED(i % 4);
    tone(pinoBuzzer, melodiaAcerto[i], duracaoNotas[i]);
    delay(25);
    apagaLEDs();
    delay(duracaoNotas[i]); // Espera até a nota terminar
  }
  
  noTone(pinoBuzzer); // Desliga o buzzer após tocar a melodia
}

void tocaMelodiaErro() {
  // Define a melodia de erro com notas curtas
  int melodiaErro[] = {
    NOTE_C5, NOTE_DS5, NOTE_F5, NOTE_G5
  };
  int duracaoNotas[] = {
    100, 100, 100, 100
  };

  // Toca a melodia de erro
  for (int i = 0; i < 4; i++) {
    tone(pinoBuzzer, melodiaErro[i], duracaoNotas[i]);
    delay(duracaoNotas[i]); // Espera até a nota terminar
  }
  noTone(pinoBuzzer); // Desliga o buzzer após tocar a melodia
}

void tocaMelodiaNota(int nota, int duracao) {
  tone(pinoBuzzer, nota, duracao);
  delay(duracao + 50);  // Pequena pausa entre as notas
  noTone(pinoBuzzer);
}

// Música "Game Over"
void exibeErro() {
  int notas[] = {
    NOTE_E5, NOTE_E4, NOTE_E4, NOTE_DS5,
    NOTE_E5, NOTE_DS5, NOTE_DS5, NOTE_D5,
    NOTE_DS5, NOTE_D5, NOTE_D5, NOTE_CS5,
    NOTE_D5, NOTE_CS5, NOTE_CS5, NOTE_C5
  }; 

  int tempos[] = {90, 45, 45, 90, 90, 45, 45, 90, 90, 45, 45, 90, 90, 45, 45, 90};  // Tempo um pouco mais longo entre as notas

  for (int i = 0; i < 16; i++) {
    acendeLED(i % 4);  // Vai alternar entre os LEDs

    if (i % 4 == 0 && i > 0) {
      // Pequena pausa entre cada sequência
      delay(0);
    }

    tocaMelodiaNota(notas[i], tempos[i]);  // Ajuste a duração conforme necessário
    delayMicroseconds(25);
    apagaLEDs();
    delayMicroseconds(25);
  }
}

void piscaLED(int cor, int vezes) {
  for (int i = 0; i < vezes; i++) {
    acendeLED(cor);
    delay(175);
    apagaLEDs();
    delay(175);
  }
}
