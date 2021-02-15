PVC
======================

This is widget that extends default Plasma OSD ( OnScreenDisplay ) sound volume widget and allows you to control your media players (pause/play) from them
Note that you can manage players that was created before OSD shown

All doxygen documentation are stored in **Docs** folder

## How to install:
TODO: Create guide for debian/ubuntu

## Usage
```shell
./PVC help
./PVC install {Name of plasma theme}
./PVC remove {Name of plasma theme}
```

## Modules:
  * **Application** ( Application logic )
  * **Include** ( Public headers directory )
  * **Core** ( DBus, players control system etc)
  * **Widgets** ( Standalone controller widget )
  * **CLI** ( Installer/Uninstaller ( cpp that depends only on stdlib ), make script )
  * **qml** ( Injectable qml widget, qml container for QPixmap, qml utils )
  ***

## Dependencies:
  * KDE Plasma
  * make
  * Qt5 

## Cmake targets
  * Library (everything under Src/ folder)
  * Tests (everything under Tests/ folder, contains standalone test app 
           and example qml application)

## Tested on:
  * Debian 10

### Links:
https://specifications.freedesktop.org/mpris-spec/latest/