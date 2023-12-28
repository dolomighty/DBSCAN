
#undef NDEBUG
#include <assert.h>

#include <stdlib.h>
#include "main.h"
#include "XY.h" // HEADER
#include <vector> // HEADER


static float unirand(){
    // return [0,1]
    return rand()*1.0/RAND_MAX;
}

static float biporand(){
    // return [-1,1]
    return rand()*2.0/RAND_MAX-1.0;
}


// c++20 ha la lerp
static float lerp( float a, float b, float x ){
    return a+(b-a)*x;
}


static float randgauss(){
    return (biporand()+biporand()+biporand()+biporand()+biporand())/5.0;
}


static XY gaussian_xy( XY center, float radius ){
    return center+XY(randgauss(),randgauss())*radius;
}







std::vector<XY> generate_xy(){  // HEADER

    std::vector<XY> xys;
    const int clusters = 3;

    for( int i=0; i<clusters; i++ ){   

        auto center = XY(
            lerp( WINW*1/10, WINW*9/10, unirand()),
            lerp( WINH*1/10, WINH*9/10, unirand())
        );

        auto minor = lerp( WINH/10, WINH/3, unirand());
        auto major = lerp( 1,       3,      unirand());
        auto angle = lerp( 0,       M_PI,   unirand());
        auto c = std::cos(angle);
        auto s = std::sin(angle);

//        1 0 X      c s 0   mM 0 0  
//        0 1 Y * ( -s c 0 * 0  m 0 )
//        0 0 1      0 0 1   0  0 1  
//
//        1 0 X   cmM  sm 0
//        0 1 Y * -smM cm 0
//        0 0 1   0    0  1
//
//        cmM  sm X
//        -smM cm Y
//        0    0  1


        double m[2][3]={
            {  c*minor*major, s*minor, center.x },
            { -s*minor*major, c*minor, center.y },
        };


        for( int j=0; j<200; j++ ){
            auto xy = gaussian_xy( XY(0,0), 1 );
            xys.push_back( XY(
                m[0][0]*xy[0] + m[0][1]*xy[1] + m[0][2],
                m[1][0]*xy[0] + m[1][1]*xy[1] + m[1][2]
            ));
        }
    }


    const int noise = 50;
    for( int i=0; i<noise; i++ ){
        xys.push_back( XY(
            lerp( 0, WINW, unirand()),
            lerp( 0, WINH, unirand())
        ));
    }

    return xys;
}





