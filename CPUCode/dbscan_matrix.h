#ifndef __DBSCAN_MATRIX_H__
#define __DBSCAN_MATRIX_H__

#include "dbscan.h"

namespace clustering{
    class DBSCAN_Matrix : public DBSCAN{
    public:
        DBSCAN_Matrix(float eps, size_t min_elems);
        virtual ~DBSCAN_Matrix();
        
        virtual void fit();
        virtual void test();

    private:
        /*****************************************************************************************/
        // Variables and functions for distance matrix method
        // Brute force algorithm, calculate a distance matrix before the real algorithm
        // The algorithm is the original version of DBSCAN in the paper in KDD-96
        // Use this version as a baseline
        // Implemented in dbscan_matrix.cpp
        std::vector<uint8_t> m_visited;

        const DistanceMatrix calc_dist_matrix();
        Neighbors find_neighbors_distance_matrix(const DistanceMatrix & D, uint32_t pid);
        void expand_cluster_distance_matrix(Neighbors& ne, const DistanceMatrix& dm, const int cluster_id, const int pid);
        void dbscan_distance_matrix( const DistanceMatrix & dm );

    };

}

#endif

