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



#include "commands.h"


void  USB2CAN_reset(void)
{
  Reset_Device();  
}

void  USB2CAN_set_speed(u8 speed,CAN_TIMINGS *CanTimings)
{
  CAN_InitTypeDef     CAN_Struct;
  
  switch(speed)
  {
  case    CAN_BAUD_10:    
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_10K;  
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );  
    break;
  case    CAN_BAUD_20:
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_20K;
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_50:
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_50K;
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_100:    
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_100K;
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_125:  
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_125K;
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_250:   
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_250K;
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_500:   
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_500K;
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_800:   
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_800K;    
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_1000:   
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_1M;    
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  case    CAN_BAUD_MANUAL:             
      CAN_EnterInitMode(CAN_CR_CCE | CAN_CR_IE);
      CAN_SetTiming(CanTimings->tseg1,CanTimings->tseg2,CanTimings->sjw,CanTimings->brp);
      CAN_LeaveInitMode();    
    break;
  default: // 125K   
      CAN_Struct.CAN_Bitrate=CAN_BITRATE_125K;
      CAN_Struct.CAN_ConfigParameters = CAN_CR_IE ;
      CAN_Init( &CAN_Struct );        
    break;
  }  
}

void  USB2CAN_open(u32 speed,u32 flags,CAN_TIMINGS *CanTimings)
{
   CAN_ENABLE();       
   USB2CAN_set_speed(speed,CanTimings);

   switch( flags & 0x0003 )
   {
   case  CAN_MODE_NORMAL: // normal mode         
      CAN_LeaveTestMode(); 
     break;
   case  CAN_MODE_SILENT: // silent mode
      CAN_EnterTestMode( CAN_TESTR_SILENT );    
     break;
   case  CAN_MODE_LOOPBACK: // loopback mode
      CAN_EnterTestMode( CAN_TESTR_LBACK );    
     break;
   case  CAN_MODE_SILENT_LOOPBACK:
      CAN_EnterTestMode( CAN_TESTR_LBACK | CAN_TESTR_SILENT );
     break;
   default:
      CAN_LeaveTestMode(); 
     break;
   }        
     // DAR
     if( flags & 0x0004 )
      CAN->CR |= CAN_CR_DAR;  // disable auto retransmitions           
     else     
      CAN->CR &= ~CAN_CR_DAR; // enable auto retransmitions  
}

////////////////////////////////

void  USB2CAN_close(void)
{
  CAN_DISABLE();
}

////////////////////////////////

bool USB2CAN_get_status(u32 *res)
{
  u16  status = CAN->SR; 
  u16  rxtxcnt = CAN->ERR;
  
  *res = 0;
  
  if( status & CAN_SR_BOFF )
    *res |= CANAL_STATUS_BUS_OFF ;  
  else
    *res &= ~CANAL_STATUS_BUS_OFF ;
  
  if( status & CAN_SR_EWARN )  
    *res |= CANAL_STATUS_BUS_WARN ;         
  else
    *res &= ~CANAL_STATUS_BUS_WARN ;
  
  if( status & CAN_SR_EPASS )
    *res |= CANAL_STATUS_PASSIVE ;  
  else
    *res &= ~CANAL_STATUS_PASSIVE ;
         
    *res |= (u32) ( rxtxcnt & 0x7FFF );
  
  return TRUE;
}

bool  USB2CAN_get_statistics( u8 num,u32 *res )
{
  bool  ret = FALSE;
  
  switch( num )
  {
  case   0:  
    *res = Statistics.ReceivedFrames;
    ret = TRUE;
    break;
  case   1:
    *res = Statistics.ReceivedDataBytes;
    ret = TRUE;
    break;
  case   2:
    *res = Statistics.TransmitedFrames;
    ret = TRUE;    
    break;
  case   3:
    *res = Statistics.TransmitedDataBytes;
    ret = TRUE;    
    break;
  case   4:
    *res = Statistics.Overruns;
    ret = TRUE;    
    break;
  case   5:
    *res = Statistics.Warnings;
    ret = TRUE;    
    break;
  case  6:
    *res = Statistics.BusOff;
    ret = TRUE;    
    break;     
  case 7:  
     *res = 0;
     StatisticsInit();
     ret = TRUE; 
    break;
  default:
    ret = FALSE;
    break;
  }
  
  return ret;
}
