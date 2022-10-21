#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CameraController.h"
#include "BackgroundGrid.h"



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
    connect(m_ribbon, &EditorRibbon::ribbonTabChanged, this, &MainWindow::onRibbonTabChanged);

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

    BlockButtons blockButt = m_ribbon->getBlockButtons();
    connect(blockButt.load, &QToolButton::clicked, this, &MainWindow::onBlockLoad);
    connect(blockButt.save, &QToolButton::clicked, this, &MainWindow::onBlockSave);
    connect(blockButt.createBlock, &QToolButton::clicked, this, &MainWindow::onCreateNewBlock);
    connect(blockButt.addInputPin, &QToolButton::clicked, this, &MainWindow::onBlockAddBlockPinInp);
    connect(blockButt.addOutputPin, &QToolButton::clicked, this, &MainWindow::onBlockAddBlockPinOut);
    connect(blockButt.addInputGate, &QToolButton::clicked, this, &MainWindow::onBlockAddInputGate);
    connect(blockButt.addClock, &QToolButton::clicked, this, &MainWindow::onBlockAddClock);
    connect(blockButt.addAndGate, &QToolButton::clicked, this, &MainWindow::onBlockAddAndGate);
    connect(blockButt.addOrGate, &QToolButton::clicked, this, &MainWindow::onBlockAddOrGate);
    connect(blockButt.addXorGate, &QToolButton::clicked, this, &MainWindow::onBlockAddXorGate);
    connect(blockButt.addNotGate, &QToolButton::clicked, this, &MainWindow::onBlockAddNotGate);
    connect(blockButt.remove, &QToolButton::clicked, this, &MainWindow::onRemoveGate);
    m_currentEditingBlock = nullptr;

    QSFML::CanvasSettings settings;

    m_canvas = new QSFML::Canvas(ui->main_page, settings);
    m_blockCanvas = new QSFML::Canvas(ui->blockEditor_page, settings);

    createDefaultEnviroment(m_canvas);
    createDefaultEnviroment(m_blockCanvas);

    m_database = new Database();
    Database::defineSaveableObject<Gate>();
    Database::defineSaveableObject<AndGate>();
    Database::defineSaveableObject<OrGate>();
    Database::defineSaveableObject<XorGate>();
    Database::defineSaveableObject<NotGate>();
    Database::defineSaveableObject<Clock>();
    Database::defineSaveableObject<InputGate>();
    Database::defineSaveableObject<Block>();
    Database::defineSaveableObject<BlockPin>();

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





}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onRibbonTabChanged(int index)
{
    switch(index)
    {
        case 0:
        {
            ui->pages->setCurrentIndex(0);
            break;
        }
        case 1:
        {
            ui->pages->setCurrentIndex(1);
            break;
        }
    }
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

void MainWindow::onBlockLoad()
{

}
void MainWindow::onBlockSave()
{
    if(!m_currentEditingBlock)
    {
        qDebug() << "Bo Block to save";
        return;
    }
    Database database;
    m_currentEditingBlock->generateBlock();
    database.addObject(m_currentEditingBlock);

    database.save(m_currentEditingBlock->getName()+".json");
   // database.removeObjects(database.getObjects());
}
void MainWindow::onCreateNewBlock()
{
    if(m_currentEditingBlock)
    {
        createDefaultEnviroment(m_blockCanvas);
        m_currentEditingBlock = nullptr;
    }
    m_currentEditingBlock = new Block("Block");
    m_blockCanvas->addObject(m_currentEditingBlock);
    m_currentEditingBlock->setEditMode(true);
}

void MainWindow::onBlockAddBlockPinInp()
{
    BlockPin *pin = new BlockPin("Inp");
    pin->setType(Pin::Type::input);
    createBlockGate(pin);
}
void MainWindow::onBlockAddBlockPinOut()
{
    BlockPin *pin = new BlockPin("Out");
    pin->setType(Pin::Type::output);
    createBlockGate(pin);
}
void MainWindow::onBlockAddInputGate()
{
    createBlockGate(new InputGate("Input"));
}
void MainWindow::onBlockAddClock()
{
    createBlockGate(new Clock("Clock"));
}
void MainWindow::onBlockAddAndGate()
{
    createBlockGate(new AndGate("AndGate"));
}
void MainWindow::onBlockAddOrGate()
{
    createBlockGate(new OrGate("OrGate"));
}
void MainWindow::onBlockAddXorGate()
{
    createBlockGate(new XorGate("XorGate"));
}
void MainWindow::onBlockAddNotGate()
{
    createBlockGate(new NotGate("NotGate"));
}

void MainWindow::createGate(Gate *gate)
{
    EditingTool::setCurrentTool(EditingTool::Tool::moveGate);
    EditingTool::setCurrentlyMoving(gate);
    //Gate *gate = new NotGate("NotGate");
    gate->enableMouseDrag(true);
    m_canvas->addObject(gate);
    gate->snapToMouse(true);
}
void MainWindow::createBlockGate(Gate *gate)
{
    if(!m_currentEditingBlock)
    {
        delete gate;
        return;
    }

    EditingTool::setCurrentTool(EditingTool::Tool::moveGate);
    EditingTool::setCurrentlyMoving(gate);
    gate->enableMouseDrag(true);
    m_currentEditingBlock->addGate(gate);
    gate->snapToMouse(true);
}

void MainWindow::createDefaultEnviroment(QSFML::Canvas *canvas)
{
    if(!canvas) return;
    canvas->removeObject(EditingTool::getInstance());
    canvas->deleteObject(canvas->getObjects());

    QSFML::Objects::CameraController *cam  = new QSFML::Objects::CameraController("cam");
    QSFML::Objects::BackgroundGrid   *grid = new QSFML::Objects::BackgroundGrid("grid");
    canvas->addObject(grid);
    canvas->addObject(cam);
    cam->setDragButton(sf::Mouse::Button::Middle);

    QSFML::Objects::CanvasObject *keyEvents = new QSFML::Objects::CanvasObject("KeyEvents");
    m_escKey = new QSFML::Components::KeyPressEvent("Esc Key",sf::Keyboard::Key::Escape);
    connect(m_escKey, &QSFML::Components::KeyPressEvent::fallingEdge,
            this, &MainWindow::onEscapePressed);
    keyEvents->addComponent(m_escKey);
    canvas->addObject(keyEvents);
    canvas->addObject(EditingTool::getInstance());
}
