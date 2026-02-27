import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

import "pages"
import "components"
import "styles"

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: 640
    minimumWidth: 600
    minimumHeight: 400
    title: qsTr("BT Message")
    
    color: Theme.background
    
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: deviceListPage
    }
    
    Component {
        id: deviceListPage
        DeviceListPage {
            onDeviceSelected: function(address) {
                stackView.push(chatPage, { "deviceAddress": address })
            }
            onOpenSettings: {
                stackView.push(settingsPage)
            }
        }
    }
    
    Component {
        id: chatPage
        ChatPage {
            onBack: stackView.pop()
        }
    }
    
    Component {
        id: settingsPage
        SettingsPage {
        }
    }
    
    Connections {
        target: BtManager
        function onMessageReceived(message, fromAddress) {
            MessageModel.addMessage(message, false)
        }
    }
}
