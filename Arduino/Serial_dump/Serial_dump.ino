#define data_length 500 

//Serial Read Variables
char inData[20]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character

int data[data_length];
int emg_index = 0;
int emg_pin = A3;
int LED_pin = 13;


//Timing Variables
unsigned long emg_time;

void setup(){
 pinMode(emg_pin, INPUT);
 Serial.begin(9600);
 Serial.println("start");
 digitalWrite(LED_pin, LOW);
 
}


String readSerial(){
  while(Serial.available() > 0) // Don't read unless
                                                 // there you know there is data
  {
      if(index < 19) // One less than the size of the array
      {
          inChar = Serial.read(); // Read a character
          inData[index] = inChar; // Store it
          index++; // Increment where to write next
          inData[index] = '\0'; // Null terminate the string
      }
  }
  // Now do something with the string (but not using ==)
}

void dump(){
  digitalWrite(LED_pin, HIGH);
   for(int i = 0; i < data_length; i++){
      Serial.println(data[i], DEC); // check
   } 
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

void loop()
{
  if(delay_without_delaying(emg_time, 2))
{
  Serial.println(analogRead(emg_pin));
  delay(100);
 }
}
