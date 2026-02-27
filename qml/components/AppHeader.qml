import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../styles"

Rectangle {
    id: root
    height: 56
    color: Theme.surface
    
    property string title: ""
    property string subtitle: ""
    property bool showBackButton: false
    
    signal backClicked()
    
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Theme.spacingM
        anchors.rightMargin: Theme.spacingM
        spacing: Theme.spacingM
        
        Button {
            visible: showBackButton
            implicitWidth: 36
            implicitHeight: 36
            
            background: Rectangle {
                radius: Theme.radiusMedium
                color: parent.down ? Theme.surfaceVariant : (parent.hovered ? Theme.surfaceVariant : "transparent")
            }
            
            contentItem: Text {
                text: "\u2039"
                font.pixelSize: 24
                font.weight: Font.Bold
                color: Theme.textPrimary
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: root.backClicked()
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            
            Label {
                text: root.title
                font: Theme.fontSemiBold
                font.pixelSize: Theme.fontSizeXL
                color: Theme.textPrimary
                Layout.fillWidth: true
                elide: Text.ElideRight
            }
            
            Label {
                text: root.subtitle
                font.pixelSize: Theme.fontSizeS
                color: Theme.textSecondary
                visible: root.subtitle.length > 0
                Layout.fillWidth: true
                elide: Text.ElideRight
            }
        }
        
        Rectangle {
            width: 8
            height: 8
            radius: 4
            color: BtManager.isServerRunning ? Theme.online : Theme.offline
            
            ToolTip.visible: parent.hovered
            ToolTip.text: BtManager.isServerRunning ? qsTr("Server running") : qsTr("Server stopped")
        }
    }
}
