#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CameraController.h"
#include "BackgroundGrid.h"

#include "LogicGates.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ribbon = new EditorRibbon(ui->ribbonTabWidget);
    ConnectionButtons conButt = m_ribbon->getConnectionButtons();
    connect(conButt.addConnection, &QToolButton::clicked, this, &MainWindow::onAddConnection);
    connect(conButt.removeConnection, &QToolButton::clicked, this, &MainWindow::onRemoveConnection);

    AddGateButtons addGateButt = m_ribbon->getAddGateButtons();
    connect(addGateButt.addInputGate, &QToolButton::clicked, this, &MainWindow::onAddInputGate);
    connect(addGateButt.addClock, &QToolButton::clicked, this, &MainWindow::onAddClock);
    connect(addGateButt.addAndGate, &QToolButton::clicked, this, &MainWindow::onAddAndGate);
    connect(addGateButt.addOrGate, &QToolButton::clicked, this, &MainWindow::onAddOrGate);
    connect(addGateButt.addXorGate, &QToolButton::clicked, this, &MainWindow::onAddXorGate);
    connect(addGateButt.addNotGate, &QToolButton::clicked, this, &MainWindow::onAddNotGate);
    connect(addGateButt.remove, &QToolButton::clicked, this, &MainWindow::onRemoveGate);

    QSFML::CanvasSettings settings;

    m_canvas = new QSFML::Canvas(ui->canvasDisplay,settings);

    QSFML::Objects::CameraController *cam  = new QSFML::Objects::CameraController("cam");
    QSFML::Objects::BackgroundGrid   *grid = new QSFML::Objects::BackgroundGrid("grid");
    m_canvas->addObject(grid);
    m_canvas->addObject(cam);
    cam->setDragButton(sf::Mouse::Button::Middle);

    //Pin *pin = new Pin("MyPin");
    //m_canvas->addObject(pin);

   //AndGate *gate1 = new AndGate("MyAndGate");
   //gate1->setInputCount(2);
   //gate1->setPosition(sf::Vector2f(100,0));
   //gate1->enableMouseDrag(true);

   //AndGate *gate2 = new AndGate("MyAndGate");
   //gate2->setInputCount(2);
   //gate2->setPosition(sf::Vector2f(150,30));
   //gate2->enableMouseDrag(true);



   //m_canvas->addObject(gate1);
   //m_canvas->addObject(gate2);


    QSFML::Objects::CanvasObject *keyEvents = new QSFML::Objects::CanvasObject("KeyEvents");
    m_escKey = new QSFML::Components::KeyPressEvent("Esc Key",sf::Keyboard::Key::Escape);
    connect(m_escKey, &QSFML::Components::KeyPressEvent::fallingEdge,
            this, &MainWindow::onEscapePressed);
    keyEvents->addComponent(m_escKey);
    m_canvas->addObject(keyEvents);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onAddConnection()
{
    EditingTool::setCurrentTool(EditingTool::Tool::addConnection);
}

void MainWindow::onRemoveConnection()
{
    EditingTool::setCurrentTool(EditingTool::Tool::removeConnection);
}

void MainWindow::onAddInputGate()
{
    EditingTool::clear();
    InputGate *gate = new InputGate("Input");
    gate->enableMouseDrag(true);
    gate->snapToMouse(true);
    m_canvas->addObject(gate);
}
void MainWindow::onAddClock()
{
    EditingTool::clear();
    Clock *gate = new Clock("Clock");
    gate->setFrequency(2);
    gate->enableMouseDrag(true);
    gate->snapToMouse(true);
    m_canvas->addObject(gate);
}
void MainWindow::onAddAndGate()
{
    EditingTool::clear();
    AndGate *gate = new AndGate("AndGate");
    gate->setInputCount(2);
    gate->enableMouseDrag(true);
    gate->snapToMouse(true);
    m_canvas->addObject(gate);
}
void MainWindow::onAddOrGate()
{
    EditingTool::clear();
    OrGate *gate = new OrGate("OrGate");
    gate->enableMouseDrag(true);
    gate->snapToMouse(true);
    m_canvas->addObject(gate);
}
void MainWindow::onAddXorGate()
{
    EditingTool::clear();
    XorGate *gate = new XorGate("XorGate");
    gate->enableMouseDrag(true);
    gate->snapToMouse(true);
    m_canvas->addObject(gate);
}
void MainWindow::onAddNotGate()
{
    EditingTool::clear();
    NotGate *gate = new NotGate("NotGate");
    gate->enableMouseDrag(true);
    gate->snapToMouse(true);
    m_canvas->addObject(gate);
}

void MainWindow::onRemoveGate()
{
    EditingTool::setCurrentTool(EditingTool::removeGate);
}
void MainWindow::onEscapePressed()
{
    EditingTool::clear();
}