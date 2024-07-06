#ifndef _RESTORECONTROLLER_H_
#define _RESTORECONTROLLER_H_

#include "StdNamespace.h"
#include "DBUtil.h"
#include <filesystem>

class RestoreController
{
    DBUtil* dbUtil;
public:
    RestoreController();
    
    int restoreBackup(int jobId, string destination);

};

#endif