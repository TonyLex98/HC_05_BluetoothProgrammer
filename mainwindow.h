#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <QSerialPort>
#include <QFileDialog>
#include <iostream>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QByteArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    bool ok=true;

    //void Plot(std::vector<std::vector<double>> mass, std::vector<QString> VName);

    int N_points_on_period;

    QPen GetColor(int index);

    std::vector<QPen> Color;

    QStringList ScanPorts();
    
    QByteArray qba;

    QString path = "";

private slots:

    void on_pushButton_35_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_comboBox_2_activated(const QString &arg1);

    void SerPorIni();

    void PutData();

    void on_update_button_clicked();

    void Send_Data_DDS();

    void on_pushButton_Req_clicked();

    void on_pushButton_Set_clicked();

signals:
    void signalSend_Data_DDS();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;

    QString last_str;
    bool req_ok = 0;
    int req_num = 0;

    bool set_ok = 0;
    int set_num = 0;
};
void on_Calcul_pushButton_clicked();



#endif // MAINWINDOW_H

