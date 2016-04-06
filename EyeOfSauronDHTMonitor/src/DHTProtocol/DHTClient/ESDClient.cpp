//
//  ESDClient.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/6/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDClient.hpp"
#include "bencoding.h"
using namespace bencoding;

namespace esdht {
    
    ESDClient::ESDClient(std::string nodeID, std::string transactionID){
        this->nodeID        = nodeID;
        this->transactionID = transactionID;
        loop                = new uv_loop_t;
        uv_loop_init(loop);
        this->udp           = std::unique_ptr<ESDUdp>(new ESDUdp(loop));
    }
    
    ESDClient::~ESDClient(){
        delete loop;
    }
    
    void ESDClient::ping(std::string ip, int port){
        
        
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
        
        //处理time out
        //
        try {
            udp->send(ip, port, dic.c_str(), nullptr, [this](std::string pong){
                printf("response:%s\n", pong.c_str());
                
                //处理非bencode得数据
                //
                try {
                    std::unique_ptr<Decoder> decoder;
                    std::shared_ptr<BItem> bItem(decoder->decode(pong));
                    auto dict = bItem->as<BDictionary>();
                    
                    //在此处判断bencode是不是错误状态。
                    //
                    if(bencodeIsErrorPackets(dict)) return;
                    if(!checkKeyExist(dict, "r"))   return;
                    auto sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
                    if(!checkKeyExist(sDictionary, "id"))  return;
                    auto id = (*sDictionary)[BString::create("id")]->as<BString>();
                    printf("id: %s   size:%ld\n", id->value().c_str(), id->value().size());
                } catch (DecodingError error) {
                     printf("%s", error.what());
                }
 
            });
        } catch (ESDUdpError error) {
            printf("%s", error.what());
        }
        
        
    }
    
}