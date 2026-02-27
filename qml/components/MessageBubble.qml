import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../styles"

Rectangle {
    id: root
    height: contentColumn.height + Theme.spacingM * 2
    radius: Theme.radiusMedium
    color: model.isSent ? Theme.sentMessage : Theme.receivedMessage
    
    property bool isSent: model.isSent || false
    property string content: model.content || ""
    property string timeText: model.timeText || ""
    property int messageType: model.messageType || 0
    property string fileName: model.fileName || ""
    property string fileSizeText: model.fileSizeText || ""
    property real progress: model.progress || 0
    
    ColumnLayout {
        id: contentColumn
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: Theme.spacingM
        spacing: Theme.spacingXS
        
        Loader {
            id: contentLoader
            Layout.fillWidth: true
            sourceComponent: messageType === 1 ? fileComponent : textComponent
        }
        
        Label {
            text: timeText
            font.pixelSize: Theme.fontSizeXS
            color: isSent ? "rgba(255,255,255,0.7)" : Theme.textMuted
            Layout.alignment: isSent ? Qt.AlignRight : Qt.AlignLeft
        }
    }
    
    Component {
        id: textComponent
        Label {
            text: content
            wrapMode: Text.Wrap
            font.pixelSize: Theme.fontSizeM
            font.family: messageType === 1 ? Theme.fontCode.family : Theme.fontRegular.family
            color: isSent ? Theme.sentMessageText : Theme.receivedMessageText
            Layout.fillWidth: true
        }
    }
    
    Component {
        id: fileComponent
        ColumnLayout {
            spacing: Theme.spacingXS
            
            RowLayout {
                spacing: Theme.spacingS
                
                Rectangle {
                    width: 36
                    height: 36
                    radius: Theme.radiusSmall
                    color: isSent ? "rgba(255,255,255,0.2)" : Theme.surfaceVariant
                    
                    Text {
                        anchors.centerIn: parent
                        text: "\u1F4C1"
                        font.pixelSize: 18
                    }
                }
                
                ColumnLayout {
                    spacing: 2
                    
                    Label {
                        text: fileName
                        font: Theme.fontMedium
                        font.pixelSize: Theme.fontSizeM
                        color: isSent ? Theme.sentMessageText : Theme.receivedMessageText
                        elide: Text.ElideRight
                        Layout.maximumWidth: 200
                    }
                    
                    Label {
                        text: fileSizeText
                        font.pixelSize: Theme.fontSizeS
                        color: isSent ? "rgba(255,255,255,0.7)" : Theme.textSecondary
                    }
                }
            }
            
            ProgressBar {
                Layout.fillWidth: true
                from: 0
                to: 1
                value: progress
                visible: progress > 0 && progress < 1
                
                background: Rectangle {
                    implicitHeight: 4
                    radius: 2
                    color: isSent ? "rgba(255,255,255,0.3)" : Theme.surfaceVariant
                }
                
                contentItem: Item {
                    implicitHeight: 4
                    
                    Rectangle {
                        width: parent.width * parent.parent.position
                        height: parent.height
                        radius: 2
                        color: isSent ? "#FFFFFF" : Theme.primary
                    }
                }
            }
        }
    }
}
