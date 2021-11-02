#include "startwindow.h"
#include "plotwindow.h"
#include "/usr/local/include/qcustomplot/qcustomplot.h"
//#include <iostream>

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent){
    setObjectName("MainWindow_name");
    setWindowTitle("Pressure Plotter 1");

    QWidget *central_widget = new QWidget(this);
    //central_widget->setObjectName("central_widget");

    QVBoxLayout *main_layout = new QVBoxLayout;
    // button row
    button_row = new QHBoxLayout;
    main_layout->addLayout(button_row);
    open_file_btn = new QPushButton("Open File");
    button_row->addWidget(open_file_btn);
    plot_btn = new QPushButton("Plot Data");
    button_row->addWidget(plot_btn);
    help_btn = new QPushButton("Help");
    button_row->addWidget(help_btn);
    // file row
    file_row = new QHBoxLayout;
    main_layout->addLayout(file_row);
    QLabel *f_label = new QLabel("File: ");
    file_row->addWidget(f_label);
    f_status_label = new QLabel("-");
    file_row->addWidget(f_status_label);
    // pressure limits row
    pressure_row = new QHBoxLayout;
    main_layout->addLayout(pressure_row);
    QLabel *p_label = new QLabel("Pressure limits [mbar]: ");
    pressure_row->addWidget(p_label);
    p_lim_lower = new QDoubleSpinBox;
    p_lim_lower->setDecimals(1);
    p_lim_lower->setSingleStep(0.1);
    p_lim_lower->setValue(0.3);
    pressure_row->addWidget(p_lim_lower);
    p_lim_upper = new QDoubleSpinBox;
    p_lim_upper->setDecimals(1);
    p_lim_upper->setSingleStep(0.1);
    p_lim_upper->setValue(1.1);
    pressure_row->addWidget(p_lim_upper);

    central_widget->setLayout(main_layout);
    setCentralWidget(central_widget);

    // connect each button to the appropriate function.
    // connect(object pointer, action?, signal receiver, what to execute for that signal)
    connect(open_file_btn, &QAbstractButton::clicked, this, &StartWindow::open_file_dialog);
    connect(plot_btn, &QAbstractButton::clicked, this, &StartWindow::plot_data);
    connect(help_btn, &QAbstractButton::clicked, this, &StartWindow::help_window);
}

// default destructor
StartWindow::~StartWindow(){    }

/**
 * @brief StartWindow::str_to_pressure
 * @param input_str Pressure as a string of form #.#
 * @return          Pressure as a float
 */
float StartWindow::str_to_pressure(QString input_str){
    float pressure = input_str.at(0).digitValue();
    pressure += (float)(input_str.at(2).digitValue())/10.0;
    return pressure;
}

// open file and process data in file
/**
 * @brief StartWindow::open_file_dialog
 * Open the file dialog, then process the chosen file. If the target line ";DU20;NOSEN" is detected,
 * process the data to the end of the file, storing it in a vector of data_points. Otherwise close the file
 * And let the user know that the file was deemed invalid.
 */
void StartWindow::open_file_dialog(){
    filename = QFileDialog::getOpenFileName(this, "Open Data File");
    //filename = "sample_ch1.txt";

    QFile data_file(filename);
    if(!data_file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: "+data_file.errorString());
        return;
    }

    QTextStream in(&data_file);
    QString line_buffer = "";
    in.readLineInto(&line_buffer);

    // line counter to prevent an infinite loop.
    uint16_t line_ctr = 0;
    while( !line_buffer.startsWith(";DU200;NOSEN") && line_ctr < 50){
        in.readLineInto(&line_buffer);
        line_ctr++;
    }
    if( !line_buffer.startsWith(";DU200;NOSEN")){
        QMessageBox::warning(this, "Warning", "No valid data found in file. If this was a valid data file, contact program creator for update");
        data_file.close();
        return;
    }
    in.readLineInto(&line_buffer);

    // is the pressure on pos 20 or 25?
    uint16_t pressure_start_pos;
    if(line_buffer[20] == '.'){
        pressure_start_pos = 25;
    } else {
        pressure_start_pos = 20;
    }

    do{
        DataPoint *new_pt = new DataPoint;
        new_pt->time_stamp = line_buffer.left(19).toStdString();
        new_pt->pressure = str_to_pressure(line_buffer.mid(pressure_start_pos, 3));
        data_points.push_back(*new_pt);
    }while(in.readLineInto(&line_buffer));

    data_file.close();

    // delete redundant middle values:
    // ie, given [8, 9, 7, 9, 9, 9, 9, 10] -> [8, 9, 7, 9, 9, 10]
    float first = data_points[0].pressure;
    float second = data_points[1].pressure;
    uint16_t data_len = data_points.size();

    uint16_t i = 2;
    while(i < data_len){
        if(first == second && second == data_points[i].pressure){
            second = data_points[i].pressure;
            data_points.erase(data_points.begin()+i-1);
            data_len--;
        } else {
            first = second;
            second = data_points[i].pressure;
            i++;
        }
    }

    // display filename on startWindow
    f_status_label->setText(filename);
}

/**
 * @brief StartWindow::plot_data
 * create/run a PlotWindow when the plot button is clicked
 */
void StartWindow::plot_data(){
    //std::cout << "plot_data() called" << std::endl;
    if(!data_points.size()){    return; }

    // grab the pressure limits from the spinboxes
    float p_min = p_lim_lower->value();
    float p_max = p_lim_upper->value();
    PlotWindow *plot_window = new PlotWindow(data_points, p_min, p_max);
    plot_window->show();
}

// help (and about) button
void StartWindow::help_window(){
    QMessageBox::about(nullptr, "Help / About", "To use: \n\
1. Click on \"Open File\" to load a data file. \n\
2. Set the pressure limits of the plot with the number inputs below. \n\
3. Click \"Plot Data\" to plot the data. \n\
\nTo modify data plot: \n\
Double click on title or axis label to edit.\n\
\nProblem? Contact: stephensonbr5@gmail.com");
}
