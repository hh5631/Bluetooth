#include "normalkeyboard.h"
#include "normalkeyboard.h"
#include "ui_normalkeyboard.h"

normalKeyboard::normalKeyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::normalKeyboard)
{
    ui->setupUi(this);

    //this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::Popup);

    fKeyboardPropertyInit();
    fNormalKeyboardInit();

    fResetKeyboardSize(false);
}

normalKeyboard::~normalKeyboard()
{
    delete ui;
}


void normalKeyboard::fNormalKeyboardInit()
{
    g_IsFirstClick = true;
    g_CurrentChineseCount = 0;
    g_IsPressed = false;
    g_CurrentFocusType.clear();
    g_CurrentInputType = "InputUpper"; //InputLower InputChinese InputNumber_C InputNumber_E
    g_IsMousePressd = false;

    fChangeInputType("InputUpper");

    //获取桌面的宽高
//    QDesktopWidget myAppWidget;
//    g_DeskWidth = myAppWidget.availableGeometry().width();
//    g_DeskHeight = myAppWidget.availableGeometry().height();
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        g_DeskWidth = screen->geometry().width();
        g_DeskHeight = screen->geometry().height();

        //qDebug() << "g_DeskWidth = " << g_DeskWidth << "g_DeskHeight = " << g_DeskHeight;
    }

    p_LongPressTimer = new QTimer;
    connect(p_LongPressTimer, &QTimer::timeout, this, &normalKeyboard::fBtnLongClick);

    QList<QPushButton *>myBtn = this->findChildren<QPushButton *>();
    foreach(QPushButton *btn, myBtn)
        connect(btn, &QPushButton::clicked, this, &normalKeyboard::fBtnClick);

//    //绑定全部改变焦点的信号
//    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(fFocusChanged(QWidget *, QWidget *)));
//    qApp->installEventFilter(this);
}


void normalKeyboard::fKeyboardPropertyInit()
{
    ui->btnQ_1->setProperty("btnLetter", true);
    ui->btnW_2->setProperty("btnLetter", true);
    ui->btnE_3->setProperty("btnLetter", true);
    ui->btnR_4->setProperty("btnLetter", true);
    ui->btnT_5->setProperty("btnLetter", true);
    ui->btnY_6->setProperty("btnLetter", true);
    ui->btnU_7->setProperty("btnLetter", true);
    ui->btnI_8->setProperty("btnLetter", true);
    ui->btnO_9->setProperty("btnLetter", true);
    ui->btnP_0->setProperty("btnLetter", true);
    ui->btnA_->setProperty("btnLetter", true);
    ui->btnB_->setProperty("btnLetter", true);
    ui->btnD_->setProperty("btnLetter", true);
    ui->btnF_->setProperty("btnLetter", true);
    ui->btnG_->setProperty("btnLetter", true);
    ui->btnH_->setProperty("btnLetter", true);
    ui->btnJ_->setProperty("btnLetter", true);
    ui->btnK_->setProperty("btnLetter", true);
    ui->btnL_->setProperty("btnLetter", true);
    ui->btnZ_->setProperty("btnLetter", true);
    ui->btnX_->setProperty("btnLetter", true);
    ui->btnC_->setProperty("btnLetter", true);
    ui->btnV_->setProperty("btnLetter", true);
    ui->btnB_->setProperty("btnLetter", true);
    ui->btnN_->setProperty("btnLetter", true);
    ui->btnM_->setProperty("btnLetter", true);

    ui->btnSpace->setProperty("btnSpace", true);

    ui->btnAt_->setProperty("btnOther", true);
    ui->btnPeriod->setProperty("btnOther", true);
    ui->btnApostrophe->setProperty("btnOther", true);
    ui->btnMinus->setProperty("btnOther", true);
    ui->backspace->setProperty("btnOther", true);

    ui->btnShiftLeft->setProperty("btnFunc", true);
    ui->btnSymbol->setProperty("btnFunc", true);
    ui->btnLanguage->setProperty("btnFunc", true);
    ui->btnShiftRight->setProperty("btnFunc", true);
    ui->btnEnter->setProperty("btnFunc", true);

    ui->btnPageUp->setProperty("btnPage", true);
    ui->btnPageDown->setProperty("btnPage", true);

    g_SelectList << ui->btnSelect1 << ui->btnSelect2 << ui->btnSelect3 << ui->btnSelect4 << ui->btnSelect5
                    << ui->btnSelect6 << ui->btnSelect7 << ui->btnSelect8 << ui->btnSelect9;

    ui->btnSelect1->setProperty("btnSelect", true);
    ui->btnSelect2->setProperty("btnSelect", true);
    ui->btnSelect3->setProperty("btnSelect", true);
    ui->btnSelect4->setProperty("btnSelect", true);
    ui->btnSelect5->setProperty("btnSelect", true);
    ui->btnSelect6->setProperty("btnSelect", true);
    ui->btnSelect7->setProperty("btnSelect", true);
    ui->btnSelect8->setProperty("btnSelect", true);
    ui->btnSelect9->setProperty("btnSelect", true);
}


