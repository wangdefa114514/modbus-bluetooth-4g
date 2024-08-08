#include "parse.h"

parse::parse()
{

}
void parse::parse_modbus(QByteArray message){
    QByteArray send;
    QByteArray temp;
    utils::instance().append(temp,(quint8)0x01);
    temp.append(message);
    uint16_t crc=utils::instance().calc_crc16(temp);
    utils::instance().append(temp,crc);
    quint8 length=(quint8) temp.size();
    utils::instance().append(send,(quint8)0x5a);
    utils::instance().append(send,length);
    send.append(temp);
    quint16 start;
    utils::instance().number(start,message,2);
    quint8 opcode;
    utils::instance().number(opcode,message,1);
    if(opcode==0x03 || opcode==0x01){
        emit sig_send_modbus(send,start);}
    else{
        emit sig_send(send);
    }
}
void parse::parse_other(QByteArray pay,bool requires_receive,quint8 type){
    QByteArray send;
    QByteArray temp;

    utils::instance().append(temp,(quint8)type);
    temp.append(pay);
    uint16_t crc=utils::instance().calc_crc16(temp);
    utils::instance().append(temp,crc);
    quint8 length=(quint8) temp.size();
    utils::instance().append(send,(quint8)0x5a);
    utils::instance().append(send,length);
    send.append(temp);
    if(!requires_receive){
        emit sig_send(send);
    }
    else{
        emit sig_send_receive(send);
    }

}
void parse::parse_rec(QByteArray rec){
   QByteArray temp=rec.mid(2,rec.size()-4);
   quint8 u8;
   quint16 u16;
   quint8 payload8;
   QByteArray log_array;
   QByteArray log_array1;
   utils::instance().number(u16,rec,rec.size()-2);
   if(u16!=utils::instance().calc_crc16(temp)){
       emit sig_log("crc 不匹配"+log_array.setNum(u16,16)+'/'+log_array1.setNum(utils::instance().calc_crc16(temp),16));
       return;
   }
   utils::instance().number(u8,rec,2);
   if(u8==0x82){
       utils::instance().number(payload8,rec,3);

       if(payload8==0x01){
           emit sig_log("启动车辆");
       }
       else if(payload8==0x02){
           emit sig_log("熄火车辆");
       }
       else if(payload8==0x03){
           emit sig_log("车辆设防");
       }
       else if(payload8==0x04){
           emit sig_log("车辆撤防");
       }
       else if(payload8==0x05){
           emit sig_log("开鞍座锁");
       }
       else if(payload8==0x06){
           emit sig_log("寻车鸣笛");
       }
       else{
           emit sig_log("操作不匹配");
       }
       utils::instance().number(payload8,rec,4);
       if(payload8==0x00){
           emit sig_log("执行成功");
       }
       else if(payload8==0x01){
           emit sig_log("执行失败");
       }
       else{
           emit sig_log("未知结果");
       }
   }
   else if(u8==0x05){

       quint8 pay;;
       utils::instance().number(pay,rec,3);
       if(pay==0x01){
           emit sig_log("车辆已启动");
       }
       else if(pay==0x02){
           emit sig_log("车辆已熄火");
       }
       else if(pay==0x03){
           emit sig_log("单击按钮");
       }
       else if(pay==0x04){
           emit sig_log("双击按钮");
       }
       else if(pay==0x05){
           emit sig_log("长按按钮");
       }
       else if(pay==0x06){
           emit sig_log("车辆有轮动");
       }
       else if(pay==0x07){
           emit sig_log("钥匙插入，在启动位置");
       }
       else if(pay==0x08){
           emit sig_log("钥匙插入，在熄火位置");
       }

       else{
           emit sig_log("未知事件");
       }
   }
   else if(u8==0x06){
       quint8 pay1;;
       utils::instance().number(pay1,rec,3);
       QString send;
       if(pay1==0x01){
           send="手动输入的密码";
       }
       else if(pay1==0x02){
           send="输入的nfc密码";
       }
       else{
           send="未知数据类型";
       }
       emit sig_log(send+':'+rec.mid(4,4).toHex(' '));
   }
   else if(u8==0x83){
       utils::instance().number(payload8,rec,3);
       if(payload8==0x01){
            utils::instance().number(payload8,rec,4);
            if(payload8==0x00){
                emit sig_log("时间日期执行成功");
            }
            else{
                emit sig_log("时间日期执行成功");
            }
       }
       else{
           emit sig_log("时间日期数据类型不匹配");
       }
   }
   else if(u8==0x84){
       utils::instance().number(payload8,rec,3);
       if(payload8==0x01){
            utils::instance().number(payload8,rec,4);
            if(payload8==0x00){
                emit sig_log("车辆已启动");
            }
            else{
                emit sig_log("车辆已熄火");
            }
       }
       else{
           emit sig_log("信息类型不匹配");
       }
   }
   else if(u8==0x86){
       utils::instance().number(payload8,rec,3);
       if(payload8==0x01){
            emit sig_log("用户手动输入的密码");
       }
       else if(payload8==0x02){
           emit sig_log("用户输入的NFC密码");
       }
       else{
           emit sig_log("数据类型不匹配");
       }
       utils::instance().number(payload8,rec,4);
       if(payload8==0x00){
           emit sig_log("执行成功");
       }
       else if(payload8==0x01){
           emit sig_log("执行失败");
       }
       else{
           emit sig_log("未知结果");
       }
   }
   else{
       emit sig_log("未知类型");
   }
}
void parse::open_file(QString file_path){
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << QString(tr("[%1] Failed to open file: %2 "))
                      .arg(metaObject()->className())
                      .arg(file_path);
        return;
    }


    m_file = file.readAll();
    m_fileCrc32=utils::instance().crc32(m_file);
    m_fileSize=m_file.size();
    m_div = m_fileSize / 128;
    m_mod = m_fileSize % 128;

    if (0 == m_mod)
    {
        m_totalPackageCount = m_div;
    }
    else
    {
        m_totalPackageCount = m_div + 1;
    }

    m_percentStep = 0.0L;
    if (0 == m_div)
    {
        m_percentStep = 100.0L;
    }
    else
    {
        if (0 == m_mod)
        {
            m_percentStep = 100.0 / m_div;
        }
        else
        {
            m_percentStep = 100.0 / (m_div + 1);
        }
    }

    qDebug() << QString(tr("[%1] fileSize: %2, div: %3, mod: %4, percentStep: %5"))
                .arg(metaObject()->className())
                .arg(m_fileSize)
                .arg(m_div)
                .arg(m_mod)
                .arg(m_percentStep);
    start=QDateTime::currentDateTime();
    emit sig_download_bin();

    //emit sig_sendDownloadRequestByPackageIndex(pkgIdx);
}
void parse::on_sendDownloadRequestByPackageIndex(quint32 pkgIdx)
{
    if (pkgIdx == m_totalPackageCount)
    {
        // Step3
        QByteArray downloadResultRequestData;
        downloadResultRequest(downloadResultRequestData);
        emit sig_sendDownloadResultRequest(downloadResultRequestData);
        return;
    }
    else if (pkgIdx > m_totalPackageCount)
    {
        return;
    }

    int percent = (pkgIdx * m_percentStep);
    emit sig_progressBarDownload(percent);

    QByteArray downloadRequestData;
    downloadRequest(downloadRequestData, pkgIdx);
    emit sig_sendDownloadRequest(downloadRequestData, pkgIdx);
}
void parse::on_downloadBin()
{
    // Step1
    qDebug()<<"on download bin";
    QByteArray handshakesRequestData;
    handshakesRequest(handshakesRequestData);
    emit sig_sendHandshakesRequest(handshakesRequestData);
}

