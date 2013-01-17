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

#ifndef __BUFFERS_H
#define __BUFFERS_H

#include "75x_lib.h"
#include "usb_lib.h"

#define BUFF_SIZE   256
#define BUFF_MASK   BUFF_SIZE - 1

typedef struct{  
  u8    Buff[BUFF_SIZE];
  vu16   Head;
  vu16   Tail;
  vu16   Count;
}USB_buffers_List;


void USB_buffers_Init(USB_buffers_List *USB_buff_list);
void USB_buffers_Clear(USB_buffers_List *USB_buff_list);
bool USB_buffers_Put(USB_buffers_List *USB_buff_list,u8 ch);
bool USB_buffers_Get(USB_buffers_List *USB_buff_list,u8 *ch);
u16 USB_buffers_Count(USB_buffers_List *USB_buff_list);

extern USB_buffers_List  USB_TX_buf;

#endif

