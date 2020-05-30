#include "Interpreter.h"
#include "Commands.h"
#include <Arduino.h>

Commands Instrument;

Interpreter::Interpreter(/* args */)
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::Setup(SPIClass x)
{
    Instrument.Setup(x);
    Interpreter::UsedSPI = x;
}

void Interpreter::Interpret(String SerialIn)
{
    String Command = SerialIn.substring(0,4); // find command number
	String value = SerialIn.substring(4, SerialIn.length());

    switch (Command.toInt())
    {
    case COMTEST:
        {
            Serial.println(InitCom);
            break;
        }
    case BID:
        {
            Serial.println(Cid,HEX);
            break;
        }
    case ICID:
        {
            Serial.println(Instrument.GetID());
            break;
        }
    case LSTMSG:
        {
            Serial.println("Not Available");
            break;
        }
    case TEMP:
        {
            Serial.println(Instrument.Temperature());
            break;
        }
    case READCH:
        {
            int Chan = SerialIn.substring(4, 6).toInt();
			int Gain = ((SerialIn.substring(6, 8)).toInt());
			int integration = ((SerialIn.substring(8, 10)).toInt())+ 1;
			double V = 0;
            Serial.println(Chan);
			if (Chan == 3)
			{
				V = Instrument.Ohms(integration);
			}
			else
			{
				V = Instrument.Voltmeter(Chan, Gain, integration, 0xE << 0);
            }
			
			V = Instrument.Intercept[Chan][Gain] + Instrument.Slope[Chan][Gain]*V;
			Serial.print(V,8);
			Serial.print("\t");
			Serial.print(millis());
			Serial.print("\t");
			Serial.print(Chan);
			Serial.print("\t");
			Serial.print(Command);
			Serial.print(value);
			Serial.print("\t");
			Serial.println(pow(2,Gain));
			// InterpMulti::LastV = V; 
			// InterpMulti::LastG = pow(2,Gain); 
			// InterpMulti::LastT = millis();
			// InterpMulti::GainValueSPS[Chan][1] = V;
			// InterpMulti::GainValueSPS[Chan][0] = Gain;
			// InterpMulti::ChannelRead[Chan] = true;
			// InterpMulti::ChannelOn[Chan] = true;
            break;
        }
    case TGLIO:
        {
            digitalWrite((SerialIn.substring(4, 6)).toInt(), !digitalRead((SerialIn.substring(4, 6)).toInt()));
            break;
        }
    case CTRLIO:
        {
            digitalWrite((SerialIn.substring(4, 6)).toInt(),
			(SerialIn.substring(6, SerialIn.length()).toInt())
			);
            break;
        }
    case READIO:
        {
            Serial.println(digitalRead((SerialIn.substring(4, 6)).toInt()));
            break;
        }
    case AREAD:
        {
            Serial.println(analogRead((SerialIn.substring(4, 6)).toInt()));
            break;
        }
    
    default:
        {
            Serial.print(Command); Serial.println("Unknown Command");
            break;
        }
    }

}

void Interpreter::Debug(bool db)
{
    Interpreter::DB = db;
}
