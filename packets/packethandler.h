#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <functional>
#include <QMap>
#include <QObject>

#include "workertask.h"

class PacketHandler
{
public:
    static PacketHandler& getInstance()
    {
        static PacketHandler instance;
        return instance;
    }

    /**
     * @brief Tries to call the method for this specific packet in events
     * @param The packet id
     * @param The packet to handle
     * @param Pointer to the worker task instance
     * @return True if the events contains this messageId
     */
    bool handle(int messageId, const QByteArray &bytes, WorkerTask *workerTask);

private:
    PacketHandler();

    // To avoid getting copies of this singleton
    PacketHandler(PacketHandler const&) = delete;
    void operator=(PacketHandler const&) = delete;

    QMap<quint32, std::function<void(WorkerTask *workerTask, const QByteArray &bytes)>> events;

    // Message event methods
    void clientCertifyLogin(WorkerTask *workerTask, const QByteArray &bytes);
    void serverCertifyLogin(WorkerTask *workerTask, const QByteArray &bytes);
};

#endif // MESSAGEHANDLER_H