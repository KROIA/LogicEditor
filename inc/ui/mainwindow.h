#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "EditorRibbon.h"
#include "Canvas.h"
//#include "EditingTool.h"
#include "KeyPressEvent.h"
#include "database.h"
#include "LogicGates.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void onRibbonTabChanged(int index);

        void onLoad();
        void onSave();

        void onAddConnection();
        void onRemoveConnection();

        void onAddInputGate();
        void onAddClock();
        void onAddAndGate();
        void onAddOrGate();
        void onAddXorGate();
        void onAddNotGate();
        void onRemoveGate();

        void onEscapePressed();

        void onBlockLoad();
        void onBlockSave();
        void onCreateNewBlock();

        void onBlockAddBlockPinInp();
        void onBlockAddBlockPinOut();
        void onBlockAddInputGate();
        void onBlockAddClock();
        void onBlockAddAndGate();
        void onBlockAddOrGate();
        void onBlockAddXorGate();
        void onBlockAddNotGate();

    private:
        void createGate(Gate *gate);
        void createBlockGate(Gate *gate);

        void createDefaultEnviroment(QSFML::Canvas *canvas);

        Ui::MainWindow *ui;
        EditorRibbon *m_ribbon;
        QSFML::Canvas *m_canvas;
        QSFML::Canvas *m_blockCanvas;
        Block *m_currentEditingBlock;

        Database *m_database;

        QSFML::Components::KeyPressEvent *m_escKey;
};
#endif // MAINWINDOW_H
