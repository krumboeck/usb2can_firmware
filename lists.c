

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


/*
Copyright (C) 2008 Gediminas Simanskis , edevices, www.edevices.lt
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
