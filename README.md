# Pressure Plotter - "Druckprogramm"

## Objective:

Create a program to open a data file of pressure values and plot them. It should:

- Be as simple as possible to use
- Open a text file of pressure values
- Plot said pressure values
- Have a user-settable pressure range
- Have a user-settable plot title
- Be usable without external libraries / software

This project is built on the QtWidget [framework/library](https://doc.qt.io/qt-5/classes.html), as well as QCustomPlot. Ìnstalling QCustomPlot is fairly simple. Instructions can be [found here](https://www.qcustomplot.com/index.php/tutorials/settingup). Aside from the auto-generated content and the functions to edit the axis labels and plot title, the code is my own. 

## How to Use the Program

Open your pressure data file via the "Open File" button, and subsequent dialog window. Plot the data via the "Plot Data" button. The lower and upper pressure limits can be set on the start window, and will affect the next generated plot. 

## Editing the Plot

The title and axis labels can be edited by double clicking on them.

