#ifndef  NETLIST_TERM_H   // Le define *doit* être différent dans
#define  NETLIST_TERM_H   // chaque fichier (i.e. nom de classe).

// Les includes sont mis *à l'extérieur* du namespace.
#include <string>
#include <iostream>
#include  <libxml/xmlreader.h>
#include "Indentation.h"
#include "Cell.h"
#include "Node.h"

namespace Netlist {

  // Les *forward declarations* des autres objets de la
  // structure de données sont mis *à l'intérieur* du namespace.
  class Term;
  class Net;
  class Instance;

  class Term {
    public:
      enum Type      { Internal=1, External=2 };
      enum Direction { In=1, Out=2, Inout=3, Tristate=4, Transcv=5, Unknown=6 };

      //-----------------------CTOR------------------------------//
      Term ( Cell* c, const std::string& name, Direction );
      Term ( Instance* i, const Term* modelTerm );

      //-----------------------DTOR------------------------------//
      ~Term ();


      //-----------------------TOSTRING & TOXML------------------------------//
              void         toXml       ( std::ostream& ) const;
      static  Term *       fromXml     ( Cell*, xmlTextReaderPtr );
      static  std::string  toString    ( Type );
      static  std::string  toString    ( Direction );
      static  Direction    toDirection ( std::string );
      static  Type         toType      ( std::string );

      //-----------------------GESTION DU TYPE------------------------------//

      inline bool               isInternal   () const { return type_ == Internal; }
      inline bool               isExternal   () const { return type_ == External; }

      //-----------------------GETTEUR------------------------------//

      inline const std::string&     getName      () const { return name_;  }
      inline NodeTerm*              getNode      ()       { return &node_; }
      inline Net*                   getNet       () const { return net_;   }
      inline Cell*                  getCell      () const { return (isInternal() ? NULL : static_cast<Cell *>(owner_)); }
             Cell*                  getOwnerCell () const;
      inline Instance*              getInstance  () const { return (isExternal() ? NULL : static_cast<Instance *>(owner_)); }
      inline Direction              getDirection () const { return direction_; }
      inline Point                  getPosition  () const { return node_.getPosition(); }
      inline Type                   getType      () const { return type_; }
      
      //-----------------------SETTEUR------------------------------//

             void  setNet       ( Net* );
             void  setNet       ( const std::string& );
      inline void  setDirection ( Direction ){}
             void  setPosition  ( const Point& );
             void  setPosition  ( int x, int y );

    private:
      void* owner_;
      std::string name_;
      Direction direction_;
      Type type_;
      Net* net_;
      NodeTerm node_;
  };
}

#endif  // NETLIST_CELL_H