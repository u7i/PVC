PVC
======================

This is widget that extends default Plasma OSD ( OnScreenDisplay ) sound volume widget and allows you to control your Player players (pause/play) from them
Note that you can manage players that was created before OSD shown

All doxygen documentation are stored in **Docs** folder

##How to install:
TODO: Create guide for debian/ubuntu

##Usage
```shell
./PVC.sh help
./PVC.sh install {Name of plasma theme}
./PVC.sh remove {Name of plasma theme}
```

##Modules:
  * Include - Public headers directory
  * Core ( DBus, players control system etc)
  * Widgets ( Standalone controller widget )
  * sh scripts ( Installer/Uninstaller, Make )
  * qml ( Injectable qml widget )

##Dependencies:
  * KDE Plasma
  * make
  * Qt5 

##Cmake targets
  * Library (everything under Src/ dir)
  * Tests (everything under Tests/ folder, contains standalone test and example qml application)

##Tested on:
  * Debian 10

###Links:
https://specifications.freedesktop.org/mpris-spec/latest/