//
//  test_esdudp.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/4/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esdudp.hpp"

#include "ESDDns.hpp"

namespace test_esdudp {
    
    void run_test_esdudp(){
        esdht::ESDDns dns;
        dns.getIpOfURL("router.bitcomet.net", "554", [](int status, std::string address){
            printf("ip:%s   status:%d\n", address.c_str(), status);
            
//            esdht::ESDDht dht("aa","abcdefghij0123456789");
//            dht.ping(address, 6881);

        });
    }
    
}