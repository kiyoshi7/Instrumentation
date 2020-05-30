// add all functions here
#include <SPI.h>
#include <Arduino.h>
#include <stdarg.h>
#include "AD779X.h"


#define NOPSingle 0xFF
#define NOPCont	  0x00

//  AD779X::AD779X(int ChipSelect, int MISOPin)
// {
// 	AD779X::CS = ChipSelect;
// 	AD779X::MISO = MISOPin;
// }

void AD779X::SetSPI(SPIClass x, int ChipSelect, int MISOPin)
{
	AD779X::spi = x;
	AD779X::CS = ChipSelect;
	AD779X::MISO = MISOPin;
	pinMode(AD779X::CS, OUTPUT);
	digitalWrite(AD779X::CS, HIGH);
	pinMode(AD779X::MISO, INPUT);
	AD779X::spi.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0)); // SPI setup SPI communication
	delay(200);          // SPI delay for initialization
	AD779X::spi.begin();         // SPI Module 'wake up'
	
}

void AD779X::Reset(void)
{
	SPIWrite(0xFF);
	SPIWrite(0xFF);
	SPIWrite(0xFF);
	SPIWrite(0xFF);
	delay(10);
}

void AD779X::SetContinuousRead(void)
{
	digitalWrite(AD779X::CS, 0);
	SPIWrite(0x5C);
}

void AD779X::SPIWrite(byte x)
{
	AD779X::spi.transfer(x);
}

byte AD779X::SPIRead(byte x)
{
	return AD779X::spi.transfer(x);
}

void AD779X::Configure(int ComReg, int value)
{
	AD779X::ContRead = ((ComReg) & 0b100) >> 2;
	
	if (ComReg == CONFIG)
	{
		AD779X::Gain = pow(2,(value & (0x7 << 8)) >> 8);
	}
	switch (ComReg)
	{
		case IO:
		break;
		default:
		SPIWrite(ComReg);
		SPIWrite(value >> 8);
		SPIWrite(value & 0xff );
		break;
	}

}

float AD779X::Temperature(void)
{
	digitalWrite(AD779X::CS, 0);

	Configure(CONFIG, VBIASDisable | BODisable | Bipolar | BoostOff | G1
	| REFSELInternal | Ref_DetOff | BUFOn | TempSensor);
	Configure(MODE, Single | PSWOpen | AMPCMSet | Int64 | CHOPEnable | F4);
	float temp = (SingleRead() - 0x800000)*(1.17/8388608);
	temp = temp/(0.81/1000)-273;
	digitalWrite(AD779X::CS, 1);
	return temp;
}

byte AD779X::GetID(void)
{
	digitalWrite(AD779X::CS, 0);
	SPIWrite(0b01100000);
	byte reply = SPIRead(NOPSingle);
	digitalWrite(AD779X::CS, 0);
	return reply;
}

int32_t AD779X::GetOffSetRegister(void)
{
	digitalWrite(8, 0);
	SPIWrite(R | OFFSET | SREAD );
	int32_t data;
	for (int i=0; i<3; i++)
	{
		data |= SPIRead(0xFF);
		if ((i == 0) | (i == 1))
		{ data <<= 8;}
	}
	digitalWrite(8, 1);
	return data;
}

int32_t AD779X::GetFullScaleRegister(void)
{
	digitalWrite(8, 0);
	SPIWrite(R | FULL_SCALE | SREAD );
	int32_t data;
	for (int i=0; i<3; i++)
	{
		data |= SPIRead(0xFF);
		if ((i == 0) | (i == 1))
		{ data <<= 8;}
	}
	digitalWrite(8, 1);
	return data;
}

int32_t AD779X::ContinuousRead(void)
{
	while(digitalRead(AD779X::MISO)==1){;;}
	int32_t data;
	for (int i=0; i<1; i++)
	{
		data |= SPIRead(NOPSingle);
		if ((i == 0) | (i == 1))
		{ data <<= 8;}
	}
	return data;
}

int32_t AD779X::SingleRead(void)
{
	while(digitalRead(AD779X::MISO)==1){;;}
	SPIWrite(0x58);
	int32_t data;
	for (int i=0; i<3; i++)
	{
		data |= SPIRead(NOPSingle);
		if ((i == 0) | (i == 1))
		{ data <<= 8;}
	}
	return data;
}

double AD779X::ConvertToVolts(int32_t data)
{
	double v = ((double)(data-8388608)*1.17)/(8388608*AD779X::Gain);
	return v;
}

double AD779X::Volts(int Gain, int channel, int cont)
{
	digitalWrite(AD779X::CS, 0);
	Configure(CONFIG, VBIASDisable | BODisable | Bipolar | BoostOff | Gain
	| REFSELInternal | Ref_DetOff | BUFOn | channel );
	double V;

	if (cont == SREAD)
	{
		V = ConvertToVolts(SingleRead());
		digitalWrite(AD779X::CS, 1);
	}
	if (cont == CREAD)
	{
		Configure(MODE | SREAD, Cont | PSWOpen | AMPCMSet | Int64 | CHOPEnable | F4);
		V = ConvertToVolts(ContinuousRead());
	}
	return V;
}
