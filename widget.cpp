#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initUI();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initUI()
{
    initSingleMsgWidget();
    initMulitMsgWidget();

    //
    connect(ui->pushButton_close,&QPushButton::clicked,this,&Widget::slotToClose);
}

void Widget::initSingleMsgWidget()
{
    ui->lineEdit_fmi_in;
    ui->lineEdit_spn_in;

    connect(ui->lineEdit_fmi_in,&QLineEdit::textChanged,this,&Widget::slotFmiInChanged);
    connect(ui->lineEdit_spn_in,&QLineEdit::textChanged,this,&Widget::slotSpnInChanged);
}

void Widget::initMulitMsgWidget()
{
    connect(ui->lineEdit_in_can_msg,&QLineEdit::textChanged,this,&Widget::slotCanMsgInChanged);
}

QString Widget::toCreateCanMsg(unsigned int spn,unsigned int fmi)
{
    ui->label_to_can_info->setText("");
    QString t_errOut = "00 00 00 00 00 00 00 00";
    if(fmi >31){ //2^5-1

        ui->label_to_can_info->setText(tr("fmi is bigger"));
        return t_errOut;
    }
    if(spn > 524287){ //2^19 -1
        ui->label_to_can_info->setText(tr("spn is bigger"));
        return t_errOut;
    }
    QString byte_0 = "00";
    QString byte_1 = "00";
    QString byte_2 = "00";
    QString byte_3 = "00";
    QString byte_4 = "00";
    QString byte_5 = "00";
    QString byte_6 = "00";
    QString byte_7 = "00";


    int iByte2Value = spn & 0xFF;
    int iByte3Value = (spn>>8) & 0xFF;
    int iByte4Value_3 = (spn>>16) & 0x7;
    int iByte4Value_5 = fmi & 0x1F;
    int iByte4Value = (iByte4Value_3 << 5) + iByte4Value_5;

    byte_2 = QString("%1").arg(iByte2Value,2,16,QLatin1Char('0'));
    byte_3 = QString("%1").arg(iByte3Value,2,16,QLatin1Char('0'));
    byte_4 = QString("%1").arg(iByte4Value,2,16,QLatin1Char('0'));

    ui->label_to_can_info->setText(tr("success"));
    return byte_0+" "+byte_1+" "+byte_2+" "+byte_3+" "
           +byte_4+" "+byte_5+" "+byte_6+" "+byte_7+" ";
}

void Widget::slotFmiInChanged()
{
    QString t_strFmi = ui->lineEdit_fmi_in->text();
    bool t_bOk = false;
    uint t_iFmi = t_strFmi.toUInt(&t_bOk);
    if(!t_bOk){
        ui->label_to_can_info->setText(tr("fmi can not to int"));
        ui->lineEdit_out_can_msg->setText("00");
        return;
    }
    QString t_strSpn = ui->lineEdit_spn_in->text();
    bool t_bOk2 = false;
    uint t_iSpn = t_strSpn.toUInt(&t_bOk2);
    if(!t_bOk2){
        ui->label_to_can_info->setText(tr("spn can not to int"));
        return;
    }


    QString t_out = toCreateCanMsg(t_iSpn,t_iFmi);
    ui->lineEdit_out_can_msg->setText(t_out);
}

void Widget::slotSpnInChanged()
{
    QString t_strSpn = ui->lineEdit_spn_in->text();
    bool t_bOk = false;
    uint t_iSpn = t_strSpn.toUInt(&t_bOk);
    if(!t_bOk){
        ui->label_to_can_info->setText(tr("spn can not to int"));
        ui->lineEdit_out_can_msg->setText("00");
        return;
    }

    QString t_strFmi = ui->lineEdit_fmi_in->text();
    bool t_bOk2 = false;
    uint t_iFmi = t_strFmi.toUInt(&t_bOk2);
    if(!t_bOk2){
        ui->label_to_can_info->setText(tr("fmi can not to int"));
        return;
    }

    QString t_out = toCreateCanMsg(t_iSpn,t_iFmi);
    ui->lineEdit_out_can_msg->setText(t_out);
}

void Widget::slotCanMsgInChanged()
{
    QString t_inStr = ui->lineEdit_in_can_msg->text().trimmed();
    QStringList strList = t_inStr.split(" ");
    if(strList.size() != 8){
        ui->label_to_spn_fmi_info->setText(tr("input can msg is error"));
        ui->lineEdit_fmi_out->setText("");
        ui->lineEdit_spn_out->setText("");
        return;
    }

    QString byte_0 = strList.at(0);
    QString byte_1 = strList.at(1);
    QString byte_2 = strList.at(2);
    QString byte_3 = strList.at(3);
    QString byte_4 = strList.at(4);
    QString byte_5 = strList.at(5);
    QString byte_6 = strList.at(6);
    QString byte_7 = strList.at(7);
    bool t_bOk = false;
    int lowSPN = byte_2.toInt(&t_bOk,16);
    int midSPN = byte_3.toInt(&t_bOk,16);
    int byte4Value = byte_4.toInt(&t_bOk,16);
    int highSPN = byte4Value >> 5;
    int iSPN = (highSPN << 16) + (midSPN<< 8) + lowSPN;
    int iFmi = byte4Value & 0x1F;
    ui->lineEdit_spn_out->setText(QString("%1").arg(iSPN));
    ui->lineEdit_fmi_out->setText(QString("%1").arg(iFmi));


}

void Widget::slotToClose()
{
    this->close();
}
