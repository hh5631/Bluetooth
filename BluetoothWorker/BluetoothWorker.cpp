#include "BluetoothWorker.h"

BluetoothWorker::BluetoothWorker(QObject *parent)
    : QObject(parent)
{
    
}

BluetoothWorker::~BluetoothWorker()
{
    if (m_localDevice) {
        delete m_localDevice;
        m_localDevice = nullptr;
    }
    // 停止扫描
    if (m_discoveryAgent && m_discoveryAgent->isActive()) 
    {
        m_discoveryAgent->stop();
    }

    // 断开Socket连接
    if (m_socket && m_socket->state() == QBluetoothSocket::ConnectedState) 
    {
        m_socket->disconnectFromService();
    }

    // 停止定时器
    if (m_pairingTimer && m_pairingTimer->isActive()) 
    {
        m_pairingTimer->stop();
    }

    // 停止服务发现
    if (m_sDiscoveryAgent) {
        m_sDiscoveryAgent->stop();
        m_sDiscoveryAgent->deleteLater();
        m_sDiscoveryAgent = nullptr;
    }

    // 清空设备列表
    m_discoveredDevices.clear();

    // 重置当前设备信息
    m_currentDevice = QBluetoothDeviceInfo();
    m_currentPin.clear();
}

void BluetoothWorker::initializeBluetooth()
{
    qDebug() << "BluetoothWorker: 初始化蓝牙设备";
    LOG_INFO("OperateLog", "BluetoothWorker: 初始化蓝牙设备");
    
    // 创建本地蓝牙设备
    if (!m_localDevice) {
    m_localDevice = new QBluetoothLocalDevice(this);
    }

    // 创建设备发现代理
    if (!m_discoveryAgent) {
        m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    }

    // 创建配对超时定时器
    if (!m_pairingTimer) {
        m_pairingTimer = new QTimer(this);
        m_pairingTimer->setSingleShot(true);
        m_pairingTimer->setInterval(20000); // 10秒超时
    }

    // 设置信号连接
    setupConnections();

    // 检查蓝牙可用性
    bool available = isBluetoothAvailable();

    if (available) {
        qDebug() << "BluetoothWorker: 蓝牙设备初始化成功";
        LOG_INFO("OperateLog", "BluetoothWorker: 蓝牙设备初始化成功");
        emit statusMessage("蓝牙设备初始化成功");
    } else {
        qDebug() << "BluetoothWorker: 蓝牙设备不可用";
        LOG_ERROR("OperateLog", "BluetoothWorker: 蓝牙设备不可用");
        emit errorOccurred("蓝牙设备不可用");
    }

    emit bluetoothInitialized(available);
}

void BluetoothWorker::enableBluetooth(bool enable)
{
    if (!m_localDevice) {
        emit errorOccurred("蓝牙设备未初始化");
        return;
    }
    
    if (!isBluetoothAvailable()) {
        emit errorOccurred("蓝牙设备不可用");
        return;
    }
    if (enable && m_localDevice) {
        LOG_INFO("OperateLog", "BluetoothWorker: 启用蓝牙");
        m_localDevice->powerOn();
        if (m_localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff)
        {
            m_localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        }
        emit statusMessage("蓝牙已启用");
        emit bluetoothEnabled(true);
    } 
    else 
    {
        LOG_INFO("OperateLog", "BluetoothWorker: 禁用蓝牙");
        
        // 停止扫描
        if (m_discoveryAgent && m_discoveryAgent->isActive()) {
            m_discoveryAgent->stop();
        }
        
        // 关闭蓝牙
        m_localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
        
        // 清空设备列表
        m_discoveredDevices.clear();
        
        emit statusMessage("蓝牙已禁用");
        emit bluetoothDisabled();
    }
  
}

