#include "widget.h"
#include "qglobal.h"
#include "qobject.h"
#include "qwidget.h"
#include "slidebutton.h"
#include "ui_widget.h"
#include <iostream>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMinimumSize(800,600);
    init();
    connect(sbtn,&slideButton::toggled,this,&Widget::slot_onSlideBtn);
    
    // 设置蓝牙工作线程
    setupBluetoothWorker();
}

Widget::~Widget()
{
    // 停止线程
    if (m_bluetoothThread && m_bluetoothThread->isRunning()) {
        m_bluetoothThread->quit();
        m_bluetoothThread->wait(5000);  // 等待5秒
        delete m_bluetoothThread; 
        m_bluetoothThread = nullptr;
    }
    
    // 2. 删除 Worker
    if (m_bluetoothWorker) {
        m_bluetoothWorker->deleteLater();
        m_bluetoothWorker = nullptr;
    }
    
    //删除加载对话框
    if (m_loadingDialog) {
        m_loadingDialog->deleteLater();
        m_loadingDialog = nullptr;
    }
    
    delete ui;
}

void Widget::slot_onSlideBtn(bool isChecked)
{
    if(isChecked){
        // 启动蓝牙
        qDebug() << "打开蓝牙";
        LOG_INFO("OperateLog", "打开蓝牙.....");
        
        // 通过Worker启用蓝牙
        QMetaObject::invokeMethod(m_bluetoothWorker, "enableBluetooth", 
                                  Qt::QueuedConnection, Q_ARG(bool, true));
    }
    else{
        // 关闭蓝牙
        qDebug() << "关闭蓝牙";
        LOG_INFO("OperateLog", "关闭蓝牙.....");
        
        // 通过Worker禁用蓝牙
        QMetaObject::invokeMethod(m_bluetoothWorker, "enableBluetooth", 
                                  Qt::QueuedConnection, Q_ARG(bool, false));
        
        // 清理UI
        setUi_CSL_Info(false);
        ui->listWidget->clear();
        discoveredDevices.clear();
    }
}

/*重新扫描蓝牙设备*/
void Widget::on_reloadBtn_clicked()
{
    if(!sbtn->isChecked()){
        CustomMessageBox::showMessage(CustomMessageBox::Warning,"提示","请先开启蓝牙");
        return;
    }
    qDebug() << "重新扫描蓝牙设备";
    LOG_INFO("OperateLog", "重新扫描蓝牙设备.....");
    QMetaObject::invokeMethod(m_bluetoothWorker, "startScan", Qt::QueuedConnection);

    // 通过Worker开始扫描
    if (!m_loadingDialog) {
        m_loadingDialog = new Loading(this);
    }
    m_loadingDialog->setText("扫描蓝牙设备中\n请等待...");
    m_loadingDialog->start();
    m_loadingDialog->exec();

}

void Widget::on_connectBtn_clicked()
{
    qDebug()<<"connect BlueTooth";
    if(ui->listWidget->currentItem()!=nullptr){
        int currentRow = ui->listWidget->currentRow();
        if(currentRow >= 0 && currentRow < discoveredDevices.size()) {
            QBluetoothDeviceInfo device = discoveredDevices[currentRow];

            QString pin = "";
            
            // 通过Worker连接设备
            QMetaObject::invokeMethod(m_bluetoothWorker, "connectDevice", 
                                      Qt::QueuedConnection, 
                                      Q_ARG(QBluetoothDeviceInfo, device), 
                                      Q_ARG(QString, pin));
        }
    }
    else{
        CustomMessageBox::showMessage(CustomMessageBox::Warning,"提示","请选择要连接的蓝牙设备");
    }
}

/*设置界面的样式*/
void Widget::init(){
    this->setStyleSheet(
        "QWidget {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #f0f0f0, stop:1 #e0e0e0);"
        "    border: none;"
        "}"
        "QWidget#widget {"
        "    background: white;"
        "    border-radius: 15px;"
        "    border: 1px solid #d0d0d0;"
        "    margin: 10px;"
        "}"
        "QListWidget {"
        "    background-color: #fafafa;"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "    font-size: 16px;"
        "    selection-background-color: #2196F3;"
        "}"
        "QListWidget::item {"
        "    background-color: white;"
        "    border: 1px solid #e0e0e0;"
        "    border-radius: 8px;"
        "    padding: 12px;"
        "    margin: 3px;"
        "    color: #333;"
        "}"
        "QListWidget::item:hover {"
        "    background-color: #e3f2fd;"
        "    border-color: #2196F3;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border-color: #1976D2;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 12px;"
        "    margin: 3px;"
        "}"
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #2196F3, stop:1 #1976D2);"
        "    color: white;"
        "    border-radius: 20px;"
        "    padding: 10px 20px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    min-width: 150px;"
        "    min-height: 40px;"
        "    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #1976D2, stop:1 #2196F3);"
        "    box-shadow: 0 6px 12px rgba(0, 0, 0, 0.3);"
        "    transform: translateY(-2px);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "                                stop:0 #1565C0, stop:1 #1976D2);"
        "    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
        "    transform: translateY(1px);"
        "}"
        "QPushButton:disabled {"
        "    background: #cccccc;"
        "    color: #666666;"
        "    box-shadow: none;"
        "}"
    );

    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->connectBtn->setFocusPolicy(Qt::NoFocus);
    ui->reloadBtn->setIcon(QIcon(":/Images/reload.png"));
    ui->reloadBtn->setFixedSize(50,50);
    ui->reloadBtn->setIconSize(QSize(50,50));
    ui->reloadBtn->setStyleSheet("border: none; background: transparent;");
    ui->reloadBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

    sbtn = new slideButton(this);
    sbtn->setFixedSize(60, 30);  // 设置合适的尺寸

    // 正确设置topLayout中的控件布局
    QHBoxLayout *topLayout = ui->topLayout;
    
    topLayout->removeWidget(ui->wifiStatusLabel);
    
    topLayout->addWidget(ui->wifiStatusLabel);
    
    topLayout->addSpacing(10);
    
    topLayout->addWidget(sbtn);
    
    // 设置wifiStatusLabel和sliderButton的对齐方式
    ui->wifiStatusLabel->setAlignment(Qt::AlignVCenter);
}

