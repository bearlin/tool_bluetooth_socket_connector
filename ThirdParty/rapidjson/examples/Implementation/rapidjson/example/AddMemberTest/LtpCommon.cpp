//
// This file contain Common datas for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpCommon.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */


  std::string TimestampedLocation::GetLocalTimeStamp()
  {
    // Timestamps are presented as strings using a restricted ISO8601 format: 
    // YYYY-MM-DDThh:mm:ss.SSSZ ('T' and 'Z' are literal.). 
    // EX: "1997-07-16T19:20:30.450+0100".
    // EX: "2014-02-19T00:01:07.000+0800"
    // Check http://www.w3.org/TR/NOTE-datetime, http://www.cl.cam.ac.uk/~mgk25/iso-time.html
    // Check http://www.cplusplus.com/reference/ctime/strftime/ about how to create this timestamp format.
    std::string timestamp;		
    
    time_t rawtime;
    struct tm* timeinfo;
    char buffer [2048];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 2048, "%FT%T.000%z", timeinfo);
    //printf("%s\n", buffer);
    timestamp = buffer;
    
    return timestamp;
  }

  int CJsonTools::QueringObjectMembersAndTypes(const rapidjson::Value& json)
  {
    if (json.IsObject())
    {
      printf("***--------------***\n");
      for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
        printf("Type of member \"%s\" : %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
      printf("***--------------***\n");
      return 1;
    }
    else
    {
      printf("Can't QueringObjectMembersAndTypes!\n");
      //throw CLtpRuntimeException("Illegal JSON Value, must start from Object {...}");
      return 0;
    }
  }

  // Convert a rapidjson::Value to string.
  std::string CJsonTools::JsonValueToString(rapidjson::Value& json)
  {
    //printf("============ rapidjson::Value Created============ \n");
    // Create Writer to StringBuffer.
    rapidjson::StringBuffer strbuf1;
    rapidjson::Writer<rapidjson::StringBuffer> writer1(strbuf1);
    json.Accept(writer1);	// Accept() traverses the DOM and generates Handler events.
    //printf("Condense:\n%s\n", strbuf1.GetString());

    rapidjson::StringBuffer strbuf2;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer2(strbuf2);
    json.Accept(writer2);	// Accept() traverses the DOM and generates Handler events.
    //printf("Pretty:\n%s\n", strbuf2.GetString());
    //printf("============ rapidjson::Value Created============ \n");

    return strbuf1.GetString();
  }

  //rapidjson::Value can be thought of a variant type, representing a JSON value which can stores either a null value, false, true, number, string, array or object.
  void CJsonTools::CreateJsonValue(std::vector<TLocationType>& locationTypes, rapidjson::Value& json)  // REQ for Ltp_methods_getNavData.
  {
    // For REQ format: ("parameters" : [0,1,...])
    rapidjson::Document document; //a temp rapidjson::Document just for GetAllocator().

    // Set this value as an empty array.
    json.SetArray();
    json.Clear();

    // Loop through the std::vector<TLocationType>, then PushBack() each <TLocationType> to this JSON Array.
    for (std::vector<TLocationType>::iterator it=locationTypes.begin(); it!=locationTypes.end(); ++it)
      json.PushBack((*it), document.GetAllocator());
    
    // Convert a rapidjson::Value to string.
    //printf("CreateJsonValue by std::vector<TLocationType>: %s\n", JsonValueToString(json).c_str());
  }

  //rapidjson::Value can be thought of a variant type, representing a JSON value which can stores either a null value, false, true, number, string, array or object.
  void CJsonTools::CreateJsonValue(std::vector<TypedLocation>& typedLocations, rapidjson::Value& json)  // RESP for Ltp_methods_getNavData.
  {
    // For RESP format: ("parameters" : [{...},{...},...])
    rapidjson::Document document; //a temp rapidjson::Document just for GetAllocator().

    // Set this value as an empty array.
    json.SetArray();
    json.Clear();

    // Loop through the std::vector<TypedLocation>, then PushBack() each <TypedLocation> to this JSON Array.
    for (std::vector<TypedLocation>::iterator it=typedLocations.begin(); it!=typedLocations.end(); ++it)
    {
      rapidjson::Value JObjtmp(rapidjson::kObjectType);
      JObjtmp.AddMember(Ltp_methods_getNavData_param_location_type, it->getNavDataType(), document.GetAllocator());
      JObjtmp.AddMember(Ltp_methods_getNavData_param_timestamp, it->getNavData().getTimestamp().c_str(), document.GetAllocator());//it->getNavData().getTimestamp().c_str()
      JObjtmp.AddMember(Ltp_methods_getNavData_param_latitude, it->getNavData().getCoordinates().getLatitude(), document.GetAllocator());
      JObjtmp.AddMember(Ltp_methods_getNavData_param_longitude, it->getNavData().getCoordinates().getLongitude(), document.GetAllocator());
      json.PushBack(JObjtmp, document.GetAllocator());
    }

    //DBG.
    for (rapidjson::Value::ConstValueIterator itr = json.Begin(); itr != json.End(); ++itr)
    {
      CJsonTools::QueringObjectMembersAndTypes(*itr);
    }

    //DBG.
    //printf("CreateJsonValue by std::vector<TypedLocation>: %s\n", JsonValueToString(json).c_str());
  }

