#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "75x_lib.h"
#include "usb_lib.h"

#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

typedef struct
{
  u8  Serial[8];
  u8  hver_maj;
  u8  hver_min;
}CONF;

extern CONF conf;

#endif
