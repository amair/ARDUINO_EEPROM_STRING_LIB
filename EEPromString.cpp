#include <EEPROM.h>
#include <String.h>
#include <Arduino.h>


#include "EEPromString.h"

//Logger m_logger =Logger::getInstance();


//Define a lookup table at start of eeprom which allows us to identify strings
// Byte[0]: NUMBER_OF_STRINGS
// Byte[1]: ADDRESS_START_STRING_1_HIGH_BYTE
// Byte[2]: ADDRESS_START_STRING_1_LOW_BYTE
// Byte[3]: LENGTH_STRING_1
// Byte[4]: ADDRESS_START_STRING_2_HIGH_BYTE
// Byte[5]: ADDRESS_START_STRING_2_LOW_BYTE
// Byte[6]: LENGTH_STRING_2
// etc
// Data is stored from BYTE[EEPROM.length] backwards

EEPROMSTRING::EEPROMSTRING()
{
  num_strings = EEPROM.read(0);
}

// This will never be called, but if we were destroying the class
// ensure that we persist the number of strings
EEPROMSTRING::~EEPROMSTRING()
{
   EEPROM.write(0, num_strings);
}

void EEPROMSTRING::WipeEeprom()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  dump_eeprom();
  num_strings = EEPROM.read(0);
}

  unsigned int EEPROMSTRING::freeEepromSpace()
  {
    unsigned char num_strings = EEPROM.read(0);
    unsigned int used_memory = 0;
    
    for (unsigned char i=0; i< num_strings; i++)
    {
      used_memory = EEPROM.read(2*(i+1));
    }
    
    return (maxEepromSpace() - used_memory);
  }
  
  unsigned int EEPROMSTRING::maxEepromSpace()
  {
    return EEPROM.length();
  }

bool EEPROMSTRING::pushString(const char * new_string)
{
  bool bOK = false;
  int free_mem = EEPROM.length()-1-(num_strings*3); //Remove the fixed table size 1 byte for num strings, 3 bytes for each table entry

  debug("Free mem - table", free_mem);

  debug_table();
  
  for (char str=0; str<num_strings; str++)
    free_mem = free_mem - getStringLength(str);

    debug("Actual free mem", free_mem);

  if (strlen(new_string)<(free_mem-3) && strlen(new_string) <= 255) // Remember to leave room for table to grow
  {
    unsigned int start_address;
   
    if (num_strings==0)
      start_address = EEPROM.length()-1; // EEPROM is empty so start at the end
    else // Calculate the end of the last string
      start_address = getStartAddress(num_strings) - getStringLength(num_strings); // Last start address - length

    increaseNumStrings();
    
    writeStringLength(num_strings, strlen(new_string));
    writeStartAddress(num_strings, start_address);
    
    for (int i=0; i<strlen(new_string); i++)
    {
      EEPROM.write(start_address-i, new_string[i]);
    }
    bOK = true;
    dump_eeprom();
  } else
  {
    debug ("Not enough space or string too long", free_mem);
  }
  return bOK;
}

bool EEPROMSTRING::readString(const unsigned char string_num, char * string_value)
{
  bool bOK = false;
  
  if ((string_num <= num_strings)) // Need to protect against buffer overflow
  {
    for (int i=0; i<getStringLength(string_num); i++)
    {
      string_value[i]=EEPROM.read(getStartAddress(string_num)-i);
    }

    string_value[getStringLength(string_num)]=0;
    
    bOK = true;
  }
  return bOK;
}

void EEPROMSTRING::increaseNumStrings( void)
{
    num_strings++;
    EEPROM.write(0, num_strings );  
}

void EEPROMSTRING::writeStringLength(const unsigned char string_num, const unsigned char len)
{
  EEPROM.write(3*num_strings, len);
}


void EEPROMSTRING::writeStartAddress(const unsigned char string_num, const unsigned int s_a)
{
    EEPROM.write((3*string_num)-2, highByte(s_a) );
    EEPROM.write((3*string_num)-1, lowByte(s_a) );
}

const unsigned int EEPROMSTRING::getStartAddress(const unsigned char string_num )
{
  unsigned int s_a;
  byte high_byte, low_byte;

  if (string_num>0 and string_num <= num_strings)
{
  high_byte = EEPROM.read((3*string_num)-2);
  low_byte = EEPROM.read((3*string_num)-1);

  s_a = word(high_byte, low_byte);

} else
  {
    s_a=EEPROM.length(); //Default to the first string (if one even exists)
    debug("String Overflow", string_num);
  }
  return (s_a);
}

const unsigned char EEPROMSTRING::getStringLength(const unsigned char string_num )
{
  return (EEPROM.read(3*num_strings));
}

 void EEPROMSTRING::debug(char const * str, const unsigned int val)
 {
   Serial.print("DEBUG: ");
   Serial.print(str);
   Serial.print(": ");
   Serial.println(val);
 }

void EEPROMSTRING::debug_table()
{
    debug("num strings",EEPROM.read(0));
    
    for (int i=0; i< num_strings; i++)
    {
      debug("string", i);
      debug("start", getStartAddress(i));
      debug("length", getStringLength(i));
    }
}

void EEPROMSTRING::dump_eeprom()
{
  for (int i=0; i<EEPROM.length(); ++i)
  {
    Serial.print(EEPROM.read(i));
    Serial.print(" ");
  }
  Serial.println();       
}

    

