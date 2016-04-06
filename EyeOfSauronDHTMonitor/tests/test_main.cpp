//
//  test_main.c
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/31/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <uv.h>

#include "testUdp.hpp"
#include "test_dns.hpp"
#include "test_bencode.hpp"
#include "test_esdudp.hpp"
#include "test_esd_server.hpp"
#include "test_esd_client.hpp"

using namespace esdht;

int main(int argc, const char * argv[]) {

//    test_dns::run_test_dns();
//    printf("finish dns test\n");
//    test_udp::run_test_udp();
//    printf("finish udp test\n");
//    test_dns::run_bencode_test();
//    test_dns::run_encode_test();
//    test_esdudp::run_test_esdudp();
    
//    threadPool::Thread thread;
//    thread.startThread([]{
//        test_esd_server::test_server_start_up();
//    });
    
    test_esd_client::test_esd_client();
    
    return 0;
}