char inData[20]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character


void setup(){

 Serial.begin(9600);
 Serial.write("Send \"Start\r\n\" to begin");
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

void loop()
{
  Serial.print("Hello");
}
