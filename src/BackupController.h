#ifndef _BACKUPCONTROLLER_H_
#define _BACKUPCONTROLLER_H_

#include "StdNamespace.h"

class BackupController
{
public:
    int performBackup(string path, char type);
    void printAvailableBackups();
    
    void deleteJob(int jobId);
    void printPathsForJob(int jobId);
};

#endif
