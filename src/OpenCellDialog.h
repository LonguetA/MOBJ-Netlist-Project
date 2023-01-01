#ifndef NETLIST_OPENCELLDIALOG_H
#define NETLIST_OPENCELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "Cell.h"

namespace Netlist {

  class Cell;
  class OpenCellDialog : public QDialog {
    Q_OBJECT ;
    public :
    //-----------------------CTOR------------------------------//
                      OpenCellDialog  ( QWidget * parent = NULL );

    //-----------------------AFFICHAGE------------------------------//
        static  bool  run             ( QString & name);

    //-----------------------GETTEUR------------------------------//
        const QString getCellName     () const ;

    //-----------------------SETTEUR------------------------------//
              void    setCellName     ( const QString & );
    protected :
      QLineEdit * lineEdit_ ;

  };
}

#endif