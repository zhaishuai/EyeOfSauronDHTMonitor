//
//  test_esdtcp.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/19/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esdtcp.hpp"
#include "ESDTCP.hpp"
#include "ThreadPool.hpp"
#include <uv.h>
#include <stdio.h>
#include <stdlib.h>
#include "ESDHttpProtocol.hpp"
#include "ESDHttpUtility.hpp"

namespace test_esdtcp {
    using namespace esdht;

    void uv_close_cb(uv_handle_t* handle){
        free(handle);
    }
    
    void test_esdtcp(){
        
        threadPool::Thread thread;
        thread.startThread([]{
            esdht::ESDTcp tcp;
            tcp.receive("192.168.1.105", 8809, [&tcp](std::string msg, uv_stream_t *stream){

                ESDHttp *http = new ESDHttp();

//                printf("I am Server:%s\n", msg.c_str());
                http->handleRequest(msg);
                
                std::string data = http->readFile(http->rootPath + "/m/index.html");
                delete http;
                tcp.response(data, stream, nullptr);
                
            });
        });
        
//        try {
//            ESDDns dns;
//            dns.getIpOfURL("www.w3.org", 80, [](std::string address, unsigned int port){
//                esdht::ESDTcp sender;
//                sender.connect(address, port, [&sender](uv_stream_t *stream){
//                    std::string httpContent;
//                    sender.send(stream, "GET /html/ HTTP/1.1\r\nHost: www.w3.org\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n", [&sender, &stream, &httpContent](std::string msg){
//                        
//                        httpContent += msg;
//                        
//                    });
//                    sender.runDefult();
//                    sender.stopReceiveResponse(stream);
//                    ESDHttp response;
//                    response.handleResponse(httpContent);
//                    fprintf(stderr, "%s", response.responseBody.c_str());
//                });
//            });
//            
//        } catch (ESDTcpError error) {
//            printf("%s\n", error.what());
//        } catch (ESDDnsError error){
//            printf("%s\n", error.what());
//        }
        
    }
    
    
    
}