////焦点改变
//void normalKeyboard::fFocusChanged(QWidget *p_PreceWidget, QWidget *p_CurrentWidget)
//{
//    if(p_CurrentWidget != nullptr && !this->isAncestorOf(p_CurrentWidget))
//    {
//#ifndef __arm__
//        if (p_PreceWidget == 0 && !g_IsFirstClick)
//            return;
//#endif
//        g_IsFirstClick = false;

//        QWidget *p_ModalWidget = QApplication::activeModalWidget();
//        QWidget *p_PopupWidget = QApplication::activePopupWidget();
//        QWidget *p_Widget = QApplication::activeWindow();

//        /*
//        Qt::NonModal       非模态对话框
//        Qt::WindowModal      窗口级模态对话框，即只会阻塞父窗口、父窗口的父窗口及兄弟窗口。（半模态对话框）
//        Qt::ApplicationModal    应用程序级模态对话框，即会阻塞整个应用程序的所有窗口。（半模态对话框）
//        */
//        if(p_ModalWidget !=nullptr && p_ModalWidget->inherits("QDialog"))
//        {
//            Qt::WindowModality Modality = p_ModalWidget->windowModality();

//            if(Qt::ApplicationModal == Modality)
//            {
//                ui->inputEdit = nullptr;
//                g_CurrentFocusType.clear();
//                g_CurrentInputType = "InputUpper";
//                fChangeInputType(g_CurrentInputType);
//                this->setVisible(false);
//                return;
//            }
//        }
//        if(p_CurrentWidget->inherits("QLineEdit"))
//        {
//            ui->inputEdit = (QLineEdit *) p_CurrentWidget;
//            g_CurrentFocusType = "QLineEdit";
//        }
//        else
//        {
//            ui->inputEdit = nullptr;
//            g_CurrentFocusType.clear();
//            g_CurrentInputType = "InputUpper";
//            fChangeInputType(g_CurrentInputType);
//            this->setVisible(false);
//        }
//    }
//}

//void normalKeyboard::mousePressEvent(QMouseEvent *e)
//{
//    if (e->button() == Qt::LeftButton)
//    {
//        g_IsMousePressd = true;
//        g_MousePoint = e->globalPos() - this->pos();
//        e->accept();
//    }
//}

//void normalKeyboard::mouseReleaseEvent(QMouseEvent *)
//{
//    g_IsMousePressd = false;
//}

////事件过滤器
//bool normalKeyboard::eventFilter(QObject *watched, QEvent *event)
//{
//    if(event->type() == QEvent::MouseButtonPress)
//    {
//        if(!g_CurrentFocusType.isEmpty() && watched != ui->btn_keyboard_close)
//        {

