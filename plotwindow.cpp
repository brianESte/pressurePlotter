#include "plotwindow.h"
#include "ui_plotwindow.h"
#include "/usr/local/include/qcustomplot/qcustomplot.h"
#include <QHBoxLayout>

PlotWindow::PlotWindow(std::vector<DataPoint> data_points, float p_min, float p_max,
                       QWidget *parent) : QWidget(parent)/*, ui_pw(new Ui::PlotWindow)*/{
    //ui_pw->setupUi(this);
    //setObjectName("pressure_plot_window_n");
    setWindowTitle("Pressure Plotter");
    customPlot = new QCustomPlot(this);
    QBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(customPlot);
    resize(750, 320);


    // sometimes an error appears on the line above. The prgm still compiles/runs so I am ignoring it...
    this->p_min = p_min;
    this->p_max = p_max;

    //QCustomPlot *customPlot = ui_pw->customPlot;

    int n_data_points = data_points.size();     // number of data_points

    // get the full time and dates of the first and last data points:
    QDateTime temp_date_time = QDateTime::fromString(QString::fromStdString(data_points[0].time_stamp), "yyyy-MM-dd HH:mm:ss");
    double time_0 = temp_date_time.toSecsSinceEpoch();
    QDate start_date = temp_date_time.date();
    temp_date_time = QDateTime::fromString(QString::fromStdString(data_points[n_data_points-1].time_stamp), "yyyy-MM-dd HH:mm:ss");
    double time_final = temp_date_time.toSecsSinceEpoch();
    QDate end_date = temp_date_time.date();

    // write the title, based on the start and end dates of the pressure recording
    QString title;
    if(start_date == end_date){
        title = "Pressure Recording on " + start_date.toString();
    } else {
        title = "Pressure Recording from "+start_date.toString() + " to " + end_date.toString();
    }

    // create graph and assign data to it:
    customPlot->addGraph();
    QVector<QCPGraphData> timeData(n_data_points);
    //QString date_time_str;   // = "2021-05-11 12:38:35";

    for(int i = 0; i < n_data_points; i++){
        temp_date_time = QDateTime::fromString(QString::fromStdString(data_points[i].time_stamp), "yyyy-MM-dd HH:mm:ss");
        timeData[i].key = temp_date_time.toSecsSinceEpoch();
        timeData[i].value = data_points[i].pressure;
    }
    customPlot->graph()->data()->set(timeData);
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setTickCount(2*dateTicker->tickCount());    // just double the default / auto-set number of ticks
    dateTicker->setDateTimeFormat("dd.MM.yy HH:mm");
    customPlot->xAxis->setTicker(dateTicker);
    // setup x axis:
    customPlot->xAxis->setLabel("Time");
    customPlot->xAxis->setTickLabelRotation(-30);
    //customPlot->xAxis->setNumberPrecision(12);    // it... didnt seem to have an effect
    customPlot->xAxis->setRange(time_0, time_final);
    // setup y axis:
    customPlot->yAxis->setLabel("Pressure [mbar]");
    customPlot->yAxis->setRange(p_min, p_max);
    customPlot->yAxis->setSelectableParts(QCPAxis::spAxisLabel);
    // set title
    customPlot->plotLayout()->insertRow(0);
    QCPTextElement *title_obj = new QCPTextElement(customPlot, title);
    title_obj->setSelectable(true);
    customPlot->plotLayout()->addElement(0, 0, title_obj);   // , QFont("sans", 12, QFont::Bold)
    // connect a dblclick on the title_obj to the appropriate function
    connect(title_obj, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

    // connect the signal for dblclicking on an axis label
    connect(customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));

    //customPlot->setInteraction(QCP::iRangeDrag, true);
    // options: QCP::iRangeZoom, QCP::iSelectPlottables, QCP::iSelectLegend, QCP::iSelectItems, QCP::iSelectItems, |QCP::iSelectPlottablesBeyondAxisRect
    customPlot->setInteractions(QCP::iSelectAxes|QCP::iSelectOther);
    customPlot->replot();
}

PlotWindow::~PlotWindow(){  /*delete ui_pw;*/  }

void PlotWindow::titleDoubleClick(QMouseEvent* event){
    Q_UNUSED(event)
    if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender())){
        // Set the plot title by double clicking on it
        bool ok;
        QString newTitle = QInputDialog::getText(this, "Edit Title", "New plot title:", QLineEdit::Normal, title->text(), &ok);
        if (ok){
          title->setText(newTitle);
          customPlot->replot();
        }
    }
}

void PlotWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part){
    // Set an axis label by double clicking on it
    // only react when the actual axis label is clicked, not tick label or axis backbone
    if (part == QCPAxis::spAxisLabel){
        bool ok;
        QString newLabel = QInputDialog::getText(this, "Edit Axis Label", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
        if (ok){
          axis->setLabel(newLabel);
          customPlot->replot();
        }
    }
}

