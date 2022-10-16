#pragma once

#include <QObject>
#include "EditorRibbon.h"

class EditingTool : public QObject
{
        Q_OBJECT
    public:
        enum Tool
        {
            none,
            addConnection,
            removeConnection,
            removeGate
        };

        static void setCurrentTool(Tool tool);
        static Tool getCurrentTool();

        static void clear();

    signals:
        void toolChanged(Tool oldTool, Tool newTool);
    private:
        static void toolChangedInternal(Tool oldTool, Tool newTool);


        static Tool m_currentTool;
        static EditingTool m_instance;
};

