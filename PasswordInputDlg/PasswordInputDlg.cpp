#include "PasswordInputDlg.h"
#include <QFont>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

PasswordInputDlg::PasswordInputDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    
    setupUI();
    showAnimation();
}

void PasswordInputDlg::setupUI()
{
    // 主窗口样式
    setFixedSize(400, 250);
    setStyleSheet(
        "PasswordInputDlg {"
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
    iconLabel->setPixmap(QPixmap("data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjQiIGhlaWdodD0iMjQiIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTEyIDJDNi40OCAyIDIgNi40OCAyIDEyczQuNDggMTAgMTAgMTBzMTAtNC40OCAxMC0xMFMxNy41MiAyIDEyIDJ6bTAgMTVjLS41NSAwLTEtLjQ1LTEtMXMuNDUtMSAxLTEgMSAuNDUgMSAxLS40NSAxLTEgMXptMS00aC0yVjdoMnY2eiIgZmlsbD0id2hpdGUiLz4KPC9zdmc+"));

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

    // 密码输入框
    passwordEdit = new myLineEdit();
    passwordEdit->setFixedHeight(35);
    passwordEdit->setFont(QFont("Arial", 12));
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #ddd;"
        "    border-radius: 5px;"
        "    padding: 5px 10px;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #667eea;"
        "}"
    );

    // 连接点击信号到键盘显示槽函数
    connect(passwordEdit, &myLineEdit::myLineEditClick, this, &PasswordInputDlg::slot_show_keyboard);
    // 显示密码复选框
    showPasswordCheck = new QCheckBox("显示密码");
    showPasswordCheck->setFont(QFont("Arial", 11));
    showPasswordCheck->setStyleSheet("color: #666;");

    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

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

    cancelButton = new QPushButton("取消");
    cancelButton->setFixedSize(100, 35);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background: #f8f9fa;"
        "    border: 1px solid #dee2e6;"
        "    border-radius: 17px;"
        "    color: #495057;"
        "    font-size: 13px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: #e9ecef;"
        "}"
        "QPushButton:pressed {"
        "    background: #dee2e6;"
        "}"
    );

    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    contentLayout->addWidget(messageLabel);
    contentLayout->addWidget(passwordEdit);
    contentLayout->addWidget(showPasswordCheck);
    contentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(contentWidget);

    // 主布局
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(mainWidget);

    // 连接信号槽
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(confirmButton, &QPushButton::clicked, [this]{
        this->accept();
        this->close();
    });
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(showPasswordCheck, &QCheckBox::toggled, this, &PasswordInputDlg::togglePasswordVisibility);
}

void PasswordInputDlg::showAnimation()
{
    // 简单的淡入动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void PasswordInputDlg::togglePasswordVisibility()
{
    if (showPasswordCheck->isChecked()) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
    }
}

void PasswordInputDlg::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

void PasswordInputDlg::setMessage(const QString &message)
{
    messageLabel->setText(message);
}

QString PasswordInputDlg::getPassword(const QString &title, const QString &message, QWidget *parent)
{
    PasswordInputDlg dialog(parent);
    dialog.setTitle(title);
    dialog.setMessage(message);
    
   // 设置对话框为模态并立即显示
    dialog.setModal(true);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString password = dialog.passwordEdit->text();
        dialog.close();  // 确保关闭
        return password;
    }
    
    dialog.close();  // 确保关闭
    
    return QString();
}

void PasswordInputDlg::slot_show_keyboard(const QString &objName)
{
    Q_UNUSED(objName);
    myLineEdit *obj = qobject_cast<myLineEdit*>(sender());

    if (!obj)
    {
        return;
    }

    normalKeyboard *normal = new normalKeyboard(this);
    if (normal)
    {
        normal->setModal(false);
        normal->setCurText(obj->text(), obj);
        connect(normal, &normalKeyboard::sig_update_txt, this, [=](const QString &txt)
                {
                    obj->setText(txt);
                    obj->setFocus();
                });

        connect(normal, &normalKeyboard::finished, [normal]()
                {
                    //qDebug() << "normal finished...";
                    normal->deleteLater();
                });

        connect(normal, &normalKeyboard::sig_click_enter, this, [=]()
                {
                    //qDebug() << "keyboard click enter...";
                    normal->accept();

                    // 直接触发确认按钮点击
                    confirmButton->click();
                });

        normal->show();
        normal->setFocus();

        obj->setFocus();
    }
}