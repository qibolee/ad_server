#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include "process_thread.h"
#include "mlog.h"
#include "inet_sockets.h"
#include "config.h"
#include "string_util.h"


extern std::shared_ptr<config> cfg;

std::size_t process_thread::bufSize = 10240;
std::size_t process_thread::addrStrLen = 4096;

process_thread::process_thread(const std::shared_ptr<blocking_queue<int>> &q): queue(q), cfd(-1), 
    buf(new char[bufSize], std::default_delete<char[]>()), bidwords(cfg->get_max_bidword_num()), addrStr(new char[addrStrLen], std::default_delete<char[]>()) {
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
        
        MLOG(MDEBUG, "recv buf from client %s: %s", addrStr.get(), buf.get());
        parse_request();
        //request = nlohmann::json::parse(buf.get());
    
        // search ad from ad file
        search_ad();
        MLOG(MDEBUG, "search ad, get ad size: %lu", adlist.size());

        // sort & cut
        sort_and_cut();
        MLOG(MDEBUG, "sort and cut, adlist size: %lu", adlist.size());

        // calc price
        calc_price();
        MLOG(MDEBUG, "calc price, adlist size: %lu", adlist.size());

        // pack adlist into json
        std::size_t bodylen = pack_adlist();
        MLOG(MDEBUG, "pack response ad data: %s", buf.get());

        // write response
        if (send(cfd, buf.get(), bodylen, 0) != bodylen) {
            MLOG(MWARNING, "send adlist data to client %s failed", addrStr.get());
        } else {
            MLOG(MDEBUG, "send adlist data to client %s successful", addrStr.get());
        }

        close(cfd);
    }

}

void process_thread::parse_request() {
    std::vector<std::string> ret = string_util::split(buf.get(), "\t");
    if (ret.empty()) {
        MLOG(MWARNING, "parse request failed, split zero");
        return;
    }
    requestNum = std::stoul(ret[0]);
    for (int i = 1; i < ret.size(); ++i) {
        if (ret[i].size() > cfg->get_max_bidword_len()) {
            continue;
        }
        bidwords.insert(ret[i]);
        if (bidwords.size() > cfg->get_max_bidword_num()) {
            break;
        }
    }
}

void process_thread::search_ad() {
    if (requestNum == 0 || bidwords.empty()) {
        MLOG(MWARNING, "requestNum is 0 or bidwords is empty");
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

    while (reader.getline(buf.get(), bufSize)) {
        std::vector<std::string> ret = string_util::split(buf.get(), "\t");
        if (ret.size() != 9 || bidwords.find(ret[1]) == bidwords.end()) {
            MLOG(MWARNING, "read from ad file data error, line: %s", buf.get());
            continue;
        }
        ad_data adData;
        adData.winfoid = std::stoull(ret[0]);
        adData.bidword = ret[1];
        adData.bid = std::stoi(ret[2]);
        adData.q = std::stoi(ret[3]);
        adData.title = ret[4];
        adData.desc1 = ret[5];
        adData.desc2 = ret[6];
        adData.targeturl = ret[7];
        adData.showurl = ret[8];
        adlist.push_back(adData);
        if (adlist.size() > cfg->get_max_adnum()) {
            break;
        }
    }

    reader.close();
}

void process_thread::sort_and_cut() {
    if (adlist.empty()) {
        MLOG(MWARNING, "adlist data is empty");
        return;
    }
    std::sort(adlist.begin(), adlist.end(), [](const ad_data &adData1, const ad_data &adData2){ return adData1.get_value() > adData2.get_value(); });
    int adnum = std::min(requestNum, cfg->get_max_bidword_num());
    if (adlist.size() > adnum) {
        adlist.erase(std::next(adlist.cbegin(), adnum), adlist.end());
    }
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
}

std::size_t process_thread::pack_adlist() {
    
    if (adlist.empty()) {
        MLOG(MWARNING, "adlist is empty");
        return 0;
    }
    std::size_t len = 0;
    for (auto &ad : adlist) {
        int l = std::snprintf(buf.get() + len, bufSize, "%lu\t%s\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%s\n", ad.winfoid, ad.bidword.c_str(), ad.bid, ad.q, ad.charge, ad.title.c_str(), ad.desc1.c_str(), ad.desc2.c_str(), ad.showurl.c_str(), ad.targeturl.c_str());
        len += l;
    }
    return len;
}

void process_thread::clear() {
    cfd = -1;
    std::fill_n(buf.get(), bufSize, '\0');
    bidwords.clear();
    std::fill_n(addrStr.get(), addrStrLen, '\0');
    snprintf(addrStr.get(), addrStrLen, "[UNKNOW]");
    adlist.clear();
}


