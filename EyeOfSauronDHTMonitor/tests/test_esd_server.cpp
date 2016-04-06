//
//  test_esd_server.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/5/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esd_server.hpp"
#include "ESDServer.hpp"
#include "bencoding.h"
#include "ESDDns.hpp"

using namespace bencoding;
using namespace esdht;

namespace test_esd_server {
    
    void test_server_start_up(){
        

        ESDServer server("0.0.0.0", 6881);
        server.startServer();
        
//        server.stopServer();

        
    }
    
}