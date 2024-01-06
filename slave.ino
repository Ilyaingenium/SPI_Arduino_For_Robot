#include <avr/io.h>  // библиотека ввода/вывода которая объяснит компилятору какие порты ввода/вывода есть у микроконтроллера
#include <stdio.h> //определения макросов, константы и объявления функций и типов
#include <stdlib.h> //анимающиеся выделением памяти, контролем процесса выполнения программы, преобразованием типов и другие
#include <util/delay.h>  // библиотека для задержек
#define F_CPU 1000000UL
#include <avr/interrupt.h>


// Настройки SPI_slave
void SPI_Slave_Settings (void)
{
	DDRB &= ~((1<<3) | (1<<5)| (1<<2));// активируем пины на вход
	DDRB |= (1<<4);
	
	//SPCR |= (1<<SPIE); // Разрешаем прерывания по SPI, табл 19.5.1
	SPCR |= (1<<SPE); // Включаем SPI
  SPCR &= ~(1<<DORD); // Первым передается старший бит, схема 19-2
	SPCR &= ~(1<<MSTR); // МК Slave
	SPCR |= (1<<CPOL); // Импульсы отрицательной полярности (5-0 в), схема 19-3, более распр. вариант       
	SPCR |= (1<<CPHA); // Выборка данных по заднему фронту, схема 19-3
		
	PORTB |= (1<<2);
}

char SPI_SlaveReceive()
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	
	/* Return data register */
	return SPDR;
}


void PWM_settings_1 (void)
{
	TCCR1A |= (1<<COM1A1);
	TCCR1A &= ~(1<<COM1A0); // НЕинвертирующая  быстродействующая ШИМ- пилообразная форма импульсов опорного напряжения

	TCCR1A |= (1<<WGM10); // 8 бит ШИМ, быстро действ.
	TCCR1A &= ~(1<<WGM11); // 8 бит ШИМ, быстро действ.
	TCCR1B |= (1<<WGM12); // 8 бит ШИМ, быстро действ.
	TCCR1B &= ~(1<<WGM13); // 8 бит ШИМ, быстро действ.

	TCCR1B |= (1<<CS11); // Делим частоту МК на 8
	TCCR1B &= ~((1<<CS10)|(1<<CS12)); // Делим частоту МК на 8 = 125000
	// timer counter control register (0 pin B-настраиваемый, 8-битный) делим частоту счетчика на (8*1024)/1000000=0,008 сек . Частота = 1024\0,008
	// 1000000 / (8*1024)=122 частота работы шим
}



void PWM_settings_2 (void)
{
	TCCR2A |= (1<<COM2B1); // НЕинвертирующая  быстродействующая ШИМ- пилообразная форма импульсов опорного напряжения
	TCCR2A &= ~ (1<<COM2B0);

	TCCR2A |= (1<<WGM20); // 8 бит ШИМ, быстро действ.
	TCCR2A |= (1<<WGM21); // 8 бит ШИМ, быстро действ.
	TCCR2B &= ~(1<<WGM22); // 8бит ШИМ, быстро действ.
	

	TCCR2B |= (1<<CS21); // Делим частоту МК на 8
	TCCR2B &= ~((1<<CS20)|(1<<CS22)); // Делим частоту МК на 8 = 125000
	// timer counter control register (0 pin B-настраиваемый, 8-битный) делим частоту счетчика на (8*1024)/1000000=0,008 сек . Частота = 1024\0,008
	// 1000000 / (8*1024)=122 частота работы шим
}

void PWM_settings_3 (void)
{
	TCCR0A |= (1<<COM0A1);
	TCCR0A &= ~ (1<<COM0A0); // НЕинвертирующая ШИМ
	
	TCCR0A |= (1<<WGM00); // 8 бит ШИМ, быстро действ.
	TCCR0A |= (1<<WGM01); // 8 бит ШИМ, быстро действ.
	TCCR0B &= ~ (1<<WGM02); // 8 бит ШИМ, быстро действ.
	
	TCCR0B &= ~((1<<CS00) | (1<<CS02)); // Делим частоту МК на 8
	TCCR0B |= (1<<CS01); // Делим частоту МК на 8
	
}

