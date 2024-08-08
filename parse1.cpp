#include "parse1.h"

parse1::parse1()
{

}
void parse1::parse1_receive(const QByteArray &receive){
    qDebug()<<"on parsing";
    emit  sig_log_parse1("on parsing "+QString::fromLatin1(receive.toHex(' ')));
    g_receive_data data;
    struct_receive(receive,data);
    QByteArray temp=receive.mid(0,sizeof (g_receive_data)-2);
    quint16 crc=calc_modbus_crc(temp);
    emit sig_log_parse1("crc1: "+QString::number(data.crc)+" crc2: "+QString::number(crc));
    qDebug()<<m_helmet_lock<<" "<<data.addr;
    if(crc==data.crc){
        if(data.addr==m_bms_protection_board){
            if(data.func_code==g_read_switch_func_code){
                read_switch(data.addr,data.start_addr,data.nums);
            }
            else if(data.func_code==g_read_reg_func_code){
                read_reg(data.addr,data.start_addr,data.nums);
            }
        }
        else if(data.addr==m_helmet_lock){
            read_06(data.addr,data.start_addr,data.nums);

        }
        else if(data.addr==m_motor_controller){
            if(data.func_code==g_read_switch_func_code){
                read_switch_05(data.addr,data.start_addr,data.nums);
            }
            else if(data.func_code==g_read_reg_func_code){
                read_reg_05(data.addr,data.start_addr,data.nums);
            }
        }
    }
}

