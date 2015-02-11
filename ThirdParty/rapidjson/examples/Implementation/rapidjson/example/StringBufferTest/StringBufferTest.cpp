// StringBuffer example from https://code.google.com/p/rapidjson/issues/detail?id=34
// This example shows how to creating JSON string to StringBuffer(not FileStream) directly.

#include "StringBufferTest.h"		
#include <cstdio>
#include <string>
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output
#include "rapidjson/stringbuffer.h" // https://code.google.com/p/rapidjson/issues/detail?id=34
#include "rapidjson/document.h"		// rapidjson's DOM-style API

using namespace rapidjson;

int ToJsonTest()
{
/*
[
    {
        "name": "Milo YIP",
        "age": 34,
        "married": true,
        "dependents": [
            {
                "name": "Lua YIP",
                "age": 3,
                "education": {
                    "school": "Happy Kindergarten",
                    "GPA": 3.5
                }
            },
            {
                "name": "Mio YIP",
                "age": 1,
                "education": null
            }
        ]
    },
    {
        "name": "Percy TSE",
        "age": 30,
        "married": false,
        "dependents": []
    }
]
*/

  // Create Writer to StringBuffer.
  /* JSON writer
   Writer implements the concept Handler.
    It generates JSON text by events to an output stream.

    User may programmatically calls the functions of a writer to generate JSON text.

    On the other side, a writer can also be passed to objects that generates events, 

    for example Reader::Parse() and Document::Accept().
  */
	StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
  //PrettyWriter<StringBuffer> writer(strbuf);
  
  // Create JSON string to this Writer.
  writer.StartArray();
    writer.StartObject();
      writer.String("name");              writer.String("Milo YIP");
      writer.String("age");               writer.Uint(34);
      writer.String("married");           writer.Bool(1);
      writer.String("dependents");
        writer.StartArray();
          writer.StartObject();
            writer.String("name");        writer.String("Lua YIP");
            writer.String("age");         writer.Uint(3);
            writer.String("education");
              writer.StartObject();
                writer.String("school");  writer.String("Happy Kindergarten");
                writer.String("GPA");     writer.Double(3.5);
              writer.EndObject();
          writer.EndObject();
          writer.StartObject();
            writer.String("name");        writer.String("Mio YIP");
            writer.String("age");         writer.Uint(1);
            writer.String("education");   writer.Null();
          writer.EndObject();
        writer.EndArray();
    writer.EndObject();
    
    writer.StartObject();
      writer.String("name");              writer.String("Percy TSE");
      writer.String("age");               writer.Uint(30);
      writer.String("married");           writer.Bool(0);
      writer.String("dependents");
        writer.StartArray();
        writer.EndArray();
    writer.EndObject();
  writer.EndArray();

  printf("-- The condense json from Writer<StringBuffer>:\n%s\n--\n", strbuf.GetString());

  // Convert strbuf back to Document.
  printf("Converting strbuf back to rapidjson Document... \nWe want to write this new document pretty...\n");
  Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (document.Parse<0>(strbuf.GetString()).HasParseError())// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
		return 1;
  
  // Document.Accept PrettyWriter<FileStream>.
  printf("-- The pretty json from PrettyWriter<FileStream>:\n");
	FileStream so(stdout);
	PrettyWriter<FileStream> prettywriter(so);
	document.Accept(prettywriter);	// Accept() traverses the DOM and generates Handler events.
  printf("\n--\n");

  // Document.Accept PrettyWriter<StringBuffer>.
  //printf("-- The pretty json from PrettyWriter<StringBuffer>:\n");
	//StringBuffer strbufPretty;
	//PrettyWriter<StringBuffer> BufPrettyWriter(strbufPretty);
  //document.Accept(BufPrettyWriter);
  //printf("\n--\n");
  
  return 1;
}

int CreateRespJsonTest()
{
/*[RESP JSON string]
{
    "ptl_ver": {
        "major": 1,
        "minor": 1
    },
    "msg_type": "RESP",
    "msg_id": 80,
    "method": "LKCP",
    "resp_code": 0,
    "parameters": {
        "LKCP": {
            "latitude": "25048130",
            "longitude": "121517070"
        },
        "DEST": {
            "latitude": "25048130",
            "longitude": "121517070"
        }
    }
}
*/

  // Create Writer to StringBuffer.
  StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
  
  // Create RESP json string to this Writer.
  writer.StartObject();
    writer.String("ptl_ver");
      writer.StartObject();
        writer.String("major");         writer.Uint(1);
        writer.String("minor");         writer.Uint(1);
      writer.EndObject();
    writer.String("msg_type");          writer.String("RESP");
    writer.String("msg_id");            writer.Uint(80);
    writer.String("method");            writer.String("LKCP");
    writer.String("resp_code");         writer.Uint(0);
    writer.String("parameters");
      writer.StartObject();
        writer.String("LKCP");
          writer.StartObject();
            writer.String("latitude");  writer.String("25048130");
            writer.String("longitude"); writer.String("121517070");
          writer.EndObject();
          writer.String("DEST");
          writer.StartObject();
            writer.String("latitude");  writer.String("25048130");
            writer.String("longitude"); writer.String("121517070");
          writer.EndObject();
      writer.EndObject();
  writer.EndObject();

  printf("-- The condense json from Writer<StringBuffer>:\n%s\n--\n", strbuf.GetString());
  
  // Convert strbuf back to Document.
  printf("Converting strbuf back to rapidjson Document... \nWe want to write this new document pretty...\n");
  Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (document.Parse<0>(strbuf.GetString()).HasParseError())// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
		return 1;
  
  // Document.Accept PrettyWriter<FileStream>.
  printf("-- The pretty json from PrettyWriter<FileStream>:\n");
	FileStream so(stdout);
	PrettyWriter<FileStream> prettywriter(so);
	document.Accept(prettywriter);	// Accept() traverses the DOM and generates Handler events.
  printf("\n--\n");
  
  return 1;
}

int main(int, char*[]) 
{
  ToJsonTest();
  CreateRespJsonTest();
  
  // Test Create ErrorReplyMessage
  std::string message;
  int respCode;
  
  // Test for RESP Err.
  respCode = 1;
  ResponseMessage* responseMessage = new ResponseMessage();
  CVersion* ptlVer = new CVersion(1, 1);
  responseMessage->setPtlVer(ptlVer);
  responseMessage->setMsgType("RESP");
  responseMessage->setMsgId(0);
  responseMessage->setRespCode(respCode);
  responseMessage->setMethod("");
  message = responseMessage->ToJson();
  printf("RESP Err responseMessage->ToJson : %s\n", message.c_str());
  //connection.sendMessage(instance, message);
  
  // Test for RESP normal(with parameter lists).
  respCode = 0;
  responseMessage->setPtlVer(ptlVer);
  responseMessage->setMsgType("RESP");
  responseMessage->setMsgId(1);
  responseMessage->setRespCode(respCode);
  responseMessage->setMethod("LKCP");
  std::vector<std::string>* param = responseMessage->getParameters();
  param->push_back("latitude");
  param->push_back("25048130");
  param->push_back("longitude");
  param->push_back("121517070");
  message = responseMessage->ToJson();
  printf("RESP normal(with parameter lists) responseMessage->ToJson : %s\n", message.c_str());
  
	return 0;
}
