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



    class Shape {
    public:
                                    Shape           ( Symbol * );
                virtual             ~Shape          ();
                inline  Symbol *    getSymbol       () const ;
                virtual Box         getBoundingBox  () const  = 0;
        static          Shape *     fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
                virtual void        toXml           ( std::ostream& ) const = 0;

    private:
                Symbol * owner_ ;
    };

    class BoxShape : public Shape{
        public:
                                        BoxShape        ( Symbol * , const Box & );
                                        ~BoxShape       ();
                    virtual Box         getBoundingBox  () const ;
                    virtual void        toXml           ( std::ostream& ) const;
            static   BoxShape *     fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
        private:
            Symbol * owner_ ;
            Box box_ ;
    };

    class TermShape : public Shape {
        public:
            enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight };

                                    TermShape       ( Symbol * owner, std::string name , int x1 , int y1,NameAlign );
                                    ~TermShape      ();
                    Box             getBoundingBox  () const ;
                    static std::string     alignToString   ( NameAlign ) ;
                    static NameAlign       stringToAlign   (std::string str);
                    virtual void    toXml           (std::ostream &) const;
                    inline Term *   getTerm         () const {return term_;};
                    inline int      getX            () const {return x_;};
                    inline int      getY            () const {return y_;};
                    inline NameAlign getAlign       () const {return align_;};
            static   TermShape * fromXml         ( Symbol* owner, xmlTextReaderPtr reader );

        private:
            Term * term_ ;
            int x_ , y_ ;
            NameAlign align_;
    };

    class LineShape : public Shape{
    public :
                        LineShape       ( Symbol * , int x1 , int y1 , int x2 , int y2 );
                        ~LineShape      ();
        virtual Box     getBoundingBox  () const ;
        virtual void    toXml           ( std::ostream & ) const;
        inline  int     getX1           () const {return x1_;}
        inline  int     getX2           () const {return x2_;}
        inline  int     getY1           () const {return y1_;}
        inline  int     getY2           () const {return y2_;}
        static   LineShape * fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
    private :
        Symbol * owner_ ;
        int x1_ , y1_ , x2_ , y2_ ;
    };

    class ArcShape : public Shape{
        public:
            ArcShape (Symbol *,const Box &,int start, int span);
            ~ArcShape ();
            virtual Box getBoundingBox () const;
            virtual void toXml(std::ostream&)const;
            static   ArcShape * fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
            inline int getStart() const {return start_;}
            inline int getSpan() const {return span_;}
        private:
            Box box_;
            int start_, span_;
    };

    class EllipseShape : public Shape{
        public:
            EllipseShape (Symbol *,const Box &);
            ~EllipseShape ();
            virtual Box getBoundingBox () const;
            virtual void toXml (std::ostream &) const;
            static   EllipseShape * fromXml         ( Symbol* owner, xmlTextReaderPtr reader );
        private:
            Box box_;
    };


    inline Symbol * Shape::getSymbol() const { return owner_ ; }


}


#endif