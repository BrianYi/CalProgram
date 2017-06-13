#ifndef CALPROGRAM_H
#define CALPROGRAM_H

#include <QWidget>

#define MUL_NUM 30
#define DEV_NUM 30
#define MOD_NUM 30
#define MUL_DEV_MOD_NUM (MUL_NUM + DEV_NUM + MOD_NUM)

#define MUL_RANGE_MIN   0
#define MUL_RANGE_MAX   50
#define MUL_RANGE       (MUL_RANGE_MAX - MUL_RANGE_MIN)

#define DEV_RANGE_MIN   0
#define DEV_RANGE_MAX   99
#define DEV_RANGE       (DEV_RANGE_MAX - DEV_RANGE_MIN)

#define MOD_RANGE_MIN   0
#define MOD_RANGE_MAX   99
#define MOD_RANGE       (MOD_RANGE_MAX - MOD_RANGE_MIN)

#define TIME_LIMIT_SEC  30 * 60

#define COL_NUM 5

class QLabel;
class QLineEdit;
class QPushButton;
class QFont;
class QTimer;
class QLCDNumber;
class CalProgram : public QWidget
{
    Q_OBJECT
public:
    enum
    {
        Mul,
        Dev,
        Mod,
        Num
    };
public:
    CalProgram(QWidget *parent = 0);
    ~CalProgram();
    public slots:
        void submit();
        void generateCalculations();
        void timeTick();
private:
    void resetAll();
    bool generateXY(int *a, int *b, int tp);
    int                 expVal[MUL_DEV_MOD_NUM];
    QLabel              *expNumberList;
    QLabel              *expLableList;
    QLineEdit           *valLineEditList;
    QLabel              *msgLabelList;
    QPushButton         *submitPushButton;
    QPushButton         *closePushButton;
    QPushButton         *regenPushButton;
    QLabel              *gradeLabel;
    QLabel              *timeLabel;
    QLCDNumber          *timeLCD;
    QFont               *font;
    QTimer              *timer;
    bool                isAlreadySubmit;
    bool                isFirstTime;
    int                 iLeftSecs;
};

#endif // CALPROGRAM_H
