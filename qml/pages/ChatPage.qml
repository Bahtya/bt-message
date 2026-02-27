import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"
import "../styles"

Page {
    id: root
    background: Rectangle { color: Theme.background }
    
    property string deviceAddress: ""
    property string deviceName: ""
    
    signal back()
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        AppHeader {
            Layout.fillWidth: true
            title: deviceName || deviceAddress
            subtitle: BtManager.connectionState === 2 ? qsTr("Connected") : qsTr("Connecting...")
            showBackButton: true
            onBackClicked: {
                BtManager.disconnect()
                root.back()
            }
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.border
        }
        
        ListView {
            id: messageListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: Theme.spacingM
            model: MessageModel
            spacing: Theme.spacingS
            clip: true
            
            delegate: MessageBubble {
                width: messageListView.width - Theme.spacingM * 2
                anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
            }
            
            onCountChanged: Qt.callLater(function() { messageListView.positionViewAtEnd() })
        }
        
        InputBar {
            Layout.fillWidth: true
            Layout.leftMargin: Theme.spacingM
            Layout.rightMargin: Theme.spacingM
            Layout.bottomMargin: Theme.spacingM
            enabled: BtManager.connectionState === 2
            onSendMessage: function(text) {
                if (text.trim().length > 0) {
                    BtManager.sendMessage(text)
                    MessageModel.addMessage(text, true)
                }
            }
            onSendFile: function(filePath) {
                BtManager.sendFile(filePath)
            }
        }
    }
    
    Component.onCompleted: {
        for (var i = 0; i < DeviceModel.rowCount(); i++) {
            var addr = DeviceModel.data(DeviceModel.index(i, 0), 258)
            if (addr === deviceAddress) {
                deviceName = DeviceModel.data(DeviceModel.index(i, 0), 257)
                break
            }
        }
        BtManager.connectToDevice(deviceAddress)
    }
}
