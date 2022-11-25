#ifndef  NETLIST_NET_H   // Le define *doit* être différent dans
#define  NETLIST_NET_H   // chaque fichier (i.e. nom de classe).

// Les includes sont mis *à l'extérieur* du namespace.
#include <string>
#include <iostream>
#include  <libxml/xmlreader.h>
#include "Indentation.h"
#include "Cell.h"
#include "Term.h"

namespace Netlist {

  // Les *forward declarations* des autres objets de la
  // structure de données sont mis *à l'intérieur* du namespace.
  class Term;
  class Net;
  class Instance;

  class Net {
    public:
      //-----------------------TOXML------------------------------//
              void  toXml      ( std::ostream& ) const;
       static Net * fromXml    ( Cell*, xmlTextReaderPtr) ;

      //-----------------------CTOR------------------------------//
         Net     ( Cell*, const std::string&, Term::Type );

      //-----------------------DTOR------------------------------//
        ~Net     ();
        
      //-----------------------GETTEUR------------------------------//
        Cell*                             getCell       () const;
        const std::string&                getName       () const;
        unsigned int                      getId         () const;
        Term::Type                        getType       () const;
        const std::vector<Node*>&         getNodes      () const;
        size_t                            getFreeNodeId () const;
        inline const std::vector<Line*>&  getLines      () const { return lines_; }
        Node *                            getNode       ( int id )const;

      //-----------------------GESTION------------------------------//
        void  add    ( Node* );
        void  add    ( Line* line );
        bool  remove ( Node* );
        bool  remove ( Line* line );

    private:
        Cell*               owner_;
        std::string         name_;
        unsigned int        id_;
        Term::Type          type_;
        std::vector<Node*>  nodes_;
        std::vector<Line*> lines_;
  };
}

#endif  // NETLIST_NET_H