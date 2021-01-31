#include "lcd.h"
#include "my_eeprom.h"

void get_password();
uint8 compare_pass(uint8* a, uint8* b);
void open_door();
void close_door();
int main(void)
{
	LCD_init();
	MY_EEPROM_init();

	DDRB |= (1<<PD0) | (1<<PD1) ;
	PORTB &= ~ 0X03;
	DDRB |= (1<< PB2);
	PORTB &= ~ (1<< PB2);
	uint8 first_time = 1;
	uint8 my_pass1[4];
	uint8 my_pass2[4];
	uint8 memory_pass[4];
	uint8 trial = 0;
	uint8 key ;
	while(1)
	{
		if (first_time)
		{
			LCD_clearScreen();
			LCD_displayString(" SET PASSWORD  ");
			get_password(&my_pass1);
			LCD_clearScreen();
			LCD_displayString("REENTER PASSWORD");
			get_password(&my_pass2);
			if (compare_pass(my_pass2, my_pass1))
			{
				LCD_clearScreen();
				LCD_displayString("PASSWORD IS SET");
				_delay_ms(500);
				first_time = 0;
				MY_EEPROM_writePage(0x00, my_pass2,4);
				_delay_ms(50);
				MY_EEPROM_readPage(0x00, memory_pass ,4);
			}
			else
			{
				LCD_clearScreen();
				LCD_displayString("PASSWORD IS WRONG");
				_delay_ms(500);
			}
		} // end if first time

		else // start screen ( enter or reset)
		{
			LCD_clearScreen();
			LCD_displayString("*:RESET PASSWORD");
			LCD_goToRowColumn(1,0);
			LCD_displayString("#:ENTER BUILDING");
			key = KeyPad_getPressedKey();

			if (key == '*')
			{
				first_time = 1;
				EEPROM_erase();
				_delay_ms(500);

			}// if reset
			else if(key == '#')
			{
				LCD_clearScreen();
				LCD_displayString("ENTER PASSWORD");
				get_password(my_pass1);
				if (compare_pass(memory_pass, my_pass1))
				{
					LCD_clearScreen();
					LCD_displayString("OPENING DOOR");
					open_door();
					LCD_clearScreen();
					LCD_displayString("CLOSING DOOR");
					close_door();
				}
				else
				{
					LCD_clearScreen();
					LCD_goToRowColumn(0,4);
					LCD_displayString("WRONG!!");
					_delay_ms(500);
					trial ++;
					if (trial == 3 )
					{
						LCD_clearScreen();
						LCD_displayString("CALL THE POLICE");
						PORTB |=(1<< PB2);
						_delay_ms(1000);
						PORTB &= ~ (1<< PB2);
						trial = 0;
					}
				}
			}//if enter

		}// end else first time
	}// end while
}// end main


void get_password(uint8* pass)
{
	LCD_goToRowColumn(1,4);
	LCD_displayString("----");
	LCD_goToRowColumn(1,4);
	for (uint8 i=0; i<4; i++)
	{
		pass[i] = KeyPad_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
}
uint8 compare_pass(uint8* a, uint8* b)
{
	if ( b[1]==a[1] && b[2]==a[2] && b[3]==a[3] && b[0]==a[0])
		return 1;
	else
		return 0;
}

void open_door()
{
	PORTB &= (~(1<<PC0));
	PORTB |= (1<<PC1);
	_delay_ms(2000);
}
void close_door()
{
	PORTB |= (1<<PC0);
	PORTB &= (~(1<<PC1));
	_delay_ms(2000);
	PORTB &= ~ 0X03;
}
