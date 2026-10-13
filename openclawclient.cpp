#include "openclawclient.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkReply>
#include <QFile>
#include <QCoreApplication>

OpenClawClient::OpenClawClient(QObject *parent)
    : QObject{parent}
{
    m_network = new QNetworkAccessManager(this);

    m_ReqUrl ="http://127.0.0.1:18789/v1/chat/completions";

    QString configPath = QCoreApplication::applicationDirPath()+"/config/token.txt";

    QFile f(configPath);

    if(f.open(QIODevice::ReadOnly|QIODevice::Text)){
        m_token = f.readAll().trimmed();
        f.close();
    }

}

void OpenClawClient::sendMessage(const QString &text)
{
    QNetworkRequest req{QUrl(m_ReqUrl)};
    req.setRawHeader("Authorization",("Bearer "+ m_token).toUtf8());

    QJsonObject body;
    body["model"] = "openclaw/main";

    QJsonObject msg;
    msg["role"] = "user";
    msg["content"] = text;

    QJsonArray msgs;
    msgs.append(msg);

    body["messages"] = msgs;

    QByteArray data = QJsonDocument(body).toJson();

    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QNetworkReply *reply = m_network->post(req,data);

    connect(reply,&QNetworkReply::finished,this,[this,reply](){
        QByteArray resp = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(resp);
        QString content = doc["choices"][0]["message"]["content"].toString();
        reply->deleteLater();
        emit replyReceived(content);
    });
}
