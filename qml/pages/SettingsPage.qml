import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../styles"

Page {
    id: root
    background: Rectangle { color: Theme.background }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        AppHeader {
            Layout.fillWidth: true
            title: qsTr("Settings")
            subtitle: qsTr("App preferences")
            showBackButton: true
            onBackClicked: root.StackView.view.pop()
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.border
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            Layout.margins: Theme.spacingL
            spacing: Theme.spacingM
            
            Label {
                text: qsTr("Appearance")
                font: Theme.fontSemiBold
                font.pixelSize: Theme.fontSizeL
                color: Theme.textPrimary
            }
            
            Rectangle {
                Layout.fillWidth: true
                height: childrenRect.height
                color: Theme.surface
                radius: Theme.radiusMedium
                
                ColumnLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    spacing: 0
                    
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.margins: Theme.spacingM
                        spacing: Theme.spacingM
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            
                            Label {
                                text: qsTr("Dark Mode")
                                font: Theme.fontMedium
                                font.pixelSize: Theme.fontSizeM
                                color: Theme.textPrimary
                            }
                            
                            Label {
                                text: qsTr("Use dark theme for better visibility in low light")
                                font.pixelSize: Theme.fontSizeS
                                color: Theme.textSecondary
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }
                        
                        Switch {
                            id: darkModeSwitch
                            checked: Theme.darkMode
                            onCheckedChanged: {
                                if (checked !== Theme.darkMode) {
                                    Theme.darkMode = checked
                                }
                            }
                        }
                    }
                    
                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: Theme.divider
                        Layout.leftMargin: Theme.spacingM
                        Layout.rightMargin: Theme.spacingM
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.margins: Theme.spacingM
                        spacing: Theme.spacingM
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            
                            Label {
                                text: qsTr("Device Name")
                                font: Theme.fontMedium
                                font.pixelSize: Theme.fontSizeM
                                color: Theme.textPrimary
                            }
                            
                            Label {
                                text: BtManager.localDeviceName || qsTr("Not available")
                                font.pixelSize: Theme.fontSizeS
                                color: Theme.textSecondary
                            }
                        }
                    }
                }
            }
            
            Label {
                Layout.topMargin: Theme.spacingL
                text: qsTr("About")
                font: Theme.fontSemiBold
                font.pixelSize: Theme.fontSizeL
                color: Theme.textPrimary
            }
            
            Rectangle {
                Layout.fillWidth: true
                height: childrenRect.height
                color: Theme.surface
                radius: Theme.radiusMedium
                
                ColumnLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    spacing: 0
                    
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.margins: Theme.spacingM
                        spacing: Theme.spacingM
                        
                        Label {
                            text: qsTr("Version")
                            font.pixelSize: Theme.fontSizeM
                            color: Theme.textPrimary
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        Label {
                            text: "1.0.0"
                            font.pixelSize: Theme.fontSizeM
                            color: Theme.textSecondary
                        }
                    }
                    
                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: Theme.divider
                        Layout.leftMargin: Theme.spacingM
                        Layout.rightMargin: Theme.spacingM
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.margins: Theme.spacingM
                        spacing: Theme.spacingM
                        
                        Label {
                            text: qsTr("Bluetooth Address")
                            font.pixelSize: Theme.fontSizeM
                            color: Theme.textPrimary
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        Label {
                            text: BtManager.localDeviceAddress || qsTr("Not available")
                            font.pixelSize: Theme.fontSizeS
                            color: Theme.textSecondary
                            font.family: Theme.fontCode.family
                        }
                    }
                }
            }
            
            Item { Layout.fillHeight: true }
            
            Label {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Made with Qt/QML")
                font.pixelSize: Theme.fontSizeS
                color: Theme.textMuted
            }
            
            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: Theme.spacingL
                text: qsTr("github.com/Bahtya/bt-message")
                font.pixelSize: Theme.fontSizeS
                color: Theme.primary
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
    }
    
    Connections {
        target: Theme
        function onDarkModeChanged() {
            darkModeSwitch.checked = Theme.darkMode
        }
    }
}
