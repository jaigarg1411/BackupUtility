#ifndef _DBUTIL_H_
#define _DBUTIL_H_

#include "StdNamespace.h"
#include <sqlite3.h>
#include <vector>

struct JobEntry
{
    int jobId;
    string jobType;
    string startTime;
    string endTime;
    string status;
    string path;
};

class DBUtil
{
    sqlite3* db;
    char* errMsg;
    const string dbName = "../DB/BackupUtilityDB.db";
    
    public:
        DBUtil();
        ~DBUtil();
        
        void createPathsTable();
        void insertPathEntry(int jobId, string sourcePath, string destPath, string backupType);
        void createJobsTable();
        void insertJobEntry(int jobId, string jobType, string startTime, string endTime, string status, string path);
        string getJobStatus(int jobId);
        char getJobType(int jobId);
        string getBackupPath(int jobId);
        string getStartTime(int jobId);
        string getLastBackupTimeFromPath(string path);
        int getLastJobId();
        
        vector<JobEntry> getAvailableJobs();
        int deleteJobAndPaths(int jobId);
        vector<string> getPathsForJob(int jobId);
};

#endif