Firmware for 8 devices USB2CAN converter
========================================

Note
----
THIS SOFTWARE COMES WITHOUT ANY WARRANTY!


License
-------
License can be found at the top of every source file.


Build the Firmware
------------------
You need "IAR Embedded Workbench for ARM" for building the sources.


Flash the device (Windows)
--------------------------
* Install "DfuSe Demonstrator"
* Press the button with a paper clip while connecting to USB port
* Flash new Firmware with "DfuSe Demonstrator"


Flash the device (Linux, Experimental)
--------------------------------------
* Download latest source code for dfu-util (http://dfu-util.gnumonks.org/)
* Download patch from http://lists.gnumonks.org/pipermail/dfu-util/attachments/20140124/1abbc049/attachment.bin
* Download patch from http://marc.info/?l=linux-can&m=139064789514969&q=p3
* Apply the patches
* Build dfu-util

```Bash
./autogen.sh
./configure
make
make install
```
* Press the button with a paper clip while connecting to USB port
* Flash new firmware as root

```Bash
    dfu-util -d 0483:df11 -a 0 -D usb2can_1_5.dfu
```
