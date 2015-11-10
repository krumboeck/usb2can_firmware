
/*
 *  USB2CAN firmware 
 *
 * Copyright (C) 2007-2015 Gediminas Simanskis (gedsim@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.
 */

#include "lists.h"
#include "messages.h"
#include "string.h" 
#include "main.h"
#include "hw_config.h"
#include "commands.h"


////////////////////////////////////////////////////////////////////////////////
//   DATA messages
//

bool  USBdataCANmsg(canmsg *CanMsg)
{
  u8  buflen;
  u8  buf[64];  
  
//  if( bRun == FALSE )    
//    return FALSE;    
  
  buflen = GetEPRxCount(ENDP2);
  
  if( buflen >= sizeof(buf))
    return FALSE;
  
  PMAToUserBufferCopy( buf, ENDP2_BUFF_ADDR, buflen );     
  
  if( (buf[0] != 0x55) || (buf[15] != 0xAA) || (buflen > 16) )     
    return FALSE;   
  
  // flags
  if( buf[1] & CAN_EXT_ID ) // EXT
  {
   CanMsg->IdType = buf[1] & 0x03; // RTR & EXT_ID      
   CanMsg->Id = 
   (((u32)buf[2]<<24) & 0x1f000000) |
   (((u32)buf[3]<<16) & 0x00ff0000) |
   (((u32)buf[4]<<8 ) & 0x0000ff00) |
   (((u32)buf[5]    ) & 0x000000ff) ;
  }
  else // STD
  {
   CanMsg->IdType = buf[1] & 0x03; // RTR & STD_ID
   CanMsg->Id = 
   (((u32)buf[4]<<8 ) & 0x00007f00) |
   (((u32)buf[5]    ) & 0x000000ff) ;  
  }
  
    CanMsg->Dlc = buf[6]; 
    memcpy(&CanMsg->Data,&buf[7], 8);        
       
    return  TRUE;
}    

////////////////////////////////////////////////////

u8  GetUSBdataCount(void)
{
  return count_data_in;
}

///////////////////////////////////////////////////

void ClearUSBdataBuffer(void)
{
  count_data_in = 0; 
}

///////////////////////////////////////////////////

void  CANtoUSBdata( canmsg *rxmsg0, u8 num )
{
  u8  sendData[64];
  u8  size = 0;
  u8  x;
  canmsg  *msg0; 
    
    for( x=0; x< num;x++ )
    {
        msg0 = &rxmsg0[x];
        
	sendData[ size++ ] = 0x55; // FrameBegin       
        sendData[ size++ ] = (u8)( msg0->IdType >> 8 );  // Data frame type (aprasyta messages.h)  
        sendData[ size++ ] = (u8) msg0->IdType ;  // RTR and EXT_ID flags only       
	sendData[ size++ ] = (u8)( msg0->Id >> 24 ) & 0x1f;
	sendData[ size++ ] = (u8)( msg0->Id >> 16 ) & 0xff;
	sendData[ size++ ] = (u8)( msg0->Id >> 8 )  & 0xff;
	sendData[ size++ ] = (u8)( msg0->Id ) & 0xff; // Message ID
        sendData[ size++ ] = msg0->Dlc; // NoOfData   
                
	memcpy( &sendData[size], msg0->Data, 8 ); // DATA
	size += 8;    
             		
	sendData[ size++ ] = (u8)( msg0->Timestamp >> 24 ) & 0x1f;
	sendData[ size++ ] = (u8)( msg0->Timestamp >> 16 ) & 0xff;
	sendData[ size++ ] = (u8)( msg0->Timestamp >> 8 )  & 0xff;
	sendData[ size++ ] = (u8)( msg0->Timestamp ) & 0xff;  // Message TIMESTAMP        
        sendData[ size++ ] = 0xAA; // FrameEnd                             
    }   
              
        UserToPMABufferCopy(sendData,ENDP1_BUFF_ADDR,size);
        SetEPTxCount(ENDP1,size);
        SetEPTxValid(ENDP1);                           
}

///////////////////////////////////////////////////////////////////////////////
//    CMD messages
//

////////////////////////////////////////////////////

u8  GetUSBcmdCount(void)
{
  return count_cmd_in;
}

///////////////////////////////////////////////////

void ClearUSBcmdBuffer(void)
{
  count_cmd_in = 0; 
}

///////////////////////////////////////////////////

