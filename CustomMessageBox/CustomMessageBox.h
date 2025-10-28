#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>

class CustomMessageBox : public QDialog
{
    Q_OBJECT

public:
    enum MessageType {
        Info,
        Success,
        Warning,
        Error
    };

    explicit CustomMessageBox(QWidget *parent = nullptr);
    static void showMessage(MessageType type, const QString &title, const QString &message, QWidget *parent = nullptr);

private:
    void setupUI();
    void setMessageType(MessageType type);
    void setTitle(const QString &title);
    void setMessage(const QString &message);
    void showAnimation();

    QLabel *iconLabel;
    QLabel *titleLabel;
    QLabel *messageLabel;
    QPushButton *closeButton;
    QPushButton *confirmButton;
    QWidget *headerWidget;
    MessageType currentType;
};

#endif // CUSTOMMESSAGEBOX_H