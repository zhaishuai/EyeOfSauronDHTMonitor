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
        this->krpc          = std::unique_ptr<ESDKrpc>(new ESDKrpc);
        loop                = new uv_loop_t;
        uv_loop_init(loop);
        this->udp           = std::unique_ptr<ESDUdp>(new ESDUdp(loop));
    }
    
    ESDClient::~ESDClient(){
        delete loop;
    }
    
    void ESDClient::ping(std::string ip, int port){
        
        //处理time out
        //
        try {
//            udp->send(ip, port, krpc->ping(transactionID, nodeID), nullptr, [this](std::string pong){
//                //处理非bencode得数据
//                //
//                try {
//                    std::unique_ptr<Decoder> decoder;
//                    std::shared_ptr<BItem> bItem(decoder->decode(pong));
//                    auto dict = bItem->as<BDictionary>();
//                    
//                    //在此处判断bencode是不是错误状态。
//                    //
//                    if(bencodeIsErrorPackets(dict)) return;
//                    if(!checkKeyExist(dict, "r"))   return;
//                    auto sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
//                    if(!checkKeyExist(sDictionary, "id"))  return;
//                    auto id = (*sDictionary)[BString::create("id")]->as<BString>();
//                    pingCallback(id->value());
//                    
//                } catch (DecodingError error) {
//                     fprintf(stderr ,"%s\n", error.what());
//                }
// 
//            });
            udp->sendAsync(ip, port, krpc->ping(transactionID, nodeID), nullptr);
        } catch (ESDUdpError error) {
            fprintf(stderr ,"%s", error.what());
        }
    }
    
    void ESDClient::pingCallback(std::string nodeId){
        //TODO:在此处处理ping的响应
        printf("id: %s\n", nodeId.c_str());
    }
    
}