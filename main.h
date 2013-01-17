
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


#ifndef __MAIN_H
#define __MAIN_H

#define FIRMWARE_MAJ_VER  1
#define FIRMWARE_MIN_VER  4
//1.4 ver. pataisyta CAN high speed USB dubliavimo problema (buvo 1.3 versijoje)

typedef struct{
  u32   ReceivedFrames;
  u32   TransmitedFrames;
  u32   ReceivedDataBytes;
  u32   TransmitedDataBytes;
  u32   Overruns;
  u32   Warnings;
  u32   BusOff;
}
STATISTICS;

typedef struct{
  u8    ConfigOk;  // must be 0xAA
  u8    SerialNumber[8];  // Serial ED000001
  u8    State; // open,close
  u8    FirmwareVersion;
  u8    FirmwareSubversion;
  u8    FirmwareSubversionExt;
  u8    HardwareVersion;
  u8    HardwareSubversion;
  u8    Speed;
  u8    SpeedReg1;
  u8    SpeedReg2;
  u32   Mask;
  u32   Filter;
}USB2CAN_CONFIG;

//EEPROM config address BLOCK_7

#define   EEPROM_SERIAL_NUMBER               0
#define   EEPROM_HARDWARE_VERSION            8
#define   EEPROM_HARDWARE_SUBVERSION         9

// EEPROM config address BLOCK_0

#define   EEPROM_FIRMWARE_VERSION            0
#define   EEPROM_FIRMWARE_SUBVERSION         1
#define   EEPROM_USB2CAN_STATE              11  // open,close
#define   EEPROM_CAN_SPEED                  12
#define   EEPROM_CAN_REG1                   13 
#define   EEPROM_CAN_REG2                   14
#define   EEPROM_MASK                       15  
#define   EEPROM_FILTER                     19
#define   EEPROM_...                        23

//Status LEC values
#define   LEC_NO_ERROR                      0
#define   LEC_STUFF_ERROR                   1
#define   LEC_FORM_ERROR                    2
#define   LEC_ACK_ERROR                     3
#define   LEC_BIT1_ERROR                    4
#define   LEC_BIT0_ERROR                    5
#define   LEC_CRC_ERROR                     6
#define   LEC_UNUSED_ERROR                  7

// ERROR status bits

#define USB2CAN_EPASS  1
#define USB2CAN_EWARN  2
#define USB2CAN_BOFF   4

extern STATISTICS Statistics;
extern void StatisticsInit(void);
// USB2CAN state . open,closed
extern volatile bool  bRun;
extern volatile bool  bRunNext;
extern volatile u16 USB2CAN_status;
extern volatile u16 USB2CAN_next_status;

///////////////////////////////////////////////////////////////////////////
//      Kinatmieji
//

extern u8 buffer_cmd_out[];
extern u8 buffer_cmd_in[];
extern volatile u8 count_cmd_out;
extern volatile u8 count_cmd_in;

extern u8 buffer_data_out[];
extern u8 buffer_data_in[];
extern volatile u8 count_data_out;
extern volatile u8 count_data_in;

// buffer for receive messages 
extern canmsg      TxCanMsg;
extern canmsg      RxCanMsg;
extern bool        TxCanFlag;
extern bool        RxCanFlag;
extern canmsg      TmpCanMsg;

#endif
