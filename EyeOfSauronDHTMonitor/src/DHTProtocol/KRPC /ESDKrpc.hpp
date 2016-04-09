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

namespace esdht {
    using namespace bencoding;
    
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
        void handleFindNodeResponse(const std::string response, std::string &id , std::shared_ptr<BList> &nodes);
        
        bool checkKeyExist(std::shared_ptr<BDictionary> dict, std::string key);
        bool bencodeIsErrorPackets(std::shared_ptr<BDictionary> dict);
    };
    
}

#endif /* ESDKrpc_hpp */
