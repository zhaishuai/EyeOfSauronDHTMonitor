//
//  ESDHttpProtocol.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/25/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include <fstream>

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
        int index        = STARTLINE;
        int postState    = 0;
        std::string name = "";
        for(std::string line; std::getline(stream, line) ; ){
            if(index == STARTLINE){
                std::deque<std::string> substrings = split(line, ' ');
                if(substrings.size() == 3){
                    requestMethod      = substrings[0];
                    requestURI         = substrings[1];
                    requestHttpVersion = substrings[2];
                    std::deque<std::string> uriList = split(requestURI, '?');
                    if(uriList.size() >= 1)
                        requestURL         = uriList[0];
                    printf("");
                    if(uriList.size() >= 2){
                        std::deque<std::string> paramItems = split(uriList[1], '&');
                        for(int i = 0; i < paramItems.size(); i++){
                            std::deque<std::string> paramItem = split(paramItems[i], '=');
                            if(paramItem.size() == 2){
                                requestParam.insert(std::pair<std::string, std::string>(paramItem[0], paramItem[1]));
                            }
                        }
                    }
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
                
                if(checkKey("Content-Type", &requestHeader)){
                    std::string requsetHeader = requestHeader["Content-Type"];
                    if(requestHeader["Content-Type"] == "application/x-www-form-urlencoded\r"){
                        std::deque<std::string> paramItems = split(line, '&');
                        for(int i = 0; i < paramItems.size(); i++){
                            std::deque<std::string> paramItem = split(paramItems[i], '=');
                            if(paramItem.size() == 2){
                                requestForm.insert(std::pair<std::string, std::string>(paramItem[0], paramItem[1]));
                            }
                        }

                    }else{

                        std::deque<std::string> paramItem = split(requestHeader["Content-Type"], ';');
                        if(paramItem.size() == 2){
                            if(paramItem[0] == "multipart/form-data"){
                                paramItem[1] = paramItem[1].substr(1, paramItem[1].size()-1);
                                switch (postState) {
                                    case 0:{
                                        std::deque<std::string> nameItem = split(paramItem[1], '=');
                                        if(nameItem.size() == 2){
                                            if(line == "--"+nameItem[1]){
                                                
                                                postState = 1;
                                            }
                                        }
                                        
                                        
                                    }
                                        break;
                                    case 1:{
                                        std::deque<std::string> nameItem = split(line, '=');
                                        if(nameItem.size() == 2){
                                            name = nameItem[1].substr(1, nameItem[1].size()-3);
                                            postState = 2;
                                        }
                                    }
                                        break;
                                    case 2:{
                                        if(line == "\r"){
                                            postState = 3;
                                        }
                                    }
                                        break;
                                    case 3:{
                                        if(name != ""){
                                            requestForm.insert(std::pair<std::string, std::string>(name, line.substr(0, line.size()-1)));
                                            postState = 0;
                                            name = "";
                                        }
                                    }
                                        break;
                                        
                                    default:
                                        break;
                                }
                            }
                        }
                        
                        
                        
                    }
                }
                
                requestBody += line + "\n";
            }
            
            
        }
    }
    
    std::deque<std::string>&ESDHttp::split(const std::string &s, char delim, std::deque<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
    
    std::deque<std::string> ESDHttp::split(const std::string &s, char delim) {
        std::deque<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
    
    bool ESDHttp::checkKey(std::string key, std::map<std::string, std::string> *map){
        if ( map->find(key) == map->end() ) {
            return false;
        } else {
            return true;
        }
    }
    
    std::string ESDHttp::readFile(std::string path){
        string data = "";
        string line;
        ifstream myfile (path);
        if (myfile.is_open()){
            while ( getline (myfile,line) ){
                data += line + "\n";
            }
            myfile.close();
        }else{
            return read404File(rootPath + "/server/404.html");
        }
        
        return data;
    }
    
    std::string ESDHttp::read404File(std::string path){
        string data = "";
        string line;
        ifstream myfile (path);
        if (myfile.is_open()){
            while ( getline (myfile,line) ){
                data += line + "\n";
            }
            myfile.close();
        }else{
            return "404";//先这样
        }
        
        return data;
    }
    
}