#pragma once

#include <QObject>
#include "forwardDeclaration.h"
#include "CanvasObject.h"

class EditingTool : public QObject, public QSFML::Objects::CanvasObject
{
        Q_OBJECT
    public:
        enum Tool
        {
            none,
            addConnection,
            removeConnection,
            removeGate,
            moveGate
        };
        EditingTool(const std::string &name = "",
                     CanvasObject *parent = nullptr);

        static EditingTool *getInstance();

        void update() override;

        static void setCurrentTool(Tool tool);
        static Tool getCurrentTool();

        static void clear();

        static void setCurrentlyMoving(Gate *gate);
        static Gate* getCurrentlyMoving();

    signals:
        void toolChanged(Tool oldTool, Tool newTool);
    private:
        static void toolChangedInternal(Tool oldTool, Tool newTool);

        static EditingTool *m_instance;

        static Tool m_currentTool;
        Tool m_nextTool;

        static Gate *m_currentlyMovingGate;
        Gate* m_nextMovingGate;
};

