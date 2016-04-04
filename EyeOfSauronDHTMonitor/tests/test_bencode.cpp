//
//  test_bencode.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/4/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_bencode.hpp"
#include "bencoding.h"
using namespace bencoding;

namespace test_dns {
    void run_bencode_test(){
        
        std::unique_ptr<Decoder> decoder;
        std::shared_ptr<BItem> bItem(decoder->decode("i-13e"));
        auto bInteger = bItem->as<BInteger>();
        printf("%lld\n", bInteger->value());
        
    }
    
    void run_encode_test(){
        std::unique_ptr<Encoder> encoder = Encoder::create();
        std::shared_ptr<BDictionary> bDictionary(BDictionary::create());
        (*bDictionary)[BString::create("t")] = BString::create("aa");
        (*bDictionary)[BString::create("y")] = BString::create("qq");
        (*bDictionary)[BString::create("q")] = BString::create("ping");
        std::shared_ptr<BDictionary> subDictionary(BDictionary::create());
        (*subDictionary)[BString::create("id")] = BString::create("abcdefghij0123456789");
        (*bDictionary)[BString::create("a")] = subDictionary;
        std::string dic = encoder->encode(bDictionary);
        printf("%s\n", dic.c_str());

        std::unique_ptr<Decoder> decoder;
        std::shared_ptr<BItem> bItem(decoder->decode(dic.c_str()));
        auto dict = bItem->as<BDictionary>();
        std::shared_ptr<BDictionary> sDictionary = (*dict)[BString::create("a")]->as<BDictionary>();
        std::shared_ptr<BString> id = (*sDictionary)[BString::create("id")]->as<BString>();
        
//        for(auto i = dict->begin(); i != dict->end(); i++){
//            
//        }
        
        printf("%s\n", id->value().c_str());
    }
}