void Widget::setUi_CSL_Info(bool isSuccess)
{
    if(isSuccess)
    {
        ui->connectionStatusLabel->setText("已连接");
        ui->connectionStatusLabel->setStyleSheet("color: #4CAF50; \
                                                 font-size: 14px; \
                                                 background-color: #e8f5e8; \
                                                 border: 1px solid #4CAF50; \
                                                 border-radius: 15px; \
                                                 padding: 8px 15px; ");
    }
    else
    {
        ui->connectionStatusLabel->setText("未连接");
        ui->connectionStatusLabel->setStyleSheet("color: #ff6b6b; \
                                                 font-size: 14px; \
                                                 background-color: #ffe0e0; \
                                                 border: 1px solid #ffb3b3;\
                                                 border-radius: 15px; \
                                                 padding: 8px 15px; ");
    }
}


void Widget::setupBluetoothWorker()
{
    // 创建工作线程
    m_bluetoothThread = new QThread(this);
    
    // 创建蓝牙工作对象
    m_bluetoothWorker = new BluetoothWorker();
    
    // 将工作对象移动到工作线程
    m_bluetoothWorker->moveToThread(m_bluetoothThread);
    
    // 连接信号槽
    connectBluetoothWorkerSignals();
    
    // 启动线程
    m_bluetoothThread->start();
    
    // 初始化蓝牙
    QMetaObject::invokeMethod(m_bluetoothWorker, "initializeBluetooth", Qt::QueuedConnection);
}

void Widget::connectBluetoothWorkerSignals()
{
    // 蓝牙初始化和启用信号
    connect(m_bluetoothWorker, &BluetoothWorker::bluetoothInitialized,
            this, &Widget::onBluetoothInitialized);
    connect(m_bluetoothWorker, &BluetoothWorker::bluetoothEnabled,
            this, &Widget::onBluetoothEnabled);
    connect(m_bluetoothWorker, &BluetoothWorker::bluetoothDisabled,
            this, &Widget::onBluetoothDisabled);
    
    // 扫描相关信号
    connect(m_bluetoothWorker, &BluetoothWorker::scanStarted,
            this, &Widget::onScanStarted);
    connect(m_bluetoothWorker, &BluetoothWorker::scanFinished,
            this, &Widget::onScanFinished);
    connect(m_bluetoothWorker, &BluetoothWorker::scanError,
            this, &Widget::onScanError);
    
    // 连接相关信号
    connect(m_bluetoothWorker, &BluetoothWorker::connectionStarted,
            this, &Widget::onConnectionStarted);
    connect(m_bluetoothWorker, &BluetoothWorker::connectionSuccess,
            this, &Widget::onConnectionSuccess);
    connect(m_bluetoothWorker, &BluetoothWorker::connectionFailed,
            this, &Widget::onConnectionFailed);

    // PIN码显示相关信号
        connect(m_bluetoothWorker, &BluetoothWorker::pinCodeDisplayed,
                this, &Widget::onPinCodeDisplayed);
        connect(m_bluetoothWorker, &BluetoothWorker::confirmationRequested,
                this, &Widget::onConfirmationRequested);
    

    connect(m_bluetoothWorker, &BluetoothWorker::errorOccurred,
            this, &Widget::onErrorOccurred);
}

// BluetoothWorker信号处理槽实现
void Widget::onBluetoothInitialized(bool success)
{
    if (success) {
        qDebug() << "蓝牙初始化成功";
        LOG_INFO("OperateLog", "蓝牙初始化成功");
    } else {
        qDebug() << "蓝牙初始化失败";
        LOG_ERROR("OperateLog", "蓝牙初始化失败");
        CustomMessageBox::showMessage(CustomMessageBox::Warning, "提示", "蓝牙设备不可用");
    }
}

