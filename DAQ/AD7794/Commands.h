#include <SPI.h>

#ifndef COMMANDS_H
#define COMMANDS_H

#define COMTEST 0x00
#define BID     0x01
#define ICID    0x02
#define LSTMSG  0x03
#define TEMP    0x04
#define READCH  0x05
#define TGLIO   0x06
#define CTRLIO  0x07
#define READIO  0x08
#define AREAD   0x09
// #define COMTEST 0x0A
// #define COMTEST 0x0B
// #define COMTEST 0x0C
// #define COMTEST 0x0D
// #define COMTEST 0x0E
// #define COMTEST 0x0F

// Return Codes
#define InitCom "sKIAjfrcfvmo2CbzOsogvkR9dm7ZOaSN"
#define Cid 0x7794  // Multimeter ID

class Commands
{
private:
    int inow = 0;
	int Current = 0;
	SPIClass UsedSPI;
	
	
public:
    Commands(/* args */);
    ~Commands();
	void Setup(SPIClass x);
    int GetID();
    void Read();
    double Ohms(int integrate);
    double Voltmeter(int Chan, int Gain, int integration, int SPS);
	double Temperature();


    double Currents[4] = {0, 0.00000973, 0.000200, 0.001};
    double Intercept[6][7]={ // coeficiente linear
		{0,0,0,0,0,0,0}, // canal zero(Ain1) g1, g2,g4,g8...
		{0,0,0,0,0,0,0}, // canal um  (Ain2) g1, g2,g4,g8...
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0}
	};
	
	double Slope[6][7]={ // coeficiente angular
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1}
	};
	double GainValueSPS[6][3]={ // potencia de 2
		{2,0,0}, // ganho, leitura (conversao) e Samples per sec na 2
		{2,0,0},
		{2,0,0},
		{2,0,0},
		{2,0,0}
	};
};

#endif
