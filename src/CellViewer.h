#ifndef NETLIST_CELLVIEWER_H
#define NETLIST_CELLVIEWER_H

#include <QDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Cell.h"
#include "CellWidget.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"
#include "InstancesWidget.h"

namespace Netlist {

  class InstancesWidget;

  class CellViewer : public QMainWindow {
    Q_OBJECT ;
    public :
                      CellViewer  ( QWidget * parent = NULL );
        virtual       ~CellViewer ();
              Cell *  getCell     () const ;
    public slots :
            void      setCell             ( Cell* );
            void      saveCell            ();
            void      openCell            ();
            void      instanceView        ();
    private :
      CellWidget * cellWidget_ ;
      SaveCellDialog * saveCellDialog_ ;
      InstancesWidget *instanceViewer_;
  };
}
#endif