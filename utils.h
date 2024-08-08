#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QDebug>
static const quint8 modbus_type=0x01;
static const quint8 g_iapHandshakesFunctionCode = 0xF0;
static const quint8 g_iapDownloadFunctionCode = 0xF1;
static const quint8 g_iapDownloadResultFunctionCode = 0xF2;
static const quint16 g_downloadResponseSuccessed = 0;
static const quint16 g_downloadResponseFailed = 1;
static const quint16 g_handshakesResponseSuccessed = 0;
static const quint16 g_handshakesResponseFirmwareOversize = 1;
static const quint16 g_handshakesResponseManufacturerMismatch = 2;
static const quint16 g_handshakesResponseIncorrectVersionNumber = 3;
#pragma pack(push, 1)
typedef struct handshakesResponse
{
    quint8 slaveId;
    quint8 functionCode;
    quint16 result; // 0 - Successed; 1 - Firmware oversize; 2 - manufacturer mismatch; 3 - Incorrect version number
    quint16 crc16;
} handshakesResponse_t;

typedef struct downloadResponse
{
    quint8 slaveId;
    quint8 functionCode;
    quint16 pkgIdx;
    quint16 result; // 0 - Successed; 1 - Failed
    quint16 crc16;
} downloadResponse_t;

typedef struct downloadResultResponse
{
    quint8 slaveId;
    quint8 functionCode;
    quint16 result; // 0 - Successed; 1 - Failed
    quint16 crc16;
} downloadResultResponse_t;

#pragma pack(pop)


#pragma pack(push, 1)
static const quint8 g_read_switch_func_code=0x01;
static const quint8 g_read_reg_func_code=0x03;
typedef struct g_receive_data {
    quint8 addr;
    quint8 func_code;
    quint16 start_addr;
    quint16 nums;
    quint16 crc;
} g_receive_data_t;

#pragma pack(pop)
// BMS保护板
static const quint8 g_bms_protection_board = 0x01;

// 仪表
static const quint8 g_instrument = 0x02;

// 测试预留
static const quint8 g_test_reservation_03 = 0x03;
static const quint8 g_test_reservation_08 = 0x08;
static const quint8 g_test_reservation_09 = 0x09;

// 智能中控
static const quint8 g_intelligent_central_control = 0x04;

// 电机控制器
static const quint8 g_motor_controller = 0x05;

// 头盔锁
static const quint8 g_helmet_lock = 0x06;

// 后轮锁（轮毂锁）
static const quint8 g_rear_wheel_lock_hub_lock = 0x07;


class utils:public QObject
{
    Q_OBJECT
public:
    utils();
    static utils& instance();
    void append(QByteArray& data, quint8 value) const;
    void append(QByteArray& data, quint16 value) const;
    void append(QByteArray& data, quint32 value) const;
    void number(quint8 &value, const QByteArray &data, int offset) const;
    void number(quint16 &value, const QByteArray &data, int offset) const;
    void number(quint32 &value, const QByteArray &data, int offset) const;
    void number(quint8& value, const unsigned char* data) const;
    void number(quint16& value, const unsigned char* data) const;
    void number(quint32& value, const unsigned char* data) const;
    void Hexstring_to_bytes(QString &s,QByteArray &send);
    unsigned short crc16(const void *s, int n) const;
    quint16 calc_crc16(const QByteArray &array);
    void encode(QByteArray &send);
    QByteArray decode(QByteArray &send);
    quint32 crc32(const QByteArray &data) const;
    QByteArray encode(const QByteArray &send);

};

#endif // UTILS_H
