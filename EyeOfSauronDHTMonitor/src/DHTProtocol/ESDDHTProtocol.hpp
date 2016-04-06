//
//  ESDDHTProtocol.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/4/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDDHTProtocol_hpp
#define ESDDHTProtocol_hpp

#include <stdio.h>
#include "bencoding.h"

namespace esdht {
    using namespace bencoding;
    class ESDDHTProtocol : public ESDObject{
    public:
        virtual void ping(std::string ip, int port) = 0;
        
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
    };
    
}

#endif /* ESDDHTProtocol_hpp */
