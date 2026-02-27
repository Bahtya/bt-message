import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../styles"

Page {
    id: root
    background: Rectangle { color: Theme.background }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXL
        spacing: Theme.spacingXL
        
        Label {
            text: qsTr("Settings")
            font: Theme.fontBold
            font.pixelSize: Theme.fontSizeXXL
            color: Theme.textPrimary
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.surface
            radius: Theme.radiusMedium
        }
    }
}
