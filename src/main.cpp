#include "includes.h"
#include <EEPROM.h>
#include <TFT_eSPI.h>

// ========================= DECLARAÇÃO DE VARIÁVEIS E OBJETOS =========================

TFT_eSPI tft = TFT_eSPI();

typedef struct
{
  uint16_t posX, posY, largura, altura;
  const char *texto;
  uint16_t corFundo;
  uint16_t corTexto;
  uint16_t corPressionado;
} Botao;

Botao botoes[] = {
    {40, 20, 160, 70, "Ligar", TFT_BLUE, TFT_WHITE, TFT_DARKGREY},
    {40, 90, 160, 70, "Desligar", TFT_RED, TFT_WHITE, TFT_DARKGREY},
    {40, 160, 160, 70, "Configurar", TFT_YELLOW, TFT_WHITE, TFT_DARKGREY},
    {
        60,
        20,
        60,
        60,
        "+",
        TFT_GREEN,
        TFT_WHITE,
        TFT_DARKGREY,
    }, // Botão de Mais
    {
        140,
        20,
        60,
        60,
        "-",
        TFT_GREEN,
        TFT_WHITE,
        TFT_DARKGREY,
    }, // Botão de Menos
    {
        60,
        100,
        140,
        60,
        "Definir",
        TFT_BLUE,
        TFT_WHITE,
        TFT_DARKGREY,
    }};
// Botao botoesConfig[] = {
//       {60, 20, 60, 60, "+", TFT_GREEN, TFT_WHITE, TFT_DARKGREY,},  // Botão de Mais
//     {140, 20, 60, 60, "-", TFT_GREEN, TFT_WHITE, TFT_DARKGREY,}, // Botão de Menos
//     {60, 100, 140, 60, "Definir", TFT_BLUE, TFT_WHITE, TFT_DARKGREY,}
// };
//====================================================================================================================//
void desenhaBotao(Botao b);
// void handleButtonActions();
void Temporizador_100mS_Cont();
void Start_Temporizador(unsigned int Tempo);
void Stop_Temporizador();
void Pausa_Temporizador();
void noPause_Temporizador();
bool Status_Temporizador();
void checkButtonPress();
void TelaConfigurar();
void touch_calibrate();
void IRAM_ATTR onTimer();
void setup();
void loop();
void atualizaDisplayValor();
//====================================================================================================================//
// === Implementações ===
void desenhaBotao(Botao b)
{
  tft.fillRect(b.posX, b.posY, b.largura, b.altura, b.corFundo);
  tft.setTextColor(b.corTexto);
  uint16_t textoX = b.posX + (b.largura - (strlen(b.texto) * 6)) / 2; // Ajuste aproximado para centralizar o texto
  uint16_t textoY = b.posY + b.altura / 2 - 4;                        // Ajuste aproximado para centralizar o texto verticalmente
  tft.setCursor(textoX, textoY);
  tft.print(b.texto);
}
//====================================================================================================================//

