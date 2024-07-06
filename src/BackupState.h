#ifndef _BACKUPSTATE_H_
#define _BACKUPSTATE_H_

#include "StdNamespace.h"
#include <vector>
#include <filesystem>
#include <algorithm>
#include "DBUtil.h"
#include "Time.h"

enum BackupType
{
    FULL,
    INCREMENTAL
};

class BackupState
{
    int jobId;
    string backupPath;
    BackupType backupType;
    vector<filesystem::path> pathsToBackup;
    DBUtil* dbUtil;
    const string backupDirectory = "/home/junix/Media";
    string startTime;
    string endTime;
    
    public:
        BackupState(int jobId);
        BackupState(string backupPath, char backupType, string startTime);
        BackupState();
        
        int findPathsToBackup();
        int performBackup();
        
        void loadJobConfigFromDB(int jobId);
        
        void endBackup(string status);
        
        void printAvailableJobs();
        void deleteJobAndPaths(int jobId);
        void printPathsForJob(int jobId);
};

#endif