# **Sinapse Recording Software**

<!-- Sinapse Recording Software is a Graphical User Interfaced designed with Qt Creator 4.1.0 based on Qt 5.7.0. 
Its purpose is to acquire and display real time signals from Neutrino II and Sylph hardware via USB and/or WiFi.

~~This program can be compiled for both Neutrino II and Sylph individually as of Version 1.0 
by defining NEUTRINO_II or SYLPH in the .pro file respectively, as shown below (compiling for Neutrino II)~~
```
~~DEFINES += NEUTRINO_II~~
~~# DEFINES += SYLPH~~
```
~~### NOTE: If compiled for one (e.g. NEUTRINO_I) program, the project must be cleaned before it can be compiled for the other (in this case, SYLPH) by clicking Build -> Clean all in the menu.~~

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
- Notch filter with the option of 50 Hz and 60 Hz. -->

Sinapse Recording Software is a Graphical User Interface designed with Qt Creator 4.1.0based on Qt 5.7.0. 
This software comprises of the following programs:

common:
- A static library created for almost all of the programs listed below.

AMWFlash:
- A program that takes in a setting file in .txt format and flash the AMW004 via FTDI.

Jane:
- Thor's control program (yet to be developed)

Neutrino:
- Neutrino's control program and real-time signal display of up to 10 channels.

Odin:
- Odin's control program designed for Stimulator V4.1.

SylphII & SylphX:
- Sylph's program that display real-time ENG(SylphII) and EMG(SylphX) signals.

Details of the programs are listed below:

Classes included in common:
- Connection Dialog -> Takes in User input of IP address and Port to attempt connection via TCP Socket.
- Filter Dialog -> Takes in User input of Notch Filter freq (None, 50Hz or 60Hz) and High Pass Filter (100-7500Hz) and makes use of Filter Class to execute Digital Signal Processing.
- Filter Class -> Includes methods of Digital Notch Filter and High Pass Filter.
- QCustomPlot -> Custom Plotting graph that is used in mainly Neutrino and Sylph Programs.
- SignalAudio -> Main class that handles all the data to audio conversion.
- SocketAbstract -> Contains all of the abstract socket operations that is general across the different programs.

Classes included in AMWFlash:
- Console -> Terminal-like User Interface extracted from Qt examples that print out all of the necessary outputs for Users.
- SerialPort Dialog -> A Dialog that allows User to choose which serial port prior flashing the AMW004.

Classes included in Neutrino:
- Channel -> A class that holds the channel properties. (Obsolete)
- Command Dialog -> Takes in all the User input for Neutrino setting and sends out the command via Socket or Serial.
- Serial Channel -> The main class that handles all the serial operations of Neutrino.
- Signal Channel -> Unused, obsolete.
- Command -> The main class that handles all of the command to be sent to Neutrino.
- Data Processor -> Inherited from Data in common, includes additional methods unique to Neutrino.
- Measurement Dialog -> Displays measurements such as Min, Max, Average and Peak to Peak for Analog measurement and Bioimpedance measurement.
- Socket Neutrino -> Inherited from Socket Abstract in common, includes additional methods unique to Neutrino.

Classes included in SylphII/SylphX:
- Data Processor -> Inherited from Data in common, includes additional methods unique to Sylph.
- Serial Channel -> The main class that handles all the serial operations of Sylph.
- Socket Sylph -> Inherited from Socket Abstract in common, includes additional methods unique to Sylph.
*Note* Difference in SylphII and SylphX *Note*
- As SylphII only has 2 Implant Channels and SylphX has 10, the only difference between the two programs can be found in Data Processor and Main Window only.
- Sylph II creates 2 QCustomPlot for the Implant Channels while Sylph X creates 10.
- Sylph II Data Processor extracts 5 bytes at a time while Sylph X extracts 21.
*Note* Difference in SylphII and SylphX *Note*

Class included in Odin:
- Command Odin -> The main class that handles all of the command to be sent to Odin.
- Looping Thread -> Executes everytime Send button is clicked and runs on a different thread to get precise timing of command sent.
- Pulse Plot -> (Still in development) The main class that creates a graphical display of the pulse Odin is generating.
- Serial Odin -> The main class that handles all the serial operations of Odin.
- Socket Odin -> Inherited from Socket Abstract in common, includes additional methods unique to Odin.

#**Compiling on Windows**
- Desktop Qt 5.7.0 MinGW 32bit kit should be used to compile Sinapse Recording Software.
- Build directory should be of the same level with src.
- For more information, refer to this [link](http://doc.qt.io/qt-5/windows-support.html) and this [link](http://doc.qt.io/qt-5/windows-deployment.html).
- For ease of reference, please refer to the following picture:
![](https://cloud.githubusercontent.com/assets/19749458/21709617/578840e2-d41d-11e6-994d-b1a0833a9b35.png)
- Build Settings should be as follows:
![](https://cloud.githubusercontent.com/assets/19749458/21709673/eba4f0b8-d41d-11e6-89c4-8335f6bbe237.png)

#**Compiling on Linux for Windows (Static 64-bit executable)**
- Install Ubuntu 16.04 LTS
- Get MXE: 
```
git clone https://github.com/mxe/mxe.git
```
- Install build dependencies
```
http://mxe.cc/#requirements
```
![](https://user-images.githubusercontent.com/19749458/27578402-8b601bfe-5b56-11e7-89ad-b9f9a409f94c.png)
- Build Qt 5 for Windows:
```
cd mxe && make MXE_TARGETS=x86_64-w64-mingw32.static qt5
```
- Set path:
```
PATH = ~/mxe/usr/bin:$PATH
```
- Althenatively: add this line into ~/.bashrc
```
export PATH=~/mxe/usr/bin:$PATH
```
- Get to the directory of your app (where the main .pro file is), and run the Qt Makefile generator tool:
```
~/mxe/usr/bin/x86_64-w64-ming32.static-qmake-qt5 
```
- Build your project:
```
make
```
You should find the binary in the ./release directory
