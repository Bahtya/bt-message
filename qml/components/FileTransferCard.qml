import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../styles"

Rectangle {
    id: root
    height: 80
    radius: Theme.radiusMedium
    color: Theme.surface
    border.width: 1
    border.color: Theme.border
    
    property string fileName: ""
    property string fileSize: ""
    property real progress: 0
    property bool isSending: true
    property int status: 0
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingM
        spacing: Theme.spacingM
        
        Rectangle {
            Layout.alignment: Qt.AlignVCenter
            width: 48
            height: 48
            radius: Theme.radiusMedium
            color: Theme.surfaceVariant
            
            Text {
                anchors.centerIn: parent
                text: "\u1F4C1"
                font.pixelSize: 24
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingXS
            
            RowLayout {
                Layout.fillWidth: true
                
                Label {
                    text: fileName
                    font: Theme.fontMedium
                    font.pixelSize: Theme.fontSizeM
                    color: Theme.textPrimary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
                
                Label {
                    text: isSending ? qsTr("Sending") : qsTr("Receiving")
                    font.pixelSize: Theme.fontSizeS
                    color: Theme.primary
                }
            }
            
            Label {
                text: fileSize
                font.pixelSize: Theme.fontSizeS
                color: Theme.textSecondary
            }
            
            ProgressBar {
                Layout.fillWidth: true
                from: 0
                to: 1
                value: progress
                
                background: Rectangle {
                    implicitHeight: 4
                    radius: 2
                    color: Theme.surfaceVariant
                }
                
                contentItem: Item {
                    implicitHeight: 4
                    
                    Rectangle {
                        width: parent.width * parent.parent.position
                        height: parent.height
                        radius: 2
                        color: Theme.primary
                    }
                }
            }
        }
        
        Button {
            implicitWidth: 36
            implicitHeight: 36
            
            background: Rectangle {
                radius: Theme.radiusMedium
                color: parent.down ? Theme.surfaceVariant : (parent.hovered ? Theme.surfaceVariant : "transparent")
            }
            
            contentItem: Text {
                text: "\u2715"
                font.pixelSize: 16
                color: Theme.textSecondary
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
