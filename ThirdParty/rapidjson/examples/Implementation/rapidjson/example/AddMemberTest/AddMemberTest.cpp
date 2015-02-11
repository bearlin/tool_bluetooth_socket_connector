// StringBuffer example from https://code.google.com/p/rapidjson/issues/detail?id=34
// This example shows how to creating JSON string to StringBuffer(not FileStream) directly.

#include "AddMemberTest.h"
#include "LtpCommon.h"
#include <cstdio>
#include <string>
#include <vector>
#include "rapidjson/prettywriter.h"   // for stringify JSON
#include "rapidjson/filestream.h"     // wrapper of C stream for prettywriter as output
#include "rapidjson/stringbuffer.h"   // https://code.google.com/p/rapidjson/issues/detail?id=34
#include "rapidjson/document.h"       // rapidjson's DOM-style API

using namespace rapidjson;

std::string RequestMessage::ToJson()  // REQ
{
  printf("RequestMessage::ToJson()\n");
  using namespace rapidjson;
#if 1 // use GenericValue::AddMember() to create the whole string.
  #if 0
  rapidjson::Document doc;
  doc.SetObject();

  rapidjson::Value value_obj(rapidjson::kObjectType);

  //const rapidjson::Value& b = *(getParameters());
  //assert(b.IsArray());
  //printf("1cc0:%d\n", b[rapidjson::SizeType(0)].GetInt());
  //printf("1cc1:%d\n", b[rapidjson::SizeType(1)].GetInt());
  
  value_obj.AddMember("string", "v1", doc.GetAllocator());

  const rapidjson::Value& b = *(getParameters());
  assert(b.IsArray());
  printf("1cc0:%d\n", b[rapidjson::SizeType(0)].GetInt());
  printf("1cc1:%d\n", b[rapidjson::SizeType(1)].GetInt());
  
  return "";
  #else
  //rapidjson::Document document; //a temp rapidjson::Document just for GetAllocator().
  rapidjson::Document document;
  document.SetObject();
  
  rapidjson::Value JObjFinal(rapidjson::kObjectType);// Set this value as an empty object.
  rapidjson::Value JObjTmp(rapidjson::kObjectType);

  printf("maj:%d, min:%d\n", getPtlVer()->getMajor(), getPtlVer()->getMinor());
  printf("msg_type:%s\n", getMsgType().c_str());
  printf("msg_id:%d\n", getMsgId());
  printf("Method:%s\n", getMethod().c_str());
  
  const rapidjson::Value& b = *(getParameters());
  assert(b.IsArray());
  printf("1cc0:%d\n", b[rapidjson::SizeType(0)].GetInt());
  printf("1cc1:%d\n", b[rapidjson::SizeType(1)].GetInt());

  JObjTmp.AddMember("major", 1, document.GetAllocator());
  
  //const rapidjson::Value& b = *(getParameters());
  assert(b.IsArray());
  printf("2cc0:%d\n", b[rapidjson::SizeType(0)].GetInt());
  printf("2cc1:%d\n", b[rapidjson::SizeType(1)].GetInt());
  
  JObjTmp.AddMember(Ltp_minor, getPtlVer()->getMinor(), document.GetAllocator());
  JObjFinal.AddMember(Ltp_ptl_ver, JObjTmp, document.GetAllocator());
  JObjFinal.AddMember(Ltp_msg_type, getMsgType().c_str(), document.GetAllocator());
  JObjFinal.AddMember(Ltp_msg_id, getMsgId(), document.GetAllocator());
  JObjFinal.AddMember(Ltp_method, getMethod().c_str(), document.GetAllocator());
  JObjFinal.AddMember(Ltp_parameters, *(getParameters()), document.GetAllocator());

  //CJsonTools::QueringObjectMembersAndTypes(JObjFinal);

  std::string str = CJsonTools::JsonValueToString(JObjFinal);
  printf("str:%s\n", str.c_str());
  
  return CJsonTools::JsonValueToString(JObjFinal).c_str();
  #endif
#else //Use Writer<StringBuffer> to create the whole string.
  // Create Writer to StringBuffer.
  StringBuffer strbuf;
  Writer<StringBuffer> writer(strbuf);
  
  // Create RESP json string to this Writer.
  writer.StartObject();
    writer.String(Ltp_ptl_ver);
      writer.StartObject();
        writer.String(Ltp_major);   writer.Uint(getPtlVer()->getMajor());
        writer.String(Ltp_minor);   writer.Uint(getPtlVer()->getMinor());
      writer.EndObject();
    writer.String(Ltp_msg_type);    writer.String(getMsgType().c_str());
    writer.String(Ltp_msg_id);      writer.Uint(getMsgId());
    writer.String(Ltp_method);      writer.String(getMethod().c_str());

  // Star to fill all the "parameters".
  //-----------------------------------------------
  writer.String(Ltp_parameters);
    writer.StartArray();
      rapidjson::Value* Parameters = getParameters();
      if (NULL == Parameters)
          throw CLtpRuntimeException("paramters list can't be NULL");
      
      if ((NULL != Parameters) && 
          (0 != Parameters->size()))
      {
        for (std::vector<TLocationType>::iterator it = Parameters->begin() ; it != Parameters->end(); ++it)
        {
          writer.Int(*it);
        }
      }
    writer.EndArray();
  //-----------------------------------------------
  writer.EndObject();
  
  return strbuf.GetString();
#endif
}

