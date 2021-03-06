#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H

#include <QByteArray>
#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include <QUdpSocket>

class ProxyClient : public QObject
{
    Q_OBJECT
public:
    explicit ProxyClient(quint16 serverId, QObject *parent = 0);

    /**
     * @brief Initializes the proxy between client and server
     * @param Socket descriptor for the client connection
     */
    void initializeProxy(QTcpSocket *clientSocket, QTcpSocket *serverSocket);

signals:
    void disconnected();

public slots:
    // socket slots
    void clientDisconnected();
    void clientReadyRead();
    void serverDisconnected();
    void serverReadyRead();

    // worker thread slots
    /**
     * @brief Called when the read was completed by the worker thread
     * @param Current reader position in the data array
     */
    void clientReadComplete(int readerIndex);
    void serverReadComplete(int readerIndex);

    /**
     * @brief Writes the specified byte array to the socket
     * @param The byte array to write
     * @param Current writer position in the data array
     */
    void clientWrite(const QByteArray &data, int writerIndex);
    void serverWrite(const QByteArray &data, int writerIndex);

    inline void updateClientWriterIndex(int writerIndex)
    {
        clientWriterIndex = writerIndex;
    }

    inline void updateServerWriterIndex(int writerIndex)
    {
        serverWriterIndex = writerIndex;
    }

    inline void setPlayerId(quint32 playerId)
    {
        this->playerId = playerId;
    }

private:
    enum ReadingState {
        NotReading,
        Reading,
        ReadingPending
    };

    QThreadPool *threadPool;

    QTcpSocket *clientSocket;
    QTcpSocket *serverSocket;

    QByteArray *clientData;
    QByteArray *serverData;

    int clientWriterIndex;
    int serverWriterIndex;

    ReadingState clientReading;
    ReadingState serverReading;

    quint16 serverId;
    quint32 playerId;
};

#endif // PROXYCLIENT_H
