#include "serial_port.h"

serial_port::serial_port()
{

}
void serial_port::satrt(){
    this->m_pQserialport=new QSerialPort();
    this->m_pPort_scan_timer=new QTimer();
    this->mp_send_timer=new QTimer();
    QObject::connect(this->m_pPort_scan_timer,&QTimer::timeout,this,&serial_port::scan_port);
    connect(mp_send_timer,&QTimer::timeout,this,&serial_port::send_next);

    m_connection=QObject::connect(m_pQserialport, &QSerialPort::readyRead, this, [this](){

        if(!addrs.empty()){
            quint16 addr=addrs.front();
            addrs.pop();
            QByteArray rec=this->m_pQserialport->readAll();
//            while (m_pQserialport->waitForReadyRead(200))
//            {
//                rec += m_pQserialport->readAll();
//            }
            emit sig_update_table(rec,addr);
            emit sig_receive_log(rec.toHex(' '));
            return;
        }
        QByteArray rec=this->m_pQserialport->readAll();
        emit sig_parse_rec(rec);
        emit sig_receive_log(rec.toHex(' '));
    });
    m_pPort_scan_timer->start(1000);
}
void serial_port::scan_port(){
    //qDebug()<<"the scan port is running";
    QVector<QString> temp={};
    for(const QSerialPortInfo &port :QSerialPortInfo::availablePorts()){
       //qDebug()<<port.portName();

       temp.push_back(port.portName());
    }
    qSort(temp.begin(),temp.end());
    if(temp!=port_names){
        this->port_names=temp;

    }
    emit sig_set_com_names(temp);

}
void serial_port::open_serial_port(QString port_name,int baudrate,int parity,int stop,int databits){
    m_pQserialport->setBaudRate(baudrate);
    m_pQserialport->setParity(static_cast<QSerialPort::Parity>(parity));
    m_pQserialport->setStopBits(static_cast<QSerialPort::StopBits>(stop));
    m_pQserialport->setDataBits(static_cast<QSerialPort::DataBits>(databits));
    m_pQserialport->setFlowControl(QSerialPort::NoFlowControl);
    m_pQserialport->setPortName(port_name);
    if (m_pQserialport->open(QIODevice::ReadWrite))
    {
        // 获取串口参数
        qint32 baudRate = m_pQserialport->baudRate();
        QSerialPort::Parity paritySetting = m_pQserialport->parity();
        QSerialPort::StopBits stopBits = m_pQserialport->stopBits();
        QSerialPort::DataBits dataBits = m_pQserialport->dataBits();

        // 打印调试信息
        qDebug() << "The port opened successfully"
                 << baudRate << paritySetting << stopBits << dataBits;

        // 生成日志字符串
        QString logStr = QString("Port opened: BaudRate=%1, Parity=%2, StopBits=%3, DataBits=%4")
                .arg(baudRate)
                .arg(paritySetting)
                .arg(stopBits)
                .arg(dataBits);

        // 发射信号
        emit sig_log(logStr);
    }
    else
    {
        qWarning() << "Failed to open port" << port_name;
        emit sig_log("Failed to open port: " + port_name);
    }
}
bool serial_port::is_open(){
    return m_pQserialport->isOpen();
}
void serial_port::close_serial_port(){
    m_pQserialport->close();
    qDebug()<<"the port is turned off";
    emit sig_log("port closed");
}
void serial_port::send_data(const QByteArray &data_to_send){
    emit sig_send_log(data_to_send.toHex(' '));
    bool result = false;
    int numberOfWriteBytes = 0;
    int totalRequestDataSize = data_to_send.size();

    qDebug() << "requestData:" << data_to_send.toHex(' ').toUpper();

    for (int retryIndex = 0; retryIndex <= 0; ++retryIndex)
    {
        // Request
        numberOfWriteBytes = m_pQserialport->write(data_to_send);
        if (numberOfWriteBytes != totalRequestDataSize)
        {
            qDebug() << "requestData:" << data_to_send.left(numberOfWriteBytes).toHex(' ').toUpper();
            break;
        }
        result = true;
        break;

    }

}
void serial_port::set(int retry,int timeout){
    m_numberOfRetries=retry;
    m_responseTimeout=timeout;

    qDebug()<<"serial set done";
}
void serial_port::send_receive_data(const QByteArray &data_to_send){
    emit sig_send_log(data_to_send.toHex(' '));
    int numberOfWriteBytes = 0;
    int totalRequestDataSize = data_to_send.size();
    QByteArray data_received;
    qDebug() << "requestData:" << data_to_send.toHex(' ').toUpper();

    for (int retryIndex = 0; retryIndex <= m_numberOfRetries; ++retryIndex)
    {
        // Request
        numberOfWriteBytes = m_pQserialport->write(data_to_send);
        if (numberOfWriteBytes != totalRequestDataSize)
        {
            qDebug() << "requestData:" << data_to_send.left(numberOfWriteBytes).toHex(' ').toUpper();
        }
    }
}
void serial_port::cycle_send(const QList<QByteArray> &send,const QList<int> &time){
    m_datalist=send;
    m_delay_list=time;
    m_index=0;
    emit sig_send_next();
}
void serial_port::send_next(){
    if(m_index>=m_datalist.size()){
        m_index=0;
    }
    if (m_index < m_datalist.size()) {
            // 发送数据
            QByteArray data = m_datalist[m_index];
            m_pQserialport->write(data);
            qDebug() << "Sent:" << data;

            // 设置下一个延时
            if (m_index < m_delay_list.size()) {
                mp_send_timer->start(m_delay_list[m_index]);
                emit sig_send_log(data.toHex(' '));
            }

            m_index++;

        }
}
void serial_port::stop_send(){
    m_datalist.clear();
    m_delay_list.clear();
    this->mp_send_timer->stop();
}
void serial_port::file_settings(int retry,int timeout,int interval){
    m_responseTimeout=timeout;
    m_responseInterval=interval;
    m_numberOfRetries=retry;
}
void serial_port::on_sendDownloadRequest(const QByteArray downloadRequestData, quint32 pkgIdx)
{
    if (!m_pQserialport->isOpen())
    {
        qWarning() << QString(tr("[%1] The serial port is not opened."))
                   .arg(metaObject()->className());
        return;
    }

    QByteArray downloadResponseData;
    if (!sendRequest(downloadRequestData, downloadResponseData))
    {
        emit sig_downloadResultResponseFailed();
        return;
    }

    emit sig_parseDownloadResponse(downloadResponseData, pkgIdx);
}

