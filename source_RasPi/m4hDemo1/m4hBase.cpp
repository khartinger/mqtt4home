//_____h6Base.cpp_______________________________khartinger_____
// Definitions and many basic functions for mqtt4home, divided 
//into four classes:
// * Message : Class for one MQTT message with topic, payload
//             and retain-flag.
// * Message2: Class for two related MQTT messages (in, out)
//             with 2x topic, 2x payload and retain-flag (out).
// * Conf    : Class for reading the configuration file and with
//             special auxiliary methods for string processing.
// * M4hBase : Properties and methods for the basic functions
//             of mqtt4home.
// Hardware: (1) Raspberry Pi
// Updates:
// 2021-08-19 First release
// 2021-08-20 findKey(), findValue(): add check section []
// 2021-08-29 split2pairs(): add long valLen=val.length(); if()
// 2022-02-11 add reload conf file by mqtt command
// 2022-02-15 Add class Conf: DHMS2sec(), sec2DHMS(), sec2HMS()
// 2022-02-17 Add class Conf: fits()
// 2023-06-16 move helper methods to class M4hUtils, rename str...
// Released into the public domain.
#include "m4hBase.h"

//_______...init global little helpers objects ;)_______________
M4hBase g_base=M4hBase();
M4hUtils g_utils=M4hUtils();

// *************************************************************
//       Conf: constructor & co
// *************************************************************

//_______default constructor____________________________________
Conf::Conf() {
 initConf();
}

//_______constructor 2__________________________________________
Conf::Conf(std::string filename) {
 initConf();
 fname=filename;
}

//_______init command properties________________________________
void Conf::initConf(){
 fname="";
}

// *************************************************************
//       Conf: Setter and Getter methods
// *************************************************************

//_______set name of config file________________________________
void Conf::setFilename(std::string fname_)
{
 if(fname_.length()>0) fname=fname_;
}

//_______get name of config file________________________________
std::string Conf::getFilename() { return fname; }

// *************************************************************
//       Conf: working methods
// *************************************************************

//_______does config file exist?________________________________
// return: true=file exists, false=error or file not found
bool Conf::isReady()
{
 if(fname.length()<1) return false;
 std::ifstream fcnf(fname.c_str());
 if(!fcnf.good()) return false;
 return true;
}

//_______is given text a section in config file?________________
// return: true=yes, false=no or error
bool Conf::isSection(std::string text)
{
 //------is config file available?------------------------------
 std::string line_;
 if(fname.length()<1) return false;
 std::ifstream fcnf(fname.c_str());
 if(!fcnf.good()) return false;
 //------check text---------------------------------------------
 if(text.length()<1) return false;
 if(text.at(0)!='[' && text.at(text.length()-1)!=']')
  text="["+text+"]";
 //------search for section [text]------------------------------
 while(fcnf.good())
 {
  std::getline(fcnf, line_);
  g_utils.delExtBlank(line_);
  if(line_==text) { fcnf.close(); return true; }
 }
 fcnf.close();
 return false;
}

//_______get all lines of a section from config file____________
// reads only the FIRST section with given name!
// return: true: vector with the lines
bool Conf::getSection(std::string section, std::vector<std::string>& v1)
{
 bool   inSection=false;
 std::string line1;
 //------check section name-------------------------------------
 if(section.length()<1) return false;
 if(section.at(0)!='[' && section.at(section.length()-1)!=']')
  section="["+section+"]";
 //------file ready?--------------------------------------------
 if(fname.length()<1) return false;
 std::ifstream fcnf(fname.c_str());
 if(!fcnf.good()) return false;
 v1.clear();                           // clear answer
 g_utils.delExtBlank(section);         // delete surrounding blanks
 //------read lines until section found or until last line------
 while(fcnf.good())
 {
  std::getline(fcnf, line1);           // get line
  g_utils.delTrailLFCR(line1);         // delete LF CR
  g_utils.delExtBlank(line1);          // delete surrounding blanks
  if(line1.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line1.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(inSection && (line1.at(0)=='['))
    {//-------start of a new section: stopp reading-------------
     fcnf.close();
     if(v1.size()>0) return true;
     return false;
    }
    if(inSection)
    {//-------in the right section: add line--------------------
     g_utils.replaceAll(line1,"\r","");
     g_utils.replaceAll(line1,"\n","");
     if(line1.length()>0)
      v1.push_back(line1);
    }
    if(line1==section) inSection=true;
   } // end comment line
  } // end empty line
 }
 fcnf.close();
 if(v1.size()>0) return true;
 return false;
}

