#pragma once
/**
 * \brief This is the class which defines the contents
 *        of the ribbon bar on top of the application.
 *
 * \details
 *        A button is placed in an tab, in that tab it will be
 *        placed inside of a group.
 *
 *        You can search in this and in the .cpp file for the keyword: "NEW_BUTTONS"
 *        It will lead you with the numbers [x] to add all needed things for new buttons
 *
 *     1)   Button struct
 *        It makes sense to create a struct, which contains all
 *        buttons from the same group or tab.
 *        It depends on the logical separation of the functionality of
 *        the buttons.
 *
 *        The struct must contain pointers to the buttons.
 *        -> "NEW_BUTTONS [1]:"
 *
 *     2)   Declaration stuff
 *        Follow the macro, it will set it up for you. -> "NEW_BUTTONS [2]:"
 *
 *
 *     3)   Setup call
 *        Follow the macro in the .cpp: "NEW_BUTTONS [3]:"
 *
 *     4)   Define the buttons
 *        Follow the instructions in the .cpp: -> "NEW_BUTTONS [4]:"
 *
 *   Jump to "NEW_BUTTONS [1]:"
 *
 */

#include <QMenu>
#include <string>
#include <QToolButton>
#include <vector>
#include "ribbon.h"
#define CONCAT(id1, id2) id1##id2



// define the classname of the Ribbon here
// Free to change
#define RIBBON_CLASS_NAME EditorRibbon

//---------------------------------------------------------------------------+
// Do not change this section
// definition of private variable names
#define RIBBON_INSTANCE_NAME m_instance
#define RIBBON_OBJ_NAME m_ribbon
#define RIBBON_STRUCT_INSTANCE(structName) CONCAT(m_,structName)

// definition of the setup function which builds the button struct
#define RIBBON_SETUP_FUNC(structName) CONCAT(build,structName)
// definition of the function head for the implementation
// of the setupu function which build the button struct
#define RIBBON_SETUP_FUNC_IMPL(structName) \
    void RIBBON_CLASS_NAME::RIBBON_SETUP_FUNC(structName)()

// creates the necessary stuff in the class for each button struct
#define RIBBON_IMPLEMENT_STRUCT(structName) \
    public: \
    static structName CONCAT(get,structName)() \
    { \
        if(RIBBON_INSTANCE_NAME == nullptr) return structName{}; \
        return RIBBON_INSTANCE_NAME->RIBBON_STRUCT_INSTANCE(structName); \
    } \
    private: \
    void RIBBON_SETUP_FUNC(structName)(); \
    structName RIBBON_STRUCT_INSTANCE(structName); \
    public:

// Calls the setup function for the button struct
#define RIBBON_CALL_SETUP(structName) RIBBON_SETUP_FUNC(structName)();

// Lets you access to the button struct instance
#define RIBBON_BUTTONS(structName) RIBBON_STRUCT_INSTANCE(structName)
//---------------------------------------------------------------------------+

// NEW_BUTTONS [1]:
// Create new button structs here:
//  ...

struct EditButtons
{
    QToolButton *save;
    QToolButton *load;
};

struct ConnectionButtons
{
    //QToolButton *load;
    //QToolButton *save;

    QToolButton *addConnection; // You can also use from QToolButton derived buttons here
    QToolButton *removeConnection;
};

struct AddGateButtons
{
    QToolButton *addInputGate;
    QToolButton *addOutputGate;
    QToolButton *addClock;
    QToolButton *addAndGate;
    QToolButton *addOrGate;
    QToolButton *addXorGate;
    QToolButton *addNotGate;

    QToolButton *remove;
};


// End of button structs


class RIBBON_CLASS_NAME : public QWidget
{
        Q_OBJECT
    public:
        /**
         * \brief SoundboardRibbon
         * \param ribbonWidget, the widget which will display the ribbon
         */
        RIBBON_CLASS_NAME(Ribbon* ribbonWidget);
        ~RIBBON_CLASS_NAME();

        // NEW_BUTTONS [2]:
        /**
         * \brief getSoundButtons
         * \details To make it easy to add new buttons you can use this macro,
         *          which will create the necessary stuff depending on the struct name.
         *          Just add the macro below with the struct name and move on to the .cpp file
         *          for the implementation.
         *
         * \return returns a buttonstruct which contains the buttons
         *         for this group
         */
        RIBBON_IMPLEMENT_STRUCT(EditButtons);
        RIBBON_IMPLEMENT_STRUCT(ConnectionButtons);
        RIBBON_IMPLEMENT_STRUCT(AddGateButtons);


        static void selectButton(QToolButton *button);
        static void deselectButton(QToolButton *button);
    signals:
        /**
         * \brief ribbonTabChanged
         * \param tab, will be emitted, if the tab index changed
         */
        void ribbonTabChanged(int index);

    public slots:
        /**
         * \brief selectTab
         * \param tab, switches the selected ribbon tab.
         */
        static void selectTab(int tab);
    private slots:
        void onRibbonTabChanged(int index);
    private:
        /**
         * \brief buttonFactory to create a new button
         * \tparam ButtonType
         * \param text        Text which will be visible on the button
         * \param toolTip     Text which will be visible if the mouse hovers over the button
         * \param iconPath    Filepath to the icon for the button
         * \param enabled     false -> button will be grayed out
         * \param tab         Name of the tab the button will be located in
         * \param group       The name of the group the button will be located in the tab
         * \return returns the pointer to the instantiated button which will be stored in the struct
         */
        template<typename T>
        T *buttonFactory(const std::string &text,
                         const std::string &toolTip,
                         const std::string &iconPath,
                         bool enabled,
                         const std::string &tab,
                         const std::string &group);

        /**
         * \brief addTab
         * \param iconPath fullPath to the icon for the tab
         */
        void addTab(const std::string &iconPath, const std::string tabName);

        /**
         * \brief resourcePath
         * \param iconName name of an icon which is stored in the resources folder
         * \return returns the full path string to the icon
         */
        std::string resourcePath(const std::string &iconName);

        /**
         * \brief buildButtons
         * \details Builds all buttons
         */
        void buildButtons();

        static RIBBON_CLASS_NAME *RIBBON_INSTANCE_NAME;

        Ribbon *RIBBON_OBJ_NAME;
        static const std::string m_externIconsPath;
        static const std::string m_externIconsExtention;
        static const std::string m_qIconBasePath;
};


