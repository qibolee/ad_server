#ifndef AD_DATA_H
#define AD_DATA_H

#include <string>


struct ad_data {
    std::size_t winfoid;
    std::string bidword; // bidword
    int bid;
    int q;
    int charge; // gsp
    std::string title;
    std::string desc1;
    std::string desc2;
    std::string targeturl;
    std::string showurl;

    int get_value() const { return bid * q; }
};


#endif //AD_DATA_H

