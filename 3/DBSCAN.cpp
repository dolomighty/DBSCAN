
#undef NDEBUG
#include <assert.h>

#include "XY.h"
#include "generate_xy.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "main.h"
#include <thread>
#include <chrono>



static std::vector<XY>  XYs;
static std::vector<int> labels;
static double epsilon;
static int clusterCount;
static int minPts;



double distance( XY& p1, XY& p2 ){
    // Calcola la distanza euclidea tra due punti
    auto d = p1-p2;
    return std::sqrt(d*d);
}







std::vector<int> getNeighbors(
    std::vector<XY>& XYs,
    int    index,
    double epsilon
){
    std::vector<int> neighbors;

    for( int i=0; i<XYs.size(); ++i ){
        if( distance( XYs[index], XYs[i]) < epsilon ){
            neighbors.push_back(i);
        }
    }

    return neighbors;
}




void expandCluster(
    std::vector<XY>& XYs,
    std::vector<int>& labels,
    int index,
    const std::vector<int>& neighbors,
    int clusterCount,
    double epsilon
){
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ));
    
    labels[index] = clusterCount;

    for( int i: neighbors ){
        if( labels[i] == 0 ){
            labels[i] = clusterCount;

            std::vector<int> newNeighbors = getNeighbors( XYs, i, epsilon );

            if( newNeighbors.size() >= minPts ){
                // Espandi il cluster se il punto ha abbastanza vicini
                expandCluster( XYs, labels, i, newNeighbors, clusterCount, epsilon );
            }
            continue;
        }

        if( labels[i] == -1 ){
            // Aggiorna il punto rumoroso al bordo del cluster
            labels[i] = clusterCount;
            continue;
        }
    }
}







void DBSCAN(){  // HEADER

    XYs = generate_xy();
    epsilon = 20.0;
    minPts  = 4;
    clusterCount = 0;



    std::vector<double> density;
    density.resize(XYs.size());

    for( int i=0; i<XYs.size(); i++ ){
        double sum = 0;
        for( int j=0; j<XYs.size(); j++ ){
            if(i==j)continue;
            auto d = XYs[i]-XYs[j];
            sum += 1/sqrt(d*d);
        }
        density[i]=sum;
    }




    labels.clear();
    labels.resize(XYs.size());

    for( int i=0; i<XYs.size(); i++ ){

        if( labels[i] != 0 )continue; // 0 == uncategorized

//        auto neighbors = getNeighbors( XYs, i, epsilon );
        auto neighbors = getNeighbors( XYs, i, 100/density[i] );

        if( neighbors.size() < minPts ){
            // Etichetta il punto come rumore
            labels[i] = -1;
            continue;
        }

        // Crea un nuovo cluster
        ++clusterCount;
        expandCluster( XYs, labels, i, neighbors, clusterCount, epsilon );
    }
}






static void pix( XY xy, int radius=3 ){
    SDL_Rect r ={
        .x = (int)(xy.x-(radius/2)),
        .y = (int)(xy.y-(radius/2)),
        .w = radius,
        .h = radius,
    };
    SDL_RenderFillRect( renderer, &r );
}





void DBSCAN_render(){ // HEADER

    const int mac_palette[][3]={
        { 255, 255, 255 },
        { 252, 244, 4   },
        { 255, 100, 4   },
        { 220, 8,   8   },
        { 240, 8,   132 },
        { 72,  0,   164 },
        { 0,   0,   212 },
        { 0,   172, 232 },
        { 32,  184, 20  },
        { 0,   100, 16  },
        { 88,  44,  4   },
        { 144, 112, 56  },
        { 192, 192, 192 },
        { 128, 128, 128 },
        { 64,  64,  64  },
        { 0,   0,   0   },
    };

    for( int i=0; i<XYs.size(); i++ ){
        auto xy = XYs[i];
        auto label = labels[i];
        auto rgb = label < 0 ? mac_palette[14] : mac_palette[label];
        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
        pix(xy);
    }
}




void DBSCAN_loop(){
    while(true){
        DBSCAN();
        std::this_thread::sleep_for(std::chrono::seconds( 3 ));
    }
}




void DBSCAN_thread(){  // HEADER
    static std::thread helper(DBSCAN_loop);
}





