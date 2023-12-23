
#undef NDEBUG
#include <assert.h>

#include <SDL2/SDL.h> // HEADER
#include "main.h"
#include "DBSCAN.h"



void frame( SDL_Event event, float dt ){  // HEADER

    SDL_SetRenderDrawColor( renderer, 0,0,0, 255 );
    SDL_RenderClear( renderer );
                
    const float step_secs = 0.01;
    static float step_timeout = 1E9;
    step_timeout += dt;
    if( step_timeout >= step_secs ){
        step_timeout = 0;
        DBSCAN_step();
    }

    DBSCAN_render();

    const float init_secs = 5;
    static float init_timeout = 1E9;
    init_timeout += dt;
    if( init_timeout >= init_secs ){
        init_timeout = 0;
        DBSCAN_init();
    }

    {
        const int barh = 3;
        SDL_Rect r ={
            .x = 0,
            .y = WINH-barh,
            .w = (int)(init_timeout*WINW/init_secs),
            .h = barh,
        };
        SDL_SetRenderDrawColor( renderer, 255,255,255, 255 );
        SDL_RenderFillRect( renderer, &r );
    }
}


