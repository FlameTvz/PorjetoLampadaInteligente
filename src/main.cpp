// Inclusão de bibliotecas necessárias para funcionamento do sistema
#include "includes.h" // Biblioteca de inclusões customizadas, não padrão.
#include <EEPROM.h>   // Biblioteca para manipulação da memória EEPROM.
#include <TFT_eSPI.h> // Biblioteca para controle do display TFT.

// ========================= DECLARAÇÃO DE VARIÁVEIS E OBJETOS =========================

TFT_eSPI tft = TFT_eSPI();

// Definição de um tipo de estrutura chamada Botao para representar um botão na tela.
typedef struct
{
  uint16_t posX, posY, largura, altura;
  const char *texto;
  uint16_t corFundo;
  uint16_t corTexto;
  uint16_t corPressionado;
  uint16_t raioArredondamento;
} Botao;

// Array de estruturas Botao para definir vários botões na tela.
Botao botoes[] = {
    // Definição dos botões com suas respectivas propriedades (posição, tamanho, texto, cores)
    {20, 20, 200, 50, "Ligar", TFT_DARKCYAN, TFT_WHITE, TFT_DARKGREY},
    {20, 80, 200, 50, "Desligar", TFT_DARKRED, TFT_WHITE, TFT_DARKGREY},
    {20, 140, 200, 50, "funçoes", TFT_OLIVE, TFT_WHITE, TFT_DARKGREY},
    {15, 30, 100, 50, "+", TFT_DARKCYAN, TFT_WHITE, TFT_DARKGREY},   // Botão Mais
    {120, 30, 100, 50, "-", TFT_DARKRED, TFT_WHITE, TFT_DARKGREY},        // Botão Menos na parte superior
    {20, 150, 200, 50, "Definir", TFT_OLIVE, TFT_WHITE, TFT_DARKGREY}, // Botão Definir um pouco abaixo
    {20, 210, 200, 50, "Voltar", TFT_SLATEGRAY, TFT_WHITE, TFT_DARKGREY}   // Botão Voltar ainda mais abaixo
};

//====================================================================================================================//
// Declaração de funções que serão utilizadas no programa.
void desenhaBotao(Botao b);
void checkButtonPress();
void TelaConfigurar();
void touch_calibrate();
void setup();
void loop();
void atualizaDisplayValor();
//====================================================================================================================//
// Implementação da função desenhaBotao que recebe um botão e o desenha na tela.
void desenhaBotao(Botao b)
{
  tft.setFreeFont(FF18);
  // Código para desenhar o botão no display.
  tft.fillRect(b.posX, b.posY, b.largura, b.altura, b.corFundo);
  tft.setTextColor(b.corTexto);
  uint16_t textoX = b.posX + (b.largura - (strlen(b.texto) * 6)) / 2.5; // Ajuste aproximado para centralizar o texto
  uint16_t textoY = b.posY + b.altura / 1.5 - 4;                        // Ajuste aproximado para centralizar o texto verticalmente
  tft.setCursor(textoX, textoY);
  tft.print(b.texto);
  
  }
//====================================================================================================================//
// Função telaInicio para configurar a tela inicial com os botões principais.
void telaInicio()
{
  pag = 1;
  tft.fillScreen(TFT_WHITE);
  desenhaBotao(botoes[0]);
  desenhaBotao(botoes[1]);
  desenhaBotao(botoes[2]);
}
//====================================================================================================================//
// Implementação da função checkButtonPress que verifica se algum botão foi pressionado.
void checkButtonPress()
{
  // Código para verificar se um botão foi pressionado e realizar ações.
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
  {

    if (pag == 1)
    {

      delay(150);
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
        tft.fillScreen(TFT_WHITE);
        TelaConfigurar();
      }
    }
    // Verificando o toque para o botão "Mais"
    else if (pag == 2)
    {

      if (x >= botoes[3].posX && x <= (botoes[3].posX + botoes[3].largura) &&
          y >= botoes[3].posY && y <= (botoes[3].posY + botoes[3].altura))
      {
        TempoCronometro += 1000;
        delay(100);
        atualizaDisplayValor();
      }
      // Verificando o toque para o botão "Menos"
      else if (x >= botoes[4].posX && x <= (botoes[4].posX + botoes[4].largura) &&
               y >= botoes[4].posY && y <= (botoes[4].posY + botoes[4].altura))
      {
        if (TempoCronometro > 0)
        {
          TempoCronometro -= 1000;
        }
        delay(100);
        atualizaDisplayValor();
      }

      // Verificando o toque para o botão "Definir"
      else if (x >= botoes[5].posX && x <= (botoes[5].posX + botoes[5].largura) &&
               y >= botoes[5].posY && y <= (botoes[5].posY + botoes[5].altura))
      {
        // Ação do botão Definir
        varb = 1;
        crono = millis();
        digitalWrite(Saida_Rele, HIGH);
      }
      // Verificando o toque para o botão "Voltar"

      else if (x >= botoes[6].posX && x <= (botoes[6].posX + botoes[6].largura) &&
               y >= botoes[6].posY && y <= (botoes[6].posY + botoes[6].altura))
      {

        telaInicio();
        varb = 0;
      }
    }
  }
}
//====================================================================================================================//
// Função atualizaDisplayValor para atualizar o valor mostrado na tela.
void atualizaDisplayValor()
{
  // Código para atualizar o valor exibido no display.
  tft.fillRect(posX, posY - 20, 100, 40, TFT_WHITE);
  // Configura a posição e cor para o texto do valor.
  tft.setCursor(posX, posY);
  tft.setTextColor(corTexto);

  // Mostra o valor atualizado na tela.
  tft.print(TempoCronometro/1000);

  delay(100);
}
//====================================================================================================================//
// Função TelaConfigurar para mostrar a tela de configuração com botões específicos.
void TelaConfigurar()
{
  // Código para configurar a tela de configuração.
  pag = 2;
  desenhaBotao(botoes[3]);
  desenhaBotao(botoes[4]);
  desenhaBotao(botoes[5]);
  desenhaBotao(botoes[6]);
}
//====================================================================================================================//
// Função touch_calibrate para calibrar a tela de toque do display.
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

void cronometro()
{
  if (millis() > crono + TempoCronometro && varb == 1)
  {
    digitalWrite(Saida_Rele, LOW);
    crono = 0;
    // TempoCronometro = 0;
  }
}
//====================================================================================================================//
// A função setup é chamada uma vez quando o programa começa
void setup()
{
  Serial.begin(115200); // UART 0
  tft.fillScreen(TFT_WHITE);
  pinMode(Saida_Rele, OUTPUT);
  pinMode(T_IRQ, INPUT);

  // Display:
  tft.init();
  tft.setRotation(2);
  touch_calibrate();
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_WHITE);
  telaInicio();
  //====================================================================================================================//
}

void loop()
{
  cronometro();
  checkButtonPress();
}