#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CameraController.h"
#include "BackgroundGrid.h"

#include "LogicGates.h"

DATABASE_USE_OBJECT(Gate)
DATABASE_USE_OBJECT(AndGate)
DATABASE_USE_OBJECT(OrGate)
DATABASE_USE_OBJECT(XorGate)
DATABASE_USE_OBJECT(NotGate)
DATABASE_USE_OBJECT(Clock)
DATABASE_USE_OBJECT(InputGate)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ribbon = new EditorRibbon(ui->ribbonTabWidget);
    EditButtons editButt = m_ribbon->getEditButtons();
    connect(editButt.load, &QToolButton::clicked, this, &MainWindow::onLoad);
    connect(editButt.save, &QToolButton::clicked, this, &MainWindow::onSave);

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

    m_database = new Database();
    m_database->defineSaveableObject<Gate>();
    m_database->defineSaveableObject<AndGate>();
    m_database->defineSaveableObject<OrGate>();
    m_database->defineSaveableObject<XorGate>();
    m_database->defineSaveableObject<NotGate>();
    m_database->defineSaveableObject<Clock>();
    m_database->defineSaveableObject<InputGate>();

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
    //m_canvas->addObject(EditingTool::getInstance());


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoad()
{
    m_database->load("Test.json");
    std::vector<Gate *> loaded  = m_database->getObjects<Gate>();
    for(size_t i=0; i<loaded.size(); ++i)
    {
        m_canvas->addObject(loaded[i]);
    }
}
void MainWindow::onSave()
{
    m_database->clear(false);
    std::vector<ISerializable *> toSave = m_canvas->getObjects<ISerializable>();
    qDebug() << toSave.size() << " Objects to save";
    for(size_t i=0; i<toSave.size(); ++i)
    {
        m_database->addObject(toSave[i]);
    }
    m_database->save("Test.json");

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
    createGate(new InputGate("Input"));
}
void MainWindow::onAddClock()
{
    createGate(new Clock("Clock"));
}
void MainWindow::onAddAndGate()
{
    createGate(new AndGate("AndGate"));
}
void MainWindow::onAddOrGate()
{
    createGate(new OrGate("OrGate"));
}
void MainWindow::onAddXorGate()
{
    createGate(new XorGate("XorGate"));
}
void MainWindow::onAddNotGate()
{
    createGate(new NotGate("NotGate"));
}

void MainWindow::onRemoveGate()
{
    EditingTool::setCurrentTool(EditingTool::removeGate);
}
void MainWindow::onEscapePressed()
{
    EditingTool::clear();
}

void MainWindow::createGate(Gate *gate)
{
    EditingTool::setCurrentTool(EditingTool::Tool::moveGate);
    EditingTool::setCurrentlyMoving(gate);
    //Gate *gate = new NotGate("NotGate");
    gate->enableMouseDrag(true);
    gate->snapToMouse(true);
    m_canvas->addObject(gate);
}