//        }
//        p_LongPressedBtn = (QPushButton *)watched;
//        if(fCheakPressType())
//        {
//            g_IsPressed = true;
//            p_LongPressTimer->start(500);
//        }
//        return false;
//    }
//    else if(event->type() == QEvent::MouseButtonRelease)
//    {
//        p_LongPressedBtn = (QPushButton *)watched;
//        if(fCheakPressType())
//        {
//            g_IsPressed = true;
//            p_LongPressTimer->stop();
//        }
//        return false;
//    }
//    else if(event->type() == QEvent::KeyPress)
//    {
//        if(!isVisible())
//            return QWidget::eventFilter(watched, event);
//        QString myStr;
//        QList<QPushButton *>myBtn = this->findChildren<QPushButton *>();
//        foreach(QPushButton *btn, myBtn)
//        {
//            if(btn->text() == myStr)
//            {
//                btn->click();
//                return true;
//            }
//        }
//        return false;
//    }
//    return QWidget::eventFilter(watched, event);
//}

bool normalKeyboard::fCheakPressType()
{
    //只有属于输入法键盘的合法按钮才继续处理
    if (p_LongPressedBtn->property("btnLetter").toBool() || p_LongPressedBtn->property("btnOther").toBool())
        return true;
    else
        return false;
}

//点击
void normalKeyboard::fBtnClick()
{
//    if(g_CurrentFocusType == "")
//        return;
//    else
    {
        QPushButton *myBtn = (QPushButton *)sender();
        QString objectName = myBtn->objectName();
        //qDebug() << "btn click objname:" << objectName;

        if(objectName == "btn_backspace")
            fBackspace();
        else if(objectName == "btn_keyboard_close")
        {
            //this->setVisible(false);
            accept();
        }
        else if(objectName == "btnF_")
        {
            if (g_CurrentInputType == "InputNumber_E" || g_CurrentInputType == "InputNumber_C")
            {
                fInsertStr("&");
            }
            else
            {
                fInsertStr(ui->btnF_->text());
            }
        }
        else if (objectName == "btnShiftRight" || objectName == "btnShiftLeft")
        {
            if (g_CurrentInputType == "InputUpper")
            {
                g_CurrentInputType = "InputLower";
            }
            else if (g_CurrentInputType == "InputLower")
            {
                g_CurrentInputType = "InputUpper";
            }
            fChangeInputType(g_CurrentInputType);
        }
        else if (objectName == "btnLanguage")
        {
            if (g_CurrentInputType == "InputChinese")
            {
                g_CurrentInputType = "InputUpper";
            }
            else if (g_CurrentInputType == "InputNumber_C")
            {
                g_CurrentInputType = "InputNumber_E";
            }
            else if (g_CurrentInputType == "InputNumber_E")
            {
                g_CurrentInputType = "InputNumber_C";
            }
            else if (g_CurrentInputType == "InputUpper" || g_CurrentInputType == "InputLower")
            {
                g_CurrentInputType = "InputChinese";
            }
            fChangeInputType(g_CurrentInputType);
        }
        else if (objectName == "btnSymbol")
        {
            if (g_CurrentInputType == "InputChinese")
            {
                g_CurrentInputType = "InputNumber_C";
            }
            else if (g_CurrentInputType == "InputUpper" || g_CurrentInputType == "InputLower")
            {
                g_CurrentInputType = "InputNumber_E";
            }
            else if (g_CurrentInputType == "InputNumber_E" || g_CurrentInputType == "InputNumber_C")
            {
                g_CurrentInputType = "InputUpper";
            }
            fChangeInputType(g_CurrentInputType);
        }
        else if (objectName == "backspace")
        {
            //如果当前是中文模式,则删除对应拼音,删除完拼音之后再删除对应文本输入框的内容
            if (g_CurrentInputType == "InputChinese")
            {
                QString pinyin = ui->showPinyin->text();
                int len = pinyin.length();
                if (len > 0)
                    ui->showPinyin->setText(pinyin.left(len - 1));
                else
                    fBackspace();
            }
            else
                fBackspace();
        }
        else if (objectName == "btnPageUp" || objectName == "btnPageDown")
        {
            //中文模式才能翻页
            if(objectName == "btnPageUp")
            {
                if(g_ChineseList.count() >= 9 && g_CurrentChineseCount >= 9 )
                   g_CurrentChineseCount -= 9;
            }
            else
            {
                if(g_ChineseList.count() > 9 && g_CurrentChineseCount < g_ChineseList.count() - 9)
                    g_CurrentChineseCount += 9;
            }
            //清空待选按钮
            for(int i = 0; i < 9; i++)
            {
                g_SelectList.at(i)->setText("");
            }
            fShowChinese();
        }
        else if (objectName == "btnSpace")
        {
            if(g_CurrentInputType == "InputChinese")
            {
               if (ui->btnSelect1->text() != "")
               {
                    fInsertStr(ui->btnSelect1->text());
                    ui->showPinyin->clear();
               }
               else
                    fInsertStr(" ");
            }
            else
                fInsertStr(" ");
        }
        else if (objectName == "btnEnter")
        {
            qDebug() << "click enter btn... g_CurrentInputType:" << g_CurrentInputType;
            emit sig_click_enter();
            return;

            if(g_CurrentInputType == "InputChinese")
            {
                if (ui->btnSelect1->text() != "")
                {
                     fInsertStr(ui->btnSelect1->text());
                     ui->showPinyin->clear();
                }
            }
        }
        else
        {
            if(g_CurrentInputType == "InputChinese")
            {
                if(myBtn->property("btnSelect").toBool())
                {
                    QString myStr = myBtn->text();
                    fInsertStr(myStr);
                    ui->showPinyin->setText("");
                }
                else if (myBtn->property("btnOther").toBool())
                {
                    QString myStr = myBtn->text();
                    fInsertStr(myStr);
                }
                else
                {
                    QString myStr = myBtn->text();
                    ui->showPinyin->insert(myStr);
                }

            }
            else
            {
                QString myStr = myBtn->text();
                fInsertStr(myStr);
            }
        }
    }
}

