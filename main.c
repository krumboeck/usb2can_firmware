
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


#include "75x_lib.h"
#include "stdio.h"
#include "main.h"
#include "lists.h"
#include "string.h"
#include "leds.h"
#include "i2c_ee.h"

//USB includes
#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "messages.h"
#include "configuration.h"
#include "buffers.h"

u8 buffer_cmd_out[VIRTUAL_COM_PORT_DATA_SIZE];
u8 buffer_cmd_in[VIRTUAL_COM_PORT_DATA_SIZE];
volatile u8 count_cmd_out = 0;
volatile u8 count_cmd_in = 0;

extern unsigned short slow_crc16(unsigned short,unsigned char *,unsigned int);
extern unsigned short fast_crc16(unsigned short,unsigned char *,unsigned int);

u8 buffer_data_out[VIRTUAL_COM_PORT_DATA_SIZE];
u8 buffer_data_in[VIRTUAL_COM_PORT_DATA_SIZE];
volatile u8 count_data_out = 0;
volatile u8 count_data_in = 0;

extern volatile u32 GlobalTick;

extern unsigned short __checksum;
extern unsigned int __checksum_begin;
extern unsigned int __checksum_end;

// ERROR & status variables
u32   ErrTimerTick;
u8    ErrStatus = 0;
u8    ErrStatusNext = 0;
u8    ErrLec = 0;
u8    ErrLecNext = 0;

STATISTICS Statistics;

// USB2CAN state . open,closed
volatile bool bRun = FALSE;
volatile bool bRunNext = FALSE;

// last read CAN traceiver status
volatile u16 USB2CAN_status;
volatile u16 USB2CAN_next_status;

canmsg      TxCanMsg;
canmsg      RxCanMsg;

canmsg      RxCanMsgList[3]; // ivesta 1.4 versijoje del USB dubliavimo

canmsg      RxCanMsg0;
canmsg      RxCanMsg1;
canmsg      RxCanMsg2;
u8          RxMsgCnt = 0;

canmsg      TmpCanMsg;
bool        TxCanFlag = FALSE;
bool        RxCanFlag = FALSE;

///////////////////////////////////////////////////////////////////////////////
// Statistics Init
//

void StatisticsInit(void)
{
  Statistics.ReceivedFrames = 0;
  Statistics.TransmitedFrames = 0;
  Statistics.ReceivedDataBytes = 0;
  Statistics.TransmitedDataBytes = 0;
  Statistics.Overruns = 0;
  Statistics.Warnings = 0;
  Statistics.BusOff = 0;
}

u16 usb_tx_index = 0;  
vu16 total_len = 0;

bool EP1_busy = FALSE;
bool EP2_dataReady = FALSE;

u16 sum = 0;
const int zero = 0;


///////////////////////////////////////////////////////////////////////////////
// main
//