//_______get lines of all sections with same name from conf file
// vector: 1 Section = 1 entry (json string)
// return: number of sections (-1 on error)
int Conf::getSectionsJson(std::string section, std::vector<std::string>& v1)
{
 std::string line_;
 std::string secNameOld="";
 std::string secValue="";
 bool bGetLines=true;
 //------check section name-------------------------------------
 if(section.length()<1) return false;
 if(section.at(0)=='[') 
 {//.....remove []..............................................
  if(section.length()<3) return false;
  if(section.at(section.length()-1)!=']') return false;
  section=section.substr(1,section.length()-2);
 }
 if(section.length()<1) return false;
 std::string sectionLower=section;     // section name should...
 g_utils.str2lower(sectionLower);     // ...be lower case
 //-----------file ready?---------------------------------------
 if(fname.length()<1) return -1;
 std::ifstream fcnf(fname.c_str());
 if(!fcnf.good()) return -1;
 //-----------start to read lines-------------------------------
 v1.clear();
 while(fcnf.good())
 {//----------read lines until new section found or last line---
  std::getline(fcnf, line_);           // get line
  g_utils.delTrailLFCR(line_);         // delete \r \n 
  g_utils.delExtBlank(line_);          // delete blanks
  //std::cout << "Line >" << line_ << "<" << std::endl;
  if(line_.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line_.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(line_.at(0)=='[')
    {//-------section line: save previous section---------------
     if(secNameOld.length()>0)
     {//------save previous section-----------------------------
      secValue+="}";
      v1.push_back(secValue);
      secNameOld="";
      secValue="{";
     }
     //-------start new section: get section name---------------
     int i2=line_.find_first_of(']');
     if(i2>2)
     {//------found a section name------------------------------
      std::string sec1=line_.substr(1,i2-1);
      std::string sec2=sec1;
      g_utils.str2lower(sec2);
      if(sec2!="end" && sec2!="ende" && sec2==sectionLower)
      {//-----section name matches :)---------------------------
       if(sec1.length()>0) 
       {
        bGetLines=true;
        secNameOld=sec1;
        secValue="{";
       }
      }
      else bGetLines=false;
     }
    }
    else
    {//-------in a section: add line----------------------------
     int idp=line_.find_first_of(':');      // search for...
     if(idp!=std::string::npos)             // ...colon :
     {//----- colon : found -> line is a pair <key:value>-------
      std::string key1=line_.substr(0,idp);
      std::string val1=line_.substr(idp+1);
      g_utils.delExtBlank(key1);            // delete blanks
      g_utils.delExtBlank(val1);            // delete blanks
      if(val1.length()>0) 
      {//.....add new pair to value string......................
       if(secValue.length()>1) secValue+=",";
       g_utils.replaceAll(val1,",","|");
       if(key1.at(0)!='\"') secValue+="\""; // add ""
       secValue+=key1;                      // add key1
       if(key1.at(key1.length()-1) !='\"') secValue+="\"";
       secValue+=":";
       if(val1.at(0)!='\"') secValue+="\""; // add ""
       secValue+=val1;                      // add key1
       if(val1.at(val1.length()-1) !='\"') secValue+="\"";
      }
     }
     else
     {//-----no colon : found -> no pair <key:value>------------
      // set key to "-"
      if(secValue.length()>1) secValue+=",";
      secValue+="\"-\":";
      if(line_.at(0)!='\"') secValue+="\""; // add ""
      secValue+=line_;                      // add key1
      if(line_.at(line_.length()-1) !='\"') secValue+="\"";
     }
    }
   } // end comment line
  } // end empty line
 }
 fcnf.close();
 //------add last section---------------------------------------
 if(secValue.length()>3)
 {
  secValue+="}";
  v1.push_back(secValue);
 }
 return v1.size();
}

//_______get lines of all sections with same name from conf file
// vector: 1 Section = 1 entry (map <key,val>)
// return: number of sections (-1 on error)
int Conf::getSections(std::string section, std::vector<std::map<std::string, std::string>>& v1)
{
 std::string line_;
 std::string secNameOld="";
 std::map<std::string, std::string> m1;
 bool bGetLines=true;
 //------check section name-------------------------------------
 if(section.length()<1) return false;
 if(section.at(0)=='[') 
 {//.....remove []..............................................
  if(section.length()<3) return false;
  if(section.at(section.length()-1)!=']') return false;
  section=section.substr(1,section.length()-2);
 }
 if(section.length()<1) return false;
 std::string sectionLower=section;     // section name should...
 g_utils.str2lower(sectionLower);     // ...be lower case
 //-----------file ready?---------------------------------------
 if(fname.length()<1) return -1;
 std::ifstream fcnf(fname.c_str());
 if(!fcnf.good()) return -1;
 //-----------start to read lines-------------------------------
 v1.clear();
 m1.clear();
 while(fcnf.good())
 {//----------read lines until new section found or last line---
  std::getline(fcnf, line_);           // get line
  g_utils.delTrailLFCR(line_);         // delete \r \n 
  g_utils.delExtBlank(line_);          // delete blanks
  //std::cout << "Line >" << line_ << "<" << std::endl;
  if(line_.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line_.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(line_.at(0)=='[')
    {//-------section line: save previous section---------------
     if(secNameOld.length()>0)
     {//------save previous section-----------------------------
      if(m1.size()>1) v1.push_back(m1);
      secNameOld="";
      m1.clear();
     }
     //-------start new section: get section name---------------
     int i2=line_.find_first_of(']');
     if(i2>2)
     {//------found a section name------------------------------
      std::string sec1=line_.substr(1,i2-1);
      std::string sec2=sec1;
      g_utils.str2lower(sec2);
      if(sec2!="end" && sec2!="ende" && sec2==sectionLower)
      {//-----section name matches :)---------------------------
       if(sec1.length()>0)             // section name ok
       {
        bGetLines=true;                // read lines
        secNameOld=sec1;               // remember section name
        m1.clear();
       }
      }
      else bGetLines=false;            // wrong section
     }
    }
    else
    {//-------in a section: add line----------------------------
     int idp=line_.find_first_of(':');      // search for...
     if(idp!=std::string::npos)             // ...colon :
     {//----- colon : found -> line is a pair <key:value>-------
      std::string key1=line_.substr(0,idp);
      std::string val1=line_.substr(idp+1);
      g_utils.delExtBlank(key1);            // delete blanks
      g_utils.delExtBlank(val1);            // delete blanks
      if(val1.length()>0) 
      {//.....add new pair to map...............................
       m1.insert(std::pair<std::string,std::string>(key1,val1));
      }
     }
     else
     {//-----no colon : found -> no pair <key:value>------------
      //m1.insert(std::pair<std::string,std::string>(line_,""));
     }
    }
   } // end comment line
  } // end empty line
 }
 fcnf.close();
 //------add last section---------------------------------------
 if(m1.size()>1) { v1.push_back(m1); }
 return v1.size();
}

