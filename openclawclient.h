#ifndef OPENCLAWCLIENT_H
#define OPENCLAWCLIENT_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager.h>
#include <QString.h>

class OpenClawClient : public QObject
{
    Q_OBJECT
public:
    explicit OpenClawClient(QObject *parent = nullptr);
    void sendMessage(const QString &text);

signals:
    void replyReceived(const QString &reply);
    void errorOccurred(const QString &error);

private:
    QNetworkAccessManager *m_network;
    QString m_ReqUrl;
};

#endif // OPENCLAWCLIENT_H
