# **Sinapse Recording Software**

Sinapse Recording Software is a Graphical User Interfaced designed with Qt Creator 4.1.0 based on Qt 5.7.0. 
Its purpose is to acquire and display real time signals from Neutrino II and Sylph hardware via USB and/or WiFi.

This program can be compiled for both Neutrino II and Sylph individually as of Version 1.0 
by defining NEUTRINO_II or SYLPH in the .pro file respectively, as shown below (compiling for Neutrino II)
```
DEFINES += NEUTRINO_II
# DEFINES += SYLPH
```
### NOTE: If compiled for one (e.g. NEUTRINO_I) program, the project must be cleaned before it can be compiled for the other (in this case, SYLPH) by clicking Build -> Clean all in the menu.

The following features are common for both Neutrino II and Sylph Recording Software:
- Both GUIs are capable of recording data into a .csv file with the file name of 'data_'yyyyMMdd_HHmmss'.csv' format (e.g. data_20170106_102512.csv).
- Both GUIs are capable to select save file location prior to recording; else, it will be saved to user's desktop automatically.
- Both GUIs are capable of selecting time scales of 10ms, 20ms, 50ms, 100ms, 200ms, 500ms, 1000ms, 2000ms and 5000ms for graphical display purpose.

Neutrino II Recording Software includes the following features:
- 10 channels graphical display with the option of 10x1 layout or 5x2 layout.
- Connecting via TCP sockets by defining IP address and port.
- Connecting via USB Serial Interface for wired application.
- Command dialog specifically for Neutrino II.

Sylph Recording Software includes the following features:
- 2 channels for Implant signals, 1 channel for Synchronization signal and 1 channel for Frame Markers graphical displays.
- Labelled graphs for easy reference.
- Selectable voltage scale of 50uV, 100uV, 200uV, 500uV, 1000uV, 2000uV and 5000uV.
- Selectable audio output of signals for both Implant signal channels and Synchronization signal channel.
- High pass filter with a validator of 100 Hz to 7500 Hz.
- Notch filter with the option of 50 Hz and 60 Hz.

#**Compiling on Windows**
- Desktop Qt 5.7.0 MinGW 32bit kit should be used to compile Sinapse Recording Software.
- Build directory should be of the same level with src.
- For more information, refer to this [link](http://doc.qt.io/qt-5/windows-support.html) and this [link](http://doc.qt.io/qt-5/windows-deployment.html).
- For ease of reference, please refer to the following picture:
![](https://cloud.githubusercontent.com/assets/19749458/21709617/578840e2-d41d-11e6-994d-b1a0833a9b35.png)
- Build Settings should be as follows:
![](https://cloud.githubusercontent.com/assets/19749458/21709673/eba4f0b8-d41d-11e6-89c4-8335f6bbe237.png)
