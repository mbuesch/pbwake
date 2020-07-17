Power bank keep alive device
============================

`https://bues.ch/cgit/pbwake.git/ <https://bues.ch/cgit/pbwake.git/>`_

Pbwake is a small USB stick to prevent power banks from shutting off the outputs and falling into deep sleep mode.

Pbwake uses tiny load pulses to simulate a real load on the power bank outputs. The lengths of the load pulse active (high) and inactive (low) phases are pseudo random within configurable bounds.

Pbwake is in a low power state during the inactive (low) phases. Therefore it doesn't waste too much power and guarantees high battery life of the power bank.


Tested power banks
==================

Power banks this has been tested with are:

* Anker PowerCore 20100 model A1271.
* No-name power bank 2200 mAh / 11 Wh with integrated flashlight LED.

Microcontroller
===============

Pbwake supports the following microcontrollers:

* ATTiny-13
* ATTiny-25
* ATTiny-45
* ATTiny-85
* ATMega-328P


Hardware schematics
===================

Schematics: `schematics/pbwake/pbwake.pdf <schematics/pbwake/pbwake.pdf>`_


Prebuilt firmware images
========================

The release archives of pbwake contain prebuilt `.hex` files for all supported microcontrollers in the `hex` directory. These hex files can be flashed directly to the microcontroller with a any tool of your choice (e.g. Atmel Studio or avrdude).

The corresponding fuse settings can be found in the file `fuses.txt` in the same directory.


Building the firmware
=====================

The firmware build needs the following tool chain:

* Unix-like operating system
* GNU make
* AVR GCC
* AVR Binutils
* avrdude

Run the following commands to build the firmware:

.. code:: sh

	cd firmware
	make DEV=t85

Please specify the target microcontroller using the DEV variable as shown above.

Valid values are:

===============  ===========
Microcontroller  make option
===============  ===========
ATTiny 13        `DEV=t13`
ATTiny 25        `DEV=t25`
ATTiny 45        `DEV=t45`
ATTiny 85        `DEV=t85`
ATMega 328P      `DEV=m328p`
===============  ===========

Additional build options that can be passed to `make`:

================  =======  ================================================================
make option       default  description
================  =======  ================================================================
CONF_HIGH_MIN_MS  4        Minimum length of high-phase (in milliseconds).
CONF_HIGH_MAX_MS  6        Maximum length of high-phase (in milliseconds).
CONF_LOW_MIN_MS   40000    Minimum length of low-phase (in milliseconds).
CONF_LOW_MAX_MS   55000    Maximum length of low-phase (in milliseconds).
================  =======  ================================================================


Flashing the firmware after build
=================================

Run the following commands to flash the firmware to the target microcontroller:

.. code:: sh

	cd firmware
	make DEV=t85 print_fuses
	make DEV=t85 write_fuses
	make DEV=t85 write_mem

Or alternatively use any other of the available AVR flashing tools to program the .hex file and fuses.


License
=======

Copyright (c) 2020 Michael Buesch <m@bues.ch>

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
