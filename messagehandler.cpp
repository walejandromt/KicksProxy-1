#include "messagehandler.h"

#include <QDebug>

#include "constants.h"
#include "packetid.h"

bool MessageHandler::handle(int messageId, const QByteArray &bytes, WorkerTask *workerTask)
{
    auto func = events.value(messageId, NULL);

    if (func != NULL) {
        func(workerTask, bytes);
    }

    return func != NULL;
}

MessageHandler::MessageHandler()
{
    // Client Events

    static quint32 intKey = (Constants::XOR_KEY << 24) | (Constants::XOR_KEY << 16) |
                   (Constants::XOR_KEY << 8) | Constants::XOR_KEY;

    events.insert(PacketId::CERTIFY_LOGIN ^ intKey,
                        [=] (WorkerTask *workerTask, const QByteArray &bytes) {
        clientCertifyLogin(workerTask, bytes);
    });

    // Server Events

    events.insert(PacketId::CERTIFY_LOGIN,
                        [=] (WorkerTask *workerTask, const QByteArray &bytes) {
        serverCertifyLogin(workerTask, bytes);
    });
}

void MessageHandler::clientCertifyLogin(WorkerTask *workerTask, const QByteArray &bytes)
{
    qDebug() << "Received client's CertifyLogin with size: " << bytes.size();

    emit workerTask->write(bytes, workerTask->getWriterIndex());
}

void MessageHandler::serverCertifyLogin(WorkerTask *workerTask, const QByteArray &bytes)
{
    qDebug() << "Received server's CertifyLogin with size: " << bytes.size();

    // Ignore the packet so the client will not receive it
    workerTask->ignoreCurrent();
}
