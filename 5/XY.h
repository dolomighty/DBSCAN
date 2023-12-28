
#ifndef _XY_h_
#define _XY_h_

#undef NDEBUG
#include <assert.h>

struct XY {
    double x;
    double y;

    XY(){
        XY(0,0);
    }

    XY( double _x, double _y ){
        x = _x;
        y = _y;
    }

    double dot( const XY& other ) const {
        return x*other.x + y*other.y;
    }

    double operator [] ( int idx ) const {
        if(idx==0) return x;
        if(idx==1) return y;
        assert("idx invalido"==0);
    }

    double operator * ( const XY& rhs ) const {
        return dot(rhs);
    }

    XY operator * ( double rhs ){
        return XY( x*rhs, y*rhs );
    }

    XY operator - ( const XY& rhs ) const {
        return XY( x-rhs.x, y-rhs.y );
    }

    XY operator + ( const XY& rhs ) const {
        return XY( x+rhs.x, y+rhs.y );
    }

    XY operator / ( double rhs ){
        return XY(x/rhs,y/rhs);
    }
};



#endif // _XY_h_

