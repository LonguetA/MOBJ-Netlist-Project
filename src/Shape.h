#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include <libxml/xmlreader.h>
#include <string>
#include <vector>
#include "Indentation.h"
#include "Symbol.h"       // TME7

namespace Netlist {

  class Instance;
  class Net;
  class Term;

    /************************SHAPE****************************************/

    class Shape {
    public:

    //-----------------------CTOR------------------------------//
                                    Shape           ( Symbol * );

    //-----------------------DTOR------------------------------//
                virtual             ~Shape          ();
    
    //-----------------------GETTEUR------------------------------//
                inline  Symbol *    getSymbol       () const ;
                virtual Box         getBoundingBox  () const  = 0;

    //-----------------------XML------------------------------//
        static          Shape *     fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
                virtual void        toXml           ( std::ostream& ) const = 0;

    private:
                Symbol * owner_ ;
    };

    /************************BOXSHAPE****************************************/

    class BoxShape : public Shape{
        public:

        //-----------------------CTOR------------------------------//
                                        BoxShape        ( Symbol * , const Box & );

        //-----------------------DTOR------------------------------//
                                        ~BoxShape       ();

        //-----------------------GETTEUR------------------------------//
                    virtual Box         getBoundingBox  () const ;

        //-----------------------XML------------------------------//
                    virtual void        toXml           ( std::ostream& ) const;
                    static  BoxShape *  fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
       
        private:
            Box box_ ;
    };

    /************************TERMSHAPE****************************************/

    class TermShape : public Shape {
        public:
            enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight };

            //-----------------------CTOR------------------------------//
                                           TermShape       ( Symbol * owner, std::string name , int x1 , int y1,NameAlign );

            //-----------------------DTOR------------------------------//
                                           ~TermShape      ();
            
            //-----------------------GETTEUR------------------------------//
                           Box             getBoundingBox  () const ;
                    static std::string     alignToString   ( NameAlign ) ;
                    static NameAlign       stringToAlign   (std::string str);
                    inline Term *          getTerm         () const {return term_;};
                    inline int             getX            () const {return x_;};
                    inline int             getY            () const {return y_;};
                    inline NameAlign       getAlign        () const {return align_;};

            //-----------------------XML------------------------------//
                    virtual void           toXml           (std::ostream &) const;
                    static  TermShape *    fromXml         ( Symbol* owner, xmlTextReaderPtr reader );

        private:
            Term * term_ ;
            int x_ , y_ ;
            NameAlign align_;
    };

    /************************LINESHAPE****************************************/

    class LineShape : public Shape{
    public :

    //-----------------------CTOR------------------------------//
                        LineShape       ( Symbol * , int x1 , int y1 , int x2 , int y2 );

    //-----------------------DTOR------------------------------//
                        ~LineShape      ();

    //-----------------------GETTEUR------------------------------//
        virtual Box     getBoundingBox  () const ;
        inline  int     getX1           () const {return x1_;}
        inline  int     getX2           () const {return x2_;}
        inline  int     getY1           () const {return y1_;}
        inline  int     getY2           () const {return y2_;}

    //-----------------------XML------------------------------//
        virtual void    toXml           ( std::ostream & ) const;
        static   LineShape * fromXml         ( Symbol* owner, xmlTextReaderPtr reader );

    private :
        int x1_ , y1_ , x2_ , y2_ ;
    };


    /************************ARCSHAPE****************************************/


    class ArcShape : public Shape{
        public:

        //-----------------------CTOR------------------------------//
                                ArcShape        (Symbol *,const Box &,int start, int span);
        
        //-----------------------DTOR------------------------------//
                                ~ArcShape       ();
        
        //-----------------------GETTEUR------------------------------//
            virtual Box         getBoundingBox  () const;
            inline int          getStart        () const {return start_;}
            inline int          getSpan         () const {return span_;}

        //-----------------------XML------------------------------//
            virtual void        toXml           (std::ostream&)const;
            static  ArcShape *  fromXml         ( Symbol* owner, xmlTextReaderPtr reader );


        private:
            Box box_;
            int start_, span_;
    };

    /************************ELLIPSESHAPE****************************************/

    class EllipseShape : public Shape{
        public:

        //-----------------------CTOR------------------------------//
                                    EllipseShape    (Symbol *,const Box &);

        //-----------------------DTOR------------------------------//
                                    ~EllipseShape   ();

        //-----------------------GETTEUR------------------------------//
            virtual Box             getBoundingBox  () const;

        //-----------------------XML------------------------------//
            virtual void            toXml           (std::ostream &) const;
            static  EllipseShape *  fromXml         ( Symbol* owner, xmlTextReaderPtr reader );

        private:
            Box box_;
    };


    inline Symbol * Shape::getSymbol() const { return owner_ ; }


}


#endif