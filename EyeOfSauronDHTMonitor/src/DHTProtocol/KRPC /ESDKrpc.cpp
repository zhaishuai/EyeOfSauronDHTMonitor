//
//  ESDKrpc.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/8/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDKrpc.hpp"
#include "bencoding.h"
using namespace bencoding;

namespace esdht {
    
    std::string ESDKrpc::ping(std::string transactionID, std::string id){
        std::unique_ptr<Encoder> encoder = Encoder::create();
        std::shared_ptr<BDictionary> bDictionary(BDictionary::create());
        (*bDictionary)[BString::create("t")] = BString::create(transactionID);
        (*bDictionary)[BString::create("y")] = BString::create("q");
        (*bDictionary)[BString::create("q")] = BString::create("ping");
        std::shared_ptr<BDictionary> subDictionary(BDictionary::create());
        (*subDictionary)[BString::create("id")] = BString::create(id);
        (*bDictionary)[BString::create("a")] = subDictionary;
        std::string dic = encoder->encode(bDictionary);
        return dic;
    }
}