std::string RequestMessage::BuildJson(CVersion* ptl_ver, std::string msg_type, int msg_id, std::string method, rapidjson::Value& params) // REQ
{
  printf("RequestMessage::BuildJson()\n");
  //std::cout<<"major"<<ptl_ver->getMajor()<<", minor"<<ptl_ver->getMinor()<<", msg_type:"<<msg_type<<", msg_id:"<<msg_id<<", method:"<<method<<std::endl;
  setPtlVer(ptl_ver);
  setMsgType(msg_type);
  setMsgId(msg_id);
  setMethod(method);
  setParameters(&params);
  return ToJson();
}

std::string RequestMessage::GetFormat(std::vector<TLocationType>& params) // REQ 
{
  printf("RequestMessage::GetFormat()\n");
  std::string format;
  
  // Get REQ expect_msg format string for sprintf().
  format.clear();
  format = "{\"" + 
          std::string(Ltp_ptl_ver) + "\":{\"" + 
          std::string(Ltp_major) + "\":%d,\"" + 
          std::string(Ltp_minor) + "\":%d},\"" + 
          std::string(Ltp_msg_type) + "\":\"%s\",\"" + 
          std::string(Ltp_msg_id) + "\":%d,\"" + 
          std::string(Ltp_method) + "\":\"%s\",\"" + 
          std::string(Ltp_parameters) + "\":[";

  // Tell if we want to fill those parameters in to string.
  if (0 == params.size())
  {
    // No paramters.
    format += "]}";
  }
  else
  {
    // With paramters.
    format += "%d";
    for (int i = 0; i < params.size() - 1; i++)
    {
      format += ",%d";
    }
    format += "]}";
  }
  
  return format;
}

void TEST()
{
RequestMessage* requestMessage = new RequestMessage();
CVersion* ptl_ver = new CVersion();
std::vector<TLocationType> locationTypes;

std::string format;
std::string actual_msg;
std::string expect_msg;

char* buffer;
int n;

// Temp 512k buffer for store messages.
buffer = (char*)malloc(512L * 1024);

// Test for REQ normal(with parameter lists).
//-------------------------------------
ptl_ver->setMajor(9);
ptl_ver->setMinor(9);
locationTypes.clear();
locationTypes.push_back(ELocationType_DESTINATION);
locationTypes.push_back(ELocationType_LAST_KNOWN_CAR_POSITION);

//EXPECT_NO_THROW({
  //try
  {
    // CreateJsonValue
    rapidjson::Value json;
    CJsonTools::CreateJsonValue(locationTypes, json);
    //printf("JsonValueToString: %s\n", CJsonTools::JsonValueToString(json).c_str());

    // get actual_msg
    actual_msg = requestMessage->BuildJson(ptl_ver, Ltp_msg_types_req, 12, Ltp_methods_getNavData, json);
    //actual_msg = requestMessage->BuildJson(ptl_ver, Ltp_msg_types_req, 12, Ltp_methods_getNavData, json);
    //actual_msg = requestMessage->BuildJson(ptl_ver, Ltp_msg_types_req, 12, Ltp_methods_getNavData, params);

    // get expect_msg
    format = requestMessage->GetFormat(locationTypes);
    n = sprintf(buffer, format.c_str(), ptl_ver->getMajor(), ptl_ver->getMinor(), requestMessage->getMsgType().c_str(), requestMessage->getMsgId(), requestMessage->getMethod().c_str(), locationTypes[0], locationTypes[1]);
    expect_msg = buffer;
  }
  /*catch (CLtpBaseException& e)
  {
    std::cout << e.what() << std::endl;
    throw 1; // make EXPECT_NO_THROW know we've got a throw.
  }
  catch (...)
  {
    throw 1; // make EXPECT_NO_THROW know we've got a throw.
  }*/
//});

printf("REQ normal(with parameter lists) :\n");
printf("format : %s\n", format.c_str());
printf("expect_msg : %s\n", expect_msg.c_str());
printf("actual_msg : %s\n", actual_msg.c_str());
//ASSERT_TRUE(0 == expect_msg.compare(actual_msg));
//-------------------------------------

free(buffer);

delete ptl_ver;
delete requestMessage;

}

int main(int, char*[]) 
{
  TEST();
  
	return 0;
}
