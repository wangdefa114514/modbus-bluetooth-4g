#ifndef PARSE_H
#define PARSE_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "utils.h"
class parse:public QObject
{
     Q_OBJECT
public:
    parse();
    void downloadResultRequest(QByteArray &downloadResultRequestData) const;
    void set(quint8 slaveId,quint32 m_code,quint32 version);
public slots:
    void parse_modbus(QByteArray send);
    void parse_other(QByteArray pay,bool requires_receive,quint8 type);
    void parse_rec(QByteArray rec);
    void open_file(QString file_path);
    void on_sendDownloadRequestByPackageIndex(quint32 pkgIdx);
    void downloadRequest(QByteArray &downloadRequestData, quint32 pkgIdx);
    void on_parseDownloadResponse(const QByteArray downloadResponseData, quint32 pkgIdx);
    void on_parseDownloadResultResponse(const QByteArray downloadResultResponseData);
    void on_downloadBin();
    void on_parseHandshakesResponse(const QByteArray handshakesResponseData);
    void handshakesRequest(QByteArray &handshakesRequestData) const;
    void on_downloadWrapper();
signals:
    void sig_send(QByteArray send);
    void sig_send_receive(QByteArray send);
    void sig_log(QString s);
    void sig_flog(QString s);
    void sig_progressBarDownload(int value);
    void sig_sendDownloadRequestByPackageIndex(quint32 pkgIdx);
    void sig_sendDownloadRequest(const QByteArray downloadRequestData, quint16 pkgIdx);
    void sig_sendDownloadResultRequest(const QByteArray downloadResultRequestData);
    void sig_downloadResultResponseSuccessed();
    void sig_downloadResultResponseFailed();
    void sig_send_modbus(QByteArray send,quint16 start);
    void sig_sendHandshakesRequest(const QByteArray handshakesRequestData);
    void sig_downloadWrapper();
    void sig_download_bin();

private:
    QByteArray m_file;
    quint32 m_div;
    quint32 m_mod;
    quint32 m_totalPackageCount;
    float m_percentStep;
    quint32 m_fileSize;
    quint8 m_slaveId=0x01;
    quint32 m_fileCrc32;
    quint32 m_manufacturerCode;
    quint32 m_upgradeVersionNumber;
    QDateTime start;
    QDateTime end;
};

#endif // PARSE_H