//====================================================================================================================//
void Temporizador_100mS_Cont(void)
{
  if (Tempo_100mS)
    if (!Pausa_Timer)
      Tempo_100mS--;
}
//====================================================================================================================//
void Start_Temporizador(unsigned int Tempo)
{
  Tempo_100mS = Tempo;
  Pausa_Timer = false;
}
//====================================================================================================================//
void Stop_Temporizador(void)
{
  Tempo_100mS = 0;
  Pausa_Timer = false;
}
//====================================================================================================================//
void Pausa_Temporizador(void)
{
  Pausa_Timer = true;
}
//====================================================================================================================//
void noPause_Temporizador(void)
{
  Pausa_Timer = false;
}
//====================================================================================================================//
bool Status_Temporizador(void)
{
  if (Tempo_100mS)
    return true;
  else
    return false;
}
//====================================================================================================================//
void telaInicio(){
    tft.fillScreen(TFT_BLACK);
  desenhaBotao(botoes[0]);
  desenhaBotao(botoes[1]);
  desenhaBotao(botoes[2]);
}
//====================================================================================================================//
void checkButtonPress()
{
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
  {
    if (pag == 1)
    {
      // Verificando o toque para o botão "Ligar"
      if (x >= botoes[0].posX && x <= (botoes[0].posX + botoes[0].largura) &&
          y >= botoes[0].posY && y <= (botoes[0].posY + botoes[0].altura))
      {
        digitalWrite(Saida_Rele, HIGH);
      }
      // Verificando o toque para o botão "Desligar"
      else if (x >= botoes[1].posX && x <= (botoes[1].posX + botoes[1].largura) &&
               y >= botoes[1].posY && y <= (botoes[1].posY + botoes[1].altura))
      {
        digitalWrite(Saida_Rele, LOW);
      }

      // Verificando o toque para o botão "Configurar"
      else if (x >= botoes[2].posX && x <= (botoes[2].posX + botoes[2].largura) &&
               y >= botoes[2].posY && y <= (botoes[2].posY + botoes[2].altura))
      {
        tft.fillScreen(TFT_BLACK);
        TelaConfigurar();
      }
    }
    if (pag == 2)
    {
      if (x >= botoes[3].posX && x <= (botoes[3].posX + botoes[3].largura) &&
          y >= botoes[3].posY && y <= (botoes[3].posY + botoes[3].altura))
      {
        valor++;
        atualizaDisplayValor();
      }

      // Verificando o toque para o botão "Menos"
      else if (x >= botoes[4].posX && x <= (botoes[4].posX + botoes[4].largura) &&
               y >= botoes[4].posY && y <= (botoes[4].posY + botoes[4].altura))
      {
        valor--;
        atualizaDisplayValor();
      }

      // Verificando o toque para o botão "Definir"
      else if (x >= botoes[5].posX && x <= (botoes[5].posX + botoes[5].largura) &&
               y >= botoes[5].posY && y <= (botoes[5].posY + botoes[5].altura))
      {
        // Ação do botão Definir

        tft.fillScreen(TFT_BLACK);

       telaInicio();
        pag = 1;
      }
    }
  }
}
//====================================================================================================================//
void atualizaDisplayValor()
{
  tft.fillRect(posX, posY - 20, 100, 40, TFT_BLACK);
  // Configura a posição e cor para o texto do valor.
  tft.setCursor(posX, posY);
  tft.setTextColor(corTexto);

  // Mostra o valor atualizado na tela.
  tft.print(valor);
  delay(100);
}
//====================================================================================================================//
void TelaConfigurar()
{
  pag = 2;
  desenhaBotao(botoes[3]);
  desenhaBotao(botoes[4]);
  desenhaBotao(botoes[5]);
}
//====================================================================================================================//
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin())
  {
    Serial.println("Formatting file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE_TFT))
  {

    if (REPEAT_CAL_TFT)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE_TFT);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE_TFT, "r");
      if (f)
      {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL_TFT)
  {
    // calibration data valid
    tft.setTouch(calData);
  }
  else
  {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Toque nas pontas indicadas");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL_TFT)
    {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibração Completa");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE_TFT, "w");
    if (f)
    {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
//====================================================================================================================//
void IRAM_ATTR onTimer()
{
  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //=== Leitura dos Botões ===
  //
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}
//====================================================================================================================//
void setup()
{
  Serial.begin(115200); // UART 0
  tft.fillScreen(TFT_BLACK);
  pinMode(Saida_Rele, OUTPUT);

  // Display:
  tft.init();
  tft.setRotation(2);
  touch_calibrate();
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK);
  telaInicio();
  //====================================================================================================================//

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);
}

void loop()
{

  checkButtonPress();
  // handleButtonActions();

  if (interruptCounter > 0)
  {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    //****************
    // Funções 10mS
    //
    if (++Div_P100mS < 10)
      ;
    else
    {
      Div_P100mS = 0;
      Temporizador_100mS_Cont();
    }
  }
}