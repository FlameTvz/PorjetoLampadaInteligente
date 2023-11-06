#include "includes.h"
#include <EEPROM.h>
#include <TFT_eSPI.h>

// ========================= DECLARAÇÃO DE VARIÁVEIS E OBJETOS =========================

TFT_eSPI tft = TFT_eSPI();

typedef struct {
    uint16_t posX, posY, largura, altura;
    const char* texto;
    uint16_t corFundo;
    uint16_t corTexto;
    uint16_t corPressionado;
} Botao;

Botao botoes[] = {
    {40, 20, 160, 70, "Ligar", TFT_BLUE, TFT_WHITE, TFT_DARKGREY},
    {40, 90, 160, 70, "Desligar", TFT_RED, TFT_WHITE, TFT_DARKGREY},
    {40, 160, 160, 70, "Configurar", TFT_YELLOW, TFT_WHITE, TFT_DARKGREY}
};
//====================================================================================================================//
void desenhaBotao(Botao b);
void handleButtonActions();
void Temporizador_100mS_Cont();
void Start_Temporizador(unsigned int Tempo);
void Stop_Temporizador();
void Pausa_Temporizador();
void noPause_Temporizador();
bool Status_Temporizador();
void checkButtonPress();
// void touch_calibrate();
void IRAM_ATTR onTimer();
void setup();
void loop();
//====================================================================================================================//
// === Implementações ===
void desenhaBotao(Botao b) {
    tft.fillRect(b.posX, b.posY, b.largura, b.altura, b.corFundo);
    tft.setTextColor(b.corTexto);
    uint16_t textoX = b.posX + (b.largura - (strlen(b.texto) * 6)) / 2; // Ajuste aproximado para centralizar o texto
    uint16_t textoY = b.posY + b.altura / 2 - 4; // Ajuste aproximado para centralizar o texto verticalmente
    tft.setCursor(textoX, textoY);
    tft.print(b.texto);
}
//====================================================================================================================//
void handleButtonActions() {
  if (btnTurnOnPressed) {
    digitalWrite(Saida_Rele, HIGH);
    btnTurnOnPressed = false;
  }

  if (btnTurnOffPressed) {
    digitalWrite(Saida_Rele, LOW);
    btnTurnOffPressed = false;
  }
if (btnTogglePressed) {
  switch(QLoop)
    {
      //==============================================
      case  0:
        // botoes[2] == !btnTogglePressed{
        //   Start_Temporizador(10);

        // }

        
        

        QLoop = 1;
      break;
      //==============================================
      case  1:
        
        tft.setCursor(0, 0);
        tft.println("Caso 1");
        if(!Status_Temporizador())
        {
          //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
          digitalWrite(Saida_Rele, !digitalRead(Saida_Rele));
          QLoop = 0;
        }

      break;
      //====================================================================================================================//
      case  2:

      break;
      //====================================================================================================================//
      case  3:

      break;
      //====================================================================================================================//
      default: QLoop = 0;
    }

  QLoop = 0;
  }
}
//====================================================================================================================//
void  Temporizador_100mS_Cont(void)
{
  if(Tempo_100mS)
    if(!Pausa_Timer)  
      Tempo_100mS--;
}
//====================================================================================================================//
void  Start_Temporizador(unsigned int Tempo)
{ 
  Tempo_100mS = Tempo;
  Pausa_Timer = false;
}
//====================================================================================================================//
void  Stop_Temporizador(void)
{ 
  Tempo_100mS = 0;
  Pausa_Timer = false;
}
//====================================================================================================================//
void  Pausa_Temporizador(void)
{
  Pausa_Timer = true;
}
//====================================================================================================================//
void  noPause_Temporizador(void)
{
  Pausa_Timer = false;
}
//====================================================================================================================//
bool  Status_Temporizador(void)
{
  if(Tempo_100mS)
    return  true;
  else
    return  false;    
}
//====================================================================================================================//
void checkButtonPress() {
  uint16_t x, y;
 if (tft.getTouch(&x, &y)) {
        // Verificando o toque para o botão "Ligar"
        if (x >= botoes[0].posX && x <= (botoes[0].posX + botoes[0].largura) && 
            y >= botoes[0].posY && y <= (botoes[0].posY + botoes[0].altura)) {
            btnTurnOnPressed = true;
        }
        // Verificando o toque para o botão "Desligar"
        else if (x >= botoes[1].posX && x <= (botoes[1].posX + botoes[1].largura) && 
                 y >= botoes[1].posY && y <= (botoes[1].posY + botoes[1].altura)) {
            btnTurnOffPressed = true;
        }

        // Verificando o toque para o botão "Configurar"
        else if (x >= botoes[2].posX && x <= (botoes[2].posX + botoes[2].largura) && 
                 y >= botoes[2].posY && y <= (botoes[2].posY + botoes[2].altura)) {
            btnTogglePressed = !btnTogglePressed;
        }
    }
}
//====================================================================================================================//
void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formatting file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE_TFT)) {

    if (REPEAT_CAL_TFT) {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE_TFT);
    } else {
      File f = SPIFFS.open(CALIBRATION_FILE_TFT, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL_TFT) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Toque nas pontas indicadas");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL_TFT) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibração Completa");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE_TFT, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
//====================================================================================================================//
void IRAM_ATTR onTimer()
{
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //=== Leitura dos Botões ===
  // 
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}
//====================================================================================================================//
void setup()
{
  Serial.begin(115200);     // UART 0
  tft.fillScreen(TFT_BLACK);
  pinMode(Saida_Rele, OUTPUT);
  
  
  //Display:
  tft.init();
  tft.setRotation(2);
  touch_calibrate();
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK);
  
  
  desenhaBotao(botoes[0]);
  desenhaBotao(botoes[1]);
  desenhaBotao(botoes[2]);


  //====================================================================================================================//

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);

}

void loop()
{

  checkButtonPress();
  handleButtonActions();
 
  if(interruptCounter > 0)
  {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    //****************
    // Funções 10mS
    // 
    if(++Div_P100mS < 10);
    else
    {
      Div_P100mS = 0;
      Temporizador_100mS_Cont(); 
    }
  
  }

}