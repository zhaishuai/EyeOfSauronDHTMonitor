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

#define IP      "dht.transmissionbt.com"
#define COUNTER 5

namespace test_esd_client {
    using namespace std;
    
    void test_start_server(std::string resposeList[COUNTER]){
        
        
            uv_loop_t loop;
            uv_loop_init(&loop);
            ESDUdp udp(&loop);
            int count = 0;
            udp.receive(IP, 6881, [&udp, &count, &resposeList](std::string msg){
                printf("dsg: %s\n", msg.c_str());
                udp.response(resposeList[count%COUNTER], nullptr);
                count++;
            });
        
    }
    
    
    
    void test_esd_client_ping(){
        threadPool::Thread thread;
        thread.startThread([]{
            std::string resposeList[COUNTER] = {"d1:rd2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:re", "dfdfdffdfdfdfasdgfb", "l4:spami42ee", "d3:bar4:spam3:fooi42ee", "d1:eli201e23:A Generic Error Ocurrede1:t2:aa1:y1:ee"};
            test_start_server(resposeList);
        });
        
        ESDDns dns;
        dns.getIpOfURL(IP, "6881", [](int status, std::string address){
            printf("ip:%s   status:%d\n", address.c_str(), status);
            
            ESDClient client("aa", "abcdefghij0123456789");
//            client.udp->licensingResponse([](std::string response){
//                printf("response: %s\n", response.c_str());
//            });
            
            for(int i = 0 ; i < COUNTER   ; i++){
                client.ping(address, 6881);
                printf("send\n");
                usleep(1000*500);
            }
            uv_run(client.loop, UV_RUN_DEFAULT);
            sleep(10);
        });
    }
    
    void test_esd_client_findNode(){
//        threadPool::Thread thread;
//        thread.startThread([]{
//            std::string resposeList[COUNTER] = {"d1:rd2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:re", "dfdfdffdfdfdfasdgfb", "l4:spami42ee", "d3:bar4:spam3:fooi42ee", "d1:eli201e23:A Generic Error Ocurrede1:t2:aa1:y1:ee"};
//            test_start_server(resposeList);
//        });
        
        ESDDns dns;
        dns.getIpOfURL(IP, "6881", [](int status, std::string address){
            printf("ip:%s   status:%d\n", address.c_str(), status);
            
            ESDClient client("abcdefghij0123456789","aa");
            for(int i = 0 ; i < COUNTER   ; i++){
//                client.ping(address, 6881);
                client.findNode(address, 6881);
                printf("send\n");
                usleep(1000*500);
            }
            uv_run(client.loop, UV_RUN_DEFAULT);
            sleep(10);
        });
    }
    
    void test_esd_client(){
//        test_esd_client_ping();
        test_esd_client_findNode();
    }
    
}