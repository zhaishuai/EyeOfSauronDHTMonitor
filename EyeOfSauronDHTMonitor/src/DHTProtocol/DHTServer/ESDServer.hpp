//
//  ESDServer.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/5/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDServer_hpp
#define ESDServer_hpp

#include <stdio.h>
#include "ThreadPool.hpp"

namespace esdht {
    
    class ESDServer : public ESDObject{
    protected:
        
        std::string                         ip;
        int                                 port;
        std::unique_ptr<uv_loop_t>          loop;
        std::unique_ptr<ESDUdp>             udp;
        std::unique_ptr<threadPool::Thread> thread;
        std::unique_ptr<ESDAsync>           async;
        
    public:
        
        ESDServer(std::string ip, int port);
        
        ~ESDServer();
        
        void startServer();
        
        void stopServer();
    
    
    };
    
}

#endif /* ESDServer_hpp */
