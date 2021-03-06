//
//  ESDDns.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/24/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDDns.hpp"

#define ADDRESS_LENGHT 17

namespace esdht {
    /**
     * @brief 构建DNS错误信息
     */
    
    ESDDns::ESDDns(){
        
        loop = uv_loop_new();
        uv_loop_init(loop);
        hints.ai_family = PF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = 0;
        resolver.data = this;
        this->handleIPCallback = nullptr;
        
    }//ESDDns()
    
    
    ESDDns::~ESDDns(){
        
        stopLoop(loop);

    }//~ESDDns()
    
    
   
    
    void on_resolved(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res) {
        ESDDns *object = static_cast<ESDDns *>(resolver->data);
        if (status < 0) {
            
            if(object->handleIPCallback != nullptr)
                object->handleIPCallback(nullptr, object->port);
            throw ESDDnsError(std::string("getaddrinfo call error ") + uv_err_name(status));
            return;
            
        }
        
        char addr[ADDRESS_LENGHT] = {'\0'};
        uv_ip4_name((struct sockaddr_in*) res->ai_addr, addr, sizeof(addr) - 1);
        uv_freeaddrinfo(res);
        
        if(object != NULL && object->handleIPCallback != nullptr)
            object->handleIPCallback(std::string(addr), object->port);
        
    }//on_resolved
    
    void ESDDns::getIpOfURL(std::string url, unsigned int port, std::function<void(std::string address, unsigned port)> callback){
        this->handleIPCallback = callback;
        this->port = port;
        int r = uv_getaddrinfo(loop, &resolver, on_resolved, url.c_str(), std::to_string(port).c_str(), &hints);
        if (r) {
            fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(r));
            throw ESDDnsError(std::string("getaddrinfo call error ") + uv_err_name(r));
        }
        uv_run(loop, UV_RUN_DEFAULT);
        
    }//getIpOfURL
    
}