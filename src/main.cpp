#include <Arduino.h>
#include <math.h>

const int LED = 11;

const int B_OFF = 2;
const int B_LOW = 4;
const int B_MEDIUM = 7;
const int B_HIGH = 8;

const int OPTIONS = 4;

const double INCREMENTAL_FACTOR = 50;

void setupInputs();
void setupOutputs();
void setButtonsState();
void smoothCurrent();
double easeInOutExpo(double t);


int powerBtns[OPTIONS] = {B_OFF, B_LOW, B_MEDIUM, B_HIGH};
int targetPower[OPTIONS];
int* currentBtn;
int* currentTarget;
double currentIntensity;

void setup(){
  Serial.begin(9600);

  setupInputs();
  setupOutputs();

  for(int i = 0; i < OPTIONS; ++i){
    targetPower[i] = pow(2, i*(8.0/3.0));
  }

  currentBtn = &powerBtns[0];
  currentTarget = &targetPower[0];
  currentIntensity = targetPower[0];
}

void setupInputs(){
  for(int i = 0; i < OPTIONS; ++i){
    pinMode(powerBtns[i], INPUT);
  }
}

void setupOutputs(){
  pinMode(LED, OUTPUT);
}



void loop(){
  setButtonsState();
  smoothCurrent();
  analogWrite(LED, currentIntensity);
}


void setButtonsState(){
  for(int i = 0; i < OPTIONS; ++i){
    int btnState = digitalRead(powerBtns[i]);
    if(btnState){
      currentBtn = &powerBtns[i];
      currentTarget = &targetPower[i];
    }
  }
}

void smoothCurrent(){
  if(currentIntensity > targetPower[OPTIONS - 1]){
    currentIntensity = targetPower[OPTIONS - 1];
  }
  if(currentIntensity < targetPower[0]){
    currentIntensity = targetPower[0];
  }

  Serial.println(currentIntensity);
  double scaledIntensity = currentIntensity / targetPower[OPTIONS - 1];

  if(currentIntensity < *currentTarget){
    currentIntensity += easeInOutExpo(scaledIntensity)*70;
  }
  
  if(currentIntensity > *currentTarget){
    currentIntensity -= easeInOutExpo(scaledIntensity)*70;
  }

  Serial.println(currentIntensity);
  

  double difference = *currentTarget - currentIntensity;

  if(difference < 1 && difference > -1){
    currentIntensity = *currentTarget;
  }
}

double easeInOutExpo( double t ) {
  if( t < 0.5 ) {
    return 2 * t * t;
  } else {
    return -1 + t * (4 - 2 * t);
  }
}