//_______is line_ a line in section?____________________________
bool Conf::isLine(std::string section, std::string line_)
{
 bool   inSection=false;
 std::string line1;
 if(fname.length()<1) return false;
 std::ifstream fcnf(fname.c_str());
 //------file ready?--------------------------------------------
 if(!fcnf.good()) return false;
 //------read lines until section found or until last line------
 g_utils.delExtBlank(line_);
 while(fcnf.good())
 {
  std::getline(fcnf, line1);           // get line
  g_utils.delTrailLFCR(line1);
  g_utils.delExtBlank(line1);
  if(line1.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line1.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(inSection&&(line1.at(0)=='['))
    {//-------end of section------------------------------------
     fcnf.close();
     return false;
    }
    if(inSection)
    {//-------in the right section: check line------------------
     if(line1==line_) { fcnf.close(); return true; }
    }
    if(line1==section) inSection=true;
   } // end comment line
  } // end empty line
 }
 fcnf.close();
 return false;
}

//_______is line_ a line in section?____________________________
// method is Case Insensitive
bool Conf::isLineCI(std::string section, std::string line_)
{
 bool   inSection=false;
 std::string line1;
 if(fname.length()<1) return false;
 std::ifstream fcnf(fname.c_str());
 //------file ready?--------------------------------------------
 if(!fcnf.good()) return false;
 //------read lines until section found or until last line------
 g_utils.delExtBlank(line_);
 g_utils.str2lower(line_);
 while(fcnf.good())
 {
  std::getline(fcnf, line1);           // get line
  g_utils.delTrailLFCR(line1);
  g_utils.delExtBlank(line1);
  if(line1.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line1.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(inSection&&(line1.at(0)=='['))
    {//-------end of section------------------------------------
     fcnf.close();
     return false;
    }
    if(inSection)
    {//-------in the right section: check line------------------
     g_utils.str2lower(line1);
     if(line1==line_) { fcnf.close(); return true; }
    }
    if(line1==section) inSection=true;
   } // end comment line
  } // end empty line
 }
 fcnf.close();
 return false;
}

//_______find the key to a given value in section_______________
// return: value or "" (on error or not found)
std::string Conf::findKey(std::string section, std::string value_)
{
 unsigned int iPos1, iPos2;            // help variables
 bool   inSection=false;               // in section or not
 std::string s1;                            // help string
 std::string line1;
 if(fname.length()<1) return "";
 std::ifstream fcnf(fname.c_str());
 //------file ready?--------------------------------------------
 if(!fcnf.good()) return "";
 //------check section [], key length>0-------------------------
 //delExtBlank(section);
 if(section.length()<1) return "";     // no section
 if(section.at(section.length()-1)!=']') {
  if(section.at(0)!='[') section="["+section+"]";
  else return "";
 } else {
  if(section.at(0)!='[') return "";
 }
 g_utils.delExtBlank(value_);
 if(value_.length()<1) return "";      // no value_
 //------read lines until section found or until last line------
 while(fcnf.good())
 {
  std::getline(fcnf, line1);           // get line
  g_utils.delTrailLFCR(line1);
  g_utils.delExtBlank(line1);
  if(line1.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line1.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(inSection&&(line1.at(0)=='['))
    {//-------end of section------------------------------------
     fcnf.close();
     return "";
    }
    if(inSection)
    {//-------in the right section: check line------------------
     iPos2=line1.find(value_);
     if(iPos2!=std::string::npos) { //......value_ found.............
      iPos1=line1.find(":");
      if(iPos1!=std::string::npos) { //..... = ......................
       if(iPos2>iPos1) { //............= on correct position....
        s1=line1.substr(0,iPos1);
        g_utils.delExtBlank(s1);
        fcnf.close();
        return s1;
       }
      }
     }
    }
    if(line1==section) inSection=true;
   } // end comment line
  } // end empty line
 }
 fcnf.close();
 return "";
}

