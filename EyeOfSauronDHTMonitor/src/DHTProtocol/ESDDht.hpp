//
//  ESDDht.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/4/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDDht_hpp
#define ESDDht_hpp

#include <stdio.h>
#include "ESDDHTProtocol.hpp"
#include "ESDUdp.hpp"

namespace esdht {
    
    class ESDDht : public ESDDHTProtocol{
        
    public:
        std::string             nodeID;
        std::string             transactionID;
        std::unique_ptr<ESDUdp> udp;
        uv_loop_t               *loop;
        ESDDht(std::string nodeID, std::string transactionID);
        ~ESDDht();
        virtual void ping(std::string ip, int port);
        
    };
    
    
}

#endif /* ESDDht_hpp */