void PWM_settings_4 (void)
{
	TCCR0A |= (1<<COM0B1);
	TCCR0A &= ~ (1<<COM0B0); // НЕинвертирующая ШИМ
	
	TCCR0A |= (1<<WGM00); // 8 бит ШИМ, быстро действ.
	TCCR0A |= (1<<WGM01); // 8 бит ШИМ, быстро действ.
	TCCR0B &= ~ (1<<WGM02); // 8 бит ШИМ, быстро действ.
	
	TCCR0B &= ~((1<<CS00) | (1<<CS02)); // Делим частоту МК на 8
	TCCR0B |= (1<<CS01); // Делим частоту МК на 8
	
}


int main(void)
{
	
	SPI_Slave_Settings();
	PWM_settings_1 ();
	PWM_settings_2 ();
	PWM_settings_3 ();
	PWM_settings_4 ();
	
	while (1)
	{
	if (SPDR == 1)
		{
			DDRD = 0b01000000; 
			OCR0A = 200;
					
			DDRB |=  (1<<1); 
			OCR1A = 250;
		}
		else     if (SPDR == 2)
		{
			DDRD = 0b01000000;
			OCR0A = 120;
					
	        DDRB |=  (1<<1); 
			OCR1A = 120;
		
		}
		else    if (SPDR == 3)
		{
			DDRD = 0b01000000;
			OCR0A= 100;
			
			DDRB |=  (1<<1); 
			OCR1A= 100;
			
		}
		else     if (SPDR == 4)
		{
			DDRD = 0b01000000;
			OCR0A= 90;
			
			DDRB |=  (1<<1); 
			OCR1A = 90;
		}
		else     if (SPDR == 5)
		{
			DDRD = 0b00000000;
			
			
			DDRB &= ~(1<<1);
			OCR0A = 0;
			OCR1A = 0;
		}
		else    if (SPDR == 6)
		{
			DDRD = 0b00101000;
			
			OCR0B = 110;
						
			OCR2B = 110;
		}
		else    if (SPDR == 7)
		{
				DDRD = 0b00101000;
								
				OCR0B = 130;
				
				OCR2B = 130;
		}
		else    if (SPDR == 8)
		{
			DDRD = 0b00101000;
						
			OCR0B= 150;
			
			OCR2B= 150;
		}
		else     if (SPDR == 9)
		{
			DDRD = 0b00101000;
			
			OCR0B= 160;
			
			OCR2B= 160;
		}
		else     if (SPDR == 30)
		{
			DDRD = 0b00101000;
			
			OCR0B= 254;
			
			OCR2B= 200;
			}
			
			//another ADC
			
			 else      if (SPDR == 11)
			{
			DDRB |=  (1<<1); 
				OCR1A = 250;
				
				DDRD = 0b00100000;
				
				OCR0B= 250;
			}
			
      else      if (SPDR == 12)
			{
				DDRB |=  (1<<1); 

				OCR1A = 170;
				DDRD = 0b00100000;
				
				
				OCR0B= 170;
				
			}
			else      if (SPDR == 13)
			{
					DDRB |=  (1<<1); 

				OCR1A= 150;
				DDRD = 0b00100000;
				
				
				OCR0B = 150;
				
			}
			else      if (SPDR == 14)
			{
					DDRB |=  (1<<1); 
				OCR1A = 110;
				DDRD = 0b00100000;
				
				OCR0B = 110;
			}
			else      if (SPDR == 15)
			{
								
				OCR1A = 0;
				OCR0B = 0;
			}
			else      if (SPDR == 16)
			{
				
				OCR2B = 110;
				DDRD = 0b01001000;
				
				OCR0A= 110;
			}
			else      if (SPDR == 17)
			{
				
				OCR2B = 150;
				DDRD = 0b01001000;
				
				OCR0A= 150;
			}
			else      if (SPDR == 18)
			{
				
				OCR2B= 170;
				DDRD = 0b01001000;

				OCR0A = 170;
			}
			else      if (SPDR == 19)
			{
				
				OCR2B= 100;
				DDRD = 0b01001000;

				OCR0A = 100;
			}
			else      if (SPDR == 20)
			{
				OCR2B= 250;
				DDRD = 0b01001000;

				OCR0A = 250;
				}
	}
}