void normalKeyboard::setCurText(const QString &txt, myLineEdit *cur_edit)
{
    ui->inputEdit->setText(txt);

    if (cur_edit)
    {
        ui->inputEdit->setValidator(cur_edit->validator());
    }
}

//void normalKeyboard::showEvent(QShowEvent *event)
//{
//    qDebug() << "normalKeyboard showEvent 1...";
//    QDialog::showEvent(event);

//    if (cur_input_edit)
//    {
//        qDebug() << "normalKeyboard showEvent 2...";
//        cur_input_edit->setFocus();
//        qDebug() << "normalKeyboard showEvent 3...";
//    }

//    qDebug() << "normalKeyboard showEvent 4...";
//}

//插入
void normalKeyboard::fInsertStr(QString str)
{
    //if (g_CurrentFocusType == "QLineEdit")
        ui->inputEdit->insert(str);

        QString cur_txt = ui->inputEdit->text();
        emit sig_update_txt(cur_txt);
}

//退格
void normalKeyboard::fBackspace()
{
    //if (g_CurrentFocusType == "QLineEdit")
        ui->inputEdit->backspace();

        QString cur_txt = ui->inputEdit->text();
        emit sig_update_txt(cur_txt);
}

//长按
void normalKeyboard::fBtnLongClick()
{
    if (g_IsPressed) {
        p_LongPressTimer->setInterval(30); //设置间隔时间
        p_LongPressedBtn->click();
    }
}

//改变字体大小
void normalKeyboard::fChangeKeyboardFontSize(int fontSize)
{
    QFont myFont(this->font().family(), fontSize);
    QList<QPushButton *> myBtn = this->findChildren<QPushButton *>();
    foreach (QPushButton * btn, myBtn)
    {
        btn->setFont(myFont);
    }
    ui->showPinyin->setFont(myFont);
}

