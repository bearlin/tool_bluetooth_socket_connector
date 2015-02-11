//
// This file contain common datas for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPCOMMON_H_
#define _LTPCOMMON_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"	// for stringify JSON
#include "rapidjson/stringbuffer.h" // https://code.google.com/p/rapidjson/issues/detail?id=34
#include "rapidjson/prettywriter.h"



  enum TConnectionStatus
  {
    EConnectionStatusCONNECTED   = 0,
    EConnectionStatusNOT_CONNECTED
  };
    
  enum TLocationType 
  {
      ELocationType_DESTINATION = 0,
      ELocationType_LAST_KNOWN_CAR_POSITION
  };

  // Some const strings.
  static const char* Ltp_ptl_ver = "ptl_ver";
  static const char* Ltp_major = "major";
  static const char* Ltp_minor = "minor";
  static const char* Ltp_msg_type = "msg_type";
  static const char* Ltp_msg_id = "msg_id";
  static const char* Ltp_method = "method";
  static const char* Ltp_resp_code = "resp_code";
  static const char* Ltp_parameters = "parameters";

  // rapidjson types.
  static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

  // Message types list:
  static const char* Ltp_msg_types_req = "REQ";
  static const char* Ltp_msg_types_resp = "RESP";
  
  // Methods list:
  static const char* Ltp_methods_TestFunc = "TestFunc";
  static const char* Ltp_methods_getNavData = "getNavData";

  // Method "getNavData" parameters:
  static const char* Ltp_methods_getNavData_param_location_type = "location_type";
  static const char* Ltp_methods_getNavData_param_timestamp = "timestamp";
  static const char* Ltp_methods_getNavData_param_latitude = "latitude";
  static const char* Ltp_methods_getNavData_param_longitude = "longitude";
  
  /**
   * Coordinates of a location. Coordinate's latitude and longitude are
   * represented in microdegrees (decimal degrees * 10^6). A negative
   * longitude means West (of Greenwich) and a negative latitude means South
   * (of the equator).
   */
  class WGS84Coordinates 
  {
  private:
      int latitude;
      int longitude;
      
  public:
  	int getLatitude() const
    {
  		return latitude;
  	}
  	
  	void setLatitude(int latitude) 
    {
  		this->latitude = latitude;
  	}
  	
  	int getLongitude() const
    {
  		return longitude;
  	}
  	
  	void setLongitude(int longitude) 
    {
  		this->longitude = longitude;
  	}
  };

  class TimestampedLocation 
  {
  private:
    // Timestamps are presented as strings using a restricted ISO8601 format: 
    // YYYY-MM-DDThh:mm:ss.SSSZ ('T' and 'Z' are literal.). 
    // EX: "1997-07-16T19:20:30.450+0100".
    // EX: "2014-02-19T00:01:07.000+0800"
    // Check http://www.w3.org/TR/NOTE-datetime, http://www.cl.cam.ac.uk/~mgk25/iso-time.html
    // Check http://www.cplusplus.com/reference/ctime/strftime/ about how to create this timestamp format.
    std::string timestamp;
    WGS84Coordinates coordinates;
    
  public:
  	std::string getTimestamp() const {
  		return timestamp;
  	}
  	
  	void setTimestamp(std::string timestamp) 
    {
  		this->timestamp = timestamp;
  	}
  	
  	WGS84Coordinates getCoordinates() const
    {
  		return coordinates;
  	}
  	
  	void setCoordinates(WGS84Coordinates coordinates) 
    {
  		this->coordinates = coordinates;
  	}

    static std::string GetLocalTimeStamp();
  };

  class TypedLocation 
  {
  private:
    TLocationType location_type;
    TimestampedLocation timestamped_location;

  public:
  	TLocationType getNavDataType() const
    {
  		return location_type;
  	}
  	
  	void setNavDataType(TLocationType locationType) 
    {
  		this->location_type = locationType;
  	}
  	
  	TimestampedLocation getNavData() const
    {
  		return timestamped_location;
  	}
  	
  	void setNavData(TimestampedLocation navData) 
    {
  		this->timestamped_location = navData;
  	}
      
  	void print() 
    {
  		printf("LocationType:\n");
      printf("\t %d\n", location_type);
      
  		printf("TimestampedLocation: \n");
      printf("\t getTimestamp:%s\n", timestamped_location.getTimestamp().c_str());
      printf("\t getLatitude:%d\n", timestamped_location.getCoordinates().getLatitude());
      printf("\t getLongitude:%d\n", timestamped_location.getCoordinates().getLongitude());
  	}
  };

  class CJsonTools 
  {
  private:
    
  public:
    static int QueringObjectMembersAndTypes(const rapidjson::Value& json);
    
  	static std::string JsonValueToString(rapidjson::Value& json);
    static void CreateJsonValue(std::vector<TLocationType>& locationTypes, rapidjson::Value& json);  // REQ for Ltp_methods_getNavData.
    static void CreateJsonValue(std::vector<TypedLocation>& typedLocations, rapidjson::Value& json);  // RESP for Ltp_methods_getNavData.
  };
  

  


#endif  // _LTPCOMMON_H_
