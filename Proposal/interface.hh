#pragma once

#include <tuple>
#include <string>
#include <vector>

template <typename T, typename U>
class RiverData
{
    private:
        T data;
        U data_instance;
    public:
        /* data access */
        std::tuple <int, int> shape(const T& data) const;                                       // returns a tuple (# data instances, # features)
        int rows(const T& data) const;                                                          // returns # data instances
        int cols(const T& data) const;                                                          // returns # features
        
        /* data extraction */
        U operator[](const int& i) const;                                                       // returns row i (conversion to vector)
        U get_feature(const std::string& feature_name) const;                                   // returns column data of that feature  
        U operator()(const int& i, const int& j) const;                                         // returns (i, j)th entry
        U get_associated_rivers(const std::string& river_id) const;                             // returns ids of rivers connecting to the river (id)
        double discharge(const std::string& river_id) const ;                                   // returns discharge of river provided id
        std::string main_river() const;                                                         // returns main_river ID
        std::vector <std::string> get_rivers_catchment(const std::string& river_id) const;      // returns river IDs in the catchement area of river (id)

        /* data geometry */
        std::vector <double> get_river_path(const std::string& river_id) const;                 // returns vector path of river given the id

};