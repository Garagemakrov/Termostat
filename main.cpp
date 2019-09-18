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
	TWCR = (1<<TWINT)|(1<<TWEN);// сбрасываем прерывание
}
float I2C_ReadTemp()
{
	char MSB, LSB;
	while(!(TWCR&(1<<TWINT)));		//ждем старший байт
	MSB =  TWDR;								// получаем старший байт
	TWCR = (1<<TWINT)|(1<<TWEN);// сбрасываем прерывание
	while(!(TWCR&(1<<TWINT)));		//ждем младший байт
	LSB =  TWDR;							// получаем младший байт
	TWCR = (1<<TWINT)|(1<<TWEN);// сбрасываем прерывание
	int tmp = (MSB << 8)	+ LSB;
	
	float temp = -40 + tmp * 0.01; // формула перевода из даташит
	
	return temp;
}
int main(void)
{
   // Инициализация порта
	DDRC = (1<<DDC3); // на вывод PC3 подключен нагреватель

   // Инициализация i2c
   TWBR = 0x80;         			// Настроим битрейт
   TWSR = 0x00;
   
   
    /* Replace with your application code */
    while (1) 
    {
		   	I2C_StartCondition();
		   	I2C_SendByte(0x03); // чтение температуры
		   	real_temperature = I2C_ReadTemp();
		   	I2C_StopCondition();

		
		if (real_temperature < temperature_setpoint)
		{
			PORTC |= (1 << PC3); // включаем нагрев
		} 
		else
		{
			PORTC = 0x00;			// выключаем нагрев
		}
		
		
    }
}

