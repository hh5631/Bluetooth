#ifndef WIDGET_H
#define WIDGET_H

#include "Myinclude.h"
#include "qglobal.h"
#include "slidebutton.h"
#include "Loading.h"
#include "CustomMessageBox/CustomMessageBox.h"
#include "PasswordInputDlg/PasswordInputDlg.h"
#include "BluetoothWorker/BluetoothWorker.h"
#include <QWidget>
#include <QProcess>
#include <QStringList>
#include <QSet>
#include <QList>
#include <QThread>
#include <QObject>
#include <QToolButton>
#include <QMessageBox>
#include <QSize>
// 添加蓝牙相关头文件
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>
#include <QBluetoothAddress>

#include "SpdlogManage/SpdlogManage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_connectBtn_clicked();
    void slot_onSlideBtn(bool isChecked);
    void on_reloadBtn_clicked();
    void setUi_CSL_Info(bool isSuccess);
    
    // BluetoothWorker信号处理槽
    void onBluetoothInitialized(bool success);
    void onBluetoothEnabled(bool enabled);
    void onBluetoothDisabled();
    void onScanStarted();
//    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onScanFinished(const QList<QBluetoothDeviceInfo> &devices);
    void onScanError(const QString &error);
    void onConnectionStarted(const QString &deviceName);
    void onConnectionSuccess(const QString &deviceName);
    void onConnectionFailed(const QString &error);
    void onStatusMessage(const QString &message);
    void onErrorOccurred(const QString &error);

    void onConfirmationRequested(const QString &deviceName, const QString &pin);
    void onPinCodeDisplayed(const QString &deviceName, const QString &pin);
private:
    Ui::Widget *ui;
    void init();
    void setupBluetoothWorker();
    void connectBluetoothWorkerSignals();
    
    slideButton *sbtn=nullptr;
    
    // BluetoothWorker相关
    BluetoothWorker *m_bluetoothWorker = nullptr;
    QThread *m_bluetoothThread = nullptr;
    
    // UI相关数据
    QStringList deviceList={};
    QList<QBluetoothDeviceInfo> discoveredDevices;
    Loading *m_loadingDialog = nullptr;

};

#endif // WIDGET_H
