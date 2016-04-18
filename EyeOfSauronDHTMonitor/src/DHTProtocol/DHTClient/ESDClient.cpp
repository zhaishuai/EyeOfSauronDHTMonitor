//
//  ESDClient.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/6/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDClient.hpp"
#include "bencoding.h"
using namespace bencoding;

namespace esdht {
    
    ESDClient::ESDClient(std::string nodeID, std::string transactionID){
        this->nodeID        = nodeID;
        this->transactionID = transactionID;
        this->krpc          = std::unique_ptr<ESDKrpc>(new ESDKrpc);
        loop                = new uv_loop_t;
        uv_loop_init(loop);
        this->udp           = std::unique_ptr<ESDUdp>(new ESDUdp(loop));
    }
    
    ESDClient::~ESDClient(){
        delete loop;
    }
    
    void ESDClient::ping(std::string ip, int port){

        try {
            udp->send(ip, port, krpc->ping(transactionID, nodeID), nullptr, [this](std::string pong){
                try {
                    std::string id;
                    krpc->handlePingResponse(pong, id);
                    fprintf(stderr ,"ping response id:%s\n", id.c_str());
                } catch(ESDKrpcError error){
                    fprintf(stderr ,"%s", error.what());
                } catch(DecodingError error){
                    fprintf(stderr ,"%s", error.what());
                }
            });
        } catch (ESDUdpError error) {
            fprintf(stderr ,"%s", error.what());
        }
    }
    
    void ESDClient::findNode(std::string ip, int port){
        try {
//            udp->send(ip, port, krpc->findNode(transactionID, nodeID, krpc->generateNodeID(NODE_ID_LENGTH)), nullptr, [this](std::string pong){
//                try {
//                        std::string id = "";
//                        std::string nodes = "";
//                        krpc->handleFindNodeResponse(pong, id, nodes);
//                        PeerInfo info;
//                        if(nodes.size()>=26){
//                            info = krpc->getPeerInfoFromNodeStr(nodes.substr(0, 26));
//                            printf("info:%s   port:%d\n", info.ip.c_str(), info.port);
//                        }
//
//                } catch(ESDKrpcError error){
//                    fprintf(stderr ,"%s", error.what());
//                } catch(DecodingError error){
//                    fprintf(stderr ,"%s", error.what());
//                }
//            });
            
            
            udp->sendAsync(ip, port, krpc->findNode(transactionID, nodeID, krpc->generateNodeID(NODE_ID_LENGTH)), nullptr);
            
            
        } catch (ESDUdpError error) {
            fprintf(stderr ,"%s", error.what());
        }
    }
}