void parse1::read_switch(quint8 addr,quint16 start,quint16 nums){

    quint16 start_idx=0;
    QTableWidgetItem* valueItem;
    for(;start_idx<switch_table->rowCount();start_idx++){
        valueItem = switch_table->item(start_idx, 0);
        if(valueItem->text().toUShort()==start){
            break;
        }}
    QByteArray send;
    QString s;
    quint8 n= (nums+7)/8;
    for(int i=0;i<8*n-nums;i++){
        s+="0";
    }
    for(int i=0;i<nums;i++){
        valueItem = switch_table->item(start_idx+nums-1-i, 2);
        s+=valueItem->text();
        }
    int i=0;
    while (i < s.size())
       {
           quint8 byte = 0;
           // Convert next 8 characters to a byte
           for (int j = 0; j < 8 && (i + j) < s.size(); ++j)
           {
               if (s[i + j] == '1')
                   byte |= (1 << (7 - j));
           }
           send.append(byte);
           i += 8;
       }
    std::reverse(send.begin(),send.end());
    QByteArray temp;
    emit sig_log_parse1("待发送："+send.toHex(' '));

    utils::instance().append(temp,addr);
    utils::instance().append(temp,g_read_switch_func_code);

    utils::instance().append(temp,n);
    temp.append(send);
    quint16 crc= calc_modbus_crc(temp);
    utils::instance().append(temp,crc);
    emit sig_log_parse1("数据包:"+temp.toHex(' '));
    sig_send_switch(temp);
}
void parse1::read_06(quint8 addr,quint16 start,quint16 nums){
    qDebug()<<"enter read 06";
    quint16 start_idx=0;
    QTableWidgetItem* valueItem;
    quint16 bytes;
    for(;start_idx<table_06->rowCount();start_idx++){

        valueItem = table_06->item(start_idx, 0);
        if(valueItem->text().toUShort()==start){
            break;
        }}
    quint16 temp_idx=start_idx+1;
    QByteArray send;
    quint32 send32;
    quint16 send16;
    QByteArray temp_array;
    for(int i=0;nums;i++){
        if(temp_idx==28){
            bytes=1;
        }
        else{
        bytes=table_06->item(temp_idx,0)->text().toUShort()-table_06->item(start_idx,0)->text().toUShort();
        }
        nums-=bytes;
        if(bytes==1){
            send16=table_06->item(start_idx,2)->text().toUShort();
            utils::instance().append(send,send16);
        }
        else if(bytes==2){
            send32=table_06->item(start_idx,2)->text().toUInt();
            utils::instance().append(send,send32);
        }
        else if(bytes==8){
            readAndAdjustItems_06(start_idx,1,bytes*2,temp_array);
            send.append(temp_array);
        }


        start_idx++;
        temp_idx++;
    }
    emit sig_log_parse1("待发送："+send.toHex(' '));
    temp_array.clear();
    quint8 n=nums*2;
    utils::instance().append(temp_array,addr);
    utils::instance().append(temp_array,g_read_reg_func_code);
    utils::instance().append(temp_array,n);
    temp_array.append(send);
    quint16 crc= calc_modbus_crc(temp_array);
    utils::instance().append(temp_array,crc);
    emit sig_log_parse1("数据包:"+temp_array.toHex(' '));
    sig_send_reg(temp_array);


}
void parse1::read_switch_05(quint8 addr,quint16 start,quint16 nums){
    quint16 start_idx=0;
    QTableWidgetItem* valueItem;
    for(;start_idx<stable_05->rowCount();start_idx++){
        valueItem = stable_05->item(start_idx, 0);
        if(valueItem->text().toUShort()==start){
            break;
        }}
    QByteArray send;
    QString s;
    quint8 n= (nums+7)/8;
    for(int i=0;i<8*n-nums;i++){
        s+="0";
    }
    for(int i=0;i<nums;i++){
        valueItem = stable_05->item(start_idx+nums-1-i, 2);
        s+=valueItem->text();
        }
    int i=0;
    while (i < s.size())
       {
           quint8 byte = 0;
           // Convert next 8 characters to a byte
           for (int j = 0; j < 8 && (i + j) < s.size(); ++j)
           {
               if (s[i + j] == '1')
                   byte |= (1 << (7 - j));
           }
           send.append(byte);
           i += 8;
       }
    std::reverse(send.begin(),send.end());
    QByteArray temp;
    emit sig_log_parse1("待发送："+send.toHex(' '));

    utils::instance().append(temp,addr);
    utils::instance().append(temp,g_read_switch_func_code);

    utils::instance().append(temp,n);
    temp.append(send);
    quint16 crc= calc_modbus_crc(temp);
    utils::instance().append(temp,crc);
    emit sig_log_parse1("数据包:"+temp.toHex(' '));
    sig_send_switch(temp);
}
void parse1::read_reg_05(quint8 addr,quint16 start,quint16 nums){
    quint16 start_idx=0;
    QTableWidgetItem* valueItem;
    quint16 temp=0;
    quint8 n=nums*2;
    QByteArray temp_byte_array;
    for(;start_idx<rtable_05->rowCount();start_idx++){
        valueItem = rtable_05->item(start_idx, 0);
        if(valueItem->text().toUShort()==start){
            break;
        }}
    qDebug()<<"read reg "<<start_idx;
    QByteArray send;
    for(int i=0;i<nums;i++){
        valueItem = rtable_05->item(start_idx+i, 2);
        temp=valueItem->text().toShort(nullptr,10);
        utils::instance().append(send,temp);
    }
    emit sig_log_parse1("待发送："+send.toHex(' '));
    QByteArray temp_array;

    utils::instance().append(temp_array,addr);
    utils::instance().append(temp_array,g_read_reg_func_code);
    utils::instance().append(temp_array,n);
    temp_array.append(send);
    quint16 crc= calc_modbus_crc(temp_array);
    utils::instance().append(temp_array,crc);
    emit sig_log_parse1("数据包:"+temp_array.toHex(' '));
    sig_send_reg(temp_array);
}
void parse1::read_reg(quint8 addr,quint16 start,quint16 nums){
    quint16 start_idx=0;
    QTableWidgetItem* valueItem;
    quint16 temp=0;
    quint8 n=nums*2;
    QByteArray temp1;
    QByteArray temp_byte_array;
    for(;start_idx<reg_table->rowCount();start_idx++){
        valueItem = reg_table->item(start_idx, 0);
        if(valueItem->text().toUShort()==start){
            break;
        }}
    qDebug()<<"read reg "<<start_idx;
    QByteArray send;
    if(start_idx>=49){
        if(start_idx==49){
            if(nums==8){
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
            }
            else if(nums==16){
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);

            }
            else if(nums==18){
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
            }
            else if(nums==20){
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
            }
        }
        else if(start_idx==50){
            if(nums==8){
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
            }
            else if(nums==10){
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
            }
            else if(nums==12){
                readAndAdjustItems(start_idx++,1,16,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
            }
        }
        else if(start_idx==51){
            if(nums==2){
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
            }
            else if(nums==4){
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);

            }}
        else if(start_idx==52){
            if(nums==2){
                readAndAdjustItems_hex(start_idx++,1,4,temp_byte_array);
                temp1.append(temp_byte_array);
            }

        }
        send.append(temp1);
    }

    else{
        for(int i=0;i<nums;i++){
            valueItem = reg_table->item(start_idx+i, 2);
            temp=valueItem->text().toShort(nullptr,10);
            utils::instance().append(send,temp);
        }}
    emit sig_log_parse1("待发送："+send.toHex(' '));
    QByteArray temp_array;

    utils::instance().append(temp_array,addr);
    utils::instance().append(temp_array,g_read_reg_func_code);
    utils::instance().append(temp_array,n);
    temp_array.append(send);
    quint16 crc= calc_modbus_crc(temp_array);
    utils::instance().append(temp_array,crc);
    emit sig_log_parse1("数据包:"+temp_array.toHex(' '));
    sig_send_reg(temp_array);
}




