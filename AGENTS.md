# AGENTS.md

Guidelines for coding agents working in the bt-message repository.

## Project Overview

Bluetooth-based instant messaging and file transfer application using Qt 6.x + QML + C++. Target platforms: Windows and Android. Uses Classic Bluetooth (RFCOMM/SPP) protocol.

## Build Commands

```bash
# Configure (Linux/Desktop)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Configure (Windows/MSVC)
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --parallel

# Build release (Windows)
cmake --build build --config Release --parallel

# Deploy Qt dependencies (Windows, after build)
cd build/Release && windeployqt bt-message.exe --release --no-translations

# Install dependencies (Debian/Ubuntu)
sudo apt install qt6-base-dev qt6-connectivity-dev qt6-declarative-dev cmake build-essential
```

## Test Commands

No automated tests currently exist. When adding tests:

```bash
# Run all tests (when implemented)
ctest --test-dir build --output-on-failure

# Run single test
ctest --test-dir build -R <test_name> --output-on-failure
```

## Lint/Format Commands

```bash
# C++ format (if clang-format configured)
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# QML format (Qt 6.6+)
qmlformat qml/**/*.qml -i
```

## Qt Version Compatibility

Qt 6.4 (Debian) has different API than Qt 6.6 (CI/Windows):
- `QBluetoothUuid::SerialPort` → `QBluetoothUuid::ServiceClassUuid::SerialPort`
- `QBluetoothUuid::Rfcomm` → `QBluetoothUuid::ProtocolUuid::Rfcomm`
- `QBluetoothSocket::ConnectedState` → `QBluetoothSocket::SocketState::ConnectedState`
- `connectToService(device, uuid)` → `connectToService(device.address(), uuid)`
- `QBluetoothHostInfo` → use `QBluetoothLocalDevice`

## C++ Style Guidelines

### Imports
```cpp
// Qt headers first (alphabetically)
#include <QBluetoothSocket>
#include <QObject>
#include <QTimer>

// Project headers second (with path from src/)
#include "bluetooth/btdevice.h"
#include "protocol/messageprotocol.h"
```

### Naming Conventions
- Classes: `PascalCase` (e.g., `BtManager`, `MessageProtocol`)
- Member variables: `m_camelCase` with `= nullptr` or `= 0` init
- Functions: `camelCase` (e.g., `startDiscovery`, `sendMessage`)
- Signals: `camelCase` (e.g., `deviceDiscovered`, `messageReceived`)
- Slots: `onTargetSignal` pattern (e.g., `onDeviceDiscovered`)
- Enums: `PascalCase` values, `Q_ENUM` macro for QML exposure
- Constants: `UPPER_SNAKE_CASE` or `kPascalCase` for static const

### Class Structure
```cpp
class Example : public QObject
{
    Q_OBJECT
    Q_PROPERTY(type name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Example(QObject *parent = nullptr);
    ~Example();

    type name() const;
    void setName(type value);

public slots:
    void doSomething();

signals:
    void nameChanged();
    void error(const QString &message);

private slots:
    void onInternalEvent();

private:
    void helperMethod();

    Type *m_member = nullptr;
    int m_counter = 0;
};
```

### Formatting
- Indent: 4 spaces (no tabs)
- Braces: opening brace on same line
- Member initializer list: one per line, comma at end
- Lambda captures: explicit `[this]` preferred over `[=]`
- Use `Q_UNUSED(var)` for unused parameters

### Error Handling
- Emit `error(QString)` signal with user-friendly message
- Use `tr()` for translatable error strings
- Check pointers before use: `if (m_ptr) { ... }`
- Use `emit` keyword when emitting signals

### Qt Best Practices
- Use `Q_PROPERTY` for all QML-accessible properties
- Use `Q_INVOKABLE` or `slots:` for QML-callable methods
- Connect signals with new syntax: `connect(sender, &Class::signal, receiver, &Class::slot)`
- Use `Qt::QueuedConnection` for cross-thread signal connections

## QML Style Guidelines

### Imports
```qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"
import "../styles"
```

### Naming Conventions
- Files: `PascalCase.qml`
- Component ids: `camelCase` (e.g., `root`, `messageListView`)
- Properties: `camelCase` (e.g., `deviceAddress`, `showBackButton`)
- Signals: `camelCase` (e.g., `back()`, `deviceSelected(address)`)

### Component Structure
```qml
Page {
    id: root
    
    property string exampleProperty: ""
    signal exampleSignal(string value)
    
    background: Rectangle { color: Theme.background }
    
    ColumnLayout {
        anchors.fill: parent
        
        // Child components
    }
    
    Connections {
        target: SomeCppObject
        function onSomeSignal(arg) { }
    }
    
    Component.onCompleted: { }
}
```

### Formatting
- Indent: 4 spaces
- One property per line
- Group properties: id, then properties, then signals, then visual
- Use Theme singleton for colors/spacing: `Theme.background`, `Theme.spacingM`

## Project Structure

```
src/
├── bluetooth/     # Bluetooth RFCOMM communication
├── models/        # QAbstractListModel subclasses for QML
├── protocol/      # Message encoding/decoding, file transfer
└── main.cpp       # Entry point, QML type registration
qml/
├── pages/         # Top-level screens (DeviceListPage, ChatPage, SettingsPage)
├── components/    # Reusable UI components
└── styles/        # Theme.qml singleton
```

## Key Patterns

- **Bluetooth UUID**: `e8e10f95-1a70-4b27-9ccf-02010264e9c8`
- **File chunk size**: 4096 bytes
- **Auto-reconnect**: Exponential backoff, max 5 attempts
- **Dark mode**: Toggle via `Theme.darkMode` property
