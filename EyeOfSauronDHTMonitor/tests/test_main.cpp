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

using namespace esdht;

int main(int argc, const char * argv[]) {

//    test_dns::run_test_dns();
//    printf("finish dns test\n");
//    test_udp::run_test_udp();
//    printf("finish udp test\n");
    test_dns::run_bencode_test();
    test_dns::run_encode_test();
    
    return 0;
}