
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
#include <coroutine>





static std::vector<XY>  XYs;
static std::vector<int> labels;
static double epsilon;
static int clusterCount;
static int minPts;
static int point_index;



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
    double epsilon,
    int minPts
){
    labels[index] = clusterCount;

    for( int i: neighbors ){
        if( labels[i] == 0 ){
            labels[i] = clusterCount;

            std::vector<int> newNeighbors = getNeighbors(XYs, i, epsilon);

            if( newNeighbors.size() >= minPts ){
                // Espandi il cluster se il punto ha abbastanza vicini
                expandCluster(XYs, labels, i, newNeighbors, clusterCount, epsilon, minPts);
            }
        } else
        if( labels[i] == -1 ){
            // Aggiorna il punto rumoroso al bordo del cluster
            labels[i] = clusterCount;
        }
    }
}






void DBSCAN_step(){  // HEADER
    if( point_index >= XYs.size())return; // fine

    if( labels[point_index] != 0 ){
        // Il punto è già etichettato
        point_index++;
        return;
    }

    auto neighbors = getNeighbors( XYs, point_index, epsilon );

    if( neighbors.size() < minPts ){
        // Etichetta il punto come rumore
        labels[point_index] = 0;
    } else {
        // Crea un nuovo cluster
        ++clusterCount;
        expandCluster( XYs, labels, point_index, neighbors, clusterCount, epsilon, minPts );
    }

    point_index++;
}








void DBSCAN_init(){  // HEADER
    XYs = generate_xy();
    epsilon = 20.0;
    minPts  = 4;
    clusterCount = 0;
    point_index  = 0;
    labels.clear();
    labels.resize(XYs.size());
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

    const int RGBnoise[3]=
        { 128, 128, 128 };

    const int RGBs[][3]={
        { 255, 255, 255 },   // unlabeled
        { 255, 0,   0   },
        { 0,   255, 0   },
        { 0,   0,   255 },
        { 255, 0,   255 },
        { 0,   255, 255 },
        { 255, 255, 0   },
        { 255, 128, 0   },
    };

    for( int i=0; i<XYs.size(); i++ ){
        auto xy = XYs[i];
        auto label = labels[i];
        auto rgb = label<0 ? RGBnoise : RGBs[label];
        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
        pix(xy);
    }
}