void parse1::struct_receive(const QByteArray &receive,g_receive_data_t &data){
    QByteArray ncreceive=receive;
    QDataStream ds(&ncreceive,QIODevice::ReadOnly);
    ds>>data.addr>>data.func_code>>data.start_addr>>data.nums>>data.crc;
}


uint16_t parse1::calc_modbus_crc(const QByteArray &data)
{
    unsigned short c = 0xffff;

    for (int k = 0; k < data.size(); k++)
    {
        unsigned short b = data[k];

        for (char i = 0; i < 8; i++)
        {
            c = ((b^c)&1) ? (c>>1)^0xA001 : (c>>1);
            b >>= 1;
        }
    }

    return (c<<8)|(c>>8);
}
void parse1::readAndAdjustItems(int start_idx, int item_count, int desiredSize,QByteArray &temp_byte_array) {

    temp_byte_array.clear();
    for (int i = 0; i < item_count; ++i) {
        QTableWidgetItem *valueItem = reg_table->item(start_idx + i, 2);
        temp_byte_array += valueItem->text().toLatin1();

    }

    adjustByteArraySize(temp_byte_array, desiredSize);

}
void parse1::readAndAdjustItems_hex(int start_idx, int item_count, int desiredSize,QByteArray &temp_byte_array) {

    temp_byte_array.clear();
    for (int i = 0; i < item_count; ++i) {
        QTableWidgetItem *valueItem = reg_table->item(start_idx + i, 2);
        temp_byte_array.append(QByteArray::fromHex(valueItem->text().toUtf8()));

    }

    adjustByteArraySize(temp_byte_array, desiredSize);

}
void parse1::readAndAdjustItems_06(int start_idx, int item_count, int desiredSize,QByteArray &temp_byte_array) {

    temp_byte_array.clear();
    for (int i = 0; i < item_count; ++i) {
        QTableWidgetItem *valueItem = table_06->item(start_idx + i, 2);
        temp_byte_array += valueItem->text().toLatin1();

    }

    adjustByteArraySize(temp_byte_array, desiredSize);

}
void parse1::adjustByteArraySize(QByteArray &byteArray, int desiredSize) {
    if (byteArray.size() > desiredSize) {
        byteArray.truncate(desiredSize);
    } else if (byteArray.size() < desiredSize) {
        byteArray.append(QByteArray(desiredSize - byteArray.size(), '\0'));
    }
}


void parse1::start(){
    emit sig_get_table_parse1();
}
void parse1::set(QStringList sl){
    m_bms_protection_board=sl[0].toUShort();
    m_instrument=sl[1].toUShort();
    m_intelligent_central_control=sl[2].toUShort();
    m_motor_controller=sl[3].toUShort();
    m_helmet_lock=sl[4].toUShort();
    m_rear_wheel_lock_hub_lock=sl[5].toUShort();
    qDebug()<<m_helmet_lock;
}