void GetUSBcmd(void)
{
  u8  channel;
  u8  command;
  u8  opt1;
  u8  opt2;
  u32 flags;
  u8  size;
  CAN_TIMINGS   cantimings;  
  u16 tmp;
  u8  buff[64];
  u32 res;
  
  count_cmd_out = GetEPRxCount(ENDP4);
  PMAToUserBufferCopy( buffer_cmd_out,ENDP4_BUFF_ADDR, count_cmd_out);
    
  size = count_cmd_out;
  memcpy(buff,buffer_cmd_out,16);
  
  if( (buff[0] != 0x11) || (buff[15] != 0x22) || (size > 16) )
  {
   SetEPRxValid(ENDP4);
   return;
  }  
    channel = buff[1];
    command = buff[2];
    opt1    = buff[3];   
    opt2    = buff[4];
    
  switch( command )
  {
  case    USB2CAN_RESET:            
     opt1 = USB2CAN_CMD_SUCCESS;          
    break;
  case    USB2CAN_OPEN:
    
    cantimings.tseg1 = buff[5];
    cantimings.tseg2 = buff[6];
    cantimings.sjw =   buff[7];     
    cantimings.brp =   buff[8];        
    cantimings.brp = (cantimings.brp << 8) | buff[9];    
    
    flags = buff[10];
    flags = (flags << 8)| buff[11];
    flags = (flags << 8)| buff[12];
    flags = (flags << 8)| buff[13];
    
    USB2CAN_open(opt1,flags,&cantimings);// opt1 - speed
    bRunNext = TRUE; // indikacija           
    opt1 = USB2CAN_CMD_SUCCESS;         
    
    break;    
  case    USB2CAN_CLOSE:

     USB2CAN_close();     
     bRunNext = FALSE;
     opt1 = USB2CAN_CMD_SUCCESS;     
    
    break;    
  case    USB2CAN_SET_SPEED:
    
    if( bRun )
    {  
     USB2CAN_set_speed(opt1,&cantimings);
     opt1 = USB2CAN_CMD_SUCCESS;
    }
    else
      opt1 = USB2CAN_CMD_ERROR; 
    
    break;
  case    USB2CAN_SET_MASK_FILETR:
    break;
  case    USB2CAN_GET_STATUS:
    
    if( USB2CAN_get_status(&res))
    {
      buff[5] = (u8) ( res >> 24 );
      buff[6] = (u8) ( res >> 16 );
      buff[7] = (u8) ( res >> 8 );
      buff[8] = (u8)  res ; 
      opt1 = USB2CAN_CMD_SUCCESS;
    }
    else
      opt1 = USB2CAN_CMD_ERROR;
    
    break;
  case    USB2CAN_GET_STATISTICS:
        
    if( USB2CAN_get_statistics(opt1,&res))
    {
     buff[5] = (u8) ( res >> 24 );
     buff[6] = (u8) ( res >> 16 );
     buff[7] = (u8) ( res >> 8 );
     buff[8] = (u8)  res ;
     opt1 = USB2CAN_CMD_SUCCESS;
    }
    else
      opt1 = USB2CAN_CMD_ERROR;
    
    break;
  case    USB2CAN_GET_SERIAL:
    
    memcpy( &buff[5],conf.Serial,8 );
    opt1 = USB2CAN_CMD_SUCCESS;
    
    break;
  case    USB2CAN_GET_SOFTW_VER:
    
    buff[5] = FIRMWARE_MAJ_VER;
    buff[6] = FIRMWARE_MIN_VER;
      
    opt1 = USB2CAN_CMD_SUCCESS;
    
    break;
  case    USB2CAN_GET_HARDW_VER:
    
    buff[5] = conf.hver_maj;
    buff[6] = conf.hver_min;
    opt1 = USB2CAN_CMD_SUCCESS;
    
    break;
  case   USB2CAN_GET_SOFTW_HARDW_VER:  
    
    buff[5] = FIRMWARE_MAJ_VER;
    buff[6] = FIRMWARE_MIN_VER;
    buff[7] = conf.hver_maj;
    buff[8] = conf.hver_min;  
    opt1 = USB2CAN_CMD_SUCCESS;
    
    break;    
  case    USB2CAN_RESET_TIMESTAMP:
    break;
  default:
     opt1 = USB2CAN_CMD_ERROR; 
    break;    
  }
  
  buff[0] = 0x11;
  buff[1] = channel;
  buff[2] = command;
  buff[3] = opt1;
  buff[4] = opt2;
  buff[15] = 0x22;
 
  memcpy(buffer_cmd_in,buff,16);
  
//  while( count_cmd_in != 0 );  
  count_cmd_in = size;
  
  /****** issiunciam *****/
  UserToPMABufferCopy(buffer_cmd_in,ENDP3_BUFF_ADDR, count_cmd_in);
  SetEPTxCount(ENDP3,count_cmd_in);
  SetEPTxValid(ENDP3);  

  SetEPRxValid(ENDP4); //leidziam vel priimti  
  
  if( command == USB2CAN_RESET )
  {
   tmp = 65000; 
   while(--tmp);
   USB2CAN_reset();   
  }
}

bool SendErrorMessage( u8 status )
{
  canmsg msg; 
  u16  status_cnt = CAN->ERR;
  
  msg.IdType = 0x0304; // error
  msg.Id = 0; // must be '0'
  msg.Dlc = 4;
  msg.Data[0] = status;
  msg.Data[1] = (u8)  ((status_cnt >> 8) & 0x7F);   // REC
  msg.Data[2] = (u8) status_cnt; // TEC
  msg.Data[3] = 0;

   if( CanListPut(&CanRXlist,&msg))
     return TRUE;
   else  
     return FALSE;
}
