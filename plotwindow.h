#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QWidget>
#include "datapoint.h"
#include "/usr/local/include/qcustomplot/qcustomplot.h"

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QWidget {
    Q_OBJECT

public:
    explicit PlotWindow(std::vector<DataPoint>, float, float, QWidget *parent = nullptr);
    ~PlotWindow();

private slots:
      void titleDoubleClick(QMouseEvent *event);
      void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);

private:
    //Ui::PlotWindow *ui_pw;
    QCustomPlot *customPlot;
    //QString title;
    float p_min;
    float p_max;
};

#endif // PLOTWINDOW_H
