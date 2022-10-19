#include "EditingTool.h"
#include "EditorRibbon.h"

EditingTool::Tool EditingTool::m_currentTool = Tool::none;
EditingTool *EditingTool::m_instance = new EditingTool();
Gate *EditingTool::m_currentlyMovingGate = nullptr;


EditingTool::EditingTool(const std::string &name,
                         CanvasObject *parent)
    : QObject()
    , CanvasObject(name, parent)
{
    m_nextTool = m_currentTool;
    m_nextMovingGate = m_currentlyMovingGate;
}
EditingTool *EditingTool::getInstance()
{
    return m_instance;
}
void EditingTool::update()
{

    if(m_nextMovingGate != m_currentlyMovingGate)
        m_currentlyMovingGate = m_nextMovingGate;
    if(m_currentTool != m_nextTool)
    {
        Tool oldTool = m_currentTool;
        m_currentTool = m_nextTool;
        toolChangedInternal(oldTool, m_currentTool);
    }
}

void EditingTool::setCurrentTool(Tool tool)
{
    m_instance->m_nextTool = tool;
    /*Tool oldTool = m_currentTool;
    m_currentTool = tool;
    toolChangedInternal(oldTool, m_currentTool);*/
}
EditingTool::Tool EditingTool::getCurrentTool()
{
    return m_currentTool;
}

void EditingTool::clear()
{
    m_instance->m_nextTool = Tool::none;
    /*Tool oldTool = m_currentTool;
    m_currentTool = Tool::none;
    toolChangedInternal(oldTool, m_currentTool);*/
}
void EditingTool::toolChangedInternal(Tool oldTool, Tool newTool)
{
    if(oldTool == Tool::moveGate)
        m_currentlyMovingGate = nullptr;
    if(oldTool == newTool) return;
    emit m_instance->toolChanged(oldTool, newTool);

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

void EditingTool::setCurrentlyMoving(Gate *gate)
{
    m_instance->m_nextMovingGate = gate;
    //m_currentlyMovingGate = gate;
}
Gate* EditingTool::getCurrentlyMoving()
{
    return m_currentlyMovingGate;
}
