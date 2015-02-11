
#ifndef _STRING_BUFFER_TEST_H_
#define _STRING_BUFFER_TEST_H_

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"	// for stringify JSON
#include "rapidjson/stringbuffer.h" // https://code.google.com/p/rapidjson/issues/detail?id=34
#include <stdio.h>

class CVersion 
{
private:
  int major;
  int minor;
    
public: 
  CVersion() 
  {};
    
  CVersion(int major, int minor) {
    this->major = major;
    this->minor = minor;
  }

  int getMajor() {
    return major;
  }

  void setMajor(int major) {
    this->major = major;
  }

  int getMinor() {
    return minor;
  }

  void setMinor(int minor) {
    this->minor = minor;
  }
};

class RequestHeader 
{
private: 
  CVersion* ptl_ver;
  std::string msg_type;
  int msg_id;
  std::string method;
  std::vector<std::string> parameters;// = new ArrayList<>();
    
public: 
  RequestHeader() :
  ptl_ver(NULL), 
  msg_id(0)
  {
  }
  
  ~RequestHeader()
  {
  }
  
  CVersion* getPtlVer() {
    return ptl_ver;
  }
  
  void setPtlVer(CVersion* ptlVer) {
    ptl_ver = ptlVer;
  }
  
  std::string getMsgType() {
    return msg_type;
  }
  
  void setMsgType(std::string msgType) {
    msg_type = msgType;
  }
  
  int getMsgId() {
    return msg_id;
  }
  
  void setMsgId(int msgId) {
    msg_id = msgId;
  }
  
  std::string getMethod() {
    return method;
  }
  
  void setMethod(std::string method) {
    this->method = method;
  }

  std::vector<std::string>* getParameters() {
    return &parameters;
  }
   
  std::string toString() {
    std::string finalStr;
    char buffer [50];
    int n;
    
    n=sprintf(buffer, "%d.%d", ptl_ver->getMajor(), ptl_ver->getMinor());
    printf("version string:%s, len=%d\n", buffer, n);
    finalStr = std::string(buffer) + " " + msg_type + " ";
    
    n=sprintf(buffer, "%d", msg_id);
    printf("msg_id string:%s\n, len=%d", buffer, n);
    finalStr += std::string(buffer) + " " + method;
    
    return finalStr;//std::string(buffer) + " " + msg_type + " " + std::to_string(msg_id) + " " + method;
  }
};

class RequestMessage : public RequestHeader 
{
  // Currently no extensions.
};

enum TJsonMessageStatus 
{
  EJsonMessageStatus_MESSAGE_NOT_WELL_FORMED    = 1,
  EJsonMessageStatus_INCOMPATIBLE_PROTOCOL_VERSION    = 2,
  EJsonMessageStatus_UNKNOW_METHOD    = 3
};
  
class ResponseMessage : public RequestHeader 
{
public:
  int resp_code;
  
  ResponseMessage()
  {
    //MESSAGE_NOT_WELL_FORMED = 1;
    //INCOMPATIBLE_PROTOCOL_VERSION = 2;
  }
  
  ~ResponseMessage()
  {
  }
  
  int getRespCode() {
    return resp_code;
  }
  
  void setRespCode(int respCode) {
    this->resp_code = respCode;
  }
  
  std::string ToJson()
  {
    using namespace rapidjson;

    // Convert JSON document to string
    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    
    // Create RESP json string to this Writer.
    writer.StartObject();
      writer.String("ptl_ver");
        writer.StartObject();
          writer.String("major");         writer.Uint(getPtlVer()->getMajor());
          writer.String("minor");         writer.Uint(getPtlVer()->getMinor());
        writer.EndObject();
      writer.String("msg_type");          writer.String(getMsgType().c_str());
      writer.String("msg_id");            writer.Uint(getMsgId());
      writer.String("method");            writer.String(getMethod().c_str());
      writer.String("resp_code");         writer.Uint(getRespCode());
    
    std::vector<std::string>* Parameters = getParameters();
    if (0 != Parameters->size())
    {
      // FIX ME:
      // What's the correct parameters format here?
      
      // Write parameters.
      /*
      "parameters": {
          "latitude": "25048130",
          "longitude": "121517070"
      }
      */
      writer.String("parameters");
      writer.StartObject();
      for (std::vector<std::string>::iterator it = Parameters->begin() ; it != Parameters->end(); ++it)
      {
        writer.String(it->c_str());//*it;
      }
      writer.EndObject();
    }
    writer.EndObject();
    
    return strbuf.GetString();
  }
    
};

#endif  // _STRING_BUFFER_TEST_H_