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
#include "CellsLib.h"
#include "CellsModel.h"

namespace Netlist {

  class InstancesWidget;
  class CellsLib;

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
            void      showInstancesWidget       ();
            void      showCellsLib        ();
    signals : 
            void      cellLoaded();
            
    private :
      CellWidget * cellWidget_ ;
      SaveCellDialog * saveCellDialog_ ;
      CellsLib*        cellsLib_;
      InstancesWidget* instancesWidget_;
  };
}
#endif
