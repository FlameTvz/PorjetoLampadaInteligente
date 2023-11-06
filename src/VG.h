bool              Pausa_Timer;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

unsigned  char    Div_P100mS=0;
unsigned  char    Tempo_100mS = 0;

unsigned char Aux_1 = 0;
unsigned char QLoop = 0;

volatile int  interruptCounter = 0;


unsigned char   Banco_ESenha[4];
unsigned char   Banco_NSenha[4];
bool btnTurnOnPressed = false;
bool btnTurnOffPressed = false;
bool btnTogglePressed = false;