//设置键盘的大小和位置(当前点击的lineedit,键盘的位置，键盘的宽度，键盘的高度，键盘中pushbutton的大小)
void normalKeyboard::fResetKeyboardSize(bool isChineseInput)
{
    //qDebug() << "numberKeyboard fResetKeyboardSize" << g_DeskWidth << g_DeskHeight;

    int fontSize = 18;
    int m_width = 800;
    int m_height = (isChineseInput ? 310 : 310);

    QFont font;
    font.setPointSize(25);
    ui->inputEdit->setFont(font);
    ui->inputEdit->setVisible(false);

    fChangeKeyboardFontSize(fontSize);

    QSize myIconSize(m_height / 6, m_height / 6);

    ui->btnShiftLeft->setIconSize(myIconSize);
    ui->btnShiftRight->setIconSize(myIconSize);
    ui->backspace->setIconSize(myIconSize);
    ui->btn_keyboard_close->setIconSize(myIconSize);
    ui->btnLanguage->setIconSize(myIconSize);
    ui->btnEnter->setIconSize(myIconSize);

    int x = (g_DeskWidth - m_width) / 2;
    int y = g_DeskHeight - m_height;

    this->setFixedSize(m_width, m_height);
    this->move(x, y);

//    //根据用户选择的输入法位置设置-居中显示-底部填充-显示在输入框正下方等方式
//    if (style == "center")
//    {
//        QPoint pos = QPoint(g_DeskWidth/2 - width/2, g_DeskHeight/2 - height/2);
//        this->setGeometry(pos.x(), pos.y(), width, height);
//    }
//    else if (style == "bottom")
//    {
//        this->setGeometry(0, g_DeskHeight - height, g_DeskWidth, height);
//    }
//    else if (style == "leftdown")
//    {
//        QRect rect = lineedit->rect();
//        QPoint pos = QPoint(rect.left(), rect.bottom() + 2);
//        pos = lineedit->mapToGlobal(pos);//将窗口坐标转换成桌面坐标
//        this->setGeometry(pos.x(), pos.y(), width, height);
//    }
//    else if (style == "leftup")
//    {
//        QRect rect = lineedit->rect();
//        QPoint pos = QPoint(rect.left(), rect.top()- height - 2);
//        pos = lineedit->mapToGlobal(pos);
//        this->setGeometry(pos.x(), pos.y(), width, height);
//    }
//    else if (style == "rightup")
//    {
//        QRect rect = lineedit->rect();
//        QPoint pos = QPoint(rect.right() - width, rect.top() - height - 2);
//        pos = lineedit->mapToGlobal(pos);
//        this->setGeometry(pos.x(), pos.y(), width, height);
//    }
//    else if (style == "rightdown")
//    {
//        QRect rect = lineedit->rect();
//        QPoint pos = QPoint(rect.right() - width, rect.bottom() + 2);
//        pos = lineedit->mapToGlobal(pos);
//        this->setGeometry(pos.x(), pos.y(), width, height);
//    }
}

