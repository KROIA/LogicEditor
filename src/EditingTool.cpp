#include "EditingTool.h"
#include "EditorRibbon.h"

EditingTool::Tool EditingTool::m_currentTool = Tool::none;
EditingTool EditingTool::m_instance;
void EditingTool::setCurrentTool(Tool tool)
{
    Tool oldTool = m_currentTool;
    m_currentTool = tool;
    toolChangedInternal(oldTool, m_currentTool);
}
EditingTool::Tool EditingTool::getCurrentTool()
{
    return m_currentTool;
}

void EditingTool::clear()
{
    m_currentTool = Tool::none;
    Tool oldTool = m_currentTool;
    toolChangedInternal(oldTool, m_currentTool);
}
void EditingTool::toolChangedInternal(Tool oldTool, Tool newTool)
{
    emit m_instance.toolChanged(oldTool, newTool);

    ConnectionButtons conBut = EditorRibbon::getConnectionButtons();
    AddGateButtons addGB = EditorRibbon::getAddGateButtons();
    if(newTool == Tool::addConnection)
    {
        EditorRibbon::selectButton(conBut.addConnection);
        EditorRibbon::deselectButton(conBut.removeConnection);
        EditorRibbon::deselectButton(addGB.remove);
    }else if(newTool == Tool::removeConnection)
    {
        EditorRibbon::selectButton(conBut.removeConnection);
        EditorRibbon::deselectButton(conBut.addConnection);
        EditorRibbon::deselectButton(addGB.remove);
    }else if(newTool == Tool::removeGate)
    {
        EditorRibbon::deselectButton(conBut.addConnection);
        EditorRibbon::deselectButton(conBut.removeConnection);
        EditorRibbon::selectButton(addGB.remove);
    }else
    {
        EditorRibbon::deselectButton(conBut.addConnection);
        EditorRibbon::deselectButton(conBut.removeConnection);
        EditorRibbon::deselectButton(addGB.remove);
    }
}
