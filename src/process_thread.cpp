#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include "process_thread.h"
#include "mlog.h"
#include "inet_sockets.h"
#include "config.h"


extern std::shared_ptr<config> cfg;

std::size_t process_thread::bufSize = 10240;
std::size_t process_thread::addrStrLen = 4096;

process_thread::process_thread(const std::shared_ptr<blocking_queue<int>> &q): queue(q), cfd(-1), buf(new char[bufSize], std::default_delete<char[]>()), bidwords(cfg->get_max_bidword_num()), addrStr(new char[addrStrLen], std::default_delete<char[]>()) {
    if (q.get() == NULL) {
        MLOG(MFATAL, "blocking queue is null");
    }
}

process_thread::~process_thread() {

}

void process_thread::operator()() {
    MLOG(MDEBUG, "process thread started");
    
    if (queue.get() == NULL) {
        MLOG(MFATAL, "blocking queue is null");
        return;
    }

    while (true) {
        clear();

        queue->get(cfd);
        MLOG(MDEBUG, "get socket fd from blocking queue: %d", cfd);

        if (cfd < 0) {
            MLOG(MWARNING, "blocking queue get error, cfd < 0");
            continue;
        }

        inet_sockets::get_peer_addr(cfd, (struct sockaddr *)&addr, &addrlen, addrStr.get(), addrStrLen); 

        if (recv(cfd, buf.get(), bufSize, 0) <= 0) {
            MLOG(MWARNING, "read from socket client %s null", addrStr.get());
            close(cfd);
            continue;
        }
        
        MLOG(MDEBUG, "recv buf from client %s : %s", addrStr.get(), buf.get());
        request = nlohmann::json::parse(buf.get());
    
        // search ad from ad file
        search_ad();
        MLOG(MDEBUG, "search ad size according to request: %d", adlist.size());
        // sort & cut
        sort_and_cut();

        // calc price
        calc_price();

        // pack adlist into json
        pack_adlist();

        // write response
        std::size_t bodylen = response.dump().size();
        if (send(cfd, response.dump().c_str(), bodylen, 0) != bodylen) {
            MLOG(MWARNING, "send adlist data to client %s failed", addrStr.get());
        } else {
            MLOG(MWARNING, "send adlist data to client %s successful", addrStr.get());
        }

        close(cfd);
    }

}

void process_thread::search_ad() {
    if (request.empty()) {
        MLOG(MWARNING, "no request data");
        return;
    }
    if (request["adum"] == 0) {
        MLOG(MWARNING, "adnum is 0 in request");
        return;
    }
    if (request["bidwords"].empty()) {
        MLOG(MWARNING, "bidwords is empty in request");
        return;
    }
    do_search(); 
}

void process_thread::do_search() {

    std::ifstream reader(cfg->get_ad_file(), std::ios::binary);
    if (!reader.is_open()) {
        MLOG(MWARNING, "ad file open error");
        return;
    }
    bidwords.clear();
    for (auto it = request["bidwords"].begin(); it != request["bidwords"].end(); ++it) {
        if (!it->is_string()) {
            continue;
        }
        std::string w = *it;
        if (w.size() > cfg->get_max_bidword_len()) {
            continue;
        }
        bidwords.insert(w);
        if (bidwords.size() > cfg->get_max_bidword_num()) {
            break;
        }
    }

    std::size_t winfoid;
    std::string bidword;
    int bid;
    int q;
    std::string title;
    std::string desc1;
    std::string desc2;
    std::string targeturl;
    std::string showurl;
    while (reader >> winfoid >> bidword >> bid >> q >> title >> desc1 >> desc2 >> targeturl >> showurl) {
        if (bidwords.find(bidword) == bidwords.end()) {
            continue;
        }
        ad_data adData;
        adData.winfoid = winfoid;
        adData.bidword = bidword;
        adData.bid = bid;
        adData.q = q;
        adData.title = title;
        adData.desc1 = desc1;
        adData.desc2 = desc2;
        adData.targeturl = targeturl;
        adData.showurl = showurl;
        adlist.push_back(adData);
        if (adlist.size() > cfg->get_max_adnum()) {
            break;
        }
    }
}

void process_thread::sort_and_cut() {
    if (adlist.empty()) {
        MLOG(MWARNING, "adlist data is empty");
        return;
    }
    std::sort(adlist.begin(), adlist.end(), [](const ad_data &adData1, const ad_data &adData2){ return adData1.get_value() > adData2.get_value(); });
    int adnum = std::min((std::size_t)request["adum"], cfg->get_max_bidword_num());
    adlist.erase(std::next(adlist.cbegin(), adnum), adlist.end());
}

void process_thread::calc_price() {
    for (auto it = adlist.begin(), next = std::next(it); next != adlist.end(); ++it, ++next) {
        // price1 = q2 * bid2 / q1
        if (it->q == 0) {
            it->charge = it->bid;
        } else {
            it->charge = next->q * next->bid / it->q;
        }
    }
    adlist.back().charge = adlist.back().bid;
    MLOG(MDEBUG, "adlist data is sorted and calc price");
}

void process_thread::pack_adlist() {
    
    response.clear();
    
    if (adlist.empty()) {
        MLOG(MWARNING, "adlist is empty");
        return;
    }

    for (auto &ad : adlist) {
        nlohmann::json obj = {
            {"winfoid", ad.winfoid},
            {"term", ad.bidword},
            {"bid", ad.bid},
            {"q", ad.q},
            {"charge", ad.charge},
            {"desc1", ad.desc1},
            {"desc2", ad.desc2},
            {"showurl", ad.showurl},
            {"targeturl", ad.targeturl}
        };
        response.push_back(obj);
        if (response.size() > request["adum"]) {
            break;
        }
    }
}

void process_thread::clear() {
    cfd = -1;
    std::fill_n(buf.get(), bufSize, '\0');
    request.clear();
    std::fill_n(addrStr.get(), addrStrLen, '\0');
    snprintf(addrStr.get(), addrStrLen, "[UNKNOW]");
    adlist.clear();
    response.clear();
}


