#include <avr/io.h>

float real_temperature = 0;
float temperature_setpoint = 75;


void I2C_StartCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

void I2C_StopCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void I2C_SendByte(unsigned char byte)
{
	TWDR = byte;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	TWCR = (1<<TWINT)|(1<<TWEN);
}

float I2C_ReadTemp()
{
	char MSB, LSB;
	while(!(TWCR&(1<<TWINT)));		
	MSB =  TWDR;								
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));		
	LSB =  TWDR;							
	TWCR = (1<<TWINT)|(1<<TWEN);
	int tmp = (MSB << 8)	+ LSB;
	
	float temp = -40 + tmp * 0.01; 
	
	return temp;
}

int main(void)
{
	DDRC = (1<<DDC3);   
   	TWBR = 0x80;         			
   	TWSR = 0x00;
  
    	while (1) 
    	{
		I2C_StartCondition();
	    	I2C_SendByte(0x03); 
	    	real_temperature = I2C_ReadTemp();
	    	I2C_StopCondition();

		if (real_temperature < temperature_setpoint)
		{
			PORTC |= (1 << PC3); 
		} 
		else
		{
			PORTC = 0x00;			
		}		
    	}
}

