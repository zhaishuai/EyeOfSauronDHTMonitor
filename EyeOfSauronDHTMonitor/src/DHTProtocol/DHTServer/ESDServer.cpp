//
//  ESDServer.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/5/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDServer.hpp"
#include "ESDUdp.hpp"

namespace esdht {
    
    ESDServer::ESDServer(std::string ip, int port){
        
        this->ip   = ip;
        this->port = port;
        
        loop   = std::unique_ptr<uv_loop_t>(new uv_loop_t);
        uv_loop_init(&(*loop));
        udp    = std::unique_ptr<ESDUdp>(new ESDUdp(&(*loop)));
        thread = std::unique_ptr<threadPool::Thread>(new threadPool::Thread);
        async  = std::unique_ptr<ESDAsync>(new ESDAsync);
        udp->addAsync(async.get(), udp->receiveLoop, [this](void *data){
            udp->stopReceive();
        });
        
    }
    
    ESDServer::~ESDServer(){
        uv_loop_close(&(*loop));
    }
    
    int count = 0;
    void ESDServer::startServer(){
        
        thread->startThread([this]{
            printf("server startup!\n");
            udp->receive(ip, port, [](std::string msg){
                printf("count:%d  msg: %s", count, msg.c_str());
                count++;
            });
        });
        
    }
    
    void ESDServer::stopServer(){
        async->sendAsync(nullptr);
    }
    
}