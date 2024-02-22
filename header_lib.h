/*
  Library for global STRUCTURES, ENUMS, VARIABLES and INTERRUPTIONS
  used on others libraries 
*/
#ifndef HEADER_H
#define HEADER_H

/*  
    Definition of commands
    Comands may start with #, $, %, &, @
    All commands must end with ;
*/
#define c_e_stop "#e"         // emergency stop
#define c_stop "#s"           // stop
#define c_clr_sc "#CSC"       // reset step counter   may not be needed anymore
#define c_calibrate "#CAL"    // calibrate load cell
#define c_t_test "#TTS"       // tensile test start (comparation value)

#define c_en_motor "$EM"      // enable motor
#define c_dis_motor "$DM"     // disable motor
#define c_inv_rot "$IR"       // inverted rotation
#define c_no_inv_rot "$NIR"   // no inverted rotation

#define c_f_up "%FU"        // fast continuous movement up
#define c_f_down "%FD"      // fast continuous movement down  
#define c_s_up "%SU"        // fixed step fast movement up (number steps)
#define c_s_down "%SD"      // fixed step fast movement dowm (number steps)
#define c_c_up "%CU"        // controlled speed continuous movement up (comparation value)
#define c_c_down "%CD"      // controlled speed continuous movement dowm (comparation value)
#define c_sc_up "%SCU"      // fixed step controlled speed displacement up (number stps, comparation value)
#define c_sc_down "%SCD"    // fixed step controlled speed displacement down (number stps, comparation value)

#define c_gsc "&SC"         // get step count
#define c_g_lcr "&LCR"      // get load cell read
#define c_g_thr "&THR"      // get threshold
#define c_g_dz "&DZ"        // get deadzone (float)
#define c_g_calib "&CAL"    // get load cell calibration parameters (long offset, float scale)
#define c_g_status "&STS"   // get global variables status

#define c_s_thr "@THR"      // set threshold (float)
#define c_s_dz "@DZ"        // set deadzone (float)
#define c_s_calib "@CAL"    // set load cell calibration parameters (long offset, float scale)
#define c_s_crit "@STC"     // set stop criteria (number [0,1])


enum Operation{
  NO_OP,
  E_STOP,
  STOP,
  EN_MOTOR,
  DIS_MOTOR,
  CLR_SC,
  INV_ROT,
  NO_INV_ROT,
  CALIBRATE,

  F_UP,
  F_DOWN,
  S_UP,
  S_DOWN,           
  C_UP,
  C_DOWN,
  SC_UP,
  SC_DOWN,

  T_TEST,

  G_SC,
  G_LCR,

  S_THR,
  G_THR,

  S_DZ,
  G_DZ,

  S_CRITERIA,

  S_CALIBRATION,
  G_CALIBRATION,

  G_STATUS
};

enum MovDir{    // Movment Direction
  UP,
  DOWN
};

String tokens[5];
int intTokens[5];
float floatTokens[5];
long readedOffset;
float readedScale;

volatile long int stepCount = 0;
volatile bool outState = true;

//bool rotDir;      // may not be necesary
bool invRot = false;
bool enState = false;

long int targetStp;
volatile bool stpTrgActive = false;

volatile bool stepLecture = false;

float loadCReading;

float threshold = 0.5; // it is a percentage
float deadZone = 10.0; // units: g

// stop criteria: false -> threshold near 0 reading, true -> threshold below last reading
bool stopCriteria = true;

// tensile load sign: true -> positive sign load on tensile, false -> negative load sign on tensile
//bool tensileLoadSign = true;    // may not be necesary anymore

void enTimerOC(){
  TCCR1A = 0;
  // prescaler 1024
  //TCCR1A = _BV(WGM11) | _BV(WGM10);
  //TCCR1A = _BV(COM1A0)  | _BV(WGM11) | _BV(WGM10);
  
  TCCR1B = 0;
  //TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12) | _BV(CS10);

  TCNT1  = 0;

  TIMSK1 |= (1 << OCIE1A);
}

void startTimer(int compValue = 6000){
  //cli();
  // interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (2 * prescaler * (compare match register + 1))
  // compare match register = [ 16,000,000Hz/ (2 * prescaler <1024> * desired interrupt frequency) ] - 1
  // 15,624 for 2 Hz of state change
  OCR1A = compValue;
  TCNT1  = 0;
  //stepCount = 0;

  // prescaler 1024
  TCCR1A = _BV(COM1A0)  | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12) | _BV(CS10);
  //sei();
}

void stopTimer(){
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  outState = true;
  interrupts();
}

ISR(TIMER1_COMPA_vect){
  if(stpTrgActive){
    if(stepCount >= targetStp){
      stopTimer();
      stpTrgActive = false;
    }
  }

  outState = !outState;
  if(outState){
    stepCount += 1;
    //stepCount = (rotDir)? (stepCount - 1):(stepCount + 1);
    if(stepLecture){
      stepLecture = false;
    }
  }
}


#endif //HEADER_H