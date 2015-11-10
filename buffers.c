
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

#include  "buffers.h"

USB_buffers_List  USB_TX_buf;

void USB_buffers_Init(USB_buffers_List *USB_buff_list)
{
   EIC_IRQCmd(DISABLE);
   USB_buff_list->Head = 0;
   USB_buff_list->Tail = 0;
   USB_buff_list->Count = 0;
   EIC_IRQCmd(ENABLE);
}

void USB_buffers_Clear(USB_buffers_List *USB_buff_list)
{
  USB_buffers_Init( USB_buff_list );
}

bool USB_buffers_Put(USB_buffers_List *USB_buff_list,u8 ch)
{
 u16  temp;
 bool res = FALSE;
 
      EIC_IRQCmd(DISABLE);
 
      temp = ( USB_buff_list->Head + 1 ) % BUFF_SIZE;
      
      if( temp != USB_buff_list->Tail )
      {
        USB_buff_list->Buff[temp] = ch;
        USB_buff_list->Head = temp; 
        USB_buff_list->Count++;
        res = TRUE;        
      }
      else                
        res = FALSE;
      
      EIC_IRQCmd(ENABLE);
      
return res;            
}

bool USB_buffers_Get(USB_buffers_List *USB_buff_list,u8 *ch)
{
  u16  temp;  
  bool res = FALSE;
  u16  head,tail;
  
     EIC_IRQCmd(DISABLE);
     
     head = USB_buff_list->Head;
     tail = USB_buff_list->Tail;     
     
     if( head != tail )
     {        
       temp = ( USB_buff_list->Tail + 1 ) % BUFF_SIZE;
       *ch = USB_buff_list->Buff[temp]; 
       USB_buff_list->Tail = temp; 
       USB_buff_list->Count--;
       res = TRUE;
     } 
     else
       res = FALSE;
     
      EIC_IRQCmd(ENABLE);
         
return  res;     
}

u16 USB_buffers_Count(USB_buffers_List *USB_buff_list)
{  
//  u16  ret;
//  u16  head,tail; 
  
//    EIC_IRQCmd(DISABLE);
        
/*  
     head =  (USB_buff_list->Head) & BUFF_MASK ;
     tail =  (USB_buff_list->Tail) & BUFF_MASK ;  
    
   if( head >= tail )     
     ret = head - tail;   
   else
     ret = 1024 - (tail - head);    
*/   
//    EIC_IRQCmd(ENABLE);    
   
//   return ret;   
  
  return USB_buff_list->Count;
}

