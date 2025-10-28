#ifndef PASSWORDINPUTDLG_H
#define PASSWORDINPUTDLG_H

#include "MyKeyboard/myLineEdit.h"
#include "MyKeyboard/normalkeyboard.h"
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QCheckBox>

class PasswordInputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordInputDlg(QWidget *parent = nullptr);
    static QString getPassword(const QString &title, const QString &message, QWidget *parent = nullptr);

private:
    void setupUI();
    void setTitle(const QString &title);
    void setMessage(const QString &message);
    void showAnimation();
    void togglePasswordVisibility();
    void slot_show_keyboard(const QString &objName);

    QLabel *iconLabel;
    QLabel *titleLabel;
    QLabel *messageLabel;
    myLineEdit *passwordEdit;
    QCheckBox *showPasswordCheck;
    QPushButton *closeButton;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
    QWidget *headerWidget;
};

#endif // PASSWORDINPUTDLG_H
