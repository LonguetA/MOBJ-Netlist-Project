#ifndef NETLIST_CELLSLIB_H
#define NETLIST_CELLSLIB_H

#include <QTableView>
#include <QPushButton>

namespace Netlist {
class CellsModel;
class CellViewer;


class CellsLib : public QWidget {

  Q_OBJECT;
  public:

  //-----------------------CTOR------------------------------//
                       CellsLib       ( QWidget* parent=NULL );

  //-----------------------SETTEUR------------------------------//
           void        setCellViewer  ( CellViewer* );

  //-----------------------GETTEUR------------------------------//
           int         getSelectedRow () const;
    inline CellsModel* getBaseModel   ();

  //-----------------------SLOT------------------------------//
  public slots:
           void        load           ();

        
  private:
    CellViewer*  cellViewer_;
    CellsModel*  baseModel_;
    QTableView*  view_;
    QPushButton* load_;
};

  inline CellsModel* CellsLib::getBaseModel   (){ return baseModel_;}
}

#endif