void BluetoothWorker::startScan()
{
    if (!m_discoveryAgent) {
        emit errorOccurred("蓝牙发现代理未初始化");
        return;
    }
    
    if (!isBluetoothAvailable()) {
        emit errorOccurred("蓝牙设备不可用");
        return;
    }

    qDebug() << "BluetoothWorker: 开始扫描蓝牙设备";
    LOG_INFO("OperateLog", "BluetoothWorker: 开始扫描蓝牙设备");
    
    // 停止之前的扫描
    if (m_discoveryAgent->isActive()) 
    {
        m_discoveryAgent->stop();
    }
    
    // 清空之前的结果
    m_discoveredDevices.clear();
    
    // 开始扫描
    m_discoveryAgent->start();
    
    emit scanStarted();
    emit statusMessage("正在扫描蓝牙设备...");
}

void BluetoothWorker::stopScan()
{
    if (m_discoveryAgent && m_discoveryAgent->isActive()) {
        qDebug() << "BluetoothWorker: 停止扫描";
        LOG_INFO("OperateLog", "BluetoothWorker: 停止扫描");
        m_discoveryAgent->stop();
    }
}

void BluetoothWorker::connectDevice(const QBluetoothDeviceInfo &device, const QString &pin)
{
    if (!m_localDevice) {
        emit errorOccurred("蓝牙设备未初始化");
        return;
    }
    
    if (!isBluetoothAvailable()) {
        emit errorOccurred("蓝牙设备不可用");
        return;
    }
    
    m_currentDevice = device;
//        m_currentPin = pin;
    
    // 检查设备是否已配对
    QBluetoothLocalDevice::Pairing pairingStatus = m_localDevice->pairingStatus(device.address());
    
    if (pairingStatus != QBluetoothLocalDevice::Paired) {
        // 需要配对
        qDebug() << "BluetoothWorker: 设备未配对，开始配对过程";
        LOG_INFO("OperateLog", "BluetoothWorker: 设备未配对，开始配对过程");
        
        // 启动配对超时定时器
        // m_pairingTimer->start();
        
        // 请求配对
        qDebug()<<"device.address()"<<device.address();
        qDebug()<<"QBluetoothLocalDevice::Paired"<< QBluetoothLocalDevice::Paired;
        m_localDevice->requestPairing(device.address(), QBluetoothLocalDevice::Paired);
        
    } else {
 // 已配对，直接连接
        qDebug() << "BluetoothWorker: 设备已配对，开始建立连接";
        LOG_INFO("OperateLog", "BluetoothWorker: 设备已配对，开始建立连接");

        establishSocketConnection();
    }   
}
void BluetoothWorker::establishSocketConnection()
{
    qDebug()<<"establishSocketConnection";
    // 清理旧的服务发现代理
    if (m_sDiscoveryAgent) 
    {
        m_sDiscoveryAgent->stop();
        m_sDiscoveryAgent->deleteLater();
        m_sDiscoveryAgent = nullptr;
    }

     // 创建新的服务发现代理，传入目标设备的MAC地址
     m_sDiscoveryAgent= new QBluetoothServiceDiscoveryAgent(m_currentDevice.address(), this);
     qDebug()<<"m_sDiscoveryAgent"<<m_sDiscoveryAgent;
        // 连接信号
     connect(m_sDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
            this, &BluetoothWorker::onServiceDiscovered);
     connect(m_sDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished,
             this, &BluetoothWorker::onServiceDiscoveryFinished);
     connect(m_sDiscoveryAgent, QOverload<QBluetoothServiceDiscoveryAgent::Error>::of(&QBluetoothServiceDiscoveryAgent::error),
                    this, &BluetoothWorker::onServiceDiscoveryError);
    
    if (!m_socket) {
        m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
        
        qDebug()<<"m_socket"<<m_socket;
    }
    connect(m_socket, &QBluetoothSocket::connected,
        this, [this]() {
            QString deviceName = m_currentDevice.name().isEmpty() ?
                               m_currentDevice.address().toString() : m_currentDevice.name();
            qDebug() << "BluetoothWorker: Socket连接成功:" << deviceName;
            LOG_INFO("OperateLog", "BluetoothWorker: Socket连接成功: " + deviceName.toStdString());
            emit connectionSuccess(deviceName);
        });
    connect(m_socket, &QBluetoothSocket::disconnected,
            this, [this]() {
                qDebug() << "BluetoothWorker: Socket连接断开";
                LOG_INFO("OperateLog", "BluetoothWorker: Socket连接断开");
                emit statusMessage("设备连接已断开");
            });
    connect(m_socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, [this](QBluetoothSocket::SocketError error) {
                QString errorString = QString("Socket连接错误: %1").arg(error);
                qDebug() << "BluetoothWorker:" << errorString;
                LOG_ERROR("OperateLog", errorString.toStdString());
                emit connectionFailed(errorString);
            });
   if (m_socket->state() != QBluetoothSocket::UnconnectedState)
    {
       // Socket 不在未连接状态，需要先断开
       qDebug() << "BluetoothWorker: Socket 不在未连接状态，先断开";
        
       // 连接断开完成信号，然后重新开始服务发现
       auto disconnectConn = std::make_shared<QMetaObject::Connection>();
       *disconnectConn = connect(m_socket, &QBluetoothSocket::disconnected,
               this, [this, disconnectConn]() {
                   qDebug() << "BluetoothWorker: Socket 已断开，重新开始服务发现";
                   disconnect(*disconnectConn);                   
                   // 重置服务信息
                   m_service = QBluetoothServiceInfo();
                   
                   // 开始服务发现
                   if (m_sDiscoveryAgent) {
                       m_sDiscoveryAgent->start();
                   }
               });
       
       m_socket->disconnectFromService();
       return; // 等待断开完成后再继续
    }
    else
    {
        qDebug()<<"m_socket为其他状态"<<m_socket->state();
    }
    // 重置服务信息
    m_service = QBluetoothServiceInfo();

    // 发现服务
    m_sDiscoveryAgent->start();
}