//改变键盘类型
void normalKeyboard::fChangeInputType(QString type)
{
    g_ChineseList.clear();
    if (type == "InputUpper")
    {
        ui->selectWidget->hide();
        ui->btnA_->setText("A");
        ui->btnB_->setText("B");
        ui->btnC_->setText("C");
        ui->btnD_->setText("D");
        ui->btnE_3->setText("E");
        ui->btnF_->setText("F");
        ui->btnG_->setText("G");
        ui->btnH_->setText("H");
        ui->btnI_8->setText("I");
        ui->btnJ_->setText("J");
        ui->btnK_->setText("K");
        ui->btnL_->setText("L");
        ui->btnM_->setText("M");
        ui->btnN_->setText("N");
        ui->btnO_9->setText("O");
        ui->btnP_0->setText("P");
        ui->btnQ_1->setText("Q");
        ui->btnR_4->setText("R");
        ui->btnS_->setText("S");
        ui->btnT_5->setText("T");
        ui->btnU_7->setText("U");
        ui->btnV_->setText("V");
        ui->btnW_2->setText("W");
        ui->btnX_->setText("X");
        ui->btnY_6->setText("Y");
        ui->btnZ_->setText("Z");

        ui->btnAt_->setText("@");
        ui->btnPeriod->setText(".");
        ui->btnApostrophe->setText("'");
        ui->btnMinus->setText("-");

        ui->btnSpace->setText(tr("English"));

        ui->btnShiftLeft->setEnabled(true);
        ui->btnShiftRight->setEnabled(true);
        ui->btnShiftLeft->setIcon(QPixmap(":/images/shift_checked.png"));
        ui->btnShiftRight->setIcon(QPixmap(":/images/shift_checked.png"));

    }
    else if (type == "InputLower")
    {
        ui->selectWidget->hide();
        ui->btnA_->setText("a");
        ui->btnB_->setText("b");
        ui->btnC_->setText("c");
        ui->btnD_->setText("d");
        ui->btnE_3->setText("e");
        ui->btnF_->setText("f");
        ui->btnG_->setText("g");
        ui->btnH_->setText("h");
        ui->btnI_8->setText("i");
        ui->btnJ_->setText("j");
        ui->btnK_->setText("k");
        ui->btnL_->setText("l");
        ui->btnM_->setText("m");
        ui->btnN_->setText("n");
        ui->btnO_9->setText("o");
        ui->btnP_0->setText("p");
        ui->btnQ_1->setText("q");
        ui->btnR_4->setText("r");
        ui->btnS_->setText("s");
        ui->btnT_5->setText("t");
        ui->btnU_7->setText("u");
        ui->btnV_->setText("v");
        ui->btnW_2->setText("w");
        ui->btnX_->setText("x");
        ui->btnY_6->setText("y");
        ui->btnZ_->setText("z");

        ui->btnAt_->setText("@");
        ui->btnPeriod->setText(".");
        ui->btnApostrophe->setText("'");
        ui->btnMinus->setText("-");

        ui->btnSpace->setText(tr("English"));

        ui->btnShiftLeft->setEnabled(true);
        ui->btnShiftRight->setEnabled(true);
        ui->btnShiftLeft->setIcon(QPixmap(":/images/shift_double_checked.png"));
        ui->btnShiftRight->setIcon(QPixmap(":/images/shift_double_checked.png"));
    }
    else if (type == "InputChinese")
    {
        ui->selectWidget->show();
        ui->btnA_->setText("a");
        ui->btnB_->setText("b");
        ui->btnC_->setText("c");
        ui->btnD_->setText("d");
        ui->btnE_3->setText("e");
        ui->btnF_->setText("f");
        ui->btnG_->setText("g");
        ui->btnH_->setText("h");
        ui->btnI_8->setText("i");
        ui->btnJ_->setText("j");
        ui->btnK_->setText("k");
        ui->btnL_->setText("l");
        ui->btnM_->setText("m");
        ui->btnN_->setText("n");
        ui->btnO_9->setText("o");
        ui->btnP_0->setText("p");
        ui->btnQ_1->setText("q");
        ui->btnR_4->setText("r");
        ui->btnS_->setText("s");
        ui->btnT_5->setText("t");
        ui->btnU_7->setText("u");
        ui->btnV_->setText("v");
        ui->btnW_2->setText("w");
        ui->btnX_->setText("x");
        ui->btnY_6->setText("y");
        ui->btnZ_->setText("z");

        ui->btnAt_->setText("@");
        ui->btnPeriod->setText("。");
        ui->btnApostrophe->setText("’");
        ui->btnMinus->setText("-");

        ui->btnSpace->setText(tr("中文"));
        ui->btnShiftLeft->setEnabled(false);
        ui->btnShiftRight->setEnabled(false);
        ui->btnShiftLeft->setIcon(QPixmap(":/images/shift.png"));
        ui->btnShiftRight->setIcon(QPixmap(":/images/shift.png"));

    }
    else if (type == "InputNumber_E")
    {
        ui->selectWidget->hide();
        ui->btnA_->setText("@");
        ui->btnB_->setText(",");
        ui->btnC_->setText("<");
        ui->btnD_->setText("%");
        ui->btnE_3->setText("3");
        ui->btnF_->setText("&&");
        ui->btnG_->setText("*");
        ui->btnH_->setText("-");
        ui->btnI_8->setText("8");
        ui->btnJ_->setText("+");
        ui->btnK_->setText("(");
        ui->btnL_->setText(")");
        ui->btnM_->setText(";");
        ui->btnN_->setText(":");
        ui->btnO_9->setText("9");
        ui->btnP_0->setText("0");
        ui->btnQ_1->setText("1");
        ui->btnR_4->setText("4");
        ui->btnS_->setText("#");
        ui->btnT_5->setText("5");
        ui->btnU_7->setText("7");
        ui->btnV_->setText(">");
        ui->btnW_2->setText("2");
        ui->btnX_->setText("\"");
        ui->btnY_6->setText("6");
        ui->btnZ_->setText("!");

        ui->btnAt_->setText("@");
        ui->btnPeriod->setText(".");
        ui->btnApostrophe->setText("'");
        ui->btnMinus->setText("-");

        ui->btnSpace->setText(tr("English"));

        ui->btnShiftLeft->setEnabled(false);
        ui->btnShiftRight->setEnabled(false);
        ui->btnShiftLeft->setIcon(QPixmap(":/images/shift.png"));
        ui->btnShiftRight->setIcon(QPixmap(":/images/shift.png"));
    }
    else if (type == "InputNumber_C")
    {
        ui->selectWidget->hide();
        ui->btnA_->setText("@");
        ui->btnB_->setText("，");
        ui->btnC_->setText("《");
        ui->btnD_->setText("%");
        ui->btnE_3->setText("3");
        ui->btnF_->setText(tr("&&"));
        ui->btnG_->setText("*");
        ui->btnH_->setText("_");
        ui->btnI_8->setText("8");
        ui->btnJ_->setText("+");
        ui->btnK_->setText("（");
        ui->btnL_->setText("）");
        ui->btnM_->setText("；");
        ui->btnN_->setText("：");
        ui->btnO_9->setText("9");
        ui->btnP_0->setText("0");
        ui->btnQ_1->setText("1");
        ui->btnR_4->setText("4");
        ui->btnS_->setText("#");
        ui->btnT_5->setText("5");
        ui->btnU_7->setText("7");
        ui->btnV_->setText("》");
        ui->btnW_2->setText("2");
        ui->btnX_->setText("”");
        ui->btnY_6->setText("6");
        ui->btnZ_->setText("！");

        ui->btnAt_->setText("@");
        ui->btnPeriod->setText("。");
        ui->btnApostrophe->setText("’");
        ui->btnMinus->setText("@v@");

        ui->btnSpace->setText(tr("中文"));
        ui->btnShiftLeft->setEnabled(false);
        ui->btnShiftRight->setEnabled(false);
        ui->btnShiftLeft->setIcon(QPixmap(":/images/shift.png"));
        ui->btnShiftRight->setIcon(QPixmap(":/images/shift.png"));
    }

    ui->showPinyin->clear();

    //fResetKeyboardSize((type == "InputChinese"));
}

void normalKeyboard::on_showPinyin_textChanged(const QString &arg1)
{
    //qDebug() << "normalKeyboard on_showPinyin_textChanged" << arg1;

    g_CurrentChineseCount = 0;
    PinyinSql mypinyin;
    g_ChineseList.clear();
    for(int i = 0; i < 9; i++)
    {
        g_SelectList.at(i)->setText("");
    }

    QString exePath = QCoreApplication::applicationFilePath();
    QFileInfo fileInfo(exePath);
    QString dirPath = fileInfo.absolutePath();
    //qDebug() << "dirPath" << dirPath;

    //mypinyin.fOpenPinyinSql(dirPath + "/Database/pinyin.db");

    g_ChineseList = mypinyin.fSearchPinyinData(arg1);
    //mypinyin.fCloseSql();

    fShowChinese();
}

void normalKeyboard::fShowChinese()
{
    int j = 0;
    for(int i = g_CurrentChineseCount; i < g_ChineseList.count(); i++)
    {
        if(j < 9)
        {
            g_SelectList.at(j)->setText(g_ChineseList.at(i));
            j++;
        }
    }
}
