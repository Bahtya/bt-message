pragma Singleton
import QtQuick

QtObject {
    property color primary: "#2563EB"
    property color primaryDark: "#1D4ED8"
    property color primaryLight: "#3B82F6"
    
    property color background: "#F8FAFC"
    property color surface: "#FFFFFF"
    property color surfaceVariant: "#F1F5F9"
    
    property color textPrimary: "#0F172A"
    property color textSecondary: "#475569"
    property color textMuted: "#94A3B8"
    
    property color sentMessage: "#2563EB"
    property color receivedMessage: "#E2E8F0"
    
    property color success: "#22C55E"
    property color warning: "#F59E0B"
    property color error: "#EF4444"
    property color info: "#3B82F6"
    
    property color online: "#22C55E"
    property color offline: "#94A3B8"
    property color border: "#E2E8F0"
    property color divider: "#F1F5F9"
    
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
}