void BluetoothWorker::disconnectDevice()
{
    if (m_socket && m_socket->state() == QBluetoothSocket::ConnectedState) {
        qDebug() << "BluetoothWorker: 断开Socket连接";
        LOG_INFO("OperateLog", "BluetoothWorker: 断开Socket连接");
        m_socket->disconnectFromService();
    }
    emit statusMessage("设备已断开连接");
}

void BluetoothWorker::setupConnections()
{
    if (m_discoveryAgent) 
    {
        connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                this, &BluetoothWorker::onDeviceDiscovered);
        connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
                this, &BluetoothWorker::onScanFinished);
        connect(m_discoveryAgent, 
            QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &BluetoothWorker::onScanError);
    }

    if (m_localDevice) 
    {
        connect(m_localDevice, &QBluetoothLocalDevice::pairingFinished,
                this, &BluetoothWorker::onPairingFinished);
        connect(m_localDevice, &QBluetoothLocalDevice::pairingDisplayPinCode,
                this, &BluetoothWorker::onDisplayPinCode);
        connect(m_localDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation,
                this, &BluetoothWorker::onDisplayConfirmation);
    }

    if (m_pairingTimer) 
    {
        connect(m_pairingTimer, &QTimer::timeout,
                this, &BluetoothWorker::onPairingTimeout);
    }

    if (!m_pinDisplayTimer) {
        m_pinDisplayTimer = new QTimer(this);
        m_pinDisplayTimer->setSingleShot(true);
        connect(m_pinDisplayTimer, &QTimer::timeout, this, [this]() {
            qDebug() << "BluetoothWorker: PIN码确认超时";
            LOG_ERROR("OperateLog", "BluetoothWorker: PIN码确认超时");
            emit pairingFinished(false, m_currentDevice.name());
            emit connectionFailed("PIN码确认超时");
        });
    }
}

bool BluetoothWorker::isBluetoothAvailable() const
{
    return m_localDevice && m_localDevice->isValid();
}

void BluetoothWorker::onDeviceDiscovered(const QBluetoothDeviceInfo &device)
{
    // 检查设备名称是否为空，是否已存在，以及是否符合MAC地址格式
      if (!device.name().isEmpty() && !m_discoveredDevices.contains(device) && !isMacAddressFormat(device.name())) {  // 过滤掉MAC地址格式的设备名

          m_discoveredDevices.append(device);
          qDebug() << "BluetoothWorker: 发现设备:" << device.name();

        } else if (isMacAddressFormat(device.name())) {
            // 记录被过滤的MAC地址格式设备名
            qDebug() << "BluetoothWorker: 过滤MAC地址格式设备名:" << device.name();
        }
}

