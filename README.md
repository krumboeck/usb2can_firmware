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
* Patch the sources
* Remove following rows from main.c
                if (((file.idVendor  != 0xffff && file.idVendor  != runtime_vendor) ||
                     (file.idProduct != 0xffff && file.idProduct != runtime_product)) &&
                    ((file.idVendor  != 0xffff && file.idVendor  != dfu_root->vendor) ||
                     (file.idProduct != 0xffff && file.idProduct != dfu_root->product))) {
                        errx(EX_IOERR, "Error: File ID %04x:%04x does "
                                "not match device (%04x:%04x or %04x:%04x)",
                                file.idVendor, file.idProduct,
                                runtime_vendor, runtime_product,
                                dfu_root->vendor, dfu_root->product);
                }
* Build dfu-util
    ./autogen.sh
    ./configure.sh
    make
    make install
* Press the button with a paper clip while connecting to USB port
* Flash new firmware as root
    dfu-util -d 0483:df11 -a 0 -D usb2can_1_5.dfu

