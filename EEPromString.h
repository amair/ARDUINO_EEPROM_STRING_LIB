#ifndef EEPROMSTRING_H
#define EEPROMSTRING_H

class EEPROMSTRING {
public:
  EEPROMSTRING();
  ~EEPROMSTRING();
  
  void WipeEeprom();
  
  bool pushString(const char * new_string);
  bool readString(const unsigned char string_num, char * string_value);
  
  unsigned int freeEepromSpace();
  unsigned int maxEepromSpace();
  
private:
  unsigned char num_strings;

  void increaseNumStrings(void);
  
  void writeStartAddress(const unsigned char string_num, const unsigned int s_a);
  void writeStringLength(const unsigned char string_num, const unsigned char len);

  const unsigned int getStartAddress(const unsigned char string_num );
  const unsigned char getStringLength(const unsigned char string_num );
  void debug(char const * str, const unsigned int val);
  void debug_table( void );
  void dump_eeprom( void );

};

#endif

