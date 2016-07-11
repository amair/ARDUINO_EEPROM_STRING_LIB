#include "EEPromString.h"

  EEPROMSTRING * pEeprom;

  #define number_words 10
  unsigned char index=0;
  char string_array[number_words][10];

void setup() {
  // put your setup code here, to run once:
  pEeprom = new EEPROMSTRING();
  
    Serial.begin(9600); 
  pEeprom->WipeEeprom();

    strncpy (string_array[0],"the", strlen("the")+1);
    strncpy (string_array[1],"cat", strlen("cat")+1);
    strncpy (string_array[2],"sat", strlen("sat")+1);
    strncpy (string_array[3],"on", strlen("on")+1);
    strncpy (string_array[4],"silly", strlen("silly")+1);
    strncpy (string_array[5],"furry", strlen("furry")+1);
    strncpy (string_array[6],"that", strlen("that")+1);
    strncpy (string_array[7],"it", strlen("it")+1);
    strncpy (string_array[8],"is", strlen("is")+1);

}

void loop() {
  unsigned int i;
  
  i = pEeprom->maxEepromSpace();
  
  Serial.print("Max EEProm Space: ");
  Serial.println(i);

  i = pEeprom->freeEepromSpace();
  
  Serial.print("Free EEProm Space: ");
  Serial.println(i);


  if (pEeprom->pushString(string_array[index]))
  {
    char newString[10];
    
  index=(index+1)%number_words;

    pEeprom->readString(index,newString);
    Serial.print("String: ");
    Serial.println(newString);
  }
  delay (10000);
}
