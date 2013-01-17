// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version
// 2 of the License, or (at your option) any later version.
//
// Copyright (C) 2005-2012 Gediminas Simanskis,8devices,<gediminas@8devices.com>
// 
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this file see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.


#include "leds.h"

// blink
u32 blink_state_time = 0;
u8 blink_state = 0;
u8 blink_type = LED_OFF;

extern volatile bool CANAdapter_stateChange;

///////////////////////////////////////////////////////////////////////////////
// ledOn
//

void ledOn( u8 nled )
{
  switch ( nled ) {
    case RED:
      GPIO_WriteBit( GPIO0, GPIO_Pin_1, Bit_SET );
      //GPIO_WriteBit( GPIO1, GPIO_Pin_1, Bit_SET );            
      break;
    case GREEN:
      GPIO_WriteBit( GPIO0, GPIO_Pin_2, Bit_SET );
      //GPIO_WriteBit( GPIO1, GPIO_Pin_2, Bit_SET );      
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////
// ledOff
//

void ledOff( u8 nled )
{
  switch ( nled ) {
    case RED:
      GPIO_WriteBit( GPIO0, GPIO_Pin_1, Bit_RESET );
      //GPIO_WriteBit( GPIO1, GPIO_Pin_1, Bit_RESET );      
      break;
    case GREEN:
      GPIO_WriteBit( GPIO0, GPIO_Pin_2, Bit_RESET );
      //GPIO_WriteBit( GPIO1, GPIO_Pin_2, Bit_RESET );      
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////
// setBlinkType
//

void setBlinkType( u8 type  )
{
  blink_state_time = 0;
  blink_type = type;
  //CANAdapter_stateChange = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ledOff
//
// This routine is called from the 1 ms interrupt and blink
// lamp in the selected patters
//


void blink( void ) 
{
  blink_state_time++;
  
  switch( blink_type ) {

    case LED_ON:
      ledOn( RED );
      ledOn( GREEN );
      break;    
    
    case LED_OFF:
      ledOff( RED );
      ledOff( GREEN );
      break;
    
    case LED_RED_STEADY: 
      ledOn( RED );
      ledOff( GREEN );
      break;
    
    case LED_GREEN_STEADY: 
      ledOn( GREEN );
      ledOff( RED );
      break;

   case LED_TOGGLE_SLOW:
      if ( 0 == blink_state ) {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( RED );
          ledOn( GREEN );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOn( RED );
          ledOff( GREEN );
          blink_state = 0;
        }  
      }     
     break;      
      
   case LED_TOGGLE_FAST:
      if ( 0 == blink_state ) {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( RED );
          ledOn( GREEN );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOn( RED );
          ledOff( GREEN );
          blink_state = 0;
        }  
      }     
     break;
      
    case LED_RED_BLINK_FAST:
      ledOff( GREEN );
      if ( 0 == blink_state ) {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( RED );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOn( RED );
          blink_state = 0;
        }  
      }
      break;
    
    case LED_RED_BLINK_SLOW: 
      ledOff( GREEN );
      if ( 0 == blink_state ) {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( RED );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOn( RED );
          blink_state = 0;
        }  
      }
      break;
    
    case LED_GREEN_BLINK_FAST: 
      ledOff( RED );
      if ( 0 == blink_state ) {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( GREEN );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOn( GREEN );
          blink_state = 0;
        }  
      }
      break;
    
    case LED_GREEN_BLINK_SLOW: 
      ledOff( RED );
      if ( 0 == blink_state ) {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( GREEN );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOn( GREEN );
          blink_state = 0;
        }  
      }
      break;
    
    case LED_RED_GREEN_BLINK_FAST:
      if ( 0 == blink_state ) {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( RED );
          ledOn( GREEN );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > FAST_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( GREEN );
          ledOn( RED );
          blink_state = 0;
        }  
      }
      break;
      
    case LED_RED_GREEN_BLINK_SLOW:
      if ( 0 == blink_state ) {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( RED );
          ledOn( GREEN );
          blink_state = 1;
        }
      }
      else {
        if (  blink_state_time > SLOW_BLINKING_TIME ) {
          blink_state_time = 0;
          ledOff( GREEN );
          ledOn( RED );
          blink_state = 0;
        }  
      }
      break;
      
  }
}
