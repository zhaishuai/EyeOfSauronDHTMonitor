//
//  ESDKrpc.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/8/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDKrpc_hpp
#define ESDKrpc_hpp

#include <stdio.h>
#include "bencoding.h"
#include "sha1.h"

#define NODE_ID_LENGTH 20

namespace esdht {
    using namespace bencoding;
    
    struct PeerInfo{
        std::string ip;
        std::string nodeID;
        int         port;
    };
    
    class ESDKrpcError: public std::runtime_error {
    public:
        explicit ESDKrpcError(const std:: string &what);
    };
    
    class ESDKrpc : public ESDObject{
    
    protected:
        

        
    public:
        std::string ping(const std::string transactionID, const std::string id);
        void handlePingResponse(const std::string response, std::string &id);
        
        std::string findNode(const std::string transactionID, const std::string id, const std::string target);
        void handleFindNodeResponse(const std::string response, std::string &id , std::string &nodes);
        PeerInfo getPeerInfoFromNodeStr(std::string nodeStr);
        
        void handleGetPeers(const std::string response, std::string &token);
        
        const std::string generateNodeID(int length);
        
        std::string sha1ToHex (const char *sha1);
        
        void checkKeyExist(std::shared_ptr<BDictionary> dict, std::string key);
        void bencodeIsErrorPackets(std::shared_ptr<BDictionary> dict);
    };
    
}

#endif /* ESDKrpc_hpp */
