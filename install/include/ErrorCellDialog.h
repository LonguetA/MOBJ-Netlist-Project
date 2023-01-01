#ifndef NETLIST_ERRORCELLDIALOG_H
#define NETLIST_ERRORCELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "Cell.h"

namespace Netlist {

  class Cell;
  class ErrorCellDialog : public QDialog {
    Q_OBJECT ;
    public :

    //-----------------------CTOR------------------------------//
                      ErrorCellDialog  ( QWidget * parent = NULL,std::string msg = "Error Unknown" );

    //-----------------------AFFICHAGE------------------------------//
        static  bool  run             ( std::string &msg);
    protected :
      QLineEdit * lineEdit_ ;

  };
}

#endif