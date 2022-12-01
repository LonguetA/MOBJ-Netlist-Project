#include <QApplication>
#include <QtGui>
#include "CellViewer.h"

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
using namespace Netlist;


int main ( int argc , char * argv []) {
  Cell::load( "vdd" );
  Cell::load( "gnd" );
  Cell::load( "TransistorN" );
  Cell::load( "TransistorP" );
  Cell::load( "and2" );
  Cell::load( "or2" );
  Cell* xor2      = Cell::load( "xor2" );
  //Cell* halfadder = Cell::load( "halfadder" );

  QApplication * qa = new QApplication ( argc , argv );
  CellViewer * viewer = new CellViewer ();
  //viewer -> setCell ( halfadder );
  viewer -> show ();
  int rvalue = qa -> exec ();
  delete qa ;
  return rvalue ;
}
