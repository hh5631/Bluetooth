#ifndef BLUETOOTHWORKER_H
#define BLUETOOTHWORKER_H

#include <QObject>
#include <QThread>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>
#include <QBluetoothAddress>
#include <QRegularExpression>
#include <QBluetoothServiceDiscoveryAgent>
#include <QTimer>
#include <QDebug>
#include "SpdlogManage/SpdlogManage.h"

class BluetoothWorker : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothWorker(QObject *parent = nullptr);
    ~BluetoothWorker();

public slots:
    // 核心蓝牙操作方法
    void initializeBluetooth();
    void enableBluetooth(bool enable);
    void startScan();
    void connectDevice(const QBluetoothDeviceInfo &device, const QString &pin = QString());
    void stopScan();
    void disconnectDevice();
    void verifyPinCode(bool confirmed);

signals:
    // 状态更新信号
    void bluetoothInitialized(bool success);
    void bluetoothEnabled(bool enabled);
    void bluetoothDisabled();
    
    // 扫描相关信号
    void scanStarted();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanFinished(const QList<QBluetoothDeviceInfo> &devices);
    void scanError(const QString &error);
    
    // 连接相关信号
    void connectionStarted(const QString &deviceName);
    void connectionSuccess(const QString &deviceName);
    void connectionFailed(const QString &error);
    void pairingStarted(const QString &deviceName);
    void pairingFinished(bool success, const QString &deviceName);
    void pinCodeDisplayed(const QString &deviceName, const QString &pin);
    void confirmationRequested(const QString &deviceName, const QString &pin);
    
    // 通用状态信号
    void statusMessage(const QString &message);
    void errorOccurred(const QString &error);

private slots:
    // 内部信号处理
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onScanFinished();
    void onScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void onPairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);
    void onPairingTimeout();
    void onDisplayPinCode(const QBluetoothAddress &address, QString pin);
    void onDisplayConfirmation(const QBluetoothAddress &address, QString pin);
    void onServiceDiscovered(const QBluetoothServiceInfo &service);
    void onServiceDiscoveryFinished();
    void onServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error);

private:
    // 蓝牙设备对象
    QBluetoothLocalDevice *m_localDevice=nullptr;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent=nullptr;
    QBluetoothServiceDiscoveryAgent *m_sDiscoveryAgent=nullptr;
    QBluetoothSocket *m_socket=nullptr;

    // 设备列表
    QList<QBluetoothDeviceInfo> m_discoveredDevices;
    
    // 当前操作状态
    QBluetoothDeviceInfo m_currentDevice;
    QString m_currentPin = nullptr;
    QTimer *m_pairingTimer=nullptr;
    QBluetoothAddress m_currentPairingAddress;
    QTimer *m_pinDisplayTimer=nullptr;  // PIN码显示超时定时器
    QBluetoothServiceInfo m_service;

    // 内部方法
    void setupConnections();
    bool isBluetoothAvailable() const;
    bool isMacAddressFormat(const QString &name) const;
    void establishSocketConnection();

};

#endif // BLUETOOTHWORKER_H
