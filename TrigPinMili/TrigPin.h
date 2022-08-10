#ifndef TrigPin_h
#define TrigPin_h

#include <inttypes.h>

class TrigPin
{

public:
  TrigPin(uint8_t _pino, unsigned long _highMs, unsigned long _lowMs, uint8_t _disparoContinuo, uint8_t _highLowDefault );
  void setInterval(unsigned long   _highMs, unsigned long _lowMs);
  void setModoOp(unsigned long _disparoContinuo);
  void sethighLowDefault(uint8_t _highLowDefault);
  int update();
  int read();
  void trigger();
  void enable(unsigned long _running);
  void write(uint8_t new_state);
  
private:
  unsigned long  relogioAnterior, highMs, lowMs, running;
  uint8_t state;
  uint8_t pin;
  uint8_t disparoContinuo;
  uint8_t Trig;
  uint8_t highLowDefault;
  
};

#endif


