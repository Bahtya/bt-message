#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

#include "bluetooth/btmanager.h"
#include "models/devicemodel.h"
#include "models/messagemodel.h"
#include "models/filetransfermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("BT Message");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Bahtya");

    QQmlApplicationEngine engine;

    BtManager btManager;
    DeviceModel deviceModel;
    MessageModel messageModel;
    FileTransferModel fileTransferModel;

    engine.rootContext()->setContextProperty("BtManager", &btManager);
    engine.rootContext()->setContextProperty("DeviceModel", &deviceModel);
    engine.rootContext()->setContextProperty("MessageModel", &messageModel);
    engine.rootContext()->setContextProperty("FileTransferModel", &fileTransferModel);

    const QUrl url(u"qrc:/BtMessage/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
