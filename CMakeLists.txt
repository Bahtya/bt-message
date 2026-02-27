cmake_minimum_required(VERSION 3.16)

project(bt-message VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Bluetooth Network)

qt_add_executable(bt-message
    src/main.cpp
    src/bluetooth/btmanager.cpp
    src/bluetooth/btmanager.h
    src/bluetooth/btdevice.cpp
    src/bluetooth/btdevice.h
    src/bluetooth/btserver.cpp
    src/bluetooth/btserver.h
    src/bluetooth/btclient.cpp
    src/bluetooth/btclient.h
    src/bluetooth/androidpermissions.cpp
    src/bluetooth/androidpermissions.h
    src/models/devicemodel.cpp
    src/models/devicemodel.h
    src/models/messagemodel.cpp
    src/models/messagemodel.h
    src/models/filetransfermodel.cpp
    src/models/filetransfermodel.h
    src/models/messagestore.cpp
    src/models/messagestore.h
    src/protocol/messageprotocol.cpp
    src/protocol/messageprotocol.h
    src/protocol/filetransfermanager.cpp
    src/protocol/filetransfermanager.h
)

qt_add_qml_module(bt-message
    URI BtMessage
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/pages/ChatPage.qml
        qml/pages/DeviceListPage.qml
        qml/pages/SettingsPage.qml
        qml/components/DeviceDelegate.qml
        qml/components/MessageBubble.qml
        qml/components/FileTransferCard.qml
        qml/components/InputBar.qml
        qml/components/AppHeader.qml
    RESOURCES
        qml/styles/Theme.qml
)

target_link_libraries(bt-message PRIVATE
    Qt6::Quick
    Qt6::Bluetooth
    Qt6::Network
)

target_include_directories(bt-message PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(ANDROID)
    set_target_properties(bt-message PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
    )
endif()

if(WIN32)
    set_target_properties(bt-message PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

install(TARGETS bt-message
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)
