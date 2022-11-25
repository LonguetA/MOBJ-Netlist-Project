#ifndef  NETLIST_INSTANCE_H   // Le define *doit* être différent dans
#define  NETLIST_INSTANCE_H   // chaque fichier (i.e. nom de classe).

// Les includes sont mis *à l'extérieur* du namespace.
#include <string>
#include  <libxml/xmlreader.h>
#include "Indentation.h"
#include "XmlUtil.h"

namespace Netlist {

  // Les *forward declarations* des autres objets de la
  // structure de données sont mis *à l'intérieur* du namespace.
  class Term;
  class Net;
  class Instance;

  class Instance {
    public:
    //-----------------------TOXML------------------------------//
              void        toXml       ( std::ostream& ) const;
       static Instance *  fromXml     ( Cell*, xmlTextReaderPtr );

    //-----------------------CTOR------------------------------//
       Instance      ( Cell* owner, Cell* model, const std::string& );

    //-----------------------DTOR------------------------------//
      ~Instance      ();

    //-----------------------GETTEUR------------------------------//
        const std::string&        getName       () const;
        Cell*                     getMasterCell () const;
        Cell*                     getCell       () const;
        const std::vector<Term*>& getTerms      () const;
        Term*                     getTerm       ( const std::string& ) const;
        Point                     getPosition   () const;

    //-----------------------GESTION------------------------------//
        bool  connect       ( const std::string& name, Net* );
        void  add           ( Term* );
        void  remove        ( Term* );

    //-----------------------SETTEUR------------------------------//
        void  setPosition   ( const Point& );
        void  setPosition   ( int x, int y );


    private:
        Cell*               owner_;
        Cell*               masterCell_;
        std::string         name_;
        std::vector<Term*>  terms_;
        Point               position_;
  };
}

#endif  // NETLIST_INSTANCE_H