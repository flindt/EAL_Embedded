/*
 * readkeys.c
 *
 *  Created on: Aug 23, 2012
 *      Author: flindt
 */


#include "../events.h"
#include "readkeys.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_types.h"
//#include "hw_sysctl.h"
#include "sysctl.h"
#include "gpio.h"

// This function translates a set of bits read from the key IO ports into key events.
// Adding more keys is a matter of adding a new enum value, and one reading
// one more port in ReadKeys()
int
GetKeyEvents(void)
{
  static int KeyPressFlag = 0;
  static int Count[NO_EVENT] =
    { 0 };
  int LoopCount;
  int RawKeys;

  RawKeys = ReadKeys();

  for (LoopCount = 0; LoopCount < NO_EVENT; LoopCount++)
    {
      if (RawKeys & (1 << LoopCount))
        {
          Count[LoopCount]++;
          if (( Count[LoopCount] >= KEY_PRESS_MINIMUM) && !(KeyPressFlag & (1<<LoopCount)))
            {
              KeyPressFlag |=  (1<<LoopCount);
              Count[LoopCount] = 0;
              return LoopCount;
            }
        }
      else
        {
          KeyPressFlag &=  ~(1<<LoopCount);
          Count[LoopCount] = 0;
        }


    }

  return NO_EVENT;
}

// This function will read the actual io  ports associated with each key
// This is the only function besides initHW, that will ever need to know
// where each button is connected
int
ReadKeys(void)
{
  int KeyBits = 0;
  if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0))
    KeyBits |= 1 << KEY_UP;
  if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1))
    KeyBits |= 1 << KEY_DOWN;
  if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2))
    KeyBits |= 1 << KEY_CANCEL;
  if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3))
    KeyBits |= 1 << KEY_ENTER;
  return KeyBits;
}
