
#ifndef _XY_h_
#define _XY_h_

struct XY {
    float x;
    float y;

    XY(){
        XY(0,0);
    }

    XY( float _x, float _y ){
        x = _x;
        y = _y;
    }

    float dot( const XY& other ) const {
        return x*other.x + y*other.y;
    }

    float operator * ( const XY& rhs ) const {
        return dot(rhs);
    }

    XY operator * ( float rhs ){
        return XY( x*rhs, y*rhs );
    }

    XY operator - ( const XY& rhs ) const {
        return XY( x-rhs.x, y-rhs.y );
    }

    XY operator + ( const XY& rhs ) const {
        return XY( x+rhs.x, y+rhs.y );
    }

    XY operator / ( float rhs ){
        return XY(x/rhs,y/rhs);
    }
};



#endif // _XY_h_