void parse::on_parseHandshakesResponse(const QByteArray handshakesResponseData)
{
    qDebug()<<"parse handshake response";
    int responseDataSize = handshakesResponseData.size();
    int packageSize = sizeof(handshakesResponse_t);
    handshakesResponse_t handshakesResponseContext;

    int startOffset = 0;
    const char* responseDataBuffer = handshakesResponseData.data();
    for (;;)
    {
        if ((responseDataSize - startOffset) >= packageSize)
        {
            utils::instance().number(handshakesResponseContext.slaveId, (unsigned char *)responseDataBuffer + startOffset + offsetof(handshakesResponse_t, slaveId));
            if (m_slaveId != handshakesResponseContext.slaveId)
            {
                qWarning() << QString(tr("[%1] The handshakesResponseData slaveId does not match. slaveId: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(m_slaveId)
                              .arg(handshakesResponseContext.slaveId);
                startOffset++;
                continue;
            }

            utils::instance().number(handshakesResponseContext.functionCode, (unsigned char *)responseDataBuffer + startOffset + offsetof(handshakesResponse_t, functionCode));
            if (g_iapHandshakesFunctionCode != handshakesResponseContext.functionCode)
            {
                qWarning() << QString(tr("[%1] The handshakesResponseData functionCode does not match. functionCode: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(g_iapHandshakesFunctionCode)
                              .arg(handshakesResponseContext.functionCode);
                startOffset++;
                continue;
            }

            utils::instance().number(handshakesResponseContext.crc16, (unsigned char *)responseDataBuffer + startOffset + offsetof(handshakesResponse_t, crc16));
            quint16 crc16 = utils::instance().crc16(responseDataBuffer + startOffset, offsetof(handshakesResponse_t, crc16));
            if (crc16 != handshakesResponseContext.crc16)
            {
                qWarning() << QString(tr("[%1] The handshakesResponseData crc16 does not match. crc16: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(QString("%1").arg(crc16, 4, 16, QChar('0')))
                              .arg(QString("%1").arg(handshakesResponseContext.crc16, 4, 16, QChar('0')));
                startOffset++;
                continue;
            }

            utils::instance().number(handshakesResponseContext.result, (unsigned char *)responseDataBuffer + startOffset + offsetof(handshakesResponse_t, result));
            if (g_handshakesResponseSuccessed == handshakesResponseContext.result)
            {
                qDebug() << QString(tr("[%1] The handshakesResponseData ok."))
                            .arg(metaObject()->className());

                // Step2
                emit sig_downloadWrapper();
            }
            else if (g_handshakesResponseFirmwareOversize == handshakesResponseContext.result)
            {
                qWarning() << QString(tr("[%1] The handshakesResponseData fireware oversize."))
                            .arg(metaObject()->className());
            }
            else if (g_handshakesResponseManufacturerMismatch == handshakesResponseContext.result)
            {
                qWarning() << QString(tr("[%1] The handshakesResponseData manufacture mismatch."))
                            .arg(metaObject()->className());
            }
            else if (g_handshakesResponseIncorrectVersionNumber == handshakesResponseContext.result)
            {
                qWarning() << QString(tr("[%1] The handshakesResponseData incorrect version number."))
                            .arg(metaObject()->className());
            }
            else
            {
                qWarning() << QString(tr("[%1] The handshakesResponseData unknown error: %2."))
                            .arg(metaObject()->className())
                            .arg(handshakesResponseContext.result);
            }

            break;
        }
        else
        {
            qWarning() << QString(tr("[%1] The handshakesResponseData Less than a complete package: %2"))
                          .arg(metaObject()->className())
                          .arg(QByteArray(responseDataBuffer + startOffset, responseDataSize - startOffset).toHex(' ').toUpper().data());
            break;
        }
    }
}

void parse::downloadRequest(QByteArray &downloadRequestData, quint32 pkgIdx)
{
    downloadRequestData.clear();
    utils::instance().append(downloadRequestData, m_slaveId);
    utils::instance().append(downloadRequestData, g_iapDownloadFunctionCode);
    utils::instance().append(downloadRequestData, (quint16)pkgIdx);
    if (0 == m_div)
    {
        QByteArray data(m_file.constData(), m_mod);
        downloadRequestData.append(data);
        QByteArray dataFF((128 - m_mod), (unsigned char)0xFF);
        downloadRequestData.append(dataFF);
    }
    else
    {
        if (pkgIdx < m_div)
        {
            QByteArray data(m_file.constData() + (pkgIdx * 128), 128);
            downloadRequestData.append(data);
        }
        else
        {
            QByteArray data(m_file.constData() + (pkgIdx * 128), m_mod);
            downloadRequestData.append(data);
            QByteArray dataFF((128 - m_mod), (unsigned char)0xFF);
            downloadRequestData.append(dataFF);
        }
    }
    utils::instance().append(downloadRequestData, utils::instance().calc_crc16(downloadRequestData));
}


void parse::on_downloadWrapper()
{

    quint32 pkgIdx = 0;
    emit sig_sendDownloadRequestByPackageIndex(pkgIdx);

}
void parse::downloadResultRequest(QByteArray &downloadResultRequestData) const
{
    downloadResultRequestData.clear();
    utils::instance().append(downloadResultRequestData, m_slaveId);
    utils::instance().append(downloadResultRequestData, g_iapDownloadResultFunctionCode);
    utils::instance().append(downloadResultRequestData, utils::instance().calc_crc16(downloadResultRequestData));
}
void parse::on_parseDownloadResponse(const QByteArray downloadResponseData, quint32 pkgIdx)
{
    int responseDataSize = downloadResponseData.size();
    int packageSize = sizeof(downloadResponse_t);
    downloadResponse_t downloadResponseContext;

    pkgIdx++;
    emit sig_sendDownloadRequestByPackageIndex(pkgIdx);
    return;

    int startOffset = 0;
    const char* responseDataBuffer = downloadResponseData.data();

    for (;;)
    {
        if ((responseDataSize - startOffset) >= packageSize)
        {
            utils::instance().number(downloadResponseContext.slaveId, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResponse_t, slaveId));
            if (m_slaveId != downloadResponseContext.slaveId)
            {
                qWarning() << QString(tr("[%1] The downloadResponseData slaveId does not match. slaveId: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(m_slaveId)
                              .arg(downloadResponseContext.slaveId);
                startOffset++;
                continue;
            }

            utils::instance().number(downloadResponseContext.functionCode, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResponse_t, functionCode));
            if (g_iapDownloadFunctionCode != downloadResponseContext.functionCode)
            {
                qWarning() << QString(tr("[%1] The downloadResponseData functionCode does not match. functionCode: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(g_iapDownloadFunctionCode)
                              .arg(downloadResponseContext.functionCode);
                startOffset++;
                continue;
            }

            utils::instance().number(downloadResponseContext.pkgIdx, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResponse_t, pkgIdx));
            if (pkgIdx != downloadResponseContext.pkgIdx)
            {
                qWarning() << QString(tr("[%1] The downloadResponseData pkgIdx does not match. pkgIdx: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(pkgIdx)
                              .arg(downloadResponseContext.pkgIdx);
                startOffset++;
                continue;
            }

            utils::instance().number(downloadResponseContext.crc16, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResponse_t, crc16));
            quint16 crc16 = utils::instance().crc16(responseDataBuffer + startOffset, offsetof(downloadResponse_t, crc16));
            if (crc16 != downloadResponseContext.crc16)
            {
                qWarning() << QString(tr("[%1] The downloadResponseData crc16 does not match. crc16: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(QString("%1").arg(crc16, 4, 16, QChar('0')))
                              .arg(QString("%1").arg(downloadResponseContext.crc16, 4, 16, QChar('0')));
                startOffset++;
                continue;
            }

            utils::instance().number(downloadResponseContext.result, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResponse_t, result));
            if (g_downloadResponseSuccessed == downloadResponseContext.result)
            {
                qDebug() << QString(tr("[%1] The downloadResponseData ok. pkgIdx: %2"))
                            .arg(metaObject()->className())
                            .arg(pkgIdx);

                // Step2 Send the next package
                pkgIdx++;
                emit sig_sendDownloadRequestByPackageIndex(pkgIdx);
                return;
            }
            else if (g_downloadResponseFailed == downloadResponseContext.result)
            {
                qWarning() << QString(tr("[%1] The downloadResponseData error. pkgIdx: %2"))
                            .arg(metaObject()->className())
                            .arg(pkgIdx);
            }
            else
            {
                qWarning() << QString(tr("[%1] The downloadResponseData unknown error: %2. pkgIdx: %3"))
                            .arg(metaObject()->className())
                            .arg(downloadResponseContext.result)
                            .arg(pkgIdx);
            }

            break;
        }
        else
        {
            qWarning() << QString(tr("[%1] The downloadResponseData Less than a complete package: %2"))
                          .arg(metaObject()->className())
                          .arg(QByteArray(responseDataBuffer + startOffset, responseDataSize - startOffset).toHex(' ').toUpper().data());
            break;
        }
    }

    emit sig_downloadResultResponseFailed();
}

void parse::on_parseDownloadResultResponse(const QByteArray downloadResultResponseData)
{
    int responseDataSize = downloadResultResponseData.size();
    int packageSize = sizeof(downloadResultResponse_t);
    downloadResultResponse_t downloadResultResponseContext;

    int startOffset = 0;
    const char* responseDataBuffer = downloadResultResponseData.data();

    for (;;)
    {
        if ((responseDataSize - startOffset) >= packageSize)
        {
            utils::instance().number(downloadResultResponseContext.slaveId, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResultResponse_t, slaveId));
            if (m_slaveId != downloadResultResponseContext.slaveId)
            {
                qWarning() << QString(tr("[%1] The downloadResultResponseData slaveId does not match. slaveId: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(m_slaveId)
                              .arg(downloadResultResponseContext.slaveId);
                startOffset++;
                continue;
            }

            utils::instance().number(downloadResultResponseContext.functionCode, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResultResponse_t, functionCode));
            if (g_iapDownloadResultFunctionCode != downloadResultResponseContext.functionCode)
            {
                qWarning() << QString(tr("[%1] The downloadResultResponseData functionCode does not match. functionCode: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(g_iapDownloadResultFunctionCode)
                              .arg(downloadResultResponseContext.functionCode);
                startOffset++;
                continue;
            }

            utils::instance().number(downloadResultResponseContext.crc16, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResultResponse_t, crc16));
            quint16 crc16 = utils::instance().crc16(responseDataBuffer + startOffset, offsetof(downloadResultResponse_t, crc16));
            if (crc16 != downloadResultResponseContext.crc16)
            {
                qWarning() << QString(tr("[%1] The downloadResultResponseData crc16 does not match. crc16: %2 - %3"))
                              .arg(metaObject()->className())
                              .arg(QString("%1").arg(crc16, 4, 16, QChar('0')))
                              .arg(QString("%1").arg(downloadResultResponseContext.crc16, 4, 16, QChar('0')));
                startOffset++;
                continue;
            }

            utils::instance().number(downloadResultResponseContext.result, (unsigned char *)responseDataBuffer + startOffset + offsetof(downloadResultResponse_t, result));
            if (g_downloadResponseSuccessed == downloadResultResponseContext.result)
            {
                qDebug() << QString(tr("[%1] The downloadResultResponseData ok."))
                            .arg(metaObject()->className());
                end=QDateTime::currentDateTime();
                qint64 secondsDiff = start.secsTo(end);
                emit sig_flog("时间："+QString::number(secondsDiff));
                emit sig_downloadResultResponseSuccessed();
                return;
            }
            else if (g_downloadResponseFailed == downloadResultResponseContext.result)
            {
                qWarning() << QString(tr("[%1] The downloadResultResponseData error."))
                            .arg(metaObject()->className());
            }
            else
            {
                qWarning() << QString(tr("[%1] The downloadResultResponseData unknown error: %2."))
                            .arg(metaObject()->className())
                            .arg(downloadResultResponseContext.result);
            }

            break;
        }
        else
        {
            qWarning() << QString(tr("[%1] The downloadResultResponseData Less than a complete package: %2"))
                          .arg(metaObject()->className())
                          .arg(QByteArray(responseDataBuffer + startOffset, responseDataSize - startOffset).toHex(' ').toUpper().data());
            break;
        }
    }

    emit sig_downloadResultResponseFailed();
}
void parse::handshakesRequest(QByteArray &handshakesRequestData) const
{
    handshakesRequestData.clear();
    utils::instance().append(handshakesRequestData, m_slaveId);
    utils::instance().append(handshakesRequestData, g_iapHandshakesFunctionCode);
    utils::instance().append(handshakesRequestData, m_fileSize);
    utils::instance().append(handshakesRequestData, m_manufacturerCode);
    utils::instance().append(handshakesRequestData, m_upgradeVersionNumber);
    utils::instance().append(handshakesRequestData, m_fileCrc32);
    utils::instance().append(handshakesRequestData, utils::instance().crc16(handshakesRequestData.data(), handshakesRequestData.size()));
}
void parse::set(quint8 slaveId,quint32 m_code,quint32 version){
    m_slaveId=slaveId;
    m_manufacturerCode=m_code;
    m_upgradeVersionNumber=version;
}