//_______find the value to a given key in section_______________
// return: value or "" (on error or not found)
std::string Conf::findValue(std::string section, std::string key_)
{
 unsigned int iPos1, iPos2;            // help variables
 bool   inSection=false;               // in section or not
 std::string s1;                       // help string
 std::string line1;
 if(fname.length()<1) return "";
 std::ifstream fcnf(fname.c_str());
 //------file ready?--------------------------------------------
 if(!fcnf.good()) return "";
 //------check section [], key length>0-------------------------
 //delExtBlank(section);
 if(section.length()<1) return "";     // no section
 if(section.at(section.length()-1)!=']') {
  if(section.at(0)!='[') section="["+section+"]";
  else return "";
 } else {
  if(section.at(0)!='[') return "";
 }
 g_utils.delExtBlank(key_);
 if(key_.length()<1) return "";        // no key
 //------read lines until end of section or until last line----- 
 while(fcnf.good())
 {
  std::getline(fcnf, line1);           // get line
  g_utils.delTrailLFCR(line1);
  g_utils.delExtBlank(line1);
  if(line1.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line1.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(inSection&&(line1.at(0)=='['))
    {//-------end of section------------------------------------
     fcnf.close();
     return "";
    }
    if(inSection)
    {//-------in the right section: check line------------------
     iPos1=line1.find(key_);
     if(iPos1!=std::string::npos) { //......key_ found..........
      iPos2=line1.find(":");
      if(iPos2!=std::string::npos) { //.....: ..................
       if(iPos2>iPos1) { //............= on correct position....
        s1=line1.substr(iPos2+1,std::string::npos);
        g_utils.delExtBlank(s1);
        fcnf.close();
        return s1;
       }
      }
     }
    } // end inSection
    if(line1==section) inSection=true;
   } // end not comment line
  } // end empty line
 }
 fcnf.close();
 return "";
}

//_______read all sections______________________________________
// map: 1 Section = 1 entry
// return: number of sections (-1 on error)
int Conf::readAllSections(std::multimap<std::string,std::string>& m1)
{
 std::string line_;
 std::string secNameOld="";
 std::string secValue="";
 bool bGetLines=true;
 if(fname.length()<1) return -1;
 std::ifstream fcnf(fname.c_str());
 if(!fcnf.good()) return -1;
 m1.clear();
 while(fcnf.good() && bGetLines)
 {//----------read lines until new section found or last line---
  std::getline(fcnf, line_);           // get line
  g_utils.delTrailLFCR(line_);         // delete \r \n 
  g_utils.delExtBlank(line_);          // delete blanks
  //std::cout << "Line >" << line_ << "<" << std::endl;
  if(line_.length()>0)                 // length of line
  {//---------not an empty line---------------------------------
   if(line_.at(0)!='#')                // comment line?
   {//--------not a comment line--------------------------------
    if(line_.at(0)=='[')
    {//-------section line: save previous section---------------
     if(secNameOld.length()>0)
     {//------save previous section-----------------------------
      secValue+="}";
      g_utils.delExtBlank(secNameOld);
      m1.insert(std::pair<std::string,std::string>(secNameOld,secValue));
      secNameOld="";
      secValue="{";
     }
     //-------start new section: get section name---------------
     int i2=line_.find_first_of(']');
     if(i2>2)
     {//-----section name OK------------------------------------
      std::string sec1=line_.substr(1,i2-1);
      std::string sec2=sec1;
      g_utils.str2lower(sec2);
      if(sec2!="end" && sec2!="ende")
      {
       if(sec1.length()>0) 
       {
        secNameOld=sec1;
        secValue="{";
       }
      }
      else bGetLines=false;
     }
    }
    else
    {//-------in a section: add line----------------------------
     int idp=line_.find_first_of(':');      // search for...
     if(idp!=std::string::npos)             // ...colon :
     {//----- colon : found -> line is a pair <key:value>-------
      std::string key1=line_.substr(0,idp);
      std::string val1=line_.substr(idp+1);
      g_utils.delExtBlank(key1);            // delete blanks
      g_utils.delExtBlank(val1);            // delete blanks
      if(val1.length()>0) 
      {//.....add new pair to value string......................
       if(secValue.length()>1) secValue+=",";
       g_utils.replaceAll(val1,",","|");
       if(key1.at(0)!='\"') secValue+="\""; // add ""
       secValue+=key1;                      // add key1
       if(key1.at(key1.length()-1) !='\"') secValue+="\"";
       secValue+=":";
       if(val1.at(0)!='\"') secValue+="\""; // add ""
       secValue+=val1;                      // add key1
       if(val1.at(val1.length()-1) !='\"') secValue+="\"";
      }
     }
     else
     {//-----no colon : found -> no pair <key:value>------------
      // set key to "-"
      if(secValue.length()>1) secValue+=",";
      secValue+="\"-\":";
      if(line_.at(0)!='\"') secValue+="\""; // add ""
      secValue+=line_;                      // add key1
      if(line_.at(line_.length()-1) !='\"') secValue+="\"";
     }
    }
   } // end comment line
  } // end empty line
 }
 fcnf.close();
 //------add last section---------------------------------------
 if(secValue.length()>3)
 {
  secValue+="}";
  g_utils.delExtBlank(secNameOld);
  m1.insert(std::pair<std::string,std::string>(secNameOld,secValue));
 }
 return m1.size();
}

// *************************************************************
//       Conf: helper methods
// *************************************************************

//_______Does the topic match the pattern?______________________
// MQTT placeholder: + Single-Level-Wildcard e.g. door/+/state
//                   # Multi-Level-Wildcard  e.g. door/#
// return true: yes, topic fits pattern; false: no
bool Conf::fits(std::string topic, std::string pattern)
{
 int it=0, ip=0;
 int lent=topic.length();
 int lenp=pattern.length();
 //------check plausbility, special cases-----------------------
 if(lenp<1) return false;
 //------pass through pattern-----------------------------------
 for(ip=0; ip<lenp; ip++)
 {
  if(pattern[ip]=='#') return true;
  if(pattern[ip]=='+')
  {//....+ found: next char must be slash or end of pattern.....
   if((++ip)==lenp) { // + is last char of pattern
    //...last char = +, topic: not allowed to have a next slash.
    while(topic[it]!='/')
    {
     it++;
     if(it>=lent) return true;
    }
    return false;
   } //end of last char = +
   if(pattern[ip]!='/') return false;
   //....topic: find next slash.................................
   while(topic[it]!='/')
   {
    it++;
    if(it>=lent) return false;
   }
  }
  //.....no special case: check characters......................
  if(it>=lent) return false;
  if(pattern[ip]!=topic[it++]) return false;
 }
 if(it==lent) return true;
 return false;
}

