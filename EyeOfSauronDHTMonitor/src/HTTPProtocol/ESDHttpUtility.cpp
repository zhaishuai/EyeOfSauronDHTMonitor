//
//  ESDHttpUtility.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/25/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDHttpUtility.hpp"

namespace esdht {

    regex_t ESDHttpUtility::compilerRegex(std::string pattern){
        
        regex_t regexCompiled;
        if (regcomp(&regexCompiled, pattern.c_str(), REG_EXTENDED)){
            throw ESDHttpUtilityError("Could not compile regular expression.\n");
        }
        return regexCompiled;
    }
    
    void ESDHttpUtility::clearRegex(regex_t &regex){
        regfree(&regex);
    }
    
}