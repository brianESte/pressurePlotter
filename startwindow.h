#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "datapoint.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

class StartWindow : public QMainWindow {
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private slots:
    //void on_plot_btn_clicked();
    //void on_open_file_btn_clicked();

private:
    //Ui::StartWindow *ui;
    QHBoxLayout *button_row, *file_row, *pressure_row;
    QPushButton *open_file_btn, *plot_btn, *help_btn;
    QDoubleSpinBox *p_lim_lower, *p_lim_upper;
    QLabel *f_status_label;

    void open_file_dialog();
    void plot_data();
    void help_window();
    //std::string base_name(std::string);
    float str_to_pressure(QString);

    QString filename;
    //QString disp_name;
    std::vector<DataPoint> data_points;
};
#endif // STARTWINDOW_H
