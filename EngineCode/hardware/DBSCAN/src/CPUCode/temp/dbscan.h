#ifndef __DBSCAN_H__
#define __DBSCAN_H__

#include <vector>
#include <unordered_map>
#include <boost/numeric/ublas/matrix.hpp>

#include "util.h"

using namespace boost::numeric;
using std::cout;
using std::endl;

namespace clustering
{
    class DBSCAN
    {
    public:
        // data type defination
        typedef ublas::vector<double> FeaturesWeights;
        typedef ublas::matrix<double> ClusterData;
        typedef ublas::matrix<double> DistanceMatrix;
        typedef std::vector<uint32_t> Neighbors;
        typedef std::vector<int32_t> Labels;

        /*****************************************************************************************/
        // functions for all DBSCAN methods
        // implemented in dbscan.cpp
        static void cmp_result(const Labels& a, const Labels & b);
        static double get_clock();

        // initialize, deconstruct and reset
        // use virtual deconstruct function, avoid memory leak
        DBSCAN(double eps, size_t min_elems);
        DBSCAN();
        virtual ~DBSCAN();

        // data initialization/reset functions
        void init(double eps, size_t min_elems);
        void reset();
        void gen_cluster_data( size_t features_num, size_t elements_num);
        void read_cluster_data( size_t features_num, size_t elements_num, std::string filename);
        
        // output functions
        const Labels & get_labels() const;
        void output_result(const std::string filename) const;
        void reshape_labels();

        /*****************************************************************************************/
        // pure virtual functions, just serve as an interface
        // you can not create a instance of class DBSCAN
        // different kinds of algorithm are implemented in different header files and cpp files
        virtual void fit() = 0;
        virtual void test() = 0;

    protected:
        /*****************************************************************************************/
        // variables and functions for all DBSCAN methods
        // implemented in dbscan.cpp
        double m_eps_sqr;
        size_t m_min_elems;
        Labels m_labels;
        ClusterData cl_d;

        void prepare_labels( size_t s );

    };

    std::ostream& operator<<(std::ostream& o, DBSCAN & d);
}

#endif
