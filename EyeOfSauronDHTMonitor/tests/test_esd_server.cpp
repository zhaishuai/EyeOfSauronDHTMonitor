//
//  test_esd_server.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/5/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esd_server.hpp"
#include "ESDServer.hpp"

using namespace esdht;

namespace test_esd_server {
    
    void test_server_start_up(){
        for(int j = 0; j < 100000; j++){
        
            
            
//        ESDServer server("0.0.0.0", 8221);
//        server.startServer();
        
        ESDUdp clinet;
//        clinet.setSendPort(8222);
        for(int i = 0 ; i < 1 ; i++){
            clinet.send("10.12.112.55", 8221, "hello world\n", nullptr, nullptr);
//          usleep(1000*100);
        }
        
//        server.stopServer();
        }
    }
    
}