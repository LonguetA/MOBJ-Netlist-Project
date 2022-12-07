#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "Line.h"
#include "Error.h"
namespace Netlist {

    //--------- TOXML ----------//
    void  Net::toXml ( std::ostream& stream ) const {
      stream << indent++ << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) <<"\">" << std::endl;
      for (std::vector<Node *>::const_iterator it = nodes_.begin() ; it != nodes_.end() ; it++){
        if (*it) (*it)->toXml(stream);
      }
      for (std::vector<Line *>::const_iterator it = lines_.begin() ; it != lines_.end() ; it++){
        (*it)->toXml(stream);
      }
      indent--;
      stream << indent << "</net>"  << std::endl;
    }

    //------------CTOR-----------//
    Net::Net     ( Cell* c, const std::string& str, Term::Type t )
                :owner_(c),
                name_(str),
                id_(c->newNetId()),
                type_(t),
                nodes_(){

                  //Ajout du net à la cell
                  c->add(this);
                }
    
    //------------DTOR-----------//
    Net::~Net    (){

      //Retrait du net à la cell
      owner_->remove(this);

      //On deconnecte nos term
      for (std::vector<Node *>::iterator it = nodes_.begin(); it != nodes_.end() ; it++){
        if ((*it) == NULL)continue;

        NodeTerm *nd = dynamic_cast<NodeTerm*> (*it);
        if (nd){
          nd->getTerm()->setNet(NULL);
        }
      }
    }
    
    //------------GETTEUR-----------//
    Cell* Net::getCell () const {return owner_;}

    const std::string& Net::getName () const {return name_;}

    unsigned int Net::getId () const {return id_;}

    Term::Type Net::getType () const {return type_;}

    const std::vector<Node*>& Net::getNodes      () const {return nodes_;}

    size_t Net::getFreeNodeId () const {
      for (size_t i = 0 ; i < nodes_.size() ; i++){

        //Si on a une case libre
        if (nodes_[i] == NULL){
          return i;
        }
      }

      //Si aucune case libre
      return nodes_.size();
    }

    void  Net::add ( Line* line ){ if (line) lines_.push_back( line ); }

    Node * Net::getNode(int id) const{
      for (std::vector<Node *>::const_iterator it = nodes_.begin() ; it != nodes_.end() ; it++){
        if ((*it) == NULL) continue;
        if ((*it)->getId() == id) return *it;
      }
      return NULL;
    }


    //------------FONCTION DE GESTION DES NODES-----------//
    void  Net::add ( Node* n) {

      if (not n) return;

      //Recuperation et mise a jour de l'ID
      size_t idx = n->getId();

      if (idx == Node::noid){
        idx = getFreeNodeId();
        n->setId(idx);
      }

      //Cas ou notre tableau est plein
      if (idx >= nodes_.size()){
        for (size_t i = nodes_.size() ; i < idx ; i++){
            nodes_.push_back(NULL);
        }
        nodes_.push_back(n);
        return;
      }

      //Ajout dans le tableau

      if (nodes_[idx] != NULL){
        std::cerr << "OVERRIDE" << idx << std::endl;
        nodes_[idx]->setId(Node::noid);
      }
      nodes_.insert(nodes_.begin() + idx,n);
    }

    bool  Net::remove ( Node* n){
      NodeTerm *nd = dynamic_cast<NodeTerm*> (n);
      if (nd){
        nd->getTerm()->setNet(NULL);
      }
      else return false;
      return true;
    }

    bool  Net::remove ( Line* line ){
      if (line) {
        for ( std::vector<Line*>::iterator il = lines_.begin()
            ; il != lines_.end() ; ++il ) {
          if (*il == line) {
            lines_.erase( il );
            return true;
          }
        }
      }
      return false;
    }

    //------------FROM XML-----------//
    Net *Net::fromXml(Cell* c, xmlTextReaderPtr reader){

      //Creation des balises xmlChar 
      const xmlChar* netTag      = xmlTextReaderConstString( reader, (const xmlChar*)"net" );
      const xmlChar* nodeTag      = xmlTextReaderConstString( reader, (const xmlChar*)"node" );
      const xmlChar* lineTag      = xmlTextReaderConstString( reader, (const xmlChar*)"line" );

      //Recuperation des parametres
      std::string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
      std::string typeName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ) );
      
      //Si les parametres sont vides 
      if (netName.empty() or typeName.empty()){
        std::cerr << "[ERROR] Net::fromXml(): Missing parameter" 
        << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
        return NULL;
      }   

      //Creation du net
      Net *n = new Net(c,netName,Term::toType(typeName));
      //Boucle de recuperation des infos de Node
      while(true){
        int status = xmlTextReaderRead(reader);

        if (status != 1) {
          if (status != 0) {
            std::cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << std::endl;
          }
          break;
        }


        const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

        //PASS si commentaire, espace
        switch ( xmlTextReaderNodeType(reader) ) {
          case XML_READER_TYPE_COMMENT:
          case XML_READER_TYPE_WHITESPACE:
          case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
          continue;
        }


        //Si on tombe sur une balise de Node
        if (nodeName == nodeTag and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)){
          if(Node::fromXml(n,reader)) continue;
        }

        //Si on tombe sur une balise de Line
        if (nodeName == lineTag and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)){
          if(Line::fromXml(n,reader)) continue;
        }

        //Si on tombe sur la balise fermante du Net
        if ((nodeName ==  netTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)){
          break;
        }


        //ERREUR
        throw Error ( "Net :: fromXml (): Unknown or misplaced tag. " );
        return NULL;
      } 
      return n;
    }
}