// =============================================================

// *************************************************************
//       M4hBase: constructor & co
// *************************************************************

//_______Default constructor____________________________________
M4hBase::M4hBase() { setDefaults(); }

//_______set all default properties_____________________________
void M4hBase::setDefaults()
{
 pfConfig = _CONF_PFILE_;              // path&name config file
 section  = M4H_SECTION;               // section name in config
 msgVersion=Message2(M4H_VERSION_T_IN,  M4H_VERSION_P_IN,
                     M4H_VERSION_T_OUT, M4H_VERSION_P_OUT);
 msgMqttStart=Message();
 msgMqttEnd=Message();
 msgProgEnd=Message();
 msgReadConf=Message2(M4H_CONF_IN_T,  M4H_CONF_IN_P,
                      M4H_CONF_OUT_T, M4H_CONF_OUT_P);
  timeShouldBeAdded=false;
 keys=std::string(M4H_VERSION_KEY_IN);
 keys+="|"+std::string(M4H_VERSION_KEY_OUT),
 keys+="|"+std::string(M4H_MQTTSTART_KEY)+"|"+std::string(M4H_MQTTEND_KEY);
 keys+="|"+std::string(M4H_PROGEND_KEY);
 keys+="|"+std::string(M4H_CONF_IN_KEY)+"|"+std::string(M4H_CONF_OUT_KEY);
 keys+="|"+std::string(M4H_ADDTIME_KEY);
}

// *************************************************************
//       M4hBase: Setter and Getter methods
// *************************************************************

// *************************************************************
//       M4hBase: working methods
// *************************************************************

//_______read config data from file_____________________________
bool M4hBase::readConfig() {
 return readConfig(pfConfig);
}

//_______read config data from file_____________________________
bool M4hBase::readConfig(std::string pfile_)
{
 std::multimap<std::string, std::string> mm1;
 //------(try to) read config file------------------------------
 pfConfig=pfile_;
 if(pfile_.length()<1) return false;
 Conf conf=Conf(pfile_);
 if(!conf.isReady()) return false;
 int iSec=conf.readAllSections(mm1);
 //------get my config data-------------------------------------
 std::multimap<std::string, std::string>::iterator it1;
 std::multimap<std::string, std::string>::iterator it2;
 for (it1 = mm1.begin(); it1 != mm1.end(); it1++)
 {
  //std::cout << it1->first << " | " << it1->second << std::endl;
  //-----common brokertime data---------------------------------
  if(it1->first == M4H_SECTION) {
   std::multimap<std::string, std::string> mmCom;
   g_utils.split2pairs(it1->second, mmCom);
   //....look for common properties.............................
   for(it2 = mmCom.begin(); it2 != mmCom.end(); it2++)
   {
    std::string s1=it2->first;
    g_utils.str2lower(s1);
    if(s1=="version") 
    { //......only version given, for the rest use defaults.....
     msgVersion.payloadOut=it2->second;
    }
    if(s1==M4H_VERSION_KEY_IN) {
     std::string sT, sP;
     if(!g_utils.str2str2(it2->second, sT, sP, ' ')) sT=it2->second;
     msgVersion.topicIn=sT; 
     msgVersion.payloadIn=sP;
    }
    if(s1==M4H_VERSION_KEY_OUT) {
     std::string sAll=it2->second, sT, sP;
     int pos1=sAll.find(" -r");
     if(pos1!=std::string::npos) {
      sAll=sAll.substr(0,pos1);
      msgVersion.retainOut=true;
     }
     if(!g_utils.str2str2(sAll, sT, sP, ' ')) sT=sAll;
     msgVersion.topicOut=sT; 
     msgVersion.payloadOut=sP;
    }
    if(s1==M4H_MQTTSTART_KEY) {
     std::string sAll=it2->second, sT, sP;
     int pos1=sAll.find(" -r");
     if(pos1!=std::string::npos) {
      sAll=sAll.substr(0,pos1);
      msgMqttStart.retain=true;
     }
     if(!g_utils.str2str2(sAll, sT, sP, ' ')) sT=sAll;
     msgMqttStart.topic=sT; 
     msgMqttStart.payload=sP;
    }
    if(s1==M4H_MQTTEND_KEY) {
     std::string sAll=it2->second, sT, sP;
     int pos1=sAll.find(" -r");
     if(pos1!=std::string::npos) {
      sAll=sAll.substr(0,pos1);
      msgMqttEnd.retain=true;
     }
     if(!g_utils.str2str2(sAll, sT, sP, ' ')) sT=sAll;
     msgMqttEnd.topic=sT; 
     msgMqttEnd.payload=sP;
    }
    if(s1==M4H_PROGEND_KEY) {
     std::string sT, sP;
     if(g_utils.str2str2(it2->second, sT, sP, ' '))
     {
      msgProgEnd.topic=sT; 
      msgProgEnd.payload=sP;
     }
    }
        //--------read config file (message in)---------------------
    if(s1==M4H_CONF_IN_KEY) {
     std::string sT, sP;
     if(g_utils.str2str2(it2->second, sT, sP, ' '))
     {
      if(sP=="?") sP=M4H_CONF_IN_P;
      if(sP.length()>2) {
       msgReadConf.payloadIn=sP;
       msgReadConf.topicIn=sT;
      }
     }
    }
    //--------after config file read: message out---------------
    if(s1==M4H_CONF_OUT_KEY) {
     std::string sT, sP;
     if(g_utils.str2str2(it2->second, sT, sP, ' '))
     {
      if(sP=="?") sP=M4H_CONF_OUT_P;
      if(sP.length()>0) {
       msgReadConf.payloadOut=sP;
       msgReadConf.topicOut=sT;
      }
     }
    }
    //--------messages out: add time stamp?---------------------
    if(s1==M4H_ADDTIME_KEY) {
     if(it2->second=="true") this->timeShouldBeAdded=true;
     else this->timeShouldBeAdded=false;
    }
   }
  }
 }
 return true;
}

