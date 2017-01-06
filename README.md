# SinapseRecordingSoftware

Sinapse Recording Software is a Graphical User Interfaced designed with Qt Creator 4.1.0 based on Qt 5.7.0. 
Its purpose is to acquire and display real time signals from Neutrino and Sylph hardware via USB and/or WiFi.

This program can be compiled for both Neutrino and Sylph individually as of Version 1.0 
by defining NEUTRINO_II or SYLPH in the .pro file respectively.

The following features are common for both Neutrino and Sylph Recording Software:
- Both GUIs are capable of recording data into a .csv file with the file name of 'data_'yyyyMMdd_HHmmss'.csv' format.
- Both GUIs are capable to select save file location prior to recording; else, it will be saved to user's desktop automatically.
- Both GUIs are capable of selecting time scales of 10ms, 20ms, 50ms, 100ms, 200ms, 500ms, 1000ms, 2000ms and 5000ms for graphical display purpose.

Neutrino Recording Software includes the following features:
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

