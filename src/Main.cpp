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
#include "Error.h"
using namespace Netlist;


int main ( int argc , char * argv []) {
  try {
    cout << "Chargement des modeles..." << endl;
    Cell * vdd = Cell::load( "vdd" );
    Cell::load( "gnd" );
    Cell::load( "TransistorN" );
    Cell::load( "TransistorP" );
    Cell::load( "and2" );
    Cell::load( "or2" );
    Cell* xor2      = Cell::load( "xor2" );
    QApplication * qa = new QApplication ( argc , argv );
    CellViewer * viewer = new CellViewer ();
    viewer -> setCell ( vdd );
    viewer -> show ();
    int rvalue = qa -> exec ();
    delete qa ;
    return rvalue ;
  }
    catch ( int & e ) {
    cerr << " [ ERROR ] code : " << e << endl ;
    exit (1);
    }
    catch ( Error & e ) {
    cerr << " [ ERROR ]  " << e . what () << endl ;
    exit (1);
    }
  catch ( ... ) {
  cerr << " [ ERROR ] Dans quel etat j’erre . " << endl ;
  exit (1);
  }
}
