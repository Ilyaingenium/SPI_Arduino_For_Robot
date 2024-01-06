#include <avr/io.h>  // библиотека ввода/вывода которая объяснит компилятору какие порты ввода/вывода есть у микроконтроллера
#include <stdio.h> //определения макросов, константы и объявления функций и типов
#include <stdlib.h> //анимающиеся выделением памяти, контролем процесса выполнения программы, преобразованием типов и другие
#include <util/delay.h>  // библиотека для задержек
#define F_CPU 1000000UL
#include <avr/interrupt.h>


// Настройки SPI_Master
void SPI_Master_Settings (void)
{
	DDRB |= (1<<3) | (1<<5) | (1<<2);// активируем пины на выход
	
	// "Этот пин нужно установить раньше
	// настроек регистра контроля, т.е. раньше команды SPCR |= (1<<MSTR)
	PORTB |= (1<<2);
	
	DDRB &= ~(1<<4);// активируем пин на вход
	//SPCR |= (1<<SPIE); // Разрешаем прерывания по SPI, табл 19.5.1
	SPCR |= (1<<SPE); // Включаем SPI
	SPCR &= ~(1<<DORD); // Первым передается старший бит, схема 19-2
	SPCR |= (1<<MSTR); // МК Master
	SPCR |= (1<<CPOL); // Импульсы отрицательной полярности (5-0 в), схема 19-3, более распр. вариант
	SPCR |= (1<<CPHA); // Выборка данных по заднему фронту, схема 19-3
  // Без удвоения частоты, т.к. славе работает только при частоте деленной на 4
	// Делим частоту МК на 16
	SPSR &= ~(1<<SPI2X);
	SPCR &= ~(1<<SPR1);
	SPCR |= (1<<SPR0);
}

void SPI_transmit(char cData)//Функция передачи байта
{
	SPDR=cData;//Начинаем передачу
	while (!(SPSR & (1<<SPIF)));//Ждем пока передача завершиться
}

void ADC_Init()			/* ADC Initialization function */
{
	ADMUX |= (1<<REFS0); // Опорное напряжение - внешние 5 В
	ADMUX &= ~(1<<REFS1);
	
	ADMUX &= ~(1<<ADLAR); // Правостороннее выравнивание
	
	ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1));// Задействуем 1-й вывод АЦП (РС1)
	ADMUX |= (1<<MUX0);
	
	ADCSRA |= (1<<ADEN); // Разрешаем работу АЦП

	ADCSRA |= (1<<ADATE); // Режим непрырывного измерения
	
	ADCSRA &= ~(1<<ADPS2); //делим частоту Мк на 8 - Устанавливаем частоту преобразования АЦП 125 000 Гц такие цифры нужны чтобы более точно отслеживать изменения (рекомендуется от 50 до 200 КГц)
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);
}

int ADC_Read(char channel)	/* ADC Read function */
{
	ADMUX = 0x40 | (channel & 0x03); //считывание показателей с разных пинов 1 и 0 (64  /  3)
	ADCSRA |= (1<<ADSC); // Запуск преобразования АЦП
	
	while (!(ADCSRA & (1<<ADIF))); // проверяем наличие 1 в реестре. Если 0, то начинаем считывание ацп.
	ADCSRA |= (1<<ADIF); // устанавливаем бит в 1, останавливаем работу АЦП. В ноль сбросится сам через несколько тактов.
	_delay_ms(1);
	return ADC;
}

int main(void)
{
	
	SPI_Master_Settings();
	
	ADC_Init();		/* Initialize ADC */
	
	//Вход АЦП
	DDRC &= ~(1<<0);
	DDRC &= ~(1<<1);
	
	while (1)
	{
		
		if(ADC_Read(0)<30)
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(1);
			_delay_ms(30);
		}
		
		else		if((ADC_Read(0)>=30) && (ADC_Read(0)<150))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(2);
			_delay_ms(30);
		}
		else		if ((ADC_Read(0)>=150) && (ADC_Read(0)<250))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(3);
			_delay_ms(30);
		}
		else		if ((ADC_Read(0)>=250) && (ADC_Read(0)<350))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(4);
			_delay_ms(30);
		}
		else		if ((ADC_Read(0)>=350) && (ADC_Read(0)<=575))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(5);
			_delay_ms(30);
		}
		else		if((ADC_Read(0)>=585) && (ADC_Read(0)<615))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(6);
			_delay_ms(30);
		}
		
		else		if ((ADC_Read(0)>=615) && (ADC_Read(0)<715))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(7);
			_delay_ms(30);
		}
		else		if ((ADC_Read(0)>=715) && (ADC_Read(0)<815))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(8);
			_delay_ms(30);
		}
		
		else		if ((ADC_Read(0)>=915) && (ADC_Read(0)<975))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(9);
			_delay_ms(30);
		}
		else		if(ADC_Read(0)>=925)
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(30);
			_delay_ms(30);
		}
		//another ADC
		// else		
    if(ADC_Read(1)<115)
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(11);
			_delay_ms(30);
		}
		 else		if((ADC_Read(1)>=0) && (ADC_Read(1)<115))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(12);
			_delay_ms(30);
		}
		else		if ((ADC_Read(1)>=115) && (ADC_Read(1)<215))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(13);
			_delay_ms(30);
		}
		else		if ((ADC_Read(1)>=215) && (ADC_Read(1)<315))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(14);
			_delay_ms(30);
		}
		else		if ((ADC_Read(1)>=415) && (ADC_Read(1)<=575))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(15);
			_delay_ms(30);
		}
		else		if((ADC_Read(1)>=585) && (ADC_Read(1)<615))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(16);
			_delay_ms(30);
		}
		else		if ((ADC_Read(1)>=615) && (ADC_Read(1)<715))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(17);
			_delay_ms(30);
		}
		else		if ((ADC_Read(1)>=715) && (ADC_Read(1)<815))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(18);
			_delay_ms(30);
		}
		
		else		if ((ADC_Read(1)>=915) && (ADC_Read(1)<975))
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(19);
			_delay_ms(30);
		}
		else		if(ADC_Read(1)>=975)
		{
			PORTB &= ~(1<<2);
			
			SPI_transmit(20);
			_delay_ms(30);
		}
		
	}
	
}

