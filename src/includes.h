#include <FS.h>

#include <TFT_eSPI.h>

#include <Adafruit_GFX.h>   

#include "Free_Fonts.h"

#include "VG.h"

#define REPEAT_CAL_TFT false

#define CALIBRATION_FILE_TFT "/TouchCalData1"

#define botao 34

#define Saida_Rele  32

#define LED_BUILTIN 2

#define T_IRQ 27

#define TFT_DARKRED    0x8000  // Exemplo de definição de cor (Valores aproximados para Dark Red)
#define TFT_SLATEGRAY  0x7412  // Exemplo de definição de cor (Valores aproximados para Slate Gray)
