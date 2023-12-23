
#undef NDEBUG
#include <assert.h>

#include <SDL2/SDL.h> // HEADER
#include "main.h"
#include "DBSCAN.h"


void frame( SDL_Event event, float dt ){  // HEADER

    SDL_SetRenderDrawColor( renderer, 0,0,0, 255 );
    SDL_RenderClear( renderer );

    static bool dbscan_started;
    if(!dbscan_started){
        dbscan_started=true;
        DBSCAN_thread();
    }
    

//    static float restart_deadline;
//    if( restart_deadline > 0 ){
//        restart_deadline -= dt;
//        if( restart_deadline <= 0 ){
//            restart_deadline = 0;
//            fprintf(stderr,"restart\n");
//            dbscan = DBSCAN(); // restart
//        }
//    }

//    const float resume_secs = 0.01;
//    static float resume_timeout = 1E9;
//    resume_timeout += dt;
//    if( resume_timeout >= resume_secs ){
//        resume_timeout = 0;
//        if(dbscan) dbscan(); // resume
//        else restart_deadline = 3;
//    }

//    const float restart_secs = 5;
//    static float restart_timeout = 0;
//    restart_timeout += dt;
//    if( restart_timeout >= restart_secs ){
//        restart_timeout = 0;
//        if(!dbscan) dbscan = DBSCAN();
//    }
                

    DBSCAN_render();

//    {
//        const int barh = 3;
//        SDL_Rect r ={
//            .x = 0,
//            .y = WINH-barh,
//            .w = (int)(restart_timeout*WINW/restart_secs),
//            .h = barh,
//        };
//        SDL_SetRenderDrawColor( renderer, 255,255,255, 255 );
//        SDL_RenderFillRect( renderer, &r );
//    }
}


