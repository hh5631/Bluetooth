#include "CustomMessageBox.h"
#include <QFont>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

CustomMessageBox::CustomMessageBox(QWidget *parent)
    : QDialog(parent)
    , currentType(Info)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    
    setupUI();
    showAnimation();
}

void CustomMessageBox::setupUI()
{
    // 主窗口样式
    setFixedSize(400, 200);
    setStyleSheet(
        "CustomMessageBox {"
        "    background: transparent;"
        "}"
    );

    // 创建主widget
    QWidget *mainWidget = new QWidget(this);
    mainWidget->setObjectName("mainWidget");
    mainWidget->setStyleSheet(
        "#mainWidget {"
        "    background: white;"
        "    border-radius: 15px;"
        "    border: 1px solid #e0e0e0;"
        "}"
    );

    // // 添加阴影效果
    // QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    // shadow->setBlurRadius(20);
    // shadow->setColor(QColor(0, 0, 0, 100));
    // shadow->setOffset(0, 5);
    // mainWidget->setGraphicsEffect(shadow);

    // 创建布局
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(0);

    // 顶部栏
    headerWidget = new QWidget();
    headerWidget->setFixedHeight(50);
    headerWidget->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #667eea, stop:1 #764ba2);"
        "border-top-left-radius: 15px;"
        "border-top-right-radius: 15px;"
    );

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 0, 15, 0);

    // 图标和标题
    iconLabel = new QLabel();
    iconLabel->setFixedSize(24, 24);
    iconLabel->setScaledContents(true);

    titleLabel = new QLabel();
    titleLabel->setFont(QFont("Arial", 14, QFont::Bold));
    titleLabel->setStyleSheet("color: white;");

    closeButton = new QPushButton("×");
    closeButton->setFixedSize(30, 30);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background: transparent;"
        "    border: none;"
        "    color: white;"
        "    font-size: 20px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: rgba(255, 255, 255, 0.2);"
        "    border-radius: 15px;"
        "}"
    );

    headerLayout->addWidget(iconLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeButton);

    // 内容区域
    QWidget *contentWidget = new QWidget();
    contentWidget->setStyleSheet("background: white;");
    
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 20, 30, 20);
    contentLayout->setSpacing(15);

    messageLabel = new QLabel();
    messageLabel->setFont(QFont("Arial", 12));
    messageLabel->setStyleSheet("color: #333;");
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignCenter);

    confirmButton = new QPushButton("确定");
    confirmButton->setFixedSize(100, 35);
    confirmButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #667eea, stop:1 #764ba2);"
        "    border: none;"
        "    border-radius: 17px;"
        "    color: white;"
        "    font-size: 13px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #764ba2, stop:1 #667eea);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #5a6fd8, stop:1 #6a4190);"
        "}"
    );

    contentLayout->addWidget(messageLabel);
    contentLayout->addWidget(confirmButton, 0, Qt::AlignCenter);

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(contentWidget);

    // 主布局
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(mainWidget);

    // 连接信号槽
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
}

void CustomMessageBox::setMessageType(MessageType type)
{
    currentType = type;
    
    switch(type) {
    case Info:
        iconLabel->setPixmap(QPixmap("data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjQiIGhlaWdodD0iMjQiIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEyIDJDNi40OCAyIDIgNi40OCAyIDEyczQuNDggMTAgMTAgMTBzMTAtNC40OCAxMC0xMFMxNy41MiAyIDEyIDJ6bTAgMTVjLS41NSAwLTEtLjQ1LTEtMXMuNDUtMSAxLTEgMSAuNDUgMSAxLS40NSAxLTEgMXptMS00aC0yVjdoMnY2eiIgZmlsbD0id2hpdGUiLz4KPC9zdmc+"));
        titleLabel->setText("提示");
        headerWidget->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
            "stop:0 #17a2b8, stop:1 #138496);"
            "border-top-left-radius: 15px;"
            "border-top-right-radius: 15px;"
        );
        break;
    case Success:
        iconLabel->setPixmap(QPixmap("data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjQiIGhlaWdodD0iMjQiIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEyIDJDNi40OCAyIDIgNi40OCAyIDEyczQuNDggMTAgMTAgMTBzMTAtNC40OCAxMC0xMFMxNy41MiAyIDEyIDJ6bS0yIDE1bC01LTUgMS40MS0xLjQxTDEwIDE0LjE3bDcuNTktNy41OUwxOSA4bC05IDl6IiBmaWxsPSJ3aGl0ZSIvPgo8L3N2Zz4="));
        titleLabel->setText("成功");
        headerWidget->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
            "stop:0 #28a745, stop:1 #1e7e34);"
            "border-top-left-radius: 15px;"
            "border-top-right-radius: 15px;"
        );
        break;
    case Warning:
        iconLabel->setPixmap(QPixmap("data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjQiIGhlaWdodD0iMjQiIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEgMjFoMjJMMTEuOTkgMyBMMSAyMXptMTItM2gtMnYyaDJ2LTJ6bTAtNGgtMnY0aDJ2LTR6IiBmaWxsPSJ3aGl0ZSIvPgo8L3N2Zz4="));
        titleLabel->setText("警告");
        headerWidget->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
            "stop:0 #ffc107, stop:1 #e0a800);"
            // "border-top-left-radius: 15px;"
            // "border-top-right-radius: 15px;"
        );
        break;
    case Error:
        iconLabel->setPixmap(QPixmap("data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjQiIGhlaWdodD0iMjQiIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEyIDJDNi40OCAyIDIgNi40OCAyIDEyczQuNDggMTAgMTAgMTBzMTAtNC40OCAxMC0xMFMxNy41MiAyIDEyIDJ6bTUgMTNoLTEwdi0yaDEwdjJ6bTAtNGgtMTB2LTZoMTB2NnoiIGZpbGw9IndoaXRlIi8+Cjwvc3ZnPg=="));
        titleLabel->setText("错误");
        headerWidget->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
            "stop:0 #dc3545, stop:1 #c82333);"
            "border-top-left-radius: 15px;"
            "border-top-right-radius: 15px;"
        );
        break;
    }
}

void CustomMessageBox::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

void CustomMessageBox::setMessage(const QString &message)
{
    messageLabel->setText(message);
}

void CustomMessageBox::showAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void CustomMessageBox::showMessage(MessageType type, const QString &title, const QString &message, QWidget *parent)
{
    CustomMessageBox *msgBox = new CustomMessageBox(parent);
    
    // 居中显示
    if (parent) {
        msgBox->move(parent->geometry().center() - msgBox->rect().center());
    } else {
        QScreen *screen = QGuiApplication::primaryScreen();
        msgBox->move(screen->geometry().center() - msgBox->rect().center());
    }
    
    msgBox->setMessageType(type);
    if (!title.isEmpty()) {
        msgBox->setTitle(title);
    }
    msgBox->setMessage(message);
    msgBox->exec();
    
    delete msgBox;
}