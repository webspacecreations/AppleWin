# Linux

* [Structure](#structure)
* [What works](#what-works)
* [Executables](#executables)
  * [applen](#applen)
  * [qapple](#qapple)
  * [sa2](#sa2)
  * [libretro](#ra2)
* [Build](#build)
  * [Checkout](#checkout)
  * [Fedora](#fedora)
  * [Raspbian](#raspbian)
* [Speed](#build)
  * [Fedora](#fedora-1)
  * [Raspbian](#raspbian-1)

## Structure

There are 4 projects

* libapple: the core emulator files
* applen: a frontend based on ncurses
* qapple: Qt frontend
* sa2: SDL2 frontend
* libra2: a libretro core

The libapple interface is a *link time* interface: some functions are not defined and must be provided in order to properly link
the application. These functions are listed in [interface.h](source/linux/interface.h).

The main goal is to reuse the AppleWin source files without changes: only where really necessary the AppleWin source files have
been modified, mostly for

* header files issues
* const char *
* exclude some Windows heavy blocks (source/MouseInterface.cpp)

##  What works

Some key files have been completely reimplemented or discarded:

* AppleWin.cpp
* WinFrame.cpp
* WinVideo.cpp

Some features totally ignored:

* ethernet
* serial port
* debugger
* speech (currently it hangs the emulator)

The rest is in a very usable state.

## Executables

### applen

Frontend based on ncurses, with a ASCII art graphic mode.

Keyboard shortcuts

* ``F2``: reset the machine
* ``F3``: terminate the emulator
* ``F11``, ``F12``: Save, Load Snapshot
* ``ALT-RIGHT``: wider hi res graphis
* ``ALT-LEFT``: narrower hi res graphics
* ``ALT-UP``: vertical hi res (smaller)
* ``ALT-DOWN``: vertical hires (bigger)

In order to properly appreciate the wider hi res graphics, open a big terminal window and choose a small font size.
Try ``CTRL-`` as well if ``ALT-`` does not work: terminals do not report a consistent keycode for these combinations.

The joystick uses evdev (``--device-name /dev/input/by-id/id_of_device``).

### qapple

This is based on Qt.

* keyboard shortcuts are listed in the menu entries
* graphics: runs the native NTSC code
* joystick: it uses QtGamepad
* emulator runs in the main UI thread
* Qt timers are very coarse: the emulator needs to dynamically adapt the cycles to execute
* the app runs at 60FPS with correction for uneven timer deltas.
* full speed when disk spins execute up to 5 ms real wall clock of emulator code (then returns to Qt)
* (standard) audio is supported and there are a few configuration options to tune the latency (default very conservative 200ms)
* plain mockingboard is supported as well (not speech, which hangs the emulator)
* Open Apple and Solid Apple can be emulated using AltGr and Menu (unfortunately, Alt does not work well)

### sa2

See [sa2](source/frontends/sa2/README.md).

### ra2

There is an initial [libretro](https://docs.libretro.com/development/cores/developing-cores/) core.

A retropad can be plugged in port 1 (with or without analog stick).

Keyboard emulation

* ``JOYPAD_R``: equivalent to ``F9`` to cycle video types
* ``JOYPAD_L``: equivalent to ``CTRL-SHIFT-F6`` to cycle 50% scan lines
* ``START``: equivalent to ``F2`` to reset the machine

In order to have a better experience with the keyboard, one should probably enable *Game Focus Mode* (normally Scroll-Lock) to disable hotkeys.

Video works, but the vertical flip is done in software.

Audio (speaker) works.

Must be manually configured:
``cmake -DLIBRETRO_PATH=/path/to/libretro-common``

Easiest way to run from the ``build`` folder:
``retroarch -L source/frontends/libretro/libra2.so ../Disks/NoSlotClockTest.dsk``

## Build

The project can be built using cmake from the top level directory.

qapple can be managed from Qt Creator as well and the 2 have coexisted so far, but YMMV.

### Checkout

```
git clone https://github.com/audetto/AppleWin.git --recursive
cd AppleWin
mkdir build
cd build
cmake ..
make
```
Use `cmake -DCMAKE_BUILD_TYPE=RELEASE` to get a *release* build.

### Fedora

On Fedora 31, from a fresh installation, install all packages from [fedora.list.txt](source/linux/fedora.list.txt).

### Raspbian

On Raspbian 10, from a fresh installation, install all packages from [raspbian.list.txt](source/linux/raspbian.list.txt).

See [Travis](.travis.yml) CI too.

## Speed

### Fedora

Intel(R) Core(TM) i5-4460  CPU @ 3.20GHz

Full update = 582 MHz

| Video Stype | Video update |
| :--- | ---: |
| RGB Monitor | 39 |
| NTSC Monitor | 27 |
| Color TV | 25 |
| B&W TV | 27 |
| Amber Monitor | 31 |

### Raspbian

Pi 3B+

Full update = 54 MHz

| Video Stype | Video update |
| :--- | ---: |
| RGB Monitor | 5.3 |
| NTSC Monitor | 3.6 |
| Color TV | 2.6 |
| B&W TV | 2.9 |
| Amber Monitor | 4.5 |
