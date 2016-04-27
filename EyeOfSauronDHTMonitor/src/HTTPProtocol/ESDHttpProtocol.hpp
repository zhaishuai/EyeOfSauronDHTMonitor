//
//  ESDHttpProtocol.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/25/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDHttpProtocol_hpp
#define ESDHttpProtocol_hpp

#include <stdio.h>
#include <map>

// response-header
//
#define AcceptRanges "Accept-Ranges"
#define Age "Age"
#define ETag "ETag"
#define Location "Location"
#define ProxyAuthenticate "Proxy-Authenticate"
#define RetryAfter "Retry-After"
#define Server "Server"
#define Vary "Vary"
#define WWWAuthenticate "WWW-Authenticate"

namespace esdht {
    
    
    static const std::map<int, std::string> statuCodemap = {
        {100, "CONTINUE"},
        {101, "SWITCHING_PROTOCOLS"},
        {200, "OK"},
        {201, "CREATED"},
        {202, "ACCEPTED"},
        {203, "NON_AUTHORITATIVE"},
        {204, "NO_CONTENT"},
        {400, "BAD_REQUEST"},
        {401, "UNAUTHORIZED"},
        {402, "PAYMENT_REQUIRED"},
        {403, "FORBIDDEN"},
        {404, "NOT_FOUND"},
        {405, "METHOD_NOT_ALLOWED"},
        {406, "NOT_ACCEPTABLE"},
        {500, "INTERNAL_SERVER_ERROR"},
        {501, "NOT_IMPLEMENTED"},
        {502, "BAD_GATEWAY"},
        {503, "BAD_GATEWAY"},
        {504, "GATEWAY_TIMEOUT"},
        {505, "HTTP_VERSION_NOT_SUPPORT"},
    };
    
    
    
    CREATE_ERROR_CLASS(ESDHttpResponseError)
    
    class ESDHttp : public ESDObject{
    public:
        std::string rootPath = "/Users/shuaizhai/Documents/MyProject/EyeOfSauronDHTMonitor/EyeOfSauronDHTMonitor/tests";
        
        int                                status = 0;
        std::string                        responseHttpVersion;
        std::string                        reasonPhrase;
        std::map<std::string, std::string> responseHeader;
        std::string                        responseBody;
        
        std::string                        requestMethod;
        std::string                        requestHttpVersion;
        std::string                        requestURI;
        std::map<std::string, std::string> requestHeader;
        std::string                        requestBody;
        std::string                        requestURL;
        std::map<std::string, std::string> requestParam;
        std::map<std::string, std::string> requestForm;
        
    public:

        void handleResponse(std::string &msg);
        
        void handleRequest(std::string &msg);
        
        std::string readFile(std::string path);
        
        std::string read404File(std::string path);
        
        
        
        std::deque<std::string> &split(const std::string &s, char delim, std::deque<std::string> &elems) {
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                elems.push_back(item);
            }
            return elems;
        }

        std::deque<std::string> split(const std::string &s, char delim) {
            std::deque<std::string> elems;
            split(s, delim, elems);
            return elems;
        }
        
        bool checkKey(std::string key, std::map<std::string, std::string> *map){
            if ( map->find(key) == map->end() ) {
                return false;
            } else {
                return true;
            }
        }

    };
    
}

#endif /* ESDHttpProtocol_hpp */
