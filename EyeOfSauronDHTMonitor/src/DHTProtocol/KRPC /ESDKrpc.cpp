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
    
    ESDKrpcError::ESDKrpcError(const std::string &what):std::runtime_error(what){};
    
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
    
    std::string ESDKrpc::findNode(const std::string transactionID, const std::string id, const std::string target){
        std::unique_ptr<Encoder> encoder = Encoder::create();
        std::shared_ptr<BDictionary> bDictionary(BDictionary::create());
        std::shared_ptr<BDictionary> subDictionary(BDictionary::create());
        (*bDictionary)[BString::create("a")] = subDictionary;
        (*subDictionary)[BString::create("id")] = BString::create(id);
        (*subDictionary)[BString::create("target")] = BString::create(target);
        
        (*bDictionary)[BString::create("t")] = BString::create(transactionID);
        (*bDictionary)[BString::create("y")] = BString::create("q");
        (*bDictionary)[BString::create("q")] = BString::create("find_node");
        
        
        std::string dic = encoder->encode(bDictionary);
        return dic;
    }
    
    void ESDKrpc::handleFindNodeResponse(const std::string response, std::string &id ,std::string &nodes){
        
        std::unique_ptr<Decoder> decoder;
        std::shared_ptr<BItem> bItem(decoder->decode(response));
        auto dict = bItem->as<BDictionary>();
        
        //在此处判断bencode是不是错误状态。
        //
        if(bencodeIsErrorPackets(dict)) return;
        if(!checkKeyExist(dict, "r"))   return;
        auto sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
        if(!checkKeyExist(sDictionary, "nodes"))  return;
        nodes = (*sDictionary)[BString::create("nodes")]->as<BString>()->value();
        if(!checkKeyExist(sDictionary, "id")) return;
        id = (*sDictionary)[BString::create("id")]->as<BString>()->value();
    }
    
    const std::string ESDKrpc::generateNodeID(int length){
        
        std::stringstream stream;
        SHA1 sha1;
        for(int i = 0 ; i < length ; i++){
            unsigned char ch = rand()%256;
            stream<<ch;
        }
        sha1.addBytes(stream.str().c_str(), length);

        return std::string{reinterpret_cast<char*>(sha1.getDigest()), (unsigned long)length};
    }
    
    
    bool ESDKrpc::checkKeyExist(std::shared_ptr<BDictionary> dict, std::string key){
        if(dict == nullptr){
            throw ESDKrpcError("Bencode error\n");
            return false;
        }
        if(dict->find(BString::create(key)) == dict->end()){
            throw ESDKrpcError("Bencode error\n");
            return false;
        }else{
            return true;
        }
    }
    
    bool ESDKrpc::bencodeIsErrorPackets(std::shared_ptr<BDictionary> dict){
        if(dict == nullptr){
            throw ESDKrpcError("Bencode error\n");
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
                throw ESDKrpcError("Bencode error\n");
                return true;
            }
            auto errorCode = errorList->front()->as<BInteger>();
            errorList->pop_front();
            if(errorList->empty()){
                std::stringstream stream;
                stream<<"Bencode error! errorCode:"<<errorCode->value()<<std::endl;
                throw ESDKrpcError(stream.str());
                return true;
            }
            auto errorInfo = errorList->front()->as<BString>();
            std::stringstream stream;
            stream<<"Bencode error! errorCode:"<<errorCode->value()<<"  "<<"errorInfo:"<<errorInfo->value().c_str()<<std::endl;
            throw ESDKrpcError(stream.str());
            return true;
        }
        return false;
    }
    
}