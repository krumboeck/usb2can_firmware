Firmware for 8 devices USB2CAN converter
========================================

Note
----
THIS SOFTWARE COMES WITHOUT ANY WARRANTY!


License
-------
Mostly GPLv2. Only some Include-Files from STMicroelectronics use a different license which can be found on the top of every affected source file.


Build the Firmware
------------------
* Install *GNU Tools for ARM Embedded Processors*
* Use **make** to build the sources


Flash your own firmware on USB2CAN device
-----------------------------------------
This is not possible, except you have the encryption key for the modified dfu boot loader.


Flash the device (Windows)
--------------------------
* Install "DfuSe Demonstrator"
* Press the button with a paper clip while connecting to USB port
* Flash new Firmware with "DfuSe Demonstrator"


Flash the device (Linux, Experimental)
--------------------------------------
* Install dfu-util (>= 0.8)
* Press the button with a paper clip while connecting to USB port
* Flash new firmware as root

```Bash
    dfu-util -d 0483:df11 -a 0 -D usb2can_1_5.dfu
```
* Optional a new serial number can be flashed

```Bash
    dfu-util -d 0483:df11 -a 1 -D E000xxx_1_0.dfu
```


Known Problems
--------------
### Few USB controllers or drivers may not work properly
Sometimes an USB hub helps. This is not a firmware issue, so it will never get fixed.

### Device could not get initialized after connection loss
This happens when a powered USB hub is used and the connection between the host and hub get lost. This is a firmware issue.
