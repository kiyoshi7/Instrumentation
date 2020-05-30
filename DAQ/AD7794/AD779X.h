// ADS.h
// add mux options

#ifndef AD779X_H
#define AD779X_H

#include <Arduino.h>
#include <stdarg.h>
#include <SPI.h>

// Region communication register

// communication register options
#define WEN			0B0 << 7 // --1 bit
#define R			0B1 << 6 // --1 bit
#define W			0B0 << 6 // --1 bit
#define CREAD		0B1 << 2 // --1 bit
#define SREAD		0B0 << 2 // --1 bit

// communication register Selection
#define COM			0x0 << 3 // --8  bits  W
#define STATUS		0x0 << 3 // --8  bits  R
#define MODE		0x1 << 3 // --16 bits  R/W
#define CONFIG		0x2 << 3 // --16 bits  R/W
#define DATA		0x3 << 3 // --16/24 bits  R
#define ID			0x4 << 3 // --8  bits  R
#define IO			0x5 << 3 // --8  bits  R/W
#define OFFSET		0x6 << 3 // --16 bits  R/W
#define FULL_SCALE	0x7 << 3 // --16 bits  R/W

// EndRegion communication register

// Region Status Register
#define RDY 0 // LOW IF DATA READY
#define ADDRDY 7 //
#define ERR 1 // HIGH IF ERROR: OVERRANGE, UNDERRANGE
#define AD92 1 // IS 92
#define AD93 0 // IS 93

// EndRegion Status Register

// Operation Options
// Region Mode Register
// Region Operating Modes
#define Cont 0x0 << 13
#define Single 0x1 << 13
#define Idle   0x2 << 13
#define PowerDown 0x3 << 13
#define IntZeroScaleCal 0x4 << 13
#define IntFullScaleCal 0x5 << 13
#define SysZeroScaleCal 0x6 << 13
#define SysFullScaleCal 0x7 << 13
// EndRegion Operating Modes
// Region Power Switch Control Bit
#define PSWClose 0x1 << 11
#define PSWOpen  0x0 << 11
// EndRegion Power Switch Control Bit
// Region Instrumentation Amplifier Common-Mode Bit
#define AMPCMSet 0x1 << 9
#define AMPCMClear 0x0 << 9
// EndRegion Instrumentation Amplifier Common-Mode Bit
// Region ADC Clock Source
#define Int64 0x0 << 6
#define Int64AtCLKPin 0x1<< 6
#define Ext64 0x2 << 6
#define ExtCLK 0x3 << 6
// EndRegion ADC Clock Source
// Region CHOP Bit
#define CHOPEnable 0x0 << 4
#define CHOPDisable 0x1 << 4
// EndRegion CHOP Bit
// Region Filter Update Rate
#define F470		0x0 << 0
#define F242		0x1 << 0
#define F123		0x2 << 0
#define F62		    0x3 << 0
#define F50			0x4 << 0
#define F39			0x5 << 0
#define F33			0x6 << 0
#define F19			0x7 << 0
#define F16_80db	0x8 << 0
#define F16_65db	0x9 << 0
#define F12			0xA << 0
#define F10			0xB << 0
#define F8			0xC << 0
#define F6			0xD << 0
#define F4  		0xF << 0
// EndRegion Filter Update Rate
// EndRegion Mode Register

// Region CONFIGURATION REGISTER
// Region Bias Voltage Generator bit
#define VBIASDisable 0x0 << 14
#define VBIASA1		 0x1 << 14
#define VBIASA2		 0x2 << 14
#define VBIASA3		 0x3 << 14
// EndRegion Bias Voltage Generator bit
// Region Burnout Current Bit
#define BOEnable  0x1 << 13
#define BODisable 0x0 << 13
// EndRegion Burnout Current Bit
// Region Unipolar/Bipolar Bit
#define Uipolar 0x1 << 12
#define Bipolar 0x0 << 12
// EndRegion Unipolar/Bipolar Bit
// Region Boost Bit
#define BoostOn  0x1 << 11
#define BoostOff 0x0 << 11
// EndRegion Boost Bit
// Region Gain Select
#define G1   0x0 << 8
#define G2   0x1 << 8
#define G4   0x2 << 8
#define G8   0x3 << 8
#define G16  0x4 << 8
#define G32  0x5 << 8
#define G64  0x6 << 8
#define G128 0x7 << 8
// EndRegion Gain Select
// Region Reference Select
#define REFSEL1		   0x0 << 6
#define REFSEL2		   0x1 << 6
#define REFSELInternal 0x2 << 6
#define REFSELReserved 0x3 << 6
// EndRegion Reference Select
// Region Reference Detect Function Bit
#define Ref_DetOff 0x0 << 5
#define Ref_DetOn 0x1 << 5
// EndRegion Reference Detect Function Bit
// Region buffer mode operation Bit
#define BUFOff 0x0 << 4
#define BUFOn  0x1 << 4
// EndRegion buffer mode operation Bit
// Region Channel Select
#define CH1 0x0
#define CH2 0x1
#define CH3 0x2
#define CH4 0x3
#define CH5 0x4
#define CH6 0x5
#define TempSensor 0x6
#define VddMonitor 0x7
#define CH1Neg 0x8
// EndRegion Channel Select
// EndRegion CONFIGURATION REGISTER

// Region IO Register
// Region IO Enable Bit
#define IODis 0x0 << 6
#define IOEn 0x1 << 6
// EndRegion IO Enable Bit
// Region IO Pin State (High/Low)
#define IOP2L_P1L	0B00 << 4
#define IOP2L_P1H	0B01 << 4
#define IOP2H_P1L	0B10 << 4
#define IOP2H_P1H	0B11 << 4
// EndRegion IO Pin State (High/Low)
// Region Direction of Current Sources
#define IEXCDIR_11_22	0x0 << 2
#define IEXCDIR_12_21	0x1 << 2
#define IEXCDIR_11_21	0x2 << 2
#define IEXCDIR_12_22	0x3 << 2
// EndRegion Direction of Current Sources
// Region Value of Excitation Current
#define IEXCDisable 0x0
#define IEXC10uA	0x1
#define IEXC210uA	0x2
#define IEXC1mA		0x3
// EndRegion Value of Excitation Current
// EndRegion IO Register



class AD779X
{
	public:
	
	//AD779X(int ChipSelect, int MISOPin);
	SPIClass spi;
	int CS;
	int MISO;
	int ContRead = false;
	int Gain = 128;
	//
	void SetSPI(SPIClass x, int ChipSelect, int MISOPin);
	void Reset(void);
	void SetContinuousRead();
	void SPIWrite(byte x);
	byte SPIRead(byte x);
	void Configure(int ComReg, int value);	// Configure register
	
	float Temperature(void); // configures to read once and returns conversion
	byte GetID(void); // returns ID register
	int32_t GetOffSetRegister(void); // returns offset register value
	int32_t GetFullScaleRegister(void); // returns full scale register
	int32_t ContinuousRead(void); // reads data without using Chip select pin
	int32_t SingleRead(void); // reads data using Chip select pin
	double ConvertToVolts(int32_t data);
	double Volts( int Gain, int channel, int cont);
	protected:
	private:
};

#endif



