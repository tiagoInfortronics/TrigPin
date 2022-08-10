#include "Arduino.h"
#include "TrigPin.h"

/// <summary>
/// Initialize TrigPin
/// </summary>
/// <param name="_pino">Output arduino pin</param>
/// <param name="_highMs">Time the output pin will stay HIGH (in milliseconds)</param>
/// <param name="_lowMs">Time the output pin will stay LOW (in milliseconds)</param>
/// <param name="_continuous">True = continuos trigger; False = only trigger</param>
/// <param name="_highLowDefault">True = continuos trigger; False = only trigger</param>
TrigPin::TrigPin(uint8_t _pino, unsigned long _highMs, unsigned long _lowMs, uint8_t _continuous, uint8_t _highLowDefault )
{
	setInterval(_highMs,_lowMs);
	setModoOp(_continuous);
	sethighLowDefault(_highLowDefault);
	relogioAnterior = millis();
	state = digitalRead(_pino);
    this->pin = _pino;
	this->running = 0;
}

void TrigPin::setInterval( unsigned long _highMs, unsigned long _lowMs)
{
  this->highMs = _highMs;
  this->lowMs = _lowMs;
}

void TrigPin::setModoOp(unsigned long _disparoContinuo)
{
  this->disparoContinuo = _disparoContinuo;
}

void TrigPin::enable(unsigned long _running)
{
  this->running = _running;
}
void TrigPin::sethighLowDefault(uint8_t _highLowDefault)
{
	this->highLowDefault = _highLowDefault;
}
 
void TrigPin::write(uint8_t new_state)
{
	state = new_state;
	digitalWrite(pin, state);
}

int TrigPin::read()
{
	return (int)state;
}

void TrigPin::trigger()
{
  this->Trig = 1;
}

int TrigPin::update()
{
	uint8_t newState = digitalRead(pin);
	if (disparoContinuo == 1)
	{
		if((highLowDefault == LOW && state == LOW && running == 0) || (highLowDefault == HIGH && state == HIGH && running == 0))
		{
			return 0;
		}
		if (state == HIGH ) 
		{
			if (millis() - relogioAnterior >= highMs) 
			{
				state = LOW;
				digitalWrite(pin, state);
				relogioAnterior = millis();
				return 1;
			}
		}
		else 
		{
			if (millis() - relogioAnterior >= lowMs) 
			{
				state = HIGH;
				digitalWrite(pin, state);
				relogioAnterior = millis();
				return 1;
			}
		}		
	}
	else
	{
		if(state != highLowDefault)
		{
			if ((millis() - relogioAnterior >= lowMs && highLowDefault == LOW) || (millis() - relogioAnterior >= highMs && highLowDefault == HIGH) ) 
			{
				state = highLowDefault;
				digitalWrite(pin, state);
				relogioAnterior = millis();
				return 1;
			}
		}
		else
		{
			if(Trig == 1)
			{
				if (highLowDefault == LOW) 
				{
					state = HIGH;
				}		
				else
				{
					state = LOW;
				}
				Trig = 0;
				digitalWrite(pin, state);
				relogioAnterior = millis();
				return 1;
			}
		}
	}

  
  return 0;

}



