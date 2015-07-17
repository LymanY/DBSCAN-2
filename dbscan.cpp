#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>
#include <map>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <vector>

#include "dbscan.h"

namespace clustering
{
    void DBSCAN::read_cluster_data(size_t features_num, size_t elements_num, std::string filename){
        cl_d = DBSCAN::ClusterData(elements_num, features_num);
        std::ifstream fin(filename.data());
        for(size_t i=0; i<elements_num; i++)
            for(size_t j=0; j<features_num; j++)
                fin>>cl_d(i, j);
        fin.close();
        /*
        for(size_t i=0; i<10; i++){
            for(size_t j=0; j<features_num; j++)
                cout<<cl_d(i, j)<<" ";
            cout<<endl;
        }
        */
    }

    void DBSCAN::gen_cluster_data( size_t features_num, size_t elements_num ){
        cl_d = DBSCAN::ClusterData(elements_num, features_num);
        for (size_t i = 0; i < elements_num; ++i){
            for (size_t j = 0; j < features_num; ++j)
                cl_d(i, j) = (-1.0 + rand() * (2.0) / RAND_MAX);
        }
    }

    void DBSCAN::cmp_result(const Labels& a, const Labels& b){
        int cnt_right = 0;
        assert(a.size() == b.size());

        for(unsigned int i=0; i<a.size(); i++)
            if(a[i] == b[i])
                cnt_right++;
        double rate = (double)cnt_right / (double)a.size();
        cout<<"similarity: "<<rate<<endl;
    }

    double DBSCAN::get_clock(){
        clock_t t = clock();
        return double(t) / double(CLOCKS_PER_SEC);
    }

    std::ostream& operator<<(std::ostream& o, DBSCAN & d)
    {
        for(const auto & l : d.get_labels())
            o << l << " ";
        o << endl;
        return o;
    }

    DBSCAN::DBSCAN(){    }

    DBSCAN::DBSCAN(double eps, size_t min_elems)
    :m_min_elems( min_elems ){
        // given eps, convert to eps_sqr for convenience
        m_eps_sqr = eps * eps;
        reset();
    }

    DBSCAN::~DBSCAN(){
    }

    void DBSCAN::init(double eps, size_t min_elems)
    {
        // given eps, convert to eps_sqr for convenience
        m_eps_sqr = eps * eps;
        m_min_elems = min_elems;
    }

    void DBSCAN::reset(){
        m_labels.clear();
    }

    const DBSCAN::Labels & DBSCAN::get_labels() const{
        return m_labels;
    }

    int DBSCAN::get_cluster_number() const{
        return uf.get_count();
    }

    void DBSCAN::output_result(const std::string filename) const {
        uint32_t size_data = cl_d.size1();
        uint32_t size_feature = cl_d.size2();
        uint32_t size_ans = m_labels.size();
        assert( size_data == size_ans);

        // map the cluster result into number from 0 to max
        std::ofstream fout(filename.data());
        for(uint32_t i=0; i < size_data; i++){
            for(uint32_t j=0; j<size_feature; j++)
                fout<<cl_d(i, j)<<" ";
            fout<<m_labels[i]<<endl;
        }
        fout.close();
    }

    void DBSCAN::reshape_labels(){
        unsigned int size_data = m_labels.size();
        int index = 0;
        std::map<int, int> mapping;
        for(uint32_t i=0; i<size_data; i++){
            if(m_labels[i] == -1)
                continue;
            if(mapping.find(m_labels[i]) == mapping.end()){
                mapping.insert(std::make_pair(m_labels[i], index));
                index++;
            }
            m_labels[i] = mapping.find(m_labels[i])->second;
        }
        cout<<"get "<<mapping.size()<<" clusters"<<endl;
    }

    void DBSCAN::prepare_labels( size_t s ){
        m_labels.resize(s);
        for( auto & l : m_labels)
            l = -1;
    }

    // two public fit interface 
    void DBSCAN::fit_distance_matrix() {
        prepare_labels( cl_d.size1() );
        const DBSCAN::DistanceMatrix D = calc_dist_matrix();
        dbscan_distance_matrix( D );
    }

    void DBSCAN::fit_grid_based(){
        prepare_labels(cl_d.size1());

        hash_construct_grid();
        determine_core_point_grid();

        double clock1 = get_clock();
        merge_clusters();
        double clock2 = get_clock();
        cout<<clock2 - clock1<<endl;

        determine_boarder_point();
    }

    void DBSCAN::fit_grid_reduced_precision() {
        prepare_labels(cl_d.size1());

        hash_construct_grid();
        //detect_cell_size();
        
        reduce_precision(m_min_elems);

        determine_core_point_grid_reduced();
        merge_clusters_reduced();
        determine_boarder_point_reduced();
    }

    void DBSCAN::test(){
        return;
    }

}
