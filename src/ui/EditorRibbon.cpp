#include "EditorRibbon.h"


using std::string;

// path to the icons resources, relative to the .pro file
const string RIBBON_CLASS_NAME::m_externIconsPath = "icons";
const string RIBBON_CLASS_NAME::m_externIconsExtention = ".png";
const string RIBBON_CLASS_NAME::m_qIconBasePath = ":/"+m_externIconsPath+"/";


RIBBON_CLASS_NAME *RIBBON_CLASS_NAME::RIBBON_INSTANCE_NAME = nullptr;
RIBBON_CLASS_NAME::RIBBON_CLASS_NAME(Ribbon* ribbonWidget)
    : QWidget(nullptr)
{
    RIBBON_INSTANCE_NAME = this;
    RIBBON_OBJ_NAME = ribbonWidget;

    // builds the buttons
    buildButtons();
    connect(m_ribbon,&QTabWidget::currentChanged,this,&RIBBON_CLASS_NAME::onRibbonTabChanged);
}
RIBBON_CLASS_NAME::~RIBBON_CLASS_NAME()
{}

// NEW_BUTTONS [3]:
// Add a new line with this macro and your new button struct name
void RIBBON_CLASS_NAME::buildButtons()
{
    RIBBON_CALL_SETUP(ConnectionButtons);
    RIBBON_CALL_SETUP(AddGateButtons);
    // RIBBON_CALL_SETUP(NEW_BUTTON_STRUCT_NAME);
}

// Implementation of the SoundsButton struct setup function
RIBBON_SETUP_FUNC_IMPL(ConnectionButtons)
{
    string tab   = "Editieren";    // define in which tab the buttons shuld be located
    string group = "Verbindung";    // define the group in the tab for the buttons

    addTab(m_qIconBasePath+"document"+m_externIconsExtention, tab);

   // RIBBON_BUTTONS(ConnectionButtons).load = buttonFactory<QToolButton>("Laden","Lädt",
   //                                                 resourcePath("refresh"), true, tab, group);

   // RIBBON_BUTTONS(ConnectionButtons).save = buttonFactory<QToolButton>("Speichern","Speichert die Software Option Datenbank",
   //                                                 resourcePath("floppy-disk"), true, tab, group);

    RIBBON_BUTTONS(ConnectionButtons).addConnection = buttonFactory<QToolButton>("Verbinden","Neue Verbindung erstellen",
                                                                      resourcePath("connection_connect"), true, tab, group);
    RIBBON_BUTTONS(ConnectionButtons).removeConnection = buttonFactory<QToolButton>("Trennen","Verbindung löschen",
                                                                      resourcePath("connection_disconnect"), true, tab, group);
}

RIBBON_SETUP_FUNC_IMPL(AddGateButtons)
{
    string tab   = "Editieren";    // define in which tab the buttons shuld be located
    string group = "Gatter einfügen";    // define the group in the tab for the buttons

    //addTab(m_qIconBasePath+"document"+m_externIconsExtention, tab);

   // RIBBON_BUTTONS(ConnectionButtons).load = buttonFactory<QToolButton>("Laden","Lädt",
   //                                                 resourcePath("refresh"), true, tab, group);

   // RIBBON_BUTTONS(ConnectionButtons).save = buttonFactory<QToolButton>("Speichern","Speichert die Software Option Datenbank",
   //                                                 resourcePath("floppy-disk"), true, tab, group);
    RIBBON_BUTTONS(AddGateButtons).addClock = buttonFactory<QToolButton>("","Neues Tacktsignal erstellen.",
                                                                      resourcePath("Gates/Clock"), true, tab, group);
    RIBBON_BUTTONS(AddGateButtons).addInputGate = buttonFactory<QToolButton>("","Neue Signalquelle erstellen. Kann von Hand geschaltet werden.",
                                                                      resourcePath("Gates/InGate"), true, tab, group);
    RIBBON_BUTTONS(AddGateButtons).addAndGate = buttonFactory<QToolButton>("","Neues UND Gatter erstellen",
                                                                      resourcePath("Gates/AndGate"), true, tab, group);
    RIBBON_BUTTONS(AddGateButtons).addOrGate = buttonFactory<QToolButton>("","Neues ODER Gatter erstellen",
                                                                      resourcePath("Gates/OrGate"), true, tab, group);
    RIBBON_BUTTONS(AddGateButtons).addXorGate = buttonFactory<QToolButton>("","Neues XOR Gatter erstellen",
                                                                      resourcePath("Gates/XorGate"), true, tab, group);
    RIBBON_BUTTONS(AddGateButtons).addNotGate = buttonFactory<QToolButton>("","Neues NICHT Gatter erstellen",
                                                                      resourcePath("Gates/NotGate"), true, tab, group);
    RIBBON_BUTTONS(AddGateButtons).remove = buttonFactory<QToolButton>("Löschen","Gatter löschem",
                                                                      resourcePath("delete"), true, tab, group);
}

