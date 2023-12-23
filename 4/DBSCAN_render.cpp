
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


void DBSCAN_render(){ // HEADER

    run();

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

    for( int i=0; i<dbscan.colors().size(); i++ ){
        auto xy = XYs[i];
        auto color = dbscan.colors()[i];
        auto rgb = color < 0 ? mac_palette[14] : mac_palette[color%16];
        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
        dot(xy);
    }
}



