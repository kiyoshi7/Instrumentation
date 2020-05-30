#ifndef INTERPRETER_H
#define	INTERPRETER_H
#include <Arduino.h>
#include <SPI.h>

class Interpreter
{
private:
	bool DB = false;
	int LSTMSG;
	SPIClass UsedSPI;
	
public:
	Interpreter(/* args */);
	~Interpreter();
	void Setup(SPIClass x);
	void Interpret(String SerialIn);
	void Debug (bool db);
};


#endif