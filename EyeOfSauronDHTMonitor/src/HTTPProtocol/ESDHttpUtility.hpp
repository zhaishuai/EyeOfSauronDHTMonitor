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
    
    static const string LOW_ALPHA    = "[a-z]";
    static const string HI_ALPHA     = "[A-Z]";
    static const string ALPHA        = "(" + string(LOW_ALPHA) + "|" + string(HI_ALPHA) + ")";
    static const string DIGIT        = "[0-9]";
    static const string SAFE         = "(\\$|-|_|\\.|\\+)";
    static const string EXTRA        = "(!|\\*|'|\\(|\\)|,)";
    static const string HEX          = "(" + string(DIGIT) + "|A|B|C|D|E|F|a|b|c|d|e|f)";
    static const string ESCAPE       = "(%" + string(HEX) + "{2})";
    static const string UNREASERVED  = "(" + string(ALPHA) + "|" + string(DIGIT) + "|" + SAFE + "|" + EXTRA + ")";
    static const string UCHAR        = string("(") + UNREASERVED + "|" + ESCAPE + ")";
    static const string RESERVED     = "(;|/|\\?|:|@|&|=)";
    static const string XCHAR        = "(" + string(UCHAR) + "|" + string(RESERVED) + ")";
    static const string DIGITS       = "(" + string(DIGIT) + "+)";
    static const string ALPTHADIGIT  = "(" + string(ALPHA) + "|" + string(DIGIT) + ")";
    static const string DOMAINLABEL  = "(" + string(ALPTHADIGIT) +"|" + string(ALPTHADIGIT) + "(" + string(ALPTHADIGIT) + "|-)*" + string(ALPTHADIGIT) + ")";
    static const string TOPLABEL     = "(" + string(ALPHA) + "|" + string(ALPHA) + "(" + string(ALPTHADIGIT) + "|-)*" + string(ALPTHADIGIT) + ")";
    static const string HOSTNAME     = "((" + string(DOMAINLABEL) + "\\.)*" + string(TOPLABEL) + ")";
    static const string HOSTNUMBER   = string(DIGITS) + "." + string(DIGITS) + "." + string(DIGITS) + "." + string(DIGITS);
    static const string HOST         = "(" + string(HOSTNAME) + "|" + string(HOSTNAME) + ")";
    static const string PORT         = "(:" + string(DIGITS) + "))";
    static const string HOSTPORT     = "(" + string(HOST) + "(" + string(PORT) + "){0,1})";
    static const string HSEGMENT     = "((" + string(UCHAR) + "|;|:|@|&|=)*)";
    static const string SEARCH       = "((" + string(UCHAR) + "|;|:|@|&|=)*)";
    static const string HPATH        = string(HSEGMENT) + "(/" + string(HSEGMENT) + ")*";
    static const string HTTPURL      = "(http|https)://" + string(HOSTPORT) + "(/" + string(HPATH) + "(\\?" + string(SEARCH) + "){0,1}){0,1}";
    
//    static const string HTTP_URL     = "^(https?://)?([\\da-z.-]+).([a-z.]{2,6})([/\\w .-]*)*/?$";
    
    static const string SP           = " ";
    static const string LWS          = " | ";
    static const string CRLF         = "\r\n";
    
    
    
    class ESDHttpUtility{
    public:
        
        
        
    public:
        regex_t compilerRegex(std::string pattern);
        
        void clearRegex(regex_t &regex);
        
        std::deque<regmatch_t> runRegex(regex_t regex);
        
//        void getLine(std::string &content, std::function<void (std::string linemsg, )>);
    };
}

#endif /* ESDHttpUtility_hpp */