// NEW_BUTTONS [4]:
/*
RIBBON_SETUP_FUNC_IMPL(NEW_BUTTON_STRUCT_NAME)
{
    string tab   = "NewTab";      // define in which tab the buttons shuld be located
    string group = "NewGroup";    // define the group in the tab for the buttons

    // If you change the Tab, you also have to create the new tab using:
    addTab(m_qIconBasePath+"document"+m_externIconsExtention, tab);

    // No function needed to change the group, just change the variable and the next
    //  button you create will be in the new group.

    // To create the buttons in the struct, you have to call the following function
    //  The macro will let you access to the struct instance by the struct name.
    //  then uset "." to access the button pointer variable you defined in the struct.
    // The buttonFactory<...> function will instantiate a new button object with the given parameters.
    //  See buttonFactory<...>(...) in .h
   // RIBBON_BUTTONS(NEW_BUTTON_STRUCT_NAME).buttonOne = buttonFactory<QToolButton>("Laden","Lädt",
   //                                                 resourcePath("refresh"), true, tab, group);

   // RIBBON_BUTTONS(NEW_BUTTON_STRUCT_NAME).buttonTwo = buttonFactory<QToolButton>("Speichern","Speichert die Software Option Datenbank",
   //                                                 resourcePath("floppy-disk"), true, tab, group);

}
*/

void RIBBON_CLASS_NAME::selectButton(QToolButton *button)
{
    button->setStyleSheet("background-color: #cadeea");
}
void RIBBON_CLASS_NAME::deselectButton(QToolButton *button)
{
    button->setStyleSheet("outline:none;");
}

void RIBBON_CLASS_NAME::selectTab(int tab)
{
    if(m_instance == nullptr) return;
    if(m_instance->m_ribbon->count() > tab)
        m_instance->m_ribbon->setCurrentIndex(tab);
}

template<typename T>
T *RIBBON_CLASS_NAME::buttonFactory(const string &text,
                                   const string &toolTip,
                                   const string &iconPath,
                                   bool enabled,
                                   const string &tab,
                                   const string &group)
{
    T *button = new T;
    button->setText(tr(text.c_str()));
    button->setToolTip(tr(toolTip.c_str()));
    button->setIcon(QIcon(iconPath.c_str()));
    m_ribbon->addButton(tab.c_str(), group.c_str(), button);
    button->setEnabled(enabled);
    return button;
}
void RIBBON_CLASS_NAME::addTab(const string &iconPath, const string tabName)
{
    m_ribbon->addTab(QIcon(iconPath.c_str()), tabName.c_str());
}
string RIBBON_CLASS_NAME::resourcePath(const string &iconName)
{
    return m_qIconBasePath+iconName+m_externIconsExtention;
}
void RIBBON_CLASS_NAME::onRibbonTabChanged(int index)
{
    emit ribbonTabChanged(index);
}
