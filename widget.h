#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


private:
    void initUI();
    void initSingleMsgWidget();
    void initMulitMsgWidget();

    QString toCreateCanMsg(unsigned int spn,unsigned int fmi);

private slots:
    void slotFmiInChanged();
    void slotSpnInChanged();
    void slotCanMsgInChanged();
    void slotToClose();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
