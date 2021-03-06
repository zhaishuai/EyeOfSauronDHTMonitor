//
//  ESDClient.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/6/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDClient_hpp
#define ESDClient_hpp

#include <stdio.h>
#include "ESDDHTProtocol.hpp"
#include "ESDKrpc.hpp"

namespace esdht {
    
    class ESDClient : public ESDDHTProtocol{
        
    protected:
        std::string key;
    public:
        std::string              nodeID;
        std::string              transactionID;
        std::unique_ptr<ESDUdp>  udp;
        std::unique_ptr<ESDKrpc> krpc;
        uv_loop_t                *loop;
        ESDClient(std::string nodeID, std::string transactionID);
        ~ESDClient();
        virtual void ping(std::string ip, int port);
        virtual void findNode(std::string ip, int port);
    
    };
    
}

#endif /* ESDClient_hpp */
