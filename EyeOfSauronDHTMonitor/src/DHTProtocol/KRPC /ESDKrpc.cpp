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
            bencodeIsErrorPackets(dict);
            checkKeyExist(dict, "r");
            auto sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
            checkKeyExist(sDictionary, "id");
            auto id = (*sDictionary)[BString::create("id")]->as<BString>();
            responseID = id->value();
            
        } catch (DecodingError error) {
            throw ESDKrpcError(error.what());
        }
    }
    
    std::string ESDKrpc::findNode(const std::string transactionID, const std::string id, const std::string target){
        try {
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
        } catch (DecodingError error) {
            throw ESDKrpcError(error.what());
        }
    }
    
    
    void ESDKrpc::handleFindNodeResponse(const std::string response, std::string &id ,std::string &nodes){
        
        std::unique_ptr<Decoder> decoder;
        std::string myPong = response;
        std::shared_ptr<BItem> bItem(decoder->decode(myPong));
        auto dict = bItem->as<BDictionary>();
        
        //在此处判断bencode是不是错误状态。
        //
        bencodeIsErrorPackets(dict);
        checkKeyExist(dict, "r");
        auto sDictionary = (*dict)[BString::create("r")]->as<BDictionary>();
        checkKeyExist(sDictionary, "nodes");
        nodes = (*sDictionary)[BString::create("nodes")]->as<BString>()->value();
        checkKeyExist(sDictionary, "id");
        id = (*sDictionary)[BString::create("id")]->as<BString>()->value();
    }
    
    PeerInfo ESDKrpc::getPeerInfoFromNodeStr(std::string nodeStr){
        PeerInfo info;
        if(nodeStr.size()!=26)
            throw ESDKrpcError("nodeStr size is not 26!");
        info.nodeID = nodeStr.substr(0, 20);
        std::stringstream stream;
        stream<<(int)(unsigned char)nodeStr.at(20)<<"."<<(int)(unsigned char)nodeStr.at(21)<<"."<<(int)(unsigned char)nodeStr.at(22)<<"."<<(int)(unsigned char)nodeStr.at(23);
        int port = 0;
        port |= (int)(unsigned char)nodeStr.at(24)<<8;
        port |= (int)(unsigned char)nodeStr.at(25);
        info.ip = stream.str();
        info.port = port;
        
        return info;
    }
    
    std::string ESDKrpc::sha1ToHex (const char *sha1){
        
        char out1[48];
        char *out = out1;
        static const char hex[] = "0123456789abcdef";
        for (int i=0; i<20; ++i) {
            const unsigned int val = (unsigned char)(*sha1++);
            *out++ = hex[val >> 4];
            *out++ = hex[val & 0xf];
        }
        *out = '\0';
        return std::string(out1);
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
    
    
    void ESDKrpc::checkKeyExist(std::shared_ptr<BDictionary> dict, std::string key){
        if(dict == nullptr){
            throw ESDKrpcError("Bencode error\n");
        }
        if(dict->find(BString::create(key)) == dict->end()){
            throw ESDKrpcError("Bencode error\n");
        }
    }
    
    void ESDKrpc::bencodeIsErrorPackets(std::shared_ptr<BDictionary> dict){
        if(dict == nullptr){
            throw ESDKrpcError("Bencode error\n");
        }
        checkKeyExist(dict, "y");

        auto y    = (*dict)[BString::create("y")]->as<BString>();
        //在此处判断bencode是不是错误状态。
        //
        if(y->value() == "e"){
            checkKeyExist(dict, "e");
            auto errorList = (*dict)[BString::create("e")]->as<BList>();
            if(errorList->empty()){
                throw ESDKrpcError("Bencode error\n");
            }
            auto errorCode = errorList->front()->as<BInteger>();
            errorList->pop_front();
            if(errorList->empty()){
                std::stringstream stream;
                stream<<"Bencode error! errorCode:"<<errorCode->value()<<std::endl;
                throw ESDKrpcError(stream.str());
            }
            auto errorInfo = errorList->front()->as<BString>();
            std::stringstream stream;
            stream<<"Bencode error! errorCode:"<<errorCode->value()<<"  "<<"errorInfo:"<<errorInfo->value().c_str()<<std::endl;
            throw ESDKrpcError(stream.str());
        }
    }
    
}