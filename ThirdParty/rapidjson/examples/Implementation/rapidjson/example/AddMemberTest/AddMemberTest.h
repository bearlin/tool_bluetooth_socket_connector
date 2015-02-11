
#ifndef _ADD_MEMBER_TEST_H_
#define _ADD_MEMBER_TEST_H_

#include "LtpCommon.h"
//#include "LtpExceptions.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <stdio.h>

  class CVersion 
  {
  private:
    int major;
    int minor;
      
  public: 
    CVersion() 
    {
      this->major = 0;
      this->minor = 0;
    };
      
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
    //std::vector<TLocationType>* parametersReq;// = new ArrayList<>();
    rapidjson::Value* parametersReq;
    std::vector<TypedLocation>* parametersResp;
  public: 
    RequestHeader() :
      ptl_ver(NULL), 
      msg_id(0),
      parametersReq(NULL),
      parametersResp(NULL)
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

    void setParameters(rapidjson::Value* params) {
      printf("setParameters(rapidjson::Value* params)\n");

      this->parametersReq = params;
      printf("setParameters:0x%x = 0x%x\n", this->parametersReq, params);

      const rapidjson::Value& a = *(this->parametersReq);
      assert(a.IsArray());
      printf("aa0:%d\n", a[rapidjson::SizeType(0)].GetInt());
      printf("aa1:%d\n", a[rapidjson::SizeType(1)].GetInt());
    }
    
    rapidjson::Value* getParameters() {
      printf("getParameters\n");
      printf("return this->parametersReq = 0x%x\n", this->parametersReq);

      const rapidjson::Value& b = *(this->parametersReq);
      assert(b.IsArray());
      printf("bb0:%d\n", b[rapidjson::SizeType(0)].GetInt());
      printf("bb1:%d\n", b[rapidjson::SizeType(1)].GetInt());
      
      return this->parametersReq;
    }

    void setParameters(std::vector<TypedLocation>* param) {
      this->parametersResp = param;
    }
    
    std::vector<TypedLocation>* getParameters(int i) {
      return this->parametersResp;
    }
  };
  
  class RequestMessage : public RequestHeader 
  {
  public:
    std::string ToJson();  // REQ
    std::string BuildJson(CVersion* ptl_ver, std::string msg_type, int msg_id, std::string method, rapidjson::Value& params); // REQ
    std::string GetFormat(std::vector<TLocationType>& params); // REQ 
  };
  
  class ResponseMessage : public RequestHeader 
  {
  public:
    int resp_code;
    enum TResponseCode 
    {
      EResponseCode_MESSAGE_OK = 0, 
      EResponseCode_MESSAGE_NOT_WELL_FORMED    = 1,
      EResponseCode_INCOMPATIBLE_PROTOCOL_VERSION    = 2,
      EResponseCode_UNKNOW_METHOD    = 3
    };
    
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
	
    std::string ToJson(); // RESP 
    std::string BuildJson(CVersion* ptl_ver, std::string msg_type, int msg_id, std::string method, int resp_code, std::vector<TypedLocation>& params); // RESP
    std::string GetFormat(std::vector<TypedLocation>& params); // RESP 
  };
  
#endif  // _ADD_MEMBER_TEST_H_