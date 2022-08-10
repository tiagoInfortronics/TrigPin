#include "Arduino.h"
#include "TrigPinMicro.h"


TrigPinMicro::TrigPinMicro(uint8_t _pino, unsigned long _highMs, unsigned long _lowMs, uint8_t _disparoContinuo, uint8_t _highLowDefault )
{
	setInterval(_highMs,_lowMs);
	setModoOp(_disparoContinuo);
	sethighLowDefault(_highLowDefault);
	relogioAnterior = micros();
	if(_disparoContinuo == 2)
		relogioAnterior -= _lowMs;
	state = digitalRead(_pino);
    this->pin = _pino;
	this->running = 0;
	this->qtd_executouPulso = 0;
}

void TrigPinMicro::setInterval( unsigned long _highMs, unsigned long _lowMs)
{
  this->highMs = _highMs;
  this->lowMs = _lowMs;
}

void TrigPinMicro::setModoOp(unsigned long _disparoContinuo)
{
  this->disparoContinuo = _disparoContinuo;
}

void TrigPinMicro::enable(unsigned long _running)
{
  this->running = _running;
  if(_running == 0)
  {
	  this->qtd_executouPulso = 0;
  }
}
void TrigPinMicro::sethighLowDefault(uint8_t _highLowDefault)
{
	this->highLowDefault = _highLowDefault;
}
 
void TrigPinMicro::write(uint8_t new_state)
{
	state = new_state;
	digitalWrite(pin, state);
}

int TrigPinMicro::read()
{
	return (int)state;
}

void TrigPinMicro::trigger()
{
  this->Trig = 1;
}

int TrigPinMicro::update()
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
			if (micros() - relogioAnterior >= highMs) 
			{
				state = LOW;
				digitalWrite(pin, state);
				relogioAnterior = micros();
				return 1;
			}
		}
		else 
		{
			if (micros() - relogioAnterior >= lowMs) 
			{
				state = HIGH;
				digitalWrite(pin, state);
				relogioAnterior = micros();
				return 1;
			}
		}		
	}
	else if (disparoContinuo == 2)
	{
		if((highLowDefault == LOW && state == LOW && running == 0) || (highLowDefault == HIGH && state == HIGH && running == 0))
		{
			return 0;
		}
		if (state == HIGH ) 
		{
			if (micros() - relogioAnterior >= highMs && qtd_executouPulso < 3) 
			{
				state = LOW;
				digitalWrite(pin, state);
				relogioAnterior = micros();
				if(qtd_executouPulso == 2)
					qtd_executouPulso++;
				return 1;
			}
		}
		else 
		{
			if (micros() - relogioAnterior >= lowMs && qtd_executouPulso < 2) 
			{
				state = HIGH;
				digitalWrite(pin, state);
				relogioAnterior = micros();
				qtd_executouPulso++;
				return 1;
			}
		}		
	}
	else
	{
		if(state != highLowDefault)
		{
			if ((micros() - relogioAnterior >= lowMs && highLowDefault == LOW) || (micros() - relogioAnterior >= highMs && highLowDefault == HIGH) ) 
			{
				state = highLowDefault;
				digitalWrite(pin, state);
				relogioAnterior = micros();
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
				relogioAnterior = micros();
				return 1;
			}
		}
	}

  
  return 0;

}



