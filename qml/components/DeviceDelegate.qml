import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../styles"

ItemDelegate {
    id: root
    height: 64
    implicitWidth: parent ? parent.width : 260
    
    property string deviceName: model.deviceName || "Unknown Device"
    property string deviceAddress: model.deviceAddress || ""
    property bool isConnected: model.isConnected || false
    
    signal deviceClicked()
    
    background: Rectangle {
        radius: Theme.radiusMedium
        color: root.down ? Theme.surfaceVariant : (root.hovered ? Theme.surfaceVariant : "transparent")
        
        Behavior on color {
            ColorAnimation { duration: 150 }
        }
    }
    
    contentItem: RowLayout {
        spacing: Theme.spacingM
        
        Rectangle {
            Layout.alignment: Qt.AlignVCenter
            width: 40
            height: 40
            radius: Theme.radiusRound
            color: Theme.primary
            
            Text {
                anchors.centerIn: parent
                text: deviceName.charAt(0).toUpperCase()
                font: Theme.fontSemiBold
                font.pixelSize: Theme.fontSizeXL
                color: "white"
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            
            Label {
                text: root.deviceName
                font: Theme.fontMedium
                font.pixelSize: Theme.fontSizeM
                color: Theme.textPrimary
                Layout.fillWidth: true
                elide: Text.ElideRight
            }
            
            Label {
                text: root.deviceAddress
                font.pixelSize: Theme.fontSizeS
                color: Theme.textSecondary
            }
        }
        
        Rectangle {
            Layout.alignment: Qt.AlignVCenter
            width: 8
            height: 8
            radius: 4
            color: root.isConnected ? Theme.online : Theme.offline
        }
    }
    
    onClicked: deviceClicked()
    
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.NoButton
    }
}
