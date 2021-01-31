
#include "i2c.h"
#include "my_eeprom.h"

void MY_EEPROM_init(void)
{
	TWI_init();
}

/*write byte
 * 1 - start bit
 * 2- control byte (write =0)  + ack
 * 3- word address + ack
 * 4- write data + ack
 * 5-stop bit
 * */
uint8 MY_EEPROM_writeByte(uint8 u8addr, uint8 u8data)
{
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;

    TWI_write(0xA8);
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;


    TWI_write(u8addr);
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    TWI_write(u8data);
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    TWI_stop();

    return SUCCESS;
}


/*random read
 * 1 - start bit
 * 2- control byte (write =0)  + ack
 * 3- word address + ack
 * 4- start bit
 * 5- control byte (read = 1 ) + ack
 * 6- read data + nack
 * 7-stop bit
 * */
uint8 MY_EEPROM_readByte(uint8 u8addr, uint8 *u8data)
{
// 1- start
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;

//2-  control byte (write =0)  + ack
    TWI_write(0xA8);
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;

//3- word address + ack
    TWI_write(u8addr);
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

// 4- start
    TWI_start();
    if (TWI_getStatus() != TW_REP_START)
        return ERROR;

 //5- control byte (read = 1 ) + ack
    TWI_write(0xA8 | 1);  // read =1
    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
        return ERROR;

// 6- read date
    *u8data = TWI_readWithNACK();
    if (TWI_getStatus() != TW_MR_DATA_NACK)
        return ERROR;
 // 7- stop
    TWI_stop();
    return SUCCESS;
}


uint8 MY_EEPROM_writePage(uint8 u8addr, uint8* u8data,  uint8 len)
{
	TWI_start();
	    if (TWI_getStatus() != TW_START)
	        return ERROR;

	    TWI_write(0xA8);
	    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
	        return ERROR;


	    TWI_write(u8addr);
	    if (TWI_getStatus() != TW_MT_DATA_ACK)
	        return ERROR;

	    for (uint8 i =0; i<len; i++)
	    {
	        TWI_write(u8data[i]);
	        if (TWI_getStatus() != TW_MT_DATA_ACK)
	            return ERROR;
	        _delay_ms(30);
	    }

	    TWI_stop();

	    return SUCCESS;
}

uint8 MY_EEPROM_readPage(uint8 u8addr, uint8 *u8data,  uint8 len)
{
	// 1- start
	    TWI_start();
	    if (TWI_getStatus() != TW_START)
	        return ERROR;

	//2-  control byte (write =0)  + ack
	    TWI_write(0xA8);
	    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
	        return ERROR;

	//3- word address + ack
	    TWI_write(u8addr);
	    if (TWI_getStatus() != TW_MT_DATA_ACK)
	        return ERROR;

	// 4- start
	    TWI_start();
	    if (TWI_getStatus() != TW_REP_START)
	        return ERROR;

	 //5- control byte (read = 1 ) + ack
	    TWI_write(0XA8 | 1);  // read =1
	    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
	        return ERROR;

	    for (uint8 i =0; i<len; i++)
	   	    {
	    		u8data[i] = TWI_readWithACK();
	    	    if (TWI_getStatus() != TW_MR_DATA_ACK)
	    	        return ERROR;
	    	    _delay_ms(50);
	   	    }
	    // 7- stop
	       TWI_stop();
	       return SUCCESS;
}

uint8 EEPROM_erase()
{
	TWI_start();
		    if (TWI_getStatus() != TW_START)
		        return ERROR;

		    TWI_write(0xA8);
		    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
		        return ERROR;


		    TWI_write(0x00);
		    if (TWI_getStatus() != TW_MT_DATA_ACK)
		        return ERROR;

		    for (uint8 i =0; i<8; i++)
		    {
		        TWI_write(0x00);
		        if (TWI_getStatus() != TW_MT_DATA_ACK)
		            return ERROR;
		        _delay_ms(30);
		    }

		    TWI_stop();

		    return SUCCESS;
}