void BluetoothWorker::onScanFinished()
{
    qDebug() << "BluetoothWorker: 扫描完成，发现" << m_discoveredDevices.size() << "个设备";
    LOG_INFO("OperateLog", "BluetoothWorker: 扫描完成，发现 " + QString::number(m_discoveredDevices.size()).toStdString() + " 个设备");
    
    emit scanFinished(m_discoveredDevices);
}

void BluetoothWorker::onScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    QString errorString;
    switch (error) {
        case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
            errorString = "蓝牙设备未开启";
            break;
        case QBluetoothDeviceDiscoveryAgent::InputOutputError:
            errorString = "输入输出错误";
            break;
        case QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError:
            errorString = "无效的蓝牙适配器";
            break;
        case QBluetoothDeviceDiscoveryAgent::UnsupportedPlatformError:
            errorString = "不支持的平台";
            break;
        case QBluetoothDeviceDiscoveryAgent::UnsupportedDiscoveryMethod:
            errorString = "不支持的发现方法";
            break;
        default:
            errorString = "未知扫描错误";
            break;
    }
    
    qDebug() << "BluetoothWorker: 扫描错误:" << errorString;
    LOG_ERROR("OperateLog", "BluetoothWorker: 扫描错误: " + errorString.toStdString());
    
    emit scanError(errorString);
    emit errorOccurred(errorString);
}

//蓝牙配对
void BluetoothWorker::onPairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    // 停止超时定时器
    if (m_pairingTimer->isActive()) {
        m_pairingTimer->stop();
    }

    QString deviceName = m_currentDevice.name().isEmpty() ? address.toString() : m_currentDevice.name();
    
    if (pairing == QBluetoothLocalDevice::Paired) {
        qDebug() << "BluetoothWorker: 配对成功:" << deviceName;
        LOG_INFO("OperateLog", "BluetoothWorker: 配对成功: " + deviceName.toStdString());
        
//        emit pairingFinished(true, deviceName);
        establishSocketConnection();
        
    } else {
        qDebug() << "BluetoothWorker: 配对失败:" << deviceName;
        LOG_ERROR("OperateLog", "BluetoothWorker: 配对失败: " + deviceName.toStdString());
        
        emit pairingFinished(false, deviceName);
        emit connectionFailed("配对失败");
    }
}

void BluetoothWorker::onPairingTimeout()
{
    QString deviceName = m_currentDevice.name().isEmpty() ? m_currentDevice.address().toString() : m_currentDevice.name();
    
    qDebug() << "BluetoothWorker: 配对超时:" << deviceName;
    LOG_ERROR("OperateLog", "BluetoothWorker: 配对超时: " + deviceName.toStdString());
    
    emit pairingFinished(false, deviceName);
    emit connectionFailed("配对超时");
}

bool BluetoothWorker::isMacAddressFormat(const QString &name) const
{
    // MAC地址格式正则表达式：XX:XX:XX:XX:XX:XX 或 XX-XX-XX-XX-XX-XX
    // X为十六进制字符（0-9, A-F, a-f）
    QRegularExpression macRegex("^([0-9A-Fa-f]{2}[:-]){5}[0-9A-Fa-f]{2}$");
    return macRegex.match(name).hasMatch();
}

void BluetoothWorker::onDisplayPinCode(const QBluetoothAddress &address, QString pin)
{
    // 记录当前配对的设备地址
    m_currentPairingAddress = address;
    
    // 获取设备名称
    QString deviceName = m_currentDevice.name().isEmpty() ? 
                        address.toString() : m_currentDevice.name();
    
    qDebug() << "BluetoothWorker: 显示PIN码:" << pin << "设备:" << deviceName;
    LOG_INFO("OperateLog", "BluetoothWorker: 显示PIN码: " + pin.toStdString() + 
             " 设备: " + deviceName.toStdString());
    
    // 启动PIN码显示超时定时器（30秒）
    if (!m_pinDisplayTimer) {
        m_pinDisplayTimer = new QTimer(this);
        m_pinDisplayTimer->setSingleShot(true);
    }
    m_pinDisplayTimer->start(60000); // 30秒超时
    
    // 发送信号通知UI显示PIN码
    emit pinCodeDisplayed(deviceName, pin);
}

