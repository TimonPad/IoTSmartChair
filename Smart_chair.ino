#include "HX711.h"

#define LOADCELL_DOUT_PIN 3
#define LOADCELL_SCK_PIN 2

HX711 scale;

float calibration_factor = -1000; //To be added later
bool timer_on = false;
unsigned long start_time;

long safe_time_for_sitting = 5000; // 5 seconds in milliseconds

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN,LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare(); //Let's assume nobody sit on chair when arduino power up
}

void loop() {
  float pressure = scale.get_units();
  Serial.print(pressure);
  if (pressure > 3.0 && !timer_on) { //Setting minimum treshold here, when something more than 3kg on chair it starts timer
    start_time = millis(); //Time is milliseconds!
    timer_on = true;
  }
  if (timer_on && (millis() - start_time) > safe_time_for_sitting) { //Alarm user that they have been sitting too long time
    //High voltage 
    start_time = millis() - 10000; //10 seconds timer before next alarm
  }
  if (timer_on && pressure < 2.0) { //Treshold set to 2kg so user has to fully get up from chair to turn timer of 
    timer_on = false;
  }
}