int main()
{  
  u8 x,y; 
  bool tmp_bRunNext,tmp_bRun;
  
  #ifdef DEBUG
    debug();
  #endif        

     sum = fast_crc16(sum,(unsigned char *)&__checksum_begin, (unsigned int)&__checksum_end -(unsigned int)&__checksum_begin + 1);

     if(sum!=__checksum)
     {
       sum++;
      // return 0;        
     }
    
    for(x=0,y=2;x<8;x++)
    {
      Virtual_Com_Port_StringSerial[y] = conf.Serial[x];   
      y += 2;
    }    
  
  //FLASH_DeInit();
  Set_System();
  Set_TB();  
  Set_USBClock();  
  USB_Interrupts_Config();
  
  setBlinkType( LED_TOGGLE_FAST );    
  USB_Init();    
  //CAN_Config();
  
  CanListInit(&CanRXlist);
  CanListInit(&CanTXlist);
  
  setBlinkType( LED_RED_STEADY );
   
  while(1)
  {    
    
//////////////// USB2CAN state changes //////////////////////    
    
    tmp_bRunNext = bRunNext;
    tmp_bRun = bRun;
    
    if( tmp_bRunNext != tmp_bRun )
    { 
      if( bRunNext == TRUE )            
        setBlinkType( LED_GREEN_STEADY  );                   
      else            
        setBlinkType( LED_RED_STEADY );                 
      
     // betkoks busenu pasikeitimas
     CanListClear( &CanTXlist );
     CanListClear( &CanRXlist ); 
     TxCanFlag = FALSE;
     
     ErrStatus = 0;
     ErrStatusNext = 0;
     ErrLec = 0;
     ErrLecNext = 0;
     USB2CAN_status = 0;
     StatisticsInit();
     
     /////// ! //////////
     EP2_dataReady = FALSE;
     SetEPRxValid(ENDP2);
     
     bRun = bRunNext;                    
    } 
    
//////////////////////////////////////////////////////////////////////////
//  if RUN
//    
    if( bRun == FALSE )   
      continue;

//////////////////////////////////////////////////////////////////////// 
//   USB2CAN EROR state changes  
//  
    
     USB2CAN_status = CAN->SR; 
     
     ErrStatusNext = (u8) (( USB2CAN_status >> 5 )& 0x07);    
     ErrLecNext = (u8) ( USB2CAN_status & 7 );   // LEC bits only

     if ( ErrStatus ^ ErrStatusNext )
     {                        
        if( ErrStatusNext & USB2CAN_BOFF )
        {
          Statistics.BusOff++;
          setBlinkType( LED_RED_BLINK_FAST ); 
          SendErrorMessage( CANAL_STATUSMSG_BUSOFF );
        }
        else if( ErrStatusNext & USB2CAN_EPASS )
        {          
          setBlinkType( LED_GREEN_BLINK_FAST );           
          SendErrorMessage( CANAL_STATUSMSG_BUSHEAVY );
        }
        else if( ErrStatusNext & USB2CAN_EWARN )        
        {
          Statistics.Warnings++;
          setBlinkType( LED_GREEN_BLINK_SLOW );
          SendErrorMessage( CANAL_STATUSMSG_BUSLIGHT );
        }
        else        
        {
          setBlinkType( LED_GREEN_STEADY );
          SendErrorMessage( CANAL_STATUSMSG_OK ); 
        }
      
      ErrStatus = ErrStatusNext;
     }       
     
     if( ErrLec ^ ErrLecNext )
     {       
       switch ( ErrLecNext )
       {         
       case   LEC_NO_ERROR:
         SendErrorMessage( CANAL_STATUSMSG_OK ); 
         break;
       case   LEC_STUFF_ERROR:
         SendErrorMessage ( CANAL_STATUSMSG_STUFF );
         break;
       case   LEC_FORM_ERROR:
         SendErrorMessage( CANAL_STATUSMSG_FORM );
         break;
       case   LEC_ACK_ERROR:
         SendErrorMessage( CANAL_STATUSMSG_ACK );
         break; 
       case   LEC_BIT1_ERROR:
         SendErrorMessage( CANAL_STATUSMSG_BIT1 );
         break;
       case   LEC_BIT0_ERROR:
         SendErrorMessage( CANAL_STATUSMSG_BIT0 );
         break;
       case   LEC_CRC_ERROR:
         SendErrorMessage( CANAL_STATUSMSG_CRC );
         break; 
       default:
         break;         
       }
             
      ErrLec = ErrLecNext; 
     }
     
///////////////////////////////////////////////////////////////////////////
//  from USB to CAN 
//   
          
      if( TxCanFlag == FALSE )
         {
           if( CanListGet(&CanTXlist,&TxCanMsg )) 
           {      
             TxCanFlag = TRUE;                     
           }  
         }
         else
         {     
           if( TxCanMsg.IdType & CAN_EXT_ID )
            {                      
             if(!CAN_IsTransmitRequested (31)) // ziurim ar laisva
             {                   
              if( CAN_SendMessage( 31,&TxCanMsg) == SUCCESS )
                TxCanFlag = FALSE;        
             }
           }
           else
           {
             if(!CAN_IsTransmitRequested (30)) // ziurim ar laisva 
             {                  
              if( CAN_SendMessage( 30,&TxCanMsg) == SUCCESS )
              TxCanFlag = FALSE;
             }
           }   
         } // TxCanFlag...
             
////////////////////////////////////////////////////////////////////////
//   from CAN to USB data frames
//
   
     if( EP1_busy == FALSE ) // ziurim ar tuscias USB buferis
     {
       RxMsgCnt = 0;       
       if( CanListGet(&CanRXlist,&RxCanMsgList[0] ))       
       {                              
         RxMsgCnt++; 
         
         if( CanListGet(&CanRXlist,&RxCanMsgList[1] ))
         {
            RxMsgCnt++;                    
            
            if( CanListGet(&CanRXlist,&RxCanMsgList[2] ))                                      
              RxMsgCnt++;              
         }
                  
//         CANtoUSBdata( &RxCanMsg0,&RxCanMsg1,&RxCanMsg2,RxMsgCnt );         
         
         CANtoUSBdata( &RxCanMsgList[0],RxMsgCnt );
         EP1_busy = TRUE;
       }      
     }   

//////////////////////////////////////////////////////////////////////
//   from USB to CAN buffer
//     
     
     if( EP2_dataReady == TRUE )
     {
        if( USBdataCANmsg(&TmpCanMsg) )
        {    
           if( CanListPut(&CanTXlist,&TmpCanMsg) )      
           {
             EP2_dataReady = FALSE; 
             SetEPRxValid(ENDP2); 
           }          
//           else              
//             Statistics.Overruns++;                 
       }
     }        
          
  }// while... 

}// main...