void BluetoothWorker::onDisplayConfirmation(const QBluetoothAddress &address, QString pin)
{
    // 记录当前配对的设备地址
    m_currentPairingAddress = address;
    
    // 获取设备名称
    QString deviceName = m_currentDevice.name().isEmpty() ? 
                        address.toString() : m_currentDevice.name();
    
    qDebug() << "BluetoothWorker: 请求确认PIN码:" << pin << "设备:" << deviceName;
    LOG_INFO("OperateLog", "BluetoothWorker: 请求确认PIN码: " + pin.toStdString() + 
             " 设备: " + deviceName.toStdString());
    
    // 启动确认超时定时器
    if (m_pinDisplayTimer) {
        m_pinDisplayTimer->start(30000);
    }
    
    // 发送信号通知UI请求用户确认
    emit confirmationRequested(deviceName, pin);
}

void BluetoothWorker::onServiceDiscovered(const QBluetoothServiceInfo &service)
{
    qDebug()<<"发现服务-uuid"<<service.serviceUuid();
    
    // 如果已经找到有效服务，跳过
    if (m_service.isValid()) {
        return;
    }
    
    // 优先选择 SPP 服务 (Serial Port Profile)
    QBluetoothUuid sppUuid(QBluetoothUuid::SerialPort);
    if (service.serviceUuid() == sppUuid) {
        m_service = service;
        qDebug()<<"选择 SPP 服务";
    } else if (!m_service.isValid()) {
        // 如果没有找到 SPP，使用第一个可用的服务
        m_service = service;
        qDebug()<<"使用服务："<<service.serviceUuid();
    }
}

void BluetoothWorker::onServiceDiscoveryFinished()
{
    qDebug()<<"onServiceDiscoveryFinished";
    
    if (!m_service.isValid()) {
        qDebug() << "BluetoothWorker: 未找到有效的蓝牙服务";
        LOG_ERROR("OperateLog", "BluetoothWorker: 未找到有效的蓝牙服务");
        emit connectionFailed("未找到有效的蓝牙服务");
        return;
    }
    
    if (m_socket) {
        m_socket->connectToService(m_currentDevice.address(), m_service.serviceUuid());
    } else {
        emit connectionFailed("Socket未初始化");
    }
}

void BluetoothWorker::onServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error)
{
    qDebug() << "BluetoothWorker: 服务发现错误:" << error;
       LOG_ERROR("OperateLog", "BluetoothWorker: 服务发现错误: " + QString::number(error).toStdString());
       emit connectionFailed("蓝牙服务发现失败");
}

void BluetoothWorker::verifyPinCode(bool confirmed)
{
    // 停止超时定时器
    if (m_pinDisplayTimer && m_pinDisplayTimer->isActive()) {
        m_pinDisplayTimer->stop();
    }
    
    QString deviceName = m_currentDevice.name().isEmpty() ? 
                        m_currentPairingAddress.toString() : m_currentDevice.name();
    
    qDebug() << "BluetoothWorker: PIN码验证结果:" << confirmed << "设备:" << deviceName;
    LOG_INFO("OperateLog", "BluetoothWorker: PIN码验证结果: " + 
             QString(confirmed ? "确认" : "拒绝").toStdString() + 
             " 设备: " + deviceName.toStdString());
    
    // 向蓝牙系统发送确认结果
    if (m_localDevice) {
        m_localDevice->pairingConfirmation(confirmed);
    }
    
    // 清除PIN码相关信息（安全考虑）
    m_currentPairingAddress = QBluetoothAddress();
    
    if (!confirmed) {
        emit pairingFinished(false, deviceName);
        emit connectionFailed("用户拒绝PIN码确认");
    }
}

