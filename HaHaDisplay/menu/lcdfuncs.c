#include <stdlib.h>
#include <iom128v.h>
#include <macros.h>
#include "ascii.h"
#include "atmel_start_pins.h"

typedef unsigned char Byte;

Screen screen[NUMSCREENS];

void set_READ(void){
	 PORTD |= 0x40;
}

void clear_READ(void){
	 PORTD &= ~0x40;
}

void set_A0(void){
	 PORTG |= 0x08;
}

void clear_A0(void){
	 PORTG &= ~0x08;
}

void set_LCD1(void){
	 PORTD |= 0x02; 
}

void clear_LCD1(void){
	 PORTD &= ~0x02;
}

void set_LCD2(void){
	 PORTG |= 0x10;
}

void clear_LCD2(void){
	 PORTG &= ~0x10;
}

void lcd_init(void){
	 Byte side;
	 DDRG  |= 0x18;
	 DDRD  |= 0x42;
	 for(side=1; side<=2; side++){
	 	 lcd_drv(side, 0, 0xAE); //Display off
		 lcd_drv(side, 0, 0xE2); //Reset
		 lcd_drv(side, 0, 0xA4); //Static drive off
		 lcd_drv(side, 0, 0xA9); //Duty 1/32
		 lcd_drv(side, 0, 0xA0); //ADC-forward
		 lcd_drv(side, 0, 0xEE); //Clear Read-Modify-Write mode
		 lcd_drv(side, 0, 0xAF); //Display on
		 lcd_clear();
	 } 
}

void lcd_drv(Byte side, Byte data, Byte lcd_dat){
	 Byte status;
	 clear_LCD1();
	 clear_LCD2();
	 DDRF = 0x00;
	 PORTF = 0xFF;
	 set_READ();
	 clear_A0();
	 do{
		if(side == 1) 
			set_LCD1();
		if(side == 2) 
			set_LCD2();
		status = PINF;
		clear_LCD1();
		clear_LCD2();
	 }while((status & 0x80) != 0);
	 PORTF = 0x00;
	 DDRF = 0xFF;
	 clear_READ();
	 if(data)
	 	set_A0();
	 else
	 	clear_A0();
	 if(side == 1) 
		set_LCD1();
	 if(side == 2) 
		set_LCD2();
	 PORTF = lcd_dat; 	
	 clear_LCD1();
	 clear_LCD2();
} 

Byte lcd_pos(Byte row, Byte seg){
	 Byte side;
	 if(seg < 61) 
	 	side = 1;
	 else{
	 	side = 2;
		seg -= 61;
	 }
	 lcd_drv(side, 0, 0xB8+row);
	 lcd_drv(side, 0, seg);
	 return side;
}

void lcd_ovflow(Byte row){
	 lcd_drv(2, 0, 0xB8+row);
	 lcd_drv(2, 0, 0);
}

void lcd_putchar(Byte row, Byte col, Byte ch){
	 Byte side, i;
	 side = lcd_pos(row, col*6);
	 
	 for(i=0;i<6;i++){
	 	if((col*6) + i == 61){
			lcd_ovflow(row);
			side=2;
		}
	 	lcd_drv(side,1,Ascii[ch-1][i]);	
	 }
}	 	   

void lcd_printstring(Byte row, Byte col, Byte *s){
	 Byte i;
	 i = strlen(s);
	 while(i--){
	 		lcd_putchar(row, col, *s++);
			col++;
	 }
}	

void lcd_printnum(Byte row, Byte col, int val){
	 char string[10];
	 sprintf(string, "%d", val);
	 lcd_printstring(row, col, string);
}

void lcd_clear(void){
	 Byte side, row, col;
	 for(side=1;side<=2;side++){
	 	for(row=0;row<4;row++){
			lcd_drv(side,0,0xB8 + row);
			lcd_drv(side,0,0);
			for(col=0;col<61;col++)
				lcd_drv(side,1,0x00);
		}
	 }
}

void lcd_black(void){
	 Byte side, row, col;
	 for(side=1;side<=2;side++){
	 	for(row=0;row<4;row++){
			lcd_drv(side,0,0xB8 + row);
			lcd_drv(side,0,0);
			for(col=0;col<61;col++)
				lcd_drv(side,1,0xFF);
		}
	 }
}
	