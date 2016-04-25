//
//  ESDHttpUtility.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/25/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDHttpUtility_hpp
#define ESDHttpUtility_hpp

#include <stdio.h>
#include <regex.h>
using namespace std;






CREATE_ERROR_CLASS(ESDHttpUtilityError)

namespace esdht {
    
    class ESDHttpUtility : public ESDObject{
    public:
        string SP           = " ";
        string LWS          = " | ";
        string CRLF         = "\r\n";
        string LOW_ALPHA    = "[a-z]";
        string HI_ALPHA     = "[A-Z]";
        string ALPHA        = "(" + string(LOW_ALPHA) + "|" + string(LOW_ALPHA) + ")";
        string DIGIT        = "[0-9]";
        string SAFE         = "(\\$|-|_|\\.|\\+)";
        string EXTRA        = "(!|\\*|'|\\(|\\)|,)";
        string HEX          = "(" + string(DIGIT) + "|A|B|C|D|E|F|a|b|c|d|e|f)";
        string ESCAPE       = "(%" + string(HEX) + "{2})";
        string UNREASERVED  = "(" + string(ALPHA) + "|" + string(DIGIT) + "|" + SAFE + "|" + EXTRA + ")";
        string UCHAR        = string("(") + UNREASERVED + "|" + ESCAPE + ")";
        string RESERVED     = "(;|/|\\?|:|@|&|=)";
        string XCHAR        = "(" + string(UCHAR) + "|" + string(RESERVED) + ")";
        string DIGITS       = "(" + string(DIGIT) + "+)";
        string ALPTHADIGIT  = "(" + string(ALPHA) + "|" + string(DIGIT) + ")";
        string DOMAINLABEL  = "(" + string(ALPTHADIGIT) +"|" + string(ALPTHADIGIT) + "(" + string(ALPTHADIGIT) + "|-)*" + string(ALPTHADIGIT) + ")";
        string TOPLABEL     = "(" + string(ALPHA) + "|" + string(ALPHA) + "(" + string(ALPTHADIGIT) + "|-)*" + string(ALPTHADIGIT) + ")";
        string HOSTNAME     = "((" + string(DOMAINLABEL) + "\\.)*" + string(TOPLABEL) + ")";
        string HOSTNUMBER   = string(DIGITS) + "\\." + string(DIGITS) + "\\." + string(DIGITS) + "\\." + string(DIGITS);
        string HOST         = "(" + string(HOSTNAME) + "|" + string(HOSTNAME) + ")";
        string PORT         = "(" + string(DIGIT) + "+)";
        string HOSTPORT     = "(" + string(HOST) + "(:" + string(PORT) + "){0,1})";
        string HSEGMENT     = "((" + string(UCHAR) + "|;|:|@|&|=)*)";
        string SEARCH       = "((" + string(UCHAR) + "|;|:|@|&|=)*)";
        string HPATH        = string(HSEGMENT) + "(/" + string(HSEGMENT) + ")*";
        string HTTPURL      = "http://" + string(HOSTPORT) + "(/" + string(HPATH) + "(\\?" + string(SEARCH) + "){0,1}){0,1}";
        
        
    public:
        regex_t compilerRegex(std::string pattern);
        
        void clearRegex(regex_t &regex);
        
        std::deque<regmatch_t> runRegex(regex_t regex);
        
    };
}

#endif /* ESDHttpUtility_hpp */
