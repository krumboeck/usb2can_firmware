
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

/******************** Messages lists *******************************/

void CanListInit(CanMsgList *canmsglist)
{
  EIC_IRQCmd(DISABLE);
  
  canmsglist->Head = 0;
  canmsglist->Tail = 0;
  canmsglist->Count = 0; 
  
  EIC_IRQCmd(ENABLE);
}

void CanListClear(CanMsgList *canmsglist)
{
  EIC_IRQCmd(DISABLE);
  
  canmsglist->Head = 0;
  canmsglist->Tail = 0;
  canmsglist->Count = 0; 
  
  EIC_IRQCmd(ENABLE);
}

bool CanListPut(CanMsgList *canmsglist,canmsg *CanMsg)
{
 u16  temp;
 bool res = FALSE;
   
      EIC_IRQCmd(DISABLE);
 
      temp = ( canmsglist->Head + 1 ) % CAN_LIST_SIZE;
      
      if( temp != canmsglist->Tail )
      {
        canmsglist->CanMsgBuf[temp] = *CanMsg;
        canmsglist->Head = temp; 
        canmsglist->Count++;
        res = TRUE;
      }
      else        
        res = FALSE;      
      
       EIC_IRQCmd(ENABLE);
       
   return  res;              
}

bool CanListGet(CanMsgList *canmsglist,canmsg *CanMsg)
{
  u16  temp; 
  bool res = FALSE;
  
     EIC_IRQCmd(DISABLE);
  
     if( canmsglist->Head != canmsglist->Tail )
     {        
       temp = ( canmsglist->Tail + 1 ) % CAN_LIST_SIZE;
       *CanMsg = canmsglist->CanMsgBuf[temp]; 
       canmsglist->Tail = temp;  
       canmsglist->Count--;
       res = TRUE;
     }
     else
       res = FALSE;     
     
     EIC_IRQCmd(ENABLE);
       
   return  res;             
}

u16 CanListCount(CanMsgList *canmsglist)
{
  return  canmsglist->Count; 
}

///////////////////////////////////////////////////////////////////////////////
//   Kintamieji
//

// CAN data lists
CanMsgList  CanRXlist;
CanMsgList  CanTXlist;

//CanMsgList  CanErrList;

//canmsg  ErrorCanMsg;
//canmsg  RespCanMsg;
