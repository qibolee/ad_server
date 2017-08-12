#ifndef NOTICE_DATA_H
#define NOTICE_DATA_H


// notice log, per show ad
struct notice_data {
    std::string ip; // client ip
    std::string query;
    size_t winfoid;
    int show; // almost 1
    int charge; // gsp charge, uint: cents
    std::string title;
    std::string desc1;
    std::string desc2;
    std::string showurl;
    std::string targeturl;

};



#endif //NOTICE_DATA_H
