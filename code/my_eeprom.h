

#ifndef MY_EEPROM_H_
#define MY_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void MY_EEPROM_init(void);
uint8 MY_EEPROM_writeByte(uint8 u8addr,uint8 u8data);
uint8 MY_EEPROM_writePage(uint8 u8addr,uint8* u8data, uint8 len);
uint8 MY_EEPROM_readByte(uint8 u8addrr,uint8 *u8data);
uint8 MY_EEPROM_readPage(uint8 u8addrr,uint8 *u8data, uint8 len);
uint8 EEPROM_erase();
#endif /* EXTERNAL_EEPROM_H_ */
