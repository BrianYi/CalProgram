#include "CalProgram.h"
#include <time.h>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFont>
#include <QFontMetrics>

CalProgram::CalProgram(QWidget *parent)
    : QWidget(parent)
{
    srand(time(0));
    QGridLayout *gridLayout = new QGridLayout;
    expNumberList   = new QLabel[MUL_DEV_MOD_NUM];
    expLableList    = new QLabel[MUL_DEV_MOD_NUM];
    valLineEditList = new QLineEdit[MUL_DEV_MOD_NUM];
    msgLabelList    = new QLabel[MUL_DEV_MOD_NUM];
    submitPushButton = new QPushButton(tr("提交"),this);
    closePushButton = new QPushButton(tr("关闭"),this);
    regenPushButton  = new QPushButton(tr("重新生成"),this);
    gradeLabel  = new QLabel(tr("<b>分数：0</b>"), this);
    font    = new QFont("Times", 13);
    isAlreadySubmit = false;
    for (int i = 0; i < MUL_DEV_MOD_NUM; i++) {
        QHBoxLayout *hLayout    = new QHBoxLayout;
        expNumberList[i].setFont(*font);
        expLableList[i].setFont(*font);
        valLineEditList[i].setFont(*font);
        msgLabelList[i].setFont(*font);
        hLayout->addWidget(&expNumberList[i],0,Qt::AlignLeft);
        hLayout->addWidget(&expLableList[i],0,Qt::AlignLeft);
        hLayout->addWidget(&valLineEditList[i],0,Qt::AlignLeft);
        hLayout->addWidget(&msgLabelList[i],0,Qt::AlignLeft);
        gridLayout->addLayout(hLayout, i / COL_NUM, i % COL_NUM);
    }
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(regenPushButton);
    hLayout->addStretch();
    hLayout->addWidget(gradeLabel);
    hLayout->addStretch();
    hLayout->addWidget(submitPushButton);
    hLayout->addWidget(closePushButton);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(gridLayout);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
    generateCalculations();
    setWindowTitle(tr("数学计算习题"));
    setWindowIcon(QIcon(":/Resources/app.ico"));
    connect(submitPushButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(regenPushButton, SIGNAL(clicked()), this, SLOT(generateCalculations()));
    connect(closePushButton, SIGNAL(clicked()), this, SLOT(close()));
}

CalProgram::~CalProgram()
{
    delete [] expNumberList;
    delete [] expLableList;
    delete [] valLineEditList;
    delete [] msgLabelList;
    delete submitPushButton;
    delete closePushButton;
    delete gradeLabel;
    delete regenPushButton;
}

void CalProgram::generateCalculations()
{
    resetAll();
    int iMulNumber = 0, iDevNumber = 0, iModNumber = 0, iNumber = 1;
    int iExpNumberWidth = expNumberList[0].fontMetrics().width(tr("(00)")),
        iExpWidth = expNumberList[0].fontMetrics().width(tr("(int)99 / 99 = ")),
        iValWidth = expNumberList[0].fontMetrics().width(tr("99999")),
        iMsgWidth = expNumberList[0].fontMetrics().width(tr("XX"));

    while (iMulNumber < MUL_NUM ||
        iDevNumber < DEV_NUM ||
        iModNumber < MOD_NUM) 
    {
        int qType = rand() % Num;
        int x, y, val;
        char qBuf[128], exp[128];
        bool bSuccess = true;
        while (!generateXY(&x, &y, qType));
        switch (qType) 
        {
        case Mul:
            if (iMulNumber >= MUL_NUM)
            {
                bSuccess = false;
                break;
            }
            val = x * y;
            sprintf(exp, "%d * %d =", x, y);
            iMulNumber++;
            break;
        case Dev:
            if (iDevNumber >= DEV_NUM)
            {
                bSuccess = false;
                break;
            }
            val = x / y;
            sprintf(exp, "(int)%d / %d =", x, y);
            iDevNumber++;
            break;
        case Mod:
            if (iModNumber >= MOD_NUM)
            {
                bSuccess = false;
                break;
            }
            val = x % y;
            sprintf(exp, "%d %% %d =", x, y);
            iModNumber++;
            break;
        default:
            break;
        }
        if (!bSuccess)
            continue;
        expNumberList[iNumber-1].setText(tr("<b>(") + QString::number(iNumber) + tr(")</b>"));
        expNumberList[iNumber-1].setFixedWidth(/*30*/iExpNumberWidth);
        expLableList[iNumber-1].setText(exp);
        expLableList[iNumber-1].setFixedWidth(/*120*/iExpWidth);
        expLableList[iNumber-1].setAlignment(Qt::AlignRight);
        valLineEditList[iNumber-1].setFixedWidth(/*50*/iValWidth);
        valLineEditList[iNumber-1].setAlignment(Qt::AlignHCenter);
        msgLabelList[iNumber-1].setFixedWidth(/*50*/iMsgWidth);
        expVal[iNumber-1]    = val;
        iNumber++;
    }
}

bool CalProgram::generateXY(int *a, int *b, int tp)
{
    int x = 0, y = 0;
    switch (tp) {
    case Mul:
        x   = MUL_RANGE_MIN + rand() % MUL_RANGE;
        y   = MUL_RANGE_MIN + rand() % MUL_RANGE;
        //*******************************
        // x 或 y 不能为0
        if (!x || !y) {
            return false;
        }
        //*******************************
        // 30以上不能有33 * 39这样的题
        if (x > 30 && y > 30) {
            if (x % 10 && y % 10)
                return false;
        }
        break;
    case Dev:
        x   = DEV_RANGE_MIN + rand() % DEV_RANGE;
        y   = DEV_RANGE_MIN + rand() % DEV_RANGE;
        //*******************************
        // y不能为0
        if (!y) {
            return false;
        }
        break;
    case Mod:
        x   = MOD_RANGE_MIN + rand() % MOD_RANGE;
        y   = MOD_RANGE_MIN + rand() % MOD_RANGE;
        //*******************************
        // 
        if (!y) {
            return false;
        }
        break;
    default:
    	break;
    }
    *a  = x;
    *b  = y;
    return true;
}

void CalProgram::submit()
{
    int iNotCompleteNum = 0;
    for (int i = 0; i < MUL_DEV_MOD_NUM; i++) {
        if (valLineEditList[i].text().isEmpty()) {
            iNotCompleteNum++;
        }
    }
    if (iNotCompleteNum) {
        QMessageBox::information(0, tr("警告"), tr("您有<b>%1</b>道题目未做完!").arg(iNotCompleteNum));
        return ;
    }

    int iCorrectNum = 0;
    for (int i = 0; i < MUL_DEV_MOD_NUM; i++) {
        if (valLineEditList[i].text().toInt() != expVal[i]) {
            msgLabelList[i].setStyleSheet("color:red;");
            msgLabelList[i].setText(tr("<b>X</b>"));
        }
        else 
        {
            msgLabelList[i].setStyleSheet("color:blue;");
            msgLabelList[i].setText(tr("<b>√</b>"));  
            iCorrectNum++;
        }
    }
    if (!isAlreadySubmit)
        gradeLabel->setText(tr("<b>分数：%1</b>").arg(iCorrectNum));
    isAlreadySubmit = true;
}

void CalProgram::resetAll()
{
    for (int i = 0; i < MUL_DEV_MOD_NUM; i++) {
        expVal[i]   = 0;
        expNumberList[i].clear();
        expLableList[i].clear();
        valLineEditList[i].clear();
        msgLabelList[i].clear();
        gradeLabel->setText(tr("<b>分数：0</b>"));
    }
    isAlreadySubmit = false;
}


