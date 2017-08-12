#ifndef PROCESS_THREAD_H
#define PROCESS_THREAD_H

#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
#include <netinet/in.h>
#include "blocking_queue.h"
#include "ad_data.h"
#include "notice_data.h"
#include "json.hpp"


class process_thread {
public:
    process_thread(const std::shared_ptr<blocking_queue<int>> &q);
    ~process_thread();

    // callbask for thread
    void operator()();
/*
    process_thread(const process_thread &) = delete;
    process_thread(process_thread &&) = delete;
    process_thread &operator=(const process_thread &) = delete;
    process_thread &operator=(process_thread &&) = delete;
*/
private:
    // search ad according to request, request -> adlist
    void search_ad();
    void do_search();

    // sort & cut
    void sort_and_cut();
    
    // calculate price per ad
    void calc_price();

    // pack adlist into json, adlist -> response
    void pack_adlist();

    // clear data, prepare for next process
    void clear();

    // get socket descriptor
    std::shared_ptr<blocking_queue<int>> queue;

    // recv client data
    int cfd;
    std::shared_ptr<char> buf;
    static size_t bufSize;
    nlohmann::json request;
    std::unordered_set<std::string> bidwords;

    // client addr
    struct sockaddr_storage addr;
    socklen_t addrlen;
    std::shared_ptr<char> addrStr;
    static size_t addrStrLen;

    // ad list data
    std::vector<ad_data> adlist;
    nlohmann::json response;
};



#endif //PROCESS_THREAD_H



