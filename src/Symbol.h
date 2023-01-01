// -*- explicit-buffer-name: "Symbol.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_SYMBOL_H
#define NETLIST_SYMBOL_H

#include  <libxml/xmlreader.h>
#include  <string>
#include  <vector>
#include  "Box.h"

namespace Netlist {

  class Term;
  class Cell;
  class Shape;
  class TermShape;


  class Symbol {
    public:

    //-----------------------CTOR------------------------------//
                                        Symbol          ( Cell* );

    //-----------------------DTOR------------------------------//
                                       ~Symbol          ();

    //-----------------------GETTEUR------------------------------//
             Cell*                      getCell         () const;
             Box                        getBoundingBox  () const;
             Point                      getTermPosition ( Term* ) const;
             TermShape*                 getTermShape    ( Term* ) const;
      inline const std::vector<Shape*>  getShapes       () const;

    //-----------------------GESTION------------------------------//
             void                       add             ( Shape* );
             void                       remove          ( Shape* );

    //-----------------------XML------------------------------//
             void                       toXml           ( std::ostream& ) const;
             static Symbol*             fromXml         ( Cell*, xmlTextReaderPtr );
             
    private:                                            
                                        Symbol          ( const Symbol& );
             Symbol&                    operator=       ( const Symbol& );
    private:
      Cell*                owner_;
      std::vector<Shape*>  shapes_;
  };


  inline const std::vector<Shape*>  Symbol::getShapes () const { return shapes_; }


}  // Netlist namespace.

#endif  // NETLIST_SYMBOL_H
