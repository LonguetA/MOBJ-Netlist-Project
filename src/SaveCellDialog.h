#ifndef NETLIST_SAVECELLDIALOG_H
#define NETLIST_SAVECELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "Cell.h"

namespace Netlist {

  class Cell;
  class SaveCellDialog : public QDialog {
    Q_OBJECT ;
    public :

    //-----------------------CTOR------------------------------//
                      SaveCellDialog  ( QWidget * parent = NULL );

    //-----------------------AFFICHAGE------------------------------//
              bool    run             ( QString & name );

    //-----------------------GETTEUR------------------------------//
        const QString getCellName     () const ;

    //-----------------------SETTEUR------------------------------//
              void    setCellName     ( const QString & );
              
    protected :
              QLineEdit * lineEdit_ ;
  };
}

#endif