#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "EditorRibbon.h"
#include "Canvas.h"
#include "EditingTool.h"
#include "KeyPressEvent.h"
#include "database.h"

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

    private:
        void createGate(Gate *gate);

        Ui::MainWindow *ui;
        EditorRibbon *m_ribbon;
        QSFML::Canvas *m_canvas;
        Database *m_database;

        QSFML::Components::KeyPressEvent *m_escKey;
};
#endif // MAINWINDOW_H
