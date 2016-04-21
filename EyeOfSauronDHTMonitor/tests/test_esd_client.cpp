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
//"router.utorrent.com"
#define IP "router.utorrent.com"
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
        std::deque<PeerInfo> queue;
        
        ESDDns dns;
        dns.getIpOfURL(IP, "6881", [&queue](int status, std::string address){
            printf("ip:%s   status:%d\n", address.c_str(), status);
            PeerInfo peerInfo{.ip = address,.port = 6881};
            queue.push_back(peerInfo);
            
            ESDClient client("abcdefghij0123456789","aa");
            
            client.udp->licensingResponse([&client, &queue](std::string pong){
                
                try {
                    std::string id = "";
                    std::string nodes = "";
                    client.krpc->handleFindNodeResponse(pong, id, nodes);//泄露
                    PeerInfo info;
                    queue.pop_front();
                    for(int i = 0 ; i < nodes.size()/26 && queue.size() < 1000 ; i++){
                        
                        info = client.krpc->getPeerInfoFromNodeStr(nodes.substr(i*26, 26));
                        printf("info:%s   port:%d    ", info.ip.c_str(), info.port);
                        queue.push_back(info);
                        printf("findNode!    queueSize:%ld\n", (long)queue.size());
                    }
                    
                } catch(ESDKrpcError error){
                    fprintf(stderr ,"%s", error.what());
                } catch(DecodingError error){
                    fprintf(stderr ,"%s", error.what());
                }
                
            });
            
            for(int i = 0 ; true ; i++){
                if(i >= queue.size()){
                    i = 0;
                }
                client.findNode(queue[i].ip, queue[i].port);
                printf("send  i:%d   ip:%s    port:%d\n", i, queue[i].ip.c_str(), queue[i].port);
//                usleep(1000*100);
                
                uv_run(client.loop, UV_RUN_NOWAIT);
            }
//            uv_run(client.loop, UV_RUN_DEFAULT);
        });
    }
    
    void test_esd_client(){
//        test_esd_client_ping();
        test_esd_client_findNode();
    }
    
}