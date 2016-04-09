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
    class ESDKrpc : public ESDObject{
    
    protected:
        bool checkKeyExist(std::shared_ptr<BDictionary> dict, std::string key){
            if(dict == nullptr){
                fprintf(stderr, "Bencode error\n");
                return false;
            }
            if(dict->find(BString::create(key)) == dict->end()){
                fprintf(stderr, "Bencode error\n");
                return false;
            }else{
                return true;
            }
        }
        
        bool bencodeIsErrorPackets(std::shared_ptr<BDictionary> dict){
            if(dict == nullptr){
                fprintf(stderr, "Bencode error\n");
                return true;
            }
            if(!checkKeyExist(dict, "y")){
                return true;
            }
            auto y    = (*dict)[BString::create("y")]->as<BString>();
            //在此处判断bencode是不是错误状态。
            //
            if(y->value() == "e"){
                if(!checkKeyExist(dict, "e")){
                    return true;
                }
                auto errorList = (*dict)[BString::create("e")]->as<BList>();
                if(errorList->empty()){
                    fprintf(stderr, "Bencode error\n");
                    return true;
                }
                auto errorCode = errorList->front()->as<BInteger>();
                errorList->pop_front();
                if(errorList->empty()){
                    fprintf(stderr, "Bencode error errorCode:%lld\n", errorCode->value());
                    return true;
                }
                auto errorInfo = errorList->front()->as<BString>();
                fprintf(stderr, "Bencode error! errorCode:%lld errorInfo:%s\n", errorCode->value(), errorInfo->value().c_str());
                return true;
            }
            return false;
        }

        
    public:
        std::string ping(const std::string transactionID, const std::string id);
        void handlePingResponse(const std::string response, std::string &id);
        
        std::string findNode(std::shared_ptr<BDictionary> dict, std::string key, std::string target);
        void handleFindNodeResponse(const std::string response, std::string &id , std::shared_ptr<BList> &nodes);
        
    };
    
}

#endif /* ESDKrpc_hpp */
