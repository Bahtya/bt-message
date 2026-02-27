pragma Singleton
import QtQuick

QtObject {
    id: theme

    property bool darkMode: false

    function toggleDarkMode() {
        darkMode = !darkMode
    }

    property color primary: "#2563EB"
    property color primaryDark: "#1D4ED8"
    property color primaryLight: "#3B82F6"

    property color background: darkMode ? "#0F172A" : "#F8FAFC"
    property color surface: darkMode ? "#1E293B" : "#FFFFFF"
    property color surfaceVariant: darkMode ? "#334155" : "#F1F5F9"

    property color textPrimary: darkMode ? "#F1F5F9" : "#0F172A"
    property color textSecondary: darkMode ? "#CBD5E1" : "#475569"
    property color textMuted: darkMode ? "#64748B" : "#94A3B8"

    property color sentMessage: "#2563EB"
    property color sentMessageText: "#FFFFFF"
    property color receivedMessage: darkMode ? "#334155" : "#E2E8F0"
    property color receivedMessageText: darkMode ? "#F1F5F9" : "#0F172A"

    property color success: "#22C55E"
    property color warning: "#F59E0B"
    property color error: "#EF4444"
    property color info: "#3B82F6"

    property color online: "#22C55E"
    property color offline: darkMode ? "#475569" : "#94A3B8"
    property color border: darkMode ? "#334155" : "#E2E8F0"
    property color divider: darkMode ? "#1E293B" : "#F1F5F9"

    property int radiusSmall: 4
    property int radiusMedium: 8
    property int radiusLarge: 12
    property int radiusRound: 999

    property int spacingXS: 4
    property int spacingS: 8
    property int spacingM: 12
    property int spacingL: 16
    property int spacingXL: 24

    property font fontRegular: Qt.font({ family: "Inter", weight: Font.Normal })
    property font fontMedium: Qt.font({ family: "Inter", weight: Font.Medium })
    property font fontSemiBold: Qt.font({ family: "Inter", weight: Font.DemiBold })
    property font fontBold: Qt.font({ family: "Inter", weight: Font.Bold })
    property font fontCode: Qt.font({ family: "JetBrains Mono", weight: Font.Normal })

    property int fontSizeXS: 11
    property int fontSizeS: 12
    property int fontSizeM: 14
    property int fontSizeL: 16
    property int fontSizeXL: 18
    property int fontSizeXXL: 24

    property var colors: QtObject {
        readonly property color blue50: "#EFF6FF"
        readonly property color blue100: "#DBEAFE"
        readonly property color blue200: "#BFDBFE"
        readonly property color blue500: "#3B82F6"
        readonly property color blue600: "#2563EB"
        readonly property color blue700: "#1D4ED8"

        readonly property color slate50: "#F8FAFC"
        readonly property color slate100: "#F1F5F9"
        readonly property color slate200: "#E2E8F0"
        readonly property color slate300: "#CBD5E1"
        readonly property color slate400: "#94A3B8"
        readonly property color slate500: "#64748B"
        readonly property color slate600: "#475569"
        readonly property color slate700: "#334155"
        readonly property color slate800: "#1E293B"
        readonly property color slate900: "#0F172A"

        readonly property color green500: "#22C55E"
        readonly property color red500: "#EF4444"
        readonly property color amber500: "#F59E0B"
    }
}
