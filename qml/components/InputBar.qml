import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import "../styles"

Rectangle {
    id: root
    height: 56
    radius: Theme.radiusMedium
    color: Theme.surface
    border.width: 1
    border.color: Theme.border
    
    property bool enabled: true
    
    signal sendMessage(string text)
    signal sendFile(string filePath)
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingS
        spacing: Theme.spacingS
        
        Button {
            id: fileButton
            implicitWidth: 40
            implicitHeight: 40
            enabled: root.enabled
            
            background: Rectangle {
                radius: Theme.radiusMedium
                color: parent.down ? Theme.surfaceVariant : (parent.hovered ? Theme.surfaceVariant : "transparent")
            }
            
            contentItem: Text {
                text: "+"
                font.pixelSize: 24
                font.weight: Font.Bold
                color: root.enabled ? Theme.primary : Theme.textMuted
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: fileDialog.open()
        }
        
        TextField {
            id: messageInput
            Layout.fillWidth: true
            Layout.fillHeight: true
            placeholderText: qsTr("Type a message...")
            enabled: root.enabled
            font.pixelSize: Theme.fontSizeM
            color: Theme.textPrimary
            placeholderTextColor: Theme.textMuted
            selectByMouse: true
            
            background: Rectangle {
                color: "transparent"
            }
            
            Keys.onReturnPressed: {
                if (event.modifiers === Qt.NoModifier) {
                    sendClicked()
                    event.accepted = true
                }
            }
            
            Keys.onEnterPressed: {
                if (event.modifiers === Qt.NoModifier) {
                    sendClicked()
                    event.accepted = true
                }
            }
        }
        
        Button {
            id: sendButton
            implicitWidth: 40
            implicitHeight: 40
            enabled: root.enabled && messageInput.text.trim().length > 0
            
            background: Rectangle {
                radius: Theme.radiusMedium
                color: parent.enabled ? (parent.down ? Theme.primaryDark : Theme.primary) : Theme.surfaceVariant
            }
            
            contentItem: Text {
                text: "\u2191"
                font.pixelSize: 20
                font.weight: Font.Bold
                color: parent.enabled ? "white" : Theme.textMuted
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: sendClicked()
        }
    }
    
    FileDialog {
        id: fileDialog
        title: qsTr("Select a file to send")
        onAccepted: {
            root.sendFile(selectedFile.toString().replace("file://", ""))
        }
    }
    
    function sendClicked() {
        if (messageInput.text.trim().length > 0) {
            root.sendMessage(messageInput.text)
            messageInput.text = ""
        }
    }
}
