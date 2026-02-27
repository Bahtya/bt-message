# BT Message

基于蓝牙的跨平台即时通讯与文件传输软件。

## 功能特性

- 蓝牙设备发现与配对
- 实时文本消息收发
- 文件传输（支持大文件分片传输）
- 跨平台支持（Windows / Android）

## 技术栈

- **框架**: Qt 6.x
- **语言**: C++17 + QML
- **蓝牙协议**: Classic Bluetooth (RFCOMM/SPP)
- **构建系统**: CMake

## 项目结构

```
bt-message/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── bluetooth/
│   │   ├── btmanager.cpp/h      # 蓝牙管理器
│   │   ├── btserver.cpp/h       # RFCOMM服务端
│   │   ├── btclient.cpp/h       # RFCOMM客户端
│   │   └── btdevice.cpp/h       # 设备数据结构
│   ├── models/
│   │   ├── devicemodel.cpp/h    # 设备列表模型
│   │   ├── messagemodel.cpp/h   # 消息列表模型
│   │   └── filetransfermodel.cpp/h # 文件传输模型
│   └── protocol/
│       └── messageprotocol.cpp/h # 消息协议
├── qml/
│   ├── main.qml
│   ├── pages/
│   │   ├── DeviceListPage.qml   # 设备列表页
│   │   ├── ChatPage.qml         # 聊天页面
│   │   └── SettingsPage.qml     # 设置页面
│   ├── components/
│   │   ├── AppHeader.qml        # 顶部栏
│   │   ├── DeviceDelegate.qml   # 设备列表项
│   │   ├── MessageBubble.qml    # 消息气泡
│   │   ├── InputBar.qml         # 输入栏
│   │   └── FileTransferCard.qml # 文件传输卡片
│   └── styles/
│       └── Theme.qml            # 主题配置
└── android/
    └── AndroidManifest.xml      # Android配置
```

## 构建说明

### Windows

1. 安装 Qt 6.x (包含 Qt Bluetooth 模块)
2. 安装 CMake 3.16+
3. 安装 Visual Studio 2019+ 或 MinGW

```bash
mkdir build && cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

### Android

1. 安装 Qt 6.x for Android
2. 配置 Android SDK / NDK
3. 设置 ANDROID_SDK_ROOT 和 ANDROID_NDK_ROOT 环境变量

```bash
mkdir build && cd build
cmake -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=$Qt6_DIR/lib/cmake/Qt6/qt.toolchain.cmake -DANDROID_BUILD_ALL_ABIS=ON ..
cmake --build .
```

## 消息协议

```
+--------+------+----------+----------+--------+
| Header | Type | Length   | Payload  | CRC16  |
| 2 bytes| 1 b  | 4 bytes  | N bytes  | 2 bytes|
+--------+------+----------+----------+--------+
```

- **Header**: 0xAA55 (固定包头)
- **Type**: 消息类型 (0x01=文本, 0x02=文件头, 0x03=文件分片, 0x04=文件结束, 0x05=心跳)
- **Length**: 负载长度
- **Payload**: 数据内容
- **CRC16**: 校验和

## 使用说明

1. 启动应用后自动开启蓝牙服务端
2. 点击刷新按钮扫描附近设备
3. 选择设备建立连接
4. 发送文本或文件

## 注意事项

- Windows 需要先在系统设置中配对蓝牙设备
- Android 需要授予蓝牙和位置权限
- 文件传输建议单文件不超过 100MB

## License

MIT License
