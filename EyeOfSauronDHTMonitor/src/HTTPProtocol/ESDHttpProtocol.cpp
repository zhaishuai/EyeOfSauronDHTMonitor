//
//  ESDHttpProtocol.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/25/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDHttpProtocol.hpp"
#include "ESDHttpUtility.hpp"

namespace esdht {
    
    enum State{STARTLINE, HTTPHEADER, HTTPBODY};
    
    void ESDHttp::handleResponse(std::string &msg){
        std::istringstream stream(msg);
        int index = STARTLINE;
        for(std::string line; std::getline(stream, line) ; ){
            if(index == STARTLINE){
                std::deque<std::string> substrings = split(line, ' ');
                if(substrings.size() == 3){
                    responseHttpVersion  = substrings[0];
                    status       = std::stoi(substrings[1]);
                    reasonPhrase = substrings[2];
                }
                index = HTTPHEADER;
                
            }else if(line != "\r" && index == HTTPHEADER){
                
                auto pos = line.find_first_of(": ");
                if(pos >= line.length())
                    continue;
                string key   = line.substr(0, pos);
                string value = line.substr(pos+2, line.length() - pos);
                responseHeader.insert(std::pair<std::string, std::string>(key, value));
                
            }else if(line == "\r" && index == HTTPHEADER){
                index = HTTPBODY;
            }else if(index == HTTPBODY){
                responseBody += line + "\n";
            }
            
            
        }
        
    }
    
    
    void ESDHttp::handleRequest(std::string &msg){
        std::istringstream stream(msg);
        int index = STARTLINE;
        for(std::string line; std::getline(stream, line) ; ){
            if(index == STARTLINE){
                std::deque<std::string> substrings = split(line, ' ');
                if(substrings.size() == 3){
                    requestMethod  = substrings[0];
                    requestURI       = substrings[1];
                    requestHttpVersion = substrings[2];
                }
                index = HTTPHEADER;
                
            }else if(line != "\r" && index == HTTPHEADER){
                
                auto pos = line.find_first_of(": ");
                if(pos >= line.length())
                    continue;
                string key   = line.substr(0, pos);
                string value = line.substr(pos+2, line.length() - pos);
                requestHeader.insert(std::pair<std::string, std::string>(key, value));
                
            }else if(line == "\r" && index == HTTPHEADER){
                index = HTTPBODY;
            }else if(index == HTTPBODY){
                requestBody += line + "\n";
            }
            
            
        }
    }
    
}