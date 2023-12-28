
#ifndef _DBSCAN_h_
#define _DBSCAN_h_

#undef NDEBUG
#include <assert.h>

#include <vector>
#include <cmath>
#include <chrono>


template<typename T>
class DBSCAN {

    std::vector<int> m_colors;
    double m_epsilon;
    int m_colors_count;
    int m_minPts;
    int m_slowdown_msec;

    double distance( const T& p1, const T& p2 ) const;

    std::vector<int> getNeighbors( const std::vector<T>& Ts, int index ) const;

    void expandCluster( std::vector<T>& Ts, int index, const std::vector<int>& neighbors );

public:
    int slowdown( int msec );
    int colors_count();
    std::vector<int> colors();
    DBSCAN();
    DBSCAN( std::vector<T> Ts );
    DBSCAN( std::vector<T> Ts, double epsilon, int minPts );
    std::vector<int> run( std::vector<T> Ts, double epsilon, int minPts );
    int classify( T& t );
};



// x rendering del processo in realtime
// il dbscan dovrà girare in un thread
// il labeling va acceduto via .colors()
#define SLOWDOWN { if(m_slowdown_msec>0) std::this_thread::sleep_for(std::chrono::milliseconds( m_slowdown_msec )); }





template<typename T>
int DBSCAN<T>::colors_count(){
    return m_colors_count;
}


template<typename T>
std::vector<int> DBSCAN<T>::colors(){
    return m_colors;
};



template<typename T>
double DBSCAN<T>::distance( const T& p1, const T& p2 ) const {
    auto d = p1-p2;
    return d*d;
}



template<typename T>
std::vector<int> DBSCAN<T>::getNeighbors( const std::vector<T>& Ts, int index ) const {
    std::vector<int> neighbors;
    for( int i=0; i<Ts.size(); ++i ){
        if( distance( Ts[index], Ts[i]) < m_epsilon*m_epsilon ){
            neighbors.push_back(i);
        }
    }
    return neighbors;
}






template<typename T>
void DBSCAN<T>::expandCluster(
    std::vector<T>& Ts,
    int index,
    const std::vector<int>& neighbors
){
    SLOWDOWN;

    m_colors[index] = m_colors_count;

    for( int i: neighbors ){

        if( m_colors[i] == 0 ){
            m_colors[i] = m_colors_count;

            std::vector<int> newNeighbors = getNeighbors( Ts, i );

            if( newNeighbors.size() >= m_minPts ){
                // Espandi il cluster se il punto ha abbastanza vicini
                expandCluster( Ts, i, newNeighbors );
            }
            continue;
        }

        if( m_colors[i] == -1 ){
            // Aggiorna il punto rumoroso al bordo del cluster
            m_colors[i] = m_colors_count;
            continue;
        }
    }
}




template<typename T>
std::vector<int> DBSCAN<T>::run( std::vector<T> Ts, double epsilon, int minPts ){
    
    m_epsilon = epsilon;
    m_minPts  = minPts;

    m_colors_count = 0;
    m_colors.clear();
    m_colors.resize(Ts.size());

    for( int i=0; i<Ts.size(); i++ ){
            
        if( m_colors[i] != 0 )continue; // 0 == uncategorized

        SLOWDOWN;

        auto neighbors = getNeighbors( Ts, i );

        if( neighbors.size() < m_minPts ){
            m_colors[i] = -1; // noise
            continue;
        }

        ++m_colors_count;
        expandCluster( Ts, i, neighbors );
    }

    return m_colors;
}


template<typename T>
DBSCAN<T>::DBSCAN(){
    m_slowdown_msec = 0;
}

// costruttore nop
// permette l'inferenza del typename
template<typename T>
DBSCAN<T>::DBSCAN( std::vector<T> Ts ){
    m_slowdown_msec = 0;
}

template<typename T>
DBSCAN<T>::DBSCAN( std::vector<T> Ts, double m_epsilon, int m_minPts ){
    m_slowdown_msec = 0;
    run( Ts, m_epsilon, m_minPts );
}


template<typename T>
int DBSCAN<T>::classify( T& t ){
    return 0;
}

template<typename T>
int DBSCAN<T>::slowdown( int msec ){
    m_slowdown_msec = msec;
    return m_slowdown_msec;
}






/*








struct DATAPOINT {
    double x,y;
    DATAPOINT(){
        x = 0;
        y = 0;
    }
    DATAPOINT( double _x, double _y ){
        x = _x;
        y = _y;
    }
    double operator* ( DATAPOINT& rhs ){
        return x*rhs.x + y*rhs.y;
    }
    DATAPOINT operator- ( DATAPOINT& rhs ){
        return DATAPOINT( x-rhs.x, y-rhs.y );
    }
};


/*


int main(){

    std::vector<DATAPOINT> dataset;
    DATAPOINT new_datapoint;

    auto dbscan = DBSCAN( dataset, 10, 3 );

    for( int i=0; i<dataset.size(); i++ ){
        auto xy = dataset[i];
        auto color = dbscan.colors()[i];
    }

    auto color = dbscan.classify( new_datapoint );

    return 0;
}


*/

#undef SLOWDOWN

#endif // _DBSCAN_h_