// *************************************************************
//       M4hBase: helper methods
// *************************************************************

//_______Show all basic properties______________________________
void M4hBase::show()
{
 Conf conf=Conf(pfConfig);
 std::cout<<"=====["<<section<<"]==========================="<<std::endl;
 std::cout << "config file         | " << pfConfig;
 if(!conf.isReady()) std::cout << " (file not found)";
 std::cout<<std::endl;
 std::cout << "all keys            | "<<getKeys()<<std::endl;
 std::cout << "version (in)        | -t " << msgVersion.topicIn << " -m " << msgVersion.payloadIn<<std::endl;
 std::cout << "version (out)       | -t " << msgVersion.topicOut << " -m " << msgVersion.payloadOut;
 if(msgVersion.retainOut) std::cout << " -r";
 std::cout<<std::endl;
 std::cout << "mqtt @ start (out,*)| -t " << msgMqttStart.topic << " -m " << msgMqttStart.payload;
 if(msgMqttStart.retain) std::cout << " -r";
 std::cout<<std::endl;
 std::cout << "mqtt @ end (out,*)  | -t " << msgMqttEnd.topic << " -m " << msgMqttEnd.payload;
 if(msgMqttEnd.retain) std::cout << " -r";
 std::cout<<std::endl;
 std::cout << "progend by mqtt (in)| -t " << msgProgEnd.topic << " -m " << msgProgEnd.payload<<std::endl;
 std::cout << "reload conf-file(in)| -t " << msgReadConf.topicIn << " -m " << msgReadConf.payloadIn<<std::endl;
 std::cout << "reload conf-fil(out)| -t " << msgReadConf.topicOut << " -m " << msgReadConf.payloadOut<<std::endl;
 std::cout << "         * add time | ";
 if(timeShouldBeAdded) std::cout<<"true"<<std::endl;
 else std::cout<<"false"<<std::endl;
}

// *************************************************************
//       M4hUtils: little helpers
// *************************************************************

//_______remove line feed and carriage return from end of line__
void M4hUtils::delTrailLFCR(std::string& s1) {
 bool goon=true;
 int i=s1.length()-1;
 if(i<0) return;
 while((i>=0)&&goon)
 {
  if((s1.at(i)=='\r')||(s1.at(i)=='\n')) i--;
  else goon=false;
 }
 s1.erase(i+1, std::string::npos);
}

//_______remove blank(s) from end of line_______________________
void M4hUtils::delTrailBlank(std::string& s1) {
 bool goon=true;
 int i=s1.length()-1;
 if(i<0) return;
 while((i>=0)&&goon)
 {
  if(s1.at(i)==' ') i--;
  else goon=false;
 }
 s1.erase(i+1, std::string::npos);
}

//_______remove blank(s) from begin of line_____________________
void M4hUtils::delLeadBlank(std::string& s1) {
 int len=s1.length();
 if(len<1) return;
 int i=0;
 while(i<len) {
  if(s1.at(i)==' ') i++; else break;
 }
 if(i>0) s1.erase(0,i);
}

//_______remove blank(s) from begin and end of line_____________
void M4hUtils::delExtBlank(std::string &s1) {
 delTrailBlank(s1);
 delLeadBlank(s1);
}

//_______replace a part of the string to another________________
void M4hUtils::replaceAll(std::string &str, const std::string& old_, const std::string& new_)
{
 size_t start_pos = 0;
 while((start_pos = str.find(old_, start_pos)) != std::string::npos) {
  str.replace(start_pos, old_.length(), new_);
  start_pos += new_.length();
 }
}

//_______convert string to lower string_________________________
void M4hUtils::str2lower(std::string &s1)
{
 std::string s2="";
 int    ilen=s1.length();
 for(int i=0; i<ilen; i++)
 {
  char c=s1.at(i);
  c=tolower(c);
  s2=s2+c;
 }
 s1=s2;
}

// ___________split string to vector of strings_________________
std::vector<std::string> M4hUtils::str2vector(const std::string& data, const std::string& delimiters) {
    auto is_delim = [&](auto & c) { return delimiters.find(c) != std::string::npos; };
    std::vector< std::string > result;
    for (std::string::size_type i(0), len(data.length()), pos(0); i <= len; i++) {
        if (is_delim(data[i]) || i == len) {
            auto tok = data.substr(pos, i - pos);
            if ( !tok.empty() )
                result.push_back( tok );
            pos = i + 1;
        }
    } return result;
}

