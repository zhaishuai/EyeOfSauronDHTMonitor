//
//  ESDHttp.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/28/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDHttp_hpp
#define ESDHttp_hpp

#include <stdio.h>

namespace esdht {
    
    class ESDHttp2{

    public:
        
//        std::string rootPath = "/Users/shuaizhai/Desktop/EyeOfSauronDHTMonitor/EyeOfSauronDHTMonitor/tests";
//        
//        int                                status = 0;
//        std::string                        responseHttpVersion;
//        std::string                        reasonPhrase;
//        std::map<std::string, std::string> responseHeader;
//        std::string                        responseBody;
//        
//        std::string                        requestMethod;
//        std::string                        requestHttpVersion;
//        std::string                        requestURI;
//        std::map<std::string, std::string> requestHeader;
//        std::string                        requestBody;
//        std::string                        requestURL;
//        std::map<std::string, std::string> requestParam;
//        std::map<std::string, std::string> requestForm;

        int a;
        
        void handleResponse(std::string &msg);
        
        void handleRequest(std::string &msg);
        
        std::string readFile(std::string path);
        
        std::string read404File(std::string path);
        
        
        
//        std::deque<std::string> &split(const std::string &s, char delim, std::deque<std::string> &elems) {
//            std::stringstream ss(s);
//            std::string item;
//            while (std::getline(ss, item, delim)) {
//                elems.push_back(item);
//            }
//            return elems;
//        }
//        
//        std::deque<std::string> split(const std::string &s, char delim) {
//            std::deque<std::string> elems;
//            split(s, delim, elems);
//            return elems;
//        }
//        
//        bool checkKey(std::string key, std::map<std::string, std::string> *map){
//            if ( map->find(key) == map->end() ) {
//                return false;
//            } else {
//                return true;
//            }
//        }
        
    };

    
}

#endif /* ESDHttp_hpp */
