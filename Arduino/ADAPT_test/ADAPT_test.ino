#include <Servo.h>
#include <avr/sleep.h>

#define BATT_FIRST_LEVEL 600
#define BATT_SECOND_LEVEL 700
//Git test
/////////////////////////
//////// Pins ///////////
/////////////////////////
int servo_pin = 9;
int servo_power_pin = 6;
int push_pin = 2;
int muscle_pin = A3;
int status_pin = A5;
int speaker_pin = 12;
int LED_pin = 13;

/////////////////////////
///// Servo things //////
/////////////////////////
Servo servo;
float toggle_threshold = 8.0;
int opened_angle = 65, closed_angle = 0;
int servo_timer = 0;
int timer_threshold = 1500;

/////////////////////////
/// General Variables ///
/////////////////////////
int count = 0;
volatile boolean off_flag = false;
volatile boolean buzzer_flag = false;
int window_size = 100;

/////////////////////////
// Position Variables ///
/////////////////////////
boolean hand_opened = true;
int last_classification = 0;

/////////////////////////
/// Feature Variables ///
/////////////////////////
int emg_data[100];
float sig_var;
int sig_mean;
int sig_max;
int sig_min;
int sig_range;
float sig_ll;
float sig_rms;

/////////////////////////
/// Timing Variables ////
/////////////////////////
unsigned long emg_time, power_time;

////////////////////////////////////////////////////////////////////////////////
/////////////// Feature Extraction Functions and Variables /////////////////////
////////////////////////////////////////////////////////////////////////////////

float getRMS(int* data){
  int sum = 0;
  for(int a = 0; a < window_size; a++){
    sum = sum + (data[a] * data[a]);
  }
  sum = sum/window_size;
  return (float) sqrt(sum);
}

int getMean(int* data){
  int sum = 0;
  for(int a = 0; a < window_size; a++){
    sum += data[a];
  }
  return sum/window_size;
}

int getMax(int* data){
  int max = 0;
  for(int a = 0; a < window_size; a++){
    if(data[a] > max){
      max = data[a];
    }
  }
  return max;
}

int getMin(int* data){
  int min = 1024;
  for(int a = 0; a < window_size; a++){
    if(data[a] < min){
      min = data[a];
    }
  }
  return min;
}

float getVariance(int* data){
  int mean = getMean(data);
  long temp = 0;
  for(int a=0; a < window_size; a++){
    temp += (mean-data[a])*(mean-data[a]);
  }
  return (float) temp/window_size;
}

float getAvgLineLength(int * data){
 int sum = 0;
 for(int a = 0; a < window_size-1; a++){
   sum = sum + abs(data[a+1] - data[a]);
 }
 return (float) sum/(window_size-1);
   
}
////////////////////////////////////////////////////////////////////////////////
/////////////// Timing Functions and Variable //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
boolean delay_without_delaying(unsigned long &since, unsigned long time) {
  unsigned long currentmillis = millis();
  if (currentmillis - since >= time) {
    since = currentmillis;
    return true;
  }
  return false;
}
////////////////////////////////////////////////////////////////////////////////
///////////// Interrupt Service Routine for Button Press////////////////////////
////////////////////////////////////////////////////////////////////////////////
void button(){
  if(digitalRead(push_pin) == HIGH){
    digitalWrite(servo_power_pin, LOW);
    off_flag = true;
  }else{
    digitalWrite(servo_power_pin, HIGH);
    off_flag = false;
  }
}
////////////////////////////////////////////////////////////////////////////////
////////////// Arduino Functions ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  pinMode(muscle_pin, INPUT);
  pinMode(push_pin, INPUT);
  pinMode(servo_power_pin, OUTPUT);
  servo.attach(servo_pin); 
  servo.write(opened_angle);
  digitalWrite(servo_power_pin, HIGH); // Initially powering servo mechanism
  attachInterrupt(1, button, HIGH);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
  sleep_enable();
  
  digitalWrite(LED_pin, HIGH);
  delay(2000);
  digitalWrite(LED_pin, LOW);
  
  
  // Initial state of switch must be set
  if(digitalRead(push_pin) == HIGH){
    digitalWrite(servo_power_pin, LOW);
    off_flag = true; //CHANGED FROM TRUE to FALSE //TODO
  }else{
    digitalWrite(servo_power_pin, HIGH);
    off_flag = false;
  }

}

void loop()
{
  
  int range;
  if(off_flag){
    sleep_mode();
  }else{
    if(delay_without_delaying(power_time, 15000)){
      int temp = analogRead(status_pin);
      if(temp < BATT_FIRST_LEVEL && temp > BATT_SECOND_LEVEL){
        tone(speaker_pin, 261);
        delay(500);
        noTone(speaker_pin);
      }else if(temp < BATT_SECOND_LEVEL){
        digitalWrite(servo_power_pin, LOW);
        off_flag = true; 
      }
    }
    if (delay_without_delaying(emg_time, 2)) {
      emg_data[count] = analogRead(muscle_pin);
      //Serial.println(emg_data[count]);
      count ++;
      
      if(count == window_size){
        count = 0;
        
        //sig_mean = getMean(emg_data);
        //sig_var = getVariance(emg_data);
        sig_max = getMax(emg_data);
        sig_min = getMin(emg_data);
        //Serial.println(sig_var);
        //Serial.println(sig_max);
        //sig_min = getMin(emg_data);
        sig_range = sig_max - sig_min;
        //sig_ll = getAvgLineLength(emg_data);
//        Serial.print("Variance: ");
//        Serial.println(sig_var);
//        Serial.print("Line Length: ");
//        Serial.println(sig_ll);
//        Serial.print("Range: ");
//        Serial.println(sig_range); 
        
        //if(true)
      
        
        if (range >= 100 && servo_timer == timer_threshold)//changed from 85 to 10 TODO
        {
          digitalWrite(LED_pin, HIGH);
          /*if(last_classification == 0){
            last_classification = 1;
          }else if(last_classification == 1){ */
          if(true){
            last_classification = 0;
            hand_opened = !hand_opened;
            if (hand_opened){
              for(int pos = opened_angle * 2; pos > closed_angle * 2; pos-=2)
              { // Closes the hand by gradually adjusting the written angle.     
                servo.write(pos);
                delay(2);
              }
            }else{
              for(int pos = closed_angle * 2; pos < opened_angle * 2; pos+=2) 
              { // Opens the hand by gradually adjusting the written angle.           
                servo.write(pos);
                delay(2);
              }
            }
            // Reset the timer
            servo_timer = 0;
          }
        }else{
          last_classification = 0;
        }
      }
      digitalWrite(LED_pin, LOW);
    }
    // Don't allow the servo_timer to get too big. Overflow errors
    // crash the Arduino.
    if (servo_timer < timer_threshold)
      servo_timer++;
    
    // Delay for the servo. Don't want to overload it.
    delay(1);
  }
}