//_______convert json-string to pairs___________________________
// return: true result ok
bool M4hUtils::split2pairs(std::string s1, std::multimap<std::string, std::string>& mm1)
{
 bool debug=false;
 int  i1;                                   // running index
 int  iB;                                   // begin index key|val
 int  iE;                                   // end index key|val
 int  iQ;                                   // index of last "
 int  iLast;                                // json end index
 std::string key;                           // key
 std::string val;                           // value 
 bool doLoop=true;                          // do while loop
 char c;                                    // actual char
 int  curv=0;                               // curved brackets
 int  quot=0;                               // number of "
 mm1.clear();
 int len=s1.length();
 if(len<2) return false;                    // s1 too short
 if(debug) std::cout << "len=" <<len;
 //-----------search for key begin------------------------------
 iB=0;
 do {
  c=s1.at(iB++);                            //
  if(c=='{') curv++;                        // brackets around
  else if(c!=' ') { doLoop=false; iB--; }   //
  if(iB>=len) {mm1.clear(); return false;}  // 
 } while(doLoop);
 if(curv>1) {mm1.clear(); return false;}    // more than one {
 if(debug) std::cout << " iB=" << iB;
 //-----------search for json end-------------------------------
 iLast=len;
 doLoop=true;
 do {
  c=s1.at(--iLast);                         //
  if(c=='}') curv--;                        // brackets around
  else if(c!=' ') { doLoop=false; iLast++; } //
  if(iLast<=iB) {mm1.clear(); return false;} //
 } while(doLoop);
 if(curv!=0) {mm1.clear(); return false;}   // wrong number {}
 if(debug) std::cout <<" iLast="<<iLast<<std::endl;
 i1=iB;                                     // start of key
 do {//=======for all pairs=====================================
  //----------search for begin of key (start with i1)-----------
  while(s1.at(i1)==' ') {
   i1++;
   if(i1>=iLast) {mm1.clear(); return false;}
  }
  //----------search for end of key-----------------------------
  iB=i1;
  doLoop=true;
  quot=0;
  do {
   c=s1.at(i1++);                           // get char
   if(c=='"') { //..........................// special char "
    if(quot==0) iB++;                       // begin after 1st "
    quot++;                                 // number of "
    iQ=i1-1;                                // index of last "
   }
   if(c==':') { //..........................// special char :
    if(quot==0)
    {//.......no surrounding quotation marks "".................
     iE=i1-1;
     doLoop=false;
    } else
    {//.......surrounding quotation marks ""....................
     //if((quot&1)==0)
     {//......even number of quotation marks "".................
      bool isEnd=true;
      for(int i=i1-2; i>iQ; i--) {
       if(s1.at(i)!=' ') isEnd=false;
      }
      if(isEnd) { doLoop=false; iE=iQ; }
     } // end even number of quotation marks
    } // end surrounding quotation marks
   } // end if :
   if(i1>=iLast) {mm1.clear(); return false;}
  } while(doLoop);
  if(debug) std::cout << " iB=" << iB << " iE="<< iE;
  key=s1.substr(iB,iE-iB);
  if(debug) std::cout << " key=|" << key << "|" << std::endl;
  //----------search for begin of val (start with i1)-----------
  while(s1.at(i1)==' ') {
   i1++;
   if(i1>=iLast) {mm1.clear(); return false;}
  }
  //----------search for end of val-----------------------------
  iB=i1;                                    // start of key
  doLoop=true;
  quot=0;
  do {
   c=s1.at(i1++);                           // get char
   if(c=='"') { //..........................// special char "
    if(quot==0) iB++;                       // begin after 1st "
    quot++;                                 // number of "
    iQ=i1-1;                                // index of last "
   }
   if(c==',') { //..........................// special char :
    if(quot==0)
    {//.......no surrounding quotation marks "".................
     iE=i1;
     doLoop=false;
    } else
    {//.......surrounding quotation marks ""....................
     //if((quot&1)==0)
     {//......even number of quotation marks "".................
      bool isEnd=true;
      for(int i=i1-2; i>iQ; i--) {
       if(s1.at(i)!=' ') isEnd=false;
      }
      if(isEnd) {
       doLoop=false; iE=iQ; 
      }
     }
    }
   }
   if(i1>=iLast) {
    doLoop=false; 
    iE=iLast;
    if(s1.at(iE-1)=='"') iE--;
   }
  } while(doLoop);
  if(debug) std::cout << " iB=" << iB << " iE="<< iE;
  val=s1.substr(iB,iE-iB);
  long valLen=val.length();
  if(valLen>0) {
   if(val.at(valLen-1)==',') val=val.substr(0,valLen-1);
  }
  if(debug) std::cout << " val=|" << val << "|" << std::endl;
  if(key.length()>0) mm1.insert(std::make_pair(key,val));
 } while(i1<iLast);
 return true;
}

//_______split string to vector_________________________________
void M4hUtils::str2vector1(std::string sIn, std::vector<std::string>&vOut, char delimiter)
{
 std::string temp="";
 for(int i=0; i<sIn.size(); i++)
 {
  if(sIn[i] == delimiter)
  {
   vOut.push_back(temp);
   temp="";
  }
  else
   temp += sIn[i];
 }
 vOut.push_back(temp);
}

