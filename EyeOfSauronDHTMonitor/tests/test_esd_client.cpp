//
//  test_esd_client.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/6/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esd_client.hpp"
#include "ESDClient.hpp"
#include "bencoding.h"
#include "ESDDns.hpp"

using namespace bencoding;
using namespace esdht;

namespace test_esd_client {
    
    void test_esd_client(){
        ESDDns dns;
        dns.getIpOfURL("10.12.112.55", "6881", [](int status, std::string address){
            printf("ip:%s   status:%d\n", address.c_str(), status);
            
            ESDClient client("aa", "abcdefghij0123456789");
            
            for(int i = 0 ; i < 1000 ; i++){
                client.ping(address, 6881);
//                usleep(1000*500);
            }
            
        });
    }
    
}