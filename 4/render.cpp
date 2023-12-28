
#undef NDEBUG
#include <assert.h>

#include "XY.h"
#include "generate_xy.h"
#include <stdlib.h>
#include <vector>
#include <SDL2/SDL.h>
#include "main.h"
#include <thread>
#include <chrono>
#include "DBSCAN.h"




static std::vector<XY> XYs;
static auto dbscan = DBSCAN( XYs );







static void thread_loop(){
    while(true){
        XYs = generate_xy();
        dbscan.slowdown( 10 );
        dbscan.run( XYs, 20, 4 );
        std::this_thread::sleep_for(std::chrono::seconds( 2 ));
    }
}




static void run(){
    static bool started;
    if(started)return;
    static std::thread helper(thread_loop);
    started=true;
}





static void dot( XY xy, int radius=3 ){
    SDL_Rect r ={
        .x = (int)(xy.x-(radius/2)),
        .y = (int)(xy.y-(radius/2)),
        .w = radius,
        .h = radius,
    };
    SDL_RenderFillRect( renderer, &r );
}


void render(){ // HEADER

    run();

    const int mac_palette[][3]={
        { 255, 255, 255 }, // 0
        { 252, 244, 4   }, // 1
        { 255, 100, 4   }, // 2
        { 220, 8,   8   }, // 3
        { 240, 8,   132 }, // 4
        { 72,  0,   164 }, // 5
        { 0,   0,   212 }, // 6
        { 0,   172, 232 }, // 7
        { 32,  184, 20  }, // 8
        { 0,   100, 16  }, // 9
        { 88,  44,  4   }, // 10
        { 144, 112, 56  }, // 11
        { 192, 192, 192 }, // 12
        { 128, 128, 128 }, // 13
        { 64,  64,  64  }, // 14
        { 0,   0,   0   }, // 15
    };

    for( int i=0; i<dbscan.colors().size(); i++ ){
        auto xy = XYs[i];
        auto color = dbscan.colors()[i];
        auto rgb = color < 0 ? mac_palette[14] : mac_palette[color%15];
        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
        dot(xy);
    }
}