//_______split string to 2 strings______________________________
// return: true=split ok, sPart1 and sPart2 changed
//         false=delimiter not found or one sPart is empty
//               sPart1 and sPart2 NOT changed!
bool M4hUtils::str2str2(std::string sIn,std::string &sPart1,
  std::string &sPart2,char delimiter)
{
 delExtBlank(sIn);                      // delete blanks begin/end
 int idp=sIn.find_first_of(delimiter);  // search for...
 if(idp!=std::string::npos)             // ...delimiter
 {//-------delimiter found -> line is a pair <key value>--------
  std::string key1=sIn.substr(0,idp);
  std::string val1=sIn.substr(idp+1);
  delExtBlank(key1);                   // delete blanks begin/end
  delExtBlank(val1);                   // delete blanks begin/end
  //if(key1.length()>0 && val1.length()>0) 
  if(key1.length()>0)
  {//--------split OK-------------------------------------------
   sPart1=key1;                        // part 1
   sPart2=val1;                        // part 2
   return true;
  } // end split OK
 } // end delimiter found
 return false;
}

//_______convert d HMS string to sec____________________________
// return: number of seconds or -1 on error
time_t M4hUtils::DHMS2sec(std::string sDHMS)
{
 time_t sec_=0;
 time_t tmp_=0;
 std::string s1=sDHMS;
 std::string sTmp="";
 int len=s1.length();
 int i=0;
 if(len<5) return -1;
 try {
  //-----remove leading blanks----------------------------------
  while(i<len) {
   if(s1.at(i)==' ') i++; else break;
  }
  if(i>0) s1.erase(0,i);
  if(s1.length()<5) return -1;
  //-----remove trailing blanks---------------------------------
  bool goon=true;
  i=s1.length()-1;
  if(i<0) return -1;
  while((i>=0)&&goon)
  {
   if(s1.at(i)==' ') i--;
   else goon=false;
  }
  s1.erase(i+1, std::string::npos);
  //-----split in days and HMS----------------------------------
  int iBlank=s1.find_first_of(' ');         // search for...
  if(iBlank!=std::string::npos)             // ...delimiter
  {//----days found---------------------------------------------
   sTmp=s1.substr(0,iBlank);                // days string
   s1=s1.substr(iBlank+1);                  // HH:MM:SS
   if(s1.length()<5) return -1;             // too short for H:M:S
   tmp_=std::stoul(sTmp);                   // days to number
   sec_=3600*24*tmp_;                       // seconds of day
  }
  //-----convert HH of s1=HH:MM:SS to sec-----------------------
  int idp=s1.find_first_of(':');            // search for...
  if(idp==std::string::npos) return -1;     // ...delimiter
  sTmp=s1.substr(0,idp);                    // HH string
  s1=s1.substr(idp+1);                      // MM:SS string
  if(s1.length()<3) return -1;              // too short for M:S
  tmp_=std::stoul(sTmp);                    // hours to number
  sec_+=tmp_*3600;                          // add secs of hours
  //-----convert s1= MM:SS to sec-------------------------------
  idp=s1.find_first_of(':');                // search for...
  if(idp==std::string::npos) return -1;     // ...delimiter
  sTmp=s1.substr(0,idp);                    // MM string
  s1=s1.substr(idp+1);                      // SS string
  tmp_=std::stoul(sTmp);                    // MM to number
  sec_+=tmp_*60;                            // add secs of mins
  tmp_=std::stoul(s1);                      // SS to number
  sec_+=tmp_;                               // add secs of mins
 }
 catch(...) { return -2; }
 return sec_;
}

//_______convert time to d HMS string___________________________
// if time 1 day ore more:  format D HH:MM:SS
// if time less then 1 day: format HH:MM:SS
std::string M4hUtils::sec2DHMS(time_t tsec)
{
 time_t min_ = tsec / 60;
 time_t sec_ = tsec - 60*min_;
 time_t h_ = min_ / 60;
 min_ = min_ - 60*h_;
 time_t d_ = h_ / 24;
 h_ = h_ - 24*d_;
 char ca[32];
 if(d_>0) sprintf(ca, "%ld %02ld:%02ld:%02d", d_, h_, min_, sec_);
     else sprintf(ca, "%02ld:%02ld:%02d", h_, min_, sec_);
 std::string s1 = ca;
 return s1;
}

//_______convert time to HMS string_____________________________
std::string M4hUtils::sec2HMS(time_t tsec)
{
 if(tsec==-1) return "";
 if(tsec<0) return "";
 time_t min_ = tsec / 60;
 time_t sec_ = tsec - 60*min_;
 time_t h_ = min_ / 60;
 min_ = min_ - 60*h_;
 char ca[32];
 sprintf(ca, "%02ld:%02ld:%02d", h_, min_, sec_);
 std::string s1 = ca;
 return s1;
}

//_______return actual system date and time_____________________
std::string M4hUtils::getDateTime() {
 std::string s1="%d.%m.%Y %H:%M:%S";
 return getDateTime(s1);
}

//_______return actual system date and time_____________________
std::string M4hUtils::getDateTime(std::string sTimeformat)
{
 int len=20+sTimeformat.length();
 char caNow[len];
 time_t now_secs;
 time(&now_secs);
 struct tm* time_=localtime(&now_secs);
 strftime(caNow,len,sTimeformat.c_str(),time_);
 std::string s(caNow);
 return s;
}
