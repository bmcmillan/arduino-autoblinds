#include <Stepper.h>

#define STEPSINROTATION 200
#define ROTATIONSINWINDOW 26
#define BUTTON_UP_PIN 13
#define BUTTON_DOWN_PIN 12
#define STEPSIZE 10
#define BUTTON_HOLD_THRESHOLD 140

Stepper stepper(STEPSINROTATION, 3,5,6,9);

int currposition = 0;
int lastup = 0;
int lastdown = 0;
int direction = 0;
unsigned long time;
unsigned long btnupdowntime = 0;
unsigned long btndndowntime = 0;
unsigned long lockuptime = 0;
unsigned long lockdowntime = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Ready");
  stepper.setSpeed(80);
}
 
void loop()
{ 
  delay(1);
  time = millis();
  int currup = digitalRead(BUTTON_UP_PIN);
  int currdn = digitalRead(BUTTON_DOWN_PIN);
  //if (currup == 1) Serial.println("UP");
  //if (currdn == 1) Serial.println("DOWN");
  
  if (currup == 1 && currdn == 1) {
    currposition = 0;
    btnupdowntime = 0;
    btndndowntime = 0;
    direction = 0;
    return;
  }

  if(currup == 1) {
    Serial.print(time); Serial.print(" ");
    Serial.println("UP PRESSED");
    if (btnupdowntime == 0) 
    {
      btnupdowntime = millis();
      if (direction == 0) direction = 1;
      else if (direction == 1) direction = 0;
      else if (direction == -1) direction = 0;
      if (direction == 0) lockuptime = millis();
      Serial.print(time); Serial.print(" ");
      Serial.print("DIRECTION ");
      Serial.println(direction);
    }
  } else if (currup == 0) {
    if (lastup == 1) {Serial.print(time); Serial.print(" ");
      Serial.println("UP RELEASED");
      if (btnupdowntime != 0 && (time - btnupdowntime) < BUTTON_HOLD_THRESHOLD) {
        if (direction == -1) direction = 0;
        else if (time - lockuptime > BUTTON_HOLD_THRESHOLD) direction = 1;      
      } else {
        if (direction == 1) direction = 0;      
      }
      btnupdowntime = 0;
      Serial.print(time); Serial.print(" ");
    Serial.print("DIRECTION ");
    Serial.println(direction);
    }
  }
    
  if(currdn == 1) { 
    Serial.print(time); Serial.print(" ");
    Serial.println("DOWN PRESSED");
    if (btndndowntime == 0) {
      btndndowntime = millis();
      if (direction == 0) direction = -1;
      else if (direction == 1) direction = 0;
      else if (direction == -1) direction = 0;
      if (direction == 0) lockdowntime = millis();
      Serial.print(time); Serial.print(" ");
      Serial.print("DIRECTION ");
      Serial.println(direction);
    }
  } else if (currdn == 0) {
    if (lastdown == 1) {
      Serial.print(time); Serial.print(" ");
      Serial.println("DOWN RELEASED");
      if (btndndowntime != 0 && (time - btndndowntime) < BUTTON_HOLD_THRESHOLD) {
        if (direction == 1) direction = 0;
        else if (time - lockdowntime > BUTTON_HOLD_THRESHOLD)  direction = -1;      
      } else {
        if (direction == -1) direction = 0;      
      }
      btndndowntime = 0;
      Serial.print(time); Serial.print(" "); 
    Serial.print("DIRECTION ");
    Serial.println(direction); 
    }
  }

  if (direction != 0) {
    if (direction == 1) Serial.print("UP ");
    if (direction == -1) Serial.print("DN ");
    Serial.print("Curr Pos = ");
    Serial.print(currposition);
    Serial.print(" / ");
    Serial.println(STEPSINROTATION * ROTATIONSINWINDOW);
    currposition = currposition + (STEPSIZE * direction);
  }
  
  if (currposition >= STEPSINROTATION * ROTATIONSINWINDOW) currposition = STEPSINROTATION * ROTATIONSINWINDOW;
  if (currposition <= 0) currposition = 0;
  
  if (currposition <= STEPSINROTATION * ROTATIONSINWINDOW - STEPSIZE && currposition >= STEPSIZE) {
    stepper.step(STEPSIZE * direction);
  } else {
    direction = 0;
  }
  
  lastup = currup;
  lastdown = currdn;
}









