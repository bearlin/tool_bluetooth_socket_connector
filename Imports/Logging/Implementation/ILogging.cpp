// ILogging.cpp
//

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include "ILogging.h"

// Namespaces
using namespace logging;
using namespace logging::internals;

// Namespace methods
//----------------------------------------
ILogControl& logging::IControl()
{
  return *(CLogImpl::Instance().GetLogControl());
}

ILogBase& logging::internals::ILog()
{
  return CLogImpl::Instance();
}
//----------------------------------------

// CLogImpl class methods
//----------------------------------------
CLogImpl::CLogImpl()
{
  iLogControl = new CLogControlImpl();
}

CLogImpl::~CLogImpl()
{
  delete iLogControl;
  iLogControl = NULL;
}

ILogControl* CLogImpl::GetLogControl()
{
  return iLogControl;
}

CLogImpl& CLogImpl::Instance()
{
  static CLogImpl logImpl;
  return logImpl;
}
//----------------------------------------

// CLogControlImpl class methods
//----------------------------------------
CLogControlImpl::CLogControlImpl()
  : iNextComponentIndex(0)
{
  // Do nothing in Logging.
  //printf("CLogControlImpl::CLogControlImpl()\n");

  iMapComponentNameToIndex.clear();
  iMapIndexToComponentName.clear();
}

CLogControlImpl::~CLogControlImpl()
{
  // Do nothing in Logging.
  //printf("CLogControlImpl::~CLogControlImpl()\n");

  iMapComponentNameToIndex.clear();
  iMapIndexToComponentName.clear();
}

void CLogControlImpl::RegisterComponent(const char* aStrComponent, TLogComponent& aComponentIndex)
{
  //printf("CLogControlImpl::RegisterComponent(%s, %d)\n", aStrComponent, iNextComponentIndex);

  std::map<std::string, TLogComponent>::iterator it = iMapComponentNameToIndex.find(aStrComponent);

  if (it != iMapComponentNameToIndex.end()) // Component already registered!
  {
    printf("CLogControlImpl::RegisterComponent() : Component already registered!\n");
    return;
  }

  iMapComponentNameToIndex.insert(std::pair<std::string, TLogComponent>(aStrComponent, iNextComponentIndex) );
  iMapIndexToComponentName.insert(std::pair<TLogComponent, std::string>(iNextComponentIndex, aStrComponent) );
  aComponentIndex = iNextComponentIndex;
  
  iNextComponentIndex++;
}

std::string CLogControlImpl::GetComponentName(TLogComponent& aComponentIndex)
{
  std::map<TLogComponent, std::string>::iterator it = iMapIndexToComponentName.find(aComponentIndex);

  if (it == iMapIndexToComponentName.end()) // Component doesn't registered!
  {
    //printf("Component doesn't registered!\n");
    return "";
  }
  
  return it->second;
}

//----------------------------------------