void Widget::onBluetoothEnabled(bool enabled)
{
    if (enabled) {
        qDebug() << "蓝牙已启用";
        LOG_INFO("OperateLog", "蓝牙已启用");
        // 自动开始扫描
        QMetaObject::invokeMethod(m_bluetoothWorker, "startScan", Qt::QueuedConnection);
        qDebug()<<"开始扫描蓝牙设备";
        // 显示加载对话框
        if (!m_loadingDialog) {
            m_loadingDialog = new Loading(this);
        }
        qDebug()<<"显示加载对话框";
        m_loadingDialog->setText("扫描蓝牙设备中\n请等待...");
        m_loadingDialog->start();
        m_loadingDialog->exec();
    } else {
        qDebug() << "蓝牙启用失败";
        LOG_ERROR("OperateLog", "蓝牙启用失败");
        sbtn->setChecked(false);
        CustomMessageBox::showMessage(CustomMessageBox::Warning, "提示", "蓝牙启用失败");
    }
}

void Widget::onBluetoothDisabled()
{
    qDebug() << "蓝牙已禁用";
    LOG_INFO("OperateLog", "蓝牙已禁用");
}

void Widget::onScanStarted()
{
    qDebug() << "开始扫描蓝牙设备";
    LOG_INFO("OperateLog", "开始扫描蓝牙设备");

    
    // 清空设备列表
    discoveredDevices.clear();
    ui->listWidget->clear();
}


void Widget::onScanFinished(const QList<QBluetoothDeviceInfo> &devices)
{

    // 关闭加载对话框
    if (m_loadingDialog) {
        m_loadingDialog->accept();
    }
    // 更新设备列表
    discoveredDevices = devices;
    deviceList.clear();
    for(const QBluetoothDeviceInfo &device : discoveredDevices) {
        deviceList.append(device.name());
    }
    qDebug()<<deviceList;
    ui->listWidget->clear();
    ui->listWidget->addItems(deviceList);

}

void Widget::onScanError(const QString &error)
{
    qDebug() << "扫描错误:" << error;
    LOG_ERROR("OperateLog", "扫描错误: " + error.toStdString());
    
    // 关闭加载对话框
    if (m_loadingDialog) {
        m_loadingDialog->accept();
    }
    
    CustomMessageBox::showMessage(CustomMessageBox::Warning, "提示", "蓝牙扫描失败: " + error);
}

void Widget::onConnectionStarted(const QString &deviceName)
{
    qDebug() << "开始连接设备:" << deviceName;
    LOG_INFO("OperateLog", "开始连接设备: " + deviceName.toStdString());
}

void Widget::onConnectionSuccess(const QString &deviceName)
{
    qDebug() << "设备连接成功:" << deviceName;
    LOG_INFO("OperateLog", "设备连接成功: " + deviceName.toStdString());
    
    setUi_CSL_Info(true);
    CustomMessageBox::showMessage(CustomMessageBox::Success,"提示", "连接成功: " + deviceName);
}

void Widget::onConnectionFailed(const QString &error)
{
    qDebug() << "连接失败:" << error;
    LOG_ERROR("OperateLog", "连接失败: " + error.toStdString());
    
    setUi_CSL_Info(false);
    CustomMessageBox::showMessage(CustomMessageBox::Warning, "提示", "连接失败: " + error);
}

void Widget::onStatusMessage(const QString &message)
{
    qDebug() << "状态消息:" << message;
    LOG_INFO("OperateLog", "状态消息: " + message.toStdString());
}

void Widget::onErrorOccurred(const QString &error)
{
    qDebug() << "错误:" << error;
    LOG_ERROR("OperateLog", "错误: " + error.toStdString());
    
    CustomMessageBox::showMessage(CustomMessageBox::Warning, "错误", error);
}

void Widget::onPinCodeDisplayed(const QString &deviceName, const QString &pin)
{
    QString message = QString("设备 %1 的配对PIN码为：\n\n%2\n\n请在目标设备上输入此PIN码")
                     .arg(deviceName).arg(pin);

    int result = QMessageBox::information(this,
        "PIN码显示",
        message,
        QMessageBox::Ok | QMessageBox::Cancel
    );

    // 根据用户选择发送确认结果
    bool confirmed = (result == QMessageBox::Ok);
    QMetaObject::invokeMethod(m_bluetoothWorker, "verifyPinCode",
                             Qt::QueuedConnection, Q_ARG(bool, confirmed));
}

void Widget::onConfirmationRequested(const QString &deviceName, const QString &pin)
{
    QString message = QString("请确认设备 %1 显示的PIN码是否为：\n\n%2")
                     .arg(deviceName).arg(pin);

    int result = QMessageBox::question(this,
        "PIN码确认",
        message,
        QMessageBox::Yes | QMessageBox::No
    );

    // 根据用户选择发送确认结果
    bool confirmed = (result == QMessageBox::Yes);
    QMetaObject::invokeMethod(m_bluetoothWorker, "verifyPinCode",
                             Qt::QueuedConnection, Q_ARG(bool, confirmed));
}
