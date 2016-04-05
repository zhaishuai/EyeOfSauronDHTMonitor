//
//  ESDDht.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/4/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDDht.hpp"
#include "bencoding.h"
using namespace bencoding;

namespace esdht {
    
    ESDDht::ESDDht(std::string nodeID, std::string transactionID){
        this->nodeID        = nodeID;
        this->transactionID = transactionID;
        loop                = new uv_loop_t;
        uv_loop_init(loop);
        this->udp           = std::unique_ptr<ESDUdp>(new ESDUdp(loop));
    }
    
    ESDDht::~ESDDht(){
        delete loop;
    }
    
    void ESDDht::ping(std::string ip, int port){
        
        
        std::unique_ptr<Encoder> encoder = Encoder::create();
        std::shared_ptr<BDictionary> bDictionary(BDictionary::create());
        (*bDictionary)[BString::create("t")] = BString::create("aa");
        (*bDictionary)[BString::create("y")] = BString::create("qq");
        (*bDictionary)[BString::create("q")] = BString::create("ping");
        std::shared_ptr<BDictionary> subDictionary(BDictionary::create());
        (*subDictionary)[BString::create("id")] = BString::create("abcdefghij0123456789");
        (*bDictionary)[BString::create("a")] = subDictionary;
        std::string dic = encoder->encode(bDictionary);
        printf("%s\n", dic.c_str());
        
        
        udp->send(ip, port, dic.c_str(), nullptr, [](std::string pong){
            printf("response:%s\n  size:%ld\n", pong.c_str(), pong.size());
            
            std::unique_ptr<Decoder> decoder;
            std::shared_ptr<BItem> bItem(decoder->decode(pong));
            auto dict = bItem->as<BDictionary>();
            std::shared_ptr<BDictionary> sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
            std::shared_ptr<BString> id = (*sDictionary)[BString::create("id")]->as<BString>();
            std::string ipAndPortStr = id->value().substr(20, 6);
            printf("id: %s   size:%ld\n", id->value().c_str(), id->value().size());
        });
    }
    
}