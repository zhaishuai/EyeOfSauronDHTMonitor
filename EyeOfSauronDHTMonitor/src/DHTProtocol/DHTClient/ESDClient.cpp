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
//            std::string id;
//            krpc->handlePingResponse(pong, &id);
//
//            });
            udp->sendAsync(ip, port, krpc->ping(transactionID, nodeID), nullptr);
        } catch (ESDUdpError error) {
            fprintf(stderr ,"%s", error.what());
        }
    }
    
    
}