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
    
    std::string ESDKrpc::ping(const std::string transactionID, const std::string id){
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
    
    void ESDKrpc::handlePingResponse(const std::string response, std::string &responseID){
        try {
            std::unique_ptr<Decoder> decoder;
            std::shared_ptr<BItem> bItem(decoder->decode(response));
            auto dict = bItem->as<BDictionary>();
            
            //在此处判断bencode是不是错误状态。
            //
            if(bencodeIsErrorPackets(dict)) return;
            if(!checkKeyExist(dict, "r"))   return;
            auto sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
            if(!checkKeyExist(sDictionary, "id"))  return;
            auto id = (*sDictionary)[BString::create("id")]->as<BString>();
            responseID = id->value();
            
        } catch (DecodingError error) {
            fprintf(stderr ,"%s\n", error.what());
        }
    }
    
    std::string ESDKrpc::findNode(std::shared_ptr<BDictionary> dict, std::string key, std::string target){
        std::unique_ptr<Encoder> encoder = Encoder::create();
        std::shared_ptr<BDictionary> bDictionary(BDictionary::create());
        (*bDictionary)[BString::create("t")] = BString::create("aa");
        (*bDictionary)[BString::create("y")] = BString::create("q");
        (*bDictionary)[BString::create("q")] = BString::create("find_node");
        std::shared_ptr<BDictionary> subDictionary(BDictionary::create());
        (*subDictionary)[BString::create("id")] = BString::create("abcdefghij0123456789");
        (*subDictionary)[BString::create("target")] = BString::create("mnorrrstuvwxyz123476");
        (*bDictionary)[BString::create("a")] = subDictionary;
        std::string dic = encoder->encode(bDictionary);
        return dic;
    }
    
    void ESDKrpc::handleFindNodeResponse(const std::string response, std::string &id ,std::shared_ptr<BList> &nodes){
        
        std::unique_ptr<Decoder> decoder;
        std::shared_ptr<BItem> bItem(decoder->decode(response));
        auto dict = bItem->as<BDictionary>();
        
        //在此处判断bencode是不是错误状态。
        //
        if(bencodeIsErrorPackets(dict)) return;
        if(!checkKeyExist(dict, "r"))   return;
        auto sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
        if(!checkKeyExist(sDictionary, "nodes"))  return;
        nodes = (*sDictionary)[BString::create("nodes")]->as<BList>();
    }
    
}