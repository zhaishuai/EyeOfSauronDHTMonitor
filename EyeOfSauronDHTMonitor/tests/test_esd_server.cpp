//
//  test_esd_server.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/5/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esd_server.hpp"
#include "ESDServer.hpp"
#include "ESDDht.hpp"
#include "bencoding.h"
#include "ESDDns.hpp"

using namespace bencoding;
using namespace esdht;

namespace test_esd_server {
    
    void test_server_start_up(){
        

        ESDServer server("0.0.0.0", 6882);
        server.startServer();
        ESDDns dns;
        dns.getIpOfURL("router.bittorrent.com", "554", [](int status, std::string address){
            printf("ip:%s   status:%d\n", address.c_str(), status);
            
            ESDDht client("aa", "abcdefghij0123456789");
            
            for(int i = 0 ; i < 1000 ; i++){
                
                client.ping(address, 6882);
                usleep(1000*500);
            }
            
        });
        
        
        server.stopServer();

        
    }
    
}