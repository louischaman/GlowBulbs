
const int ShiftPWM_latchPin = 11;

#define SHIFTPWM_NOSPI
const int ShiftPWM_dataPin = 8;
const int ShiftPWM_clockPin = 12;

const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h>

void serialCom();
void setLEDs(void);

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 200;
unsigned int numRegisters = 1;
unsigned int numOutputs = numRegisters*8;
unsigned int numRGBLeds = numRegisters *8/3;
unsigned int fadingMode = 0;

unsigned long startTime = 0;
unsigned long currentTime = 0;
int timeOut = 1000;
byte levels[8] = {0, 0, 0, 0, 0, 0, 0};

int rClear = 9;
int enable = 10;

int led1 = 4;
int led2 = 5;
int led3 = 6;
int led4 = 7;

void setup() {

  Serial.begin(19200);
  Serial.print("Initializing...");
  
  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  
  pinMode(rClear, OUTPUT);
  pinMode(enable, OUTPUT);
  
  digitalWrite(rClear, HIGH);
  digitalWrite(enable, LOW);
  
  ShiftPWM.SetAmountOfRegisters(numRegisters);
  
  ShiftPWM.Start(pwmFrequency, maxBrightness);
    
  Serial.println("done.");
  
}

void loop() {
  currentTime = millis();
  if(currentTime-startTime > timeOut){
    for (int i = 0; i < 8; i++){
      levels[i] = 0;
    }
    setLEDs();
    
    while(Serial.available() < 9){
      delay(100);
    }
  }
  serialCom();
  setLEDs();
  
}

void serialCom(){
  if(Serial.available() >= 9){
    if(Serial.read() == 'l'){
      startTime = currentTime;
      for (int i = 0;  i < numOutputs; i++){
        levels[i] = Serial.read();
      }
/*      for (int i = 0; i < numOutputs-1; i++){
        Serial.print(levels[i]);
        Serial.print(", ");
      }
      Serial.println(levels[7]);*/
    }
  }
}

void setLEDs(void){
  for(int i = 0; i < numOutputs; i++){
    ShiftPWM.SetOne(i, levels[i]);
  }
}
