cmake_minimum_required(VERSION 3.17)
project(PVC)

set(CMAKE_PREFIX_PATH /usr/include/x86_64-linux-gnu/qt5/)
set(CMAKE_INSTALL_PREFIX /usr/lib/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets DBus Quick)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::DBus Qt5::Quick)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(KDEInstallDirs)

set(cppFiles Src/Widgets/Standalone.cpp
        Src/Widgets/Standalone.h
        Src/Core/DBusUtils.h
        Src/Core/Player/Player.h
        Src/Core/Constants.h
        Src/Include/PVC.h Src/qml/PVC_Plugin.h
        Src/Core/Player/PlayerManager.h
        Src/Core/Player/PlayerManager.cpp
        Src/Core/Player/PlayerData.h
        Src/Application/ControllerLogic.cpp Src/Application/ControllerLogic.h Src/Application/Utils.h)

# Install cpp lib
add_library(PVC SHARED ${cppFiles})

set_target_properties(PVC PROPERTIES PUBLIC_HEADER "Src/Include/PVC.h")
target_include_directories(PVC PRIVATE Src)

install(TARGETS PVC LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(PVC.pc.in PVC.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/PVC.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Install qml ( doesn't copy .h include to qml folder )
#install(TARGETS PVC DESTINATION ${QML_INSTALL_DIR}/org/u7i/PVC)

install(TARGETS PVC DESTINATION x86_64-linux-gnu/qt5/qml/org/u7i/PVC)
install(FILES Src/qml/qmldir DESTINATION x86_64-linux-gnu/qt5/qml/org/u7i/PVC)

# Link qt libs
target_link_libraries(PVC ${REQUIRED_LIBS_QUALIFIED})

# Test ( executable )
add_executable(PVC_tests Tests/main.cpp Tests/resources.qrc)
target_link_libraries(PVC_tests ${REQUIRED_LIBS_QUALIFIED} PVC)