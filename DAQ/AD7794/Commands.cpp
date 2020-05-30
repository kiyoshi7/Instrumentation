#include "Commands.h"
#include "AD779X.h"
AD779X XADC;

Commands::Commands(/* args */)
{
	
}

Commands::~Commands()
{
}

void Commands::Setup(SPIClass x)
{
	XADC.SetSPI(x, 10, 12);
}

int Commands::GetID()
{
    return XADC.GetID();
}

double Commands::Temperature()
{
	return XADC.Temperature();
}


double Commands::Ohms(int integrate)
{
    double V = 0;
	int i = 0;
	while(i<= integrate-1)
	{
		double Vin = 0;
		// Change current if needed
		if (Commands::inow != Commands::Current)
		{
			digitalWrite(XADC.CS, 0);
			XADC.Configure(W | IO, IEXCDIR_11_22 | Commands::Current);
			digitalWrite(XADC.CS, 1);
			Commands::inow = Commands::Current;
			delay(100);
		}
		
		switch (Commands::Current)
		{
			case IEXCDisable:
			{
				Commands::Current = IEXC1mA;
				XADC.Configure(W | IO, IEXCDIR_11_22 | Commands::Current);
				i--;
			}
			break;
			
			case IEXC10uA:
			{
				Vin = (double)XADC.Volts(G1, 3, 0);
				if (Vin<0.045)
				{
					Commands::Current = IEXC210uA;
					XADC.Configure(W | IO, IEXCDIR_11_22 | Commands::Current);
					i--;
				}
				else
				{
					Commands::Current = IEXC10uA;
					Vin = Vin/Commands::Currents[Commands::Current];
					Vin = -68.39591 + Vin*.99946;
					V = V + (Vin);
				}
			}
			break;
			
			case IEXC210uA:
			{
				Vin = (double)XADC.Volts(G1, 3, 0);
				if (Vin<=1 && Vin>=0.2)
				{
					Commands::Current = IEXC210uA;
					Vin = Vin/Commands::Currents[Commands::Current];
					V = V + (108.00138 + 0.96123*Vin);
				}
				else if(Vin>1)
				{
					Commands::Current = IEXC10uA;
					XADC.Configure(W | IO, IEXCDIR_11_22 | Commands::Current);
					i--;
				}
				else if (Vin<=0.2)
				{
					Commands::Current = IEXC1mA;
					XADC.Configure(W | IO, IEXCDIR_11_22 | Commands::Current);
					i--;
				}
			}
			break;
			
			case IEXC1mA:
			{
				Vin = (double)XADC.Volts(G1, 3, 0);
				if (Vin>=1.1)
				{
					Commands::Current = IEXC210uA;
					XADC.Configure(W | IO, IEXCDIR_11_22 | Commands::Current);
					i--;
				}
				else
				{
					Commands::Current = IEXC1mA;
					Vin = Vin/Commands::Currents[Commands::Current];
					V = V + (-6.988 + 1.2351*Vin );
				}
			}
			break;
		}
		
	i++;	
	}
	return V;
}

double Commands::Voltmeter(int Chan, int Gain, int integration, int SPS)
{
    double V = 0;
	for (int i = 0; i<= integration-1; i++)
	{
		V += (double)XADC.Volts(Gain << 8, Chan, 0);
	}
	V = V/integration;
	
	switch (Chan)
	{
		case 0:
		{
			V= V;
		}
		case 1:
		{
			V= V*100;
		}
		break;
		case 2:
		{
			V = V*100;
		}
		break;
		case 5:
		{
			V = V/0.1;
		}
		break;
	}
	
	return V;
}
