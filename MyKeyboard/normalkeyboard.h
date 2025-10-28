#ifndef NORMALKEYBOARD_H
#define NORMALKEYBOARD_H

#include <QWidget>
#include <QScreen> 
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>
#include <QDialog>
#include <QEvent>
#include <QShowEvent>
#include <QRegularExpressionValidator>

#include "PinyinSql.h"
#include "myLineEdit.h"

namespace Ui {
class normalKeyboard;
}

class normalKeyboard : public QDialog
{
    Q_OBJECT

public:
    explicit normalKeyboard(QWidget *parent = nullptr);
    ~normalKeyboard();

//    //单例模式,保证一个程序只存在一个输入法实例对象
//    static normalKeyboard *Instance() {
//        if (!_instance) {
//            _instance = new normalKeyboard;
//        }
//        return _instance;
//    }

    void setCurText(const QString &txt, myLineEdit *cur_edit);

    void fNormalKeyboardInit();

    void fBackspace();

    void fInsertStr(QString str);

    bool fCheakPressType();

    void fChangeKeyboardFontSize(int fontSize);

    void fResetKeyboardSize(bool isChineseInput);

    void fKeyboardPropertyInit();

    void fChangeInputType(QString type);

    void fShowChinese();

signals:
    void sig_update_txt(const QString &txt);

    void sig_click_enter();

public slots:
    void fBtnLongClick();

    void fBtnClick();

    //void fFocusChanged(QWidget *, QWidget *);

protected:
//    bool eventFilter(QObject *watched, QEvent *event);

//    void mousePressEvent(QMouseEvent *e);

//    void mouseReleaseEvent(QMouseEvent *);

    //void showEvent(QShowEvent *event) override;

private slots:
    void on_showPinyin_textChanged(const QString &arg1);

private:
    Ui::normalKeyboard *ui;

    //static normalKeyboard *_instance;       //实例对象

    int g_DeskWidth = 0;                        //桌面的宽
    int g_DeskHeight = 0;                       //桌面的高
    int g_KeyboardWidth = 0;                    //键盘的宽
    int g_KeyboardHeight = 0;                   //键盘的高
    int g_CurrentChineseCount = 0;              //显示当前行第一个的下标

    QLineEdit *p_CurrentLineEdit = nullptr; //当前聚焦的输入行

    bool g_IsFirstClick = false;                   //是否首次加载
    bool g_IsPressed = false;                      //是否长按退格
    bool g_IsMousePressd = false;

    QPushButton *p_LongPressedBtn = nullptr;//长按按钮

    QTimer *p_LongPressTimer = nullptr;     //长按定时器

    QString g_CurrentFocusType;             //当前焦点控件的类型

    QString g_CurrentInputType;             //当前输入法类型

    QPoint g_MousePoint;

    QStringList g_ChineseList;

    QList <QPushButton *> g_SelectList;
};

#endif // NORMALKEYBOARD_H








