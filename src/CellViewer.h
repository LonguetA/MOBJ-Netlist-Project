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
#include "ErrorCellDialog.h"
#include "InstancesWidget.h"
#include "CellsLib.h"
#include "CellsModel.h"

namespace Netlist {

  class InstancesWidget;
  class CellsLib;

  class CellViewer : public QMainWindow {
    Q_OBJECT ;
    public :

    //-----------------------CTOR------------------------------//
                      CellViewer  ( QWidget * parent = NULL );
    
    //-----------------------DTOR------------------------------//
      virtual         ~CellViewer ();

    //-----------------------GETTEUR------------------------------//
              Cell *  getCell     () const ;

    //-----------------------SLOT------------------------------//
    public slots :
              void    setCell             ( Cell* );
              void    saveCell            ();
              void    openCell            ();
              void    showInstancesWidget       ();
              void    showCellsLib        ();


    //-----------------------SIGNAL------------------------------//
    signals : 
              void     cellLoaded();
            
    private :
      CellWidget * cellWidget_ ;
      SaveCellDialog * saveCellDialog_ ;
      CellsLib*        cellsLib_;
      InstancesWidget* instancesWidget_;
  };
}
#endif