void serial_port::on_sendDownloadResultRequest(const QByteArray downloadResultRequestData)
{
    if (!m_pQserialport->isOpen())
    {
        qWarning() << QString(tr("[%1] The serial port is not opened."))
                   .arg(metaObject()->className());
        return;
    }

    QByteArray downloadResultResponseData;
    if (!sendRequest(downloadResultRequestData, downloadResultResponseData))
    {
        emit sig_downloadResultResponseFailed();
        return;
    }

    emit sig_parseDownloadResultResponse(downloadResultResponseData);
}


bool serial_port::sendRequest(const QByteArray &requestData, QByteArray& responseData)
{
    qDebug() <<"enter send request";
    bool result = false;
    quint32 singleStep = 500; // ms
    quint32 responseTimeout = m_responseTimeout;
    quint32 numberOfWriteBytes = 0;
    quint32 totalRequestDataSize = requestData.size();

    qDebug() << "requestData:" << requestData.toHex(' ').toUpper();

    for (quint32 retryIndex = 0; retryIndex <= m_numberOfRetries; ++retryIndex)
    {
        // Request
        QByteArray send=utils::instance().encode(requestData);
        emit sig_flog("发送"+send.toHex(' '));
        totalRequestDataSize=send.size();
        numberOfWriteBytes = m_pQserialport->write(send);
        if (numberOfWriteBytes != totalRequestDataSize)
        {
            qDebug() << "requestData:" << requestData.left(numberOfWriteBytes).toHex(' ').toUpper();
        }

        // waitForReadyRead
        if (!m_pQserialport->waitForReadyRead(responseTimeout))
        {
            if (QSerialPort::TimeoutError == m_pQserialport->error())
            {
                qWarning() << QString(tr("Response timeout: %1 ms. retryIndex: %2"))
                              .arg(responseTimeout)
                              .arg(retryIndex);
                responseTimeout += singleStep; // 1s, 1.5s, 2s, 2.5s, 3s, 3.5s
            }
            else
            {
                qWarning() << QString(tr("Response error: %1. retryIndex: %2"))
                              .arg(m_pQserialport->errorString())
                              .arg(retryIndex);
            }

            continue;
        }

        // Response

        responseData = m_pQserialport->readAll();
        while (m_pQserialport->waitForReadyRead(m_responseInterval))
        {
            responseData += m_pQserialport->readAll();
        }

        qDebug() << "responseData:" << responseData.toHex(' ').toUpper();
        emit sig_flog("接收"+responseData.toHex(' '));
        responseData=utils::instance().decode(responseData);
        result = true;
        break;
    }

    return result;
}
void serial_port::send_modbus(QByteArray data_to_send,quint16 start){
    emit sig_send_log(data_to_send.toHex(' '));
    bool result = false;
    int numberOfWriteBytes = 0;
    int totalRequestDataSize = data_to_send.size();

    qDebug() << "requestData:" << data_to_send.toHex(' ').toUpper();

    for (int retryIndex = 0; retryIndex <= 0; ++retryIndex)
    {
        // Request
        numberOfWriteBytes = m_pQserialport->write(data_to_send);
        addrs.push(start);
        if (numberOfWriteBytes != totalRequestDataSize)
        {
            qDebug() << "requestData:" << data_to_send.left(numberOfWriteBytes).toHex(' ').toUpper();
            break;
        }
        result = true;
        break;

    }


}
void serial_port::clearReadBuffer() const
{
    m_pQserialport->readAll();
    while (m_pQserialport->waitForReadyRead(m_responseInterval))
    {
        m_pQserialport->readAll();
    }
}
void serial_port::on_sendHandshakesRequest(const QByteArray handshakesRequestData)
{
    qDebug()<<"send handshake request";
    if (!m_pQserialport->isOpen())
    {
        qWarning() << QString(tr("[%1] The serial port is not opened."))
                   .arg(metaObject()->className());
        return;
    }
    qDebug()<<"sending handshake request";

    clearReadBuffer();

    QByteArray handshakesResponseData;
    sendRequest(handshakesRequestData, handshakesResponseData);
//    if (!sendRequest(handshakesRequestData, handshakesResponseData))
//    {
//        emit sig_downloadResultResponseFailed();
//        return;
//    }
    qDebug()<<"sending done";
    emit sig_parseHandshakesResponse(handshakesResponseData);
}
void serial_port::reconnect(){
    m_connection=QObject::connect(m_pQserialport, &QSerialPort::readyRead, this, [this](){

        if(!addrs.empty()){
            quint16 addr=addrs.front();
            addrs.pop();
            QByteArray rec=this->m_pQserialport->readAll();
//            while (m_pQserialport->waitForReadyRead(200))
//            {
//                rec += m_pQserialport->readAll();
//            }
            emit sig_update_table(rec,addr);
            emit sig_receive_log(rec.toHex(' '));

            return;
        }
        QByteArray rec=this->m_pQserialport->readAll();
        emit sig_parse_rec(rec);
        emit sig_receive_log(rec.toHex(' '));
    });
}

void serial_port::disconnect(){
    QObject::disconnect(m_connection);
}
