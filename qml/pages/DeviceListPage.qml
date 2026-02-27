import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"
import "../styles"

Page {
    id: root
    background: Rectangle { color: Theme.background }
    
    signal deviceSelected(string address)
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        AppHeader {
            Layout.fillWidth: true
            title: qsTr("BT Message")
            subtitle: BtManager.localDeviceName || qsTr("Bluetooth Off")
            showBackButton: false
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.border
        }
        
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0
            
            Rectangle {
                Layout.fillHeight: true
                implicitWidth: 280
                color: Theme.surface
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.spacingM
                    spacing: Theme.spacingM
                    
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Theme.spacingS
                        
                        Label {
                            text: qsTr("Nearby Devices")
                            font: Theme.fontSemiBold
                            font.pixelSize: Theme.fontSizeL
                            color: Theme.textPrimary
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        Button {
                            implicitWidth: 36
                            implicitHeight: 36
                            enabled: !BtManager.isScanning
                            
                            background: Rectangle {
                                radius: Theme.radiusMedium
                                color: parent.down ? Theme.surfaceVariant : (parent.hovered ? Theme.surfaceVariant : "transparent")
                            }
                            
                            contentItem: Text {
                                text: "\u21BB"
                                font.pixelSize: 18
                                color: BtManager.isScanning ? Theme.textMuted : Theme.primary
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            
                            onClicked: {
                                DeviceModel.clear()
                                BtManager.startDiscovery()
                            }
                            
                            BusyIndicator {
                                anchors.centerIn: parent
                                running: BtManager.isScanning
                                implicitWidth: 20
                                implicitHeight: 20
                            }
                        }
                    }
                    
                    ListView {
                        id: deviceListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: DeviceModel
                        spacing: Theme.spacingS
                        
                        delegate: DeviceDelegate {
                            width: deviceListView.width
                            onDeviceClicked: root.deviceSelected(deviceAddress)
                        }
                        
                        Label {
                            anchors.centerIn: parent
                            text: BtManager.isScanning ? qsTr("Scanning...") : qsTr("Tap refresh to scan")
                            color: Theme.textMuted
                            font.pixelSize: Theme.fontSizeM
                            visible: deviceListView.count === 0
                        }
                    }
                }
                
                Rectangle {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: 1
                    color: Theme.border
                }
            }
            
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    
                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: Theme.spacingL
                        
                        Rectangle {
                            Layout.alignment: Qt.AlignHCenter
                            width: 80
                            height: 80
                            radius: Theme.radiusLarge
                            color: Theme.surfaceVariant
                            
                            Text {
                                anchors.centerIn: parent
                                text: "\u1F4F1"
                                font.pixelSize: 36
                            }
                        }
                        
                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            text: qsTr("Select a device to start chatting")
                            font: Theme.fontMedium
                            font.pixelSize: Theme.fontSizeL
                            color: Theme.textSecondary
                        }
                        
                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            text: qsTr("Make sure the other device has BT Message running")
                            font.pixelSize: Theme.fontSizeM
                            color: Theme.textMuted
                        }
                    }
                }
            }
        }
    }
    
    Connections {
        target: BtManager
        function onDeviceDiscovered(name, address) {
            DeviceModel.addDevice(name, address)
        }
    }
    
    Component.onCompleted: {
        BtManager.startServer()
        BtManager.startDiscovery()
    }
}
