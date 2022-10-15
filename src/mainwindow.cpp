#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSFML::CanvasSettings settings;

    m_canvas = new QSFML::Canvas(ui->canvasDisplay,settings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

