#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>


QSerialPort port;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    serialPort = new QSerialPort(this);
    ui->comboBox->addItems(ScanPorts());
    connect(this,&MainWindow::signalSend_Data_DDS,this,&MainWindow::Send_Data_DDS);

}

MainWindow::~MainWindow()
{
    delete ui;

}

QStringList MainWindow::ScanPorts(){

    QStringList qsl;
    //Определяем возможные порты
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts()){
         qsl.append(port.portName());
    }

    return qsl;

}

//--------------------------------------------------------------------------
void MainWindow::Send_Data_DDS()
{
    //DWORD	bread;
    serialPort->write(qba+'\r'+'\n');
    ui->textBrowser->append(qba);
    //WriteFile(hSerial, qba, 9, &bread, NULL);

}



void MainWindow::on_pushButton_clicked()
{

    QString send_str;
    send_str=ui->lineEdit_2->text();
    qba.clear();
    qba.append(send_str.toUtf8());
    emit Send_Data_DDS();

}

void MainWindow:: on_pushButton_35_clicked(){
    SerPorIni();
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    on_pushButton_35_clicked();
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
    on_pushButton_35_clicked();
}

void MainWindow::SerPorIni()
{
    serialPort->close();
    serialPort->setPortName(ui->comboBox->currentText());
    serialPort->setBaudRate(ui->comboBox_2->currentText().toInt());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->open(QIODevice::ReadWrite);
    connect(serialPort,&QSerialPort::readyRead,this,&MainWindow::PutData);
    QString str;
    if(serialPort->isReadable()){
        str = " is readable and ";
    }
    else{str = " is not readable and ";}

    if(serialPort->isWritable()){
        str.append("writable");
    }
    else{str.append("not writable");}

    ui->lineEdit_3->setText(str);
    if(str!=" is readable and writable"){
        ui->lineEdit_3->setStyleSheet("background-color:red;");
    }
    else{
        ui->lineEdit_3->setStyleSheet("background-color:green;");
    }

}

void MainWindow::PutData(){


       last_str = serialPort->readAll();

       ui->textBrowser->append(last_str);

       //Обработка запроса параметров
       if(req_ok == 1){


           if(last_str.split('\n').at(0)=="OK\r"){
               req_num=1;
               qDebug() << "req_num=1";
               on_pushButton_Req_clicked();
           }
           else if(last_str.split('\n').at(0).split(":").at(0)=="+NAME"){
               ui->lineEdit_Name->setText(last_str.split('\n').at(0).split(":").at(1).split('\r').at(0));
               req_num=2;
               qDebug() << "req_num=2";
               on_pushButton_Req_clicked();
           }
           else if(last_str.split('\n').at(0).split(":").at(0)=="+PIN"){
               ui->lineEdit_Password->setText(last_str.split('\n').at(0).split(":").at(1).split("\"").at(1));
               req_num=3;
               qDebug() << "req_num=3";
               on_pushButton_Req_clicked();
           }
           else if(last_str.split('\n').at(0).split(":").at(0)=="+UART"){
               ui->lineEdit_Speed->setText(last_str.split('\n').at(0).split(":").at(1).split(",").at(0));
               ui->lineEdit_StopBit->setText(last_str.split('\n').at(0).split(":").at(1).split(",").at(1));
               ui->lineEdit_Parity->setText(last_str.split('\n').at(0).split(":").at(1).split(",").at(2));

               req_num=4;
               qDebug() << "req_num=4";
               on_pushButton_Req_clicked();
           }
           else if(last_str.split('\n').at(0).split(":").at(0)=="VERSION"){
               ui->lineEdit_ProgrammVers->setText(last_str.split('\n').at(0).split(":").at(1));

               req_num=5;
               qDebug() << "req_num=4";
               on_pushButton_Req_clicked();
           }
            if(last_str.split(':').at(0)=="ERROR") {
               QMessageBox::warning(this,"ОШИБКА ЗАПРОСА ПАРАМЕТРОВ!","Проверьте, переведен ли модуль в режим программирования!");
           }
       }
       if(set_ok==1){
           //Обработка установки параметров
           if(last_str.split('\n').at(0)=="OK\r"){
               set_num++;
               on_pushButton_Set_clicked();
           }
           if(last_str.split(':').at(0)=="ERROR") {
               QMessageBox::warning(this,"ОШИБКА УСТАНОВКИ ПАРАМЕТРОВ!","Проверьте, переведен ли модуль в режим программирования и правильно ли записаны параметры!");
           }

       }

}


void MainWindow::on_pushButton_3_clicked()
{
    close();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser->clear();
}

//Обновляем список последовательных портов
void MainWindow::on_update_button_clicked()
{

    ui->comboBox->clear();
    ui->comboBox->addItems(ScanPorts());

}


void MainWindow::on_pushButton_Req_clicked()
{
    if(req_num==0){

        req_ok=1;
        QString send_str;
        send_str="AT";
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(req_num==1){

        QString send_str;
        send_str="AT+NAME?";
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(req_num==2){

        QString send_str;
        send_str="AT+PSWD?";
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(req_num==3){

        QString send_str;
        send_str="AT+UART?";
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(req_num==4){

        QString send_str;
        send_str="AT+VERSION?";
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(req_num==5){

        req_ok = 0;
        req_num = 0;
        QMessageBox::warning(this,"ВНИМАНИЕ!","Параметры считаны!");

    }


}

void MainWindow::on_pushButton_Set_clicked()
{
    if(set_num==0){

        set_ok=1;
        QString send_str;
        send_str="AT+NAME="+ui->lineEdit_Name->text();
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(set_num==1){

        QString send_str;
        send_str="AT+PSWD=\""+ui->lineEdit_Password->text()+"\"";
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(set_num==2){

        QString send_str;
        send_str="AT+UART="+ui->lineEdit_Speed->text()+","+ui->lineEdit_StopBit->text()+","+ui->lineEdit_Parity->text();
        qba.clear();
        qba.append(send_str.toUtf8());
        emit Send_Data_DDS();

    }
    if(set_num==3){

        set_ok = 0;
        set_num = 0;
        QMessageBox::warning(this,"ВНИМАНИЕ!","Параметры установлены!");
    }
}
