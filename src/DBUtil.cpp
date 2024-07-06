#include "DBUtil.h"
#include <filesystem>

DBUtil::DBUtil()
{
    if(sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    {
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }
    createJobsTable();
    createPathsTable();
}

DBUtil::~DBUtil()
{
    sqlite3_close(db);
}

void DBUtil::createPathsTable()
{
    string sql = "CREATE TABLE IF NOT EXISTS Paths ("
                 "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "JobId INTEGER NOT NULL,"
                 "SourcePath TEXT NOT NULL,"
                 "DestinationPath TEXT NOT NULL,"
                 "Type TEXT NOT NULL,"
                 "Created TEXT NOT NULL"
                 ");";
    
    int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
    if(rc != SQLITE_OK)
    {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void DBUtil::insertPathEntry(int jobId, string sourcePath, string destPath, string backupType)
{
    string sql = "INSERT INTO Paths (JobId, SourcePath, DestinationPath, Type, Created) VALUES ("
                 + to_string(jobId) + ", "
                 + "'" + sourcePath + "', "
                 + "'" + destPath + "', "
                 + "'" + backupType + "', "
                 + "strftime('%s', 'now')"
                 ");";
    
    int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
    if(rc != SQLITE_OK)
    {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void DBUtil::createJobsTable()
{
    string sql = "CREATE TABLE IF NOT EXISTS Jobs ("
                 "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "JobId INTEGER NOT NULL,"
                 "Type TEXT NOT NULL,"
                 "StartTime TEXT NOT NULL,"
                 "EndTime TEXT,"
                 "Status TEXT NOT NULL,"
                 "Path TEXT NOT NULL"
                 ");";
    if(sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg) != SQLITE_OK)
    {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void DBUtil::insertJobEntry(int jobId, string jobType, string startTime, string endTime, string status, string path)
{
    string sql = "INSERT INTO Jobs (JobId, Type, StartTime, EndTime, Status, Path) VALUES ("
                 + to_string(jobId) + ", "
                 + "'" + jobType + "', "
                 + "'" + startTime + "', "
                 + "'" + endTime + "', "
                 + "'" + status + "',"
                 + "'" + path + "'"
                 ");";
    if(sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg) != SQLITE_OK)
    {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

string DBUtil::getJobStatus(int jobId)
{
    string sql = "SELECT Status FROM Jobs WHERE JobId = " + to_string(jobId) + ";";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        string status = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);
        return status;
    }
    else
    {
        sqlite3_finalize(stmt);
        return "";
    }
}

string DBUtil::getBackupPath(int jobId)
{
    string sql = "SELECT Path FROM Jobs WHERE JobId = " + to_string(jobId) + ";";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        string path = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);
        return path;
    }
    else
    {
        sqlite3_finalize(stmt);
        return "";
    }
}

string DBUtil::getStartTime(int jobId)
{
    string sql = "SELECT StartTime FROM Jobs WHERE JobId = " + to_string(jobId) + ";";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        string startTime = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);
        return startTime;
    }
    else
    {
        sqlite3_finalize(stmt);
        return "";
    }
}

char DBUtil::getJobType(int jobId)
{
    string sql = "SELECT Type FROM Jobs WHERE JobId = " + to_string(jobId) + ";";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return ' ';
    }
    
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        char jobType = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))[0];
        sqlite3_finalize(stmt);
        return jobType;
    }
    else
    {
        sqlite3_finalize(stmt);
        return ' ';
    }
}

string DBUtil::getLastBackupTimeFromPath(string path)
{
    string sql = "SELECT Created FROM Paths WHERE SourcePath = '" + path + "' ORDER BY Created DESC LIMIT 1;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        string lastBackupTime = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);
        return lastBackupTime;
    }
    else
    {
        sqlite3_finalize(stmt);
        return "";
    }
}

int DBUtil::getLastJobId()
{
    string sql = "SELECT JobId FROM Jobs ORDER BY JobId DESC LIMIT 1;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return -1;
    }
    
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int jobId = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return jobId;
    }
    else
    {
        sqlite3_finalize(stmt);
        return -1;
    }
}

vector<JobEntry> DBUtil::getAvailableJobs()
{
    vector<JobEntry> jobs;
    string sql = "SELECT JobId, Type, StartTime, EndTime, Status, Path FROM Jobs;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return jobs;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        JobEntry job;
        job.jobId = sqlite3_column_int(stmt, 0);  // JobId
        job.jobType = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));  // Type
        job.startTime = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));  // StartTime
        job.endTime = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));  // EndTime
        job.status = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));  // Status
        job.path = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));  // Path
        jobs.push_back(job);
    }

    sqlite3_finalize(stmt);
    return jobs;
}

int DBUtil::deleteJobAndPaths(int jobId)
{
    string sql = "DELETE FROM Jobs WHERE JobId = " + to_string(jobId) + ";";
    if (sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg) != SQLITE_OK)
    {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return -1;
    }
    
    //check if job does not exist
    if (sqlite3_changes(db) == 0)
    {
        cout << "Job does not exist" << endl;
        return -1;
    }

    sql = "DELETE FROM Paths WHERE JobId = " + to_string(jobId) + ";";
    if (sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg) != SQLITE_OK)
    {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return -1;
    }

    return 0;
}

vector<string> DBUtil::getPathsForJob(int jobId)
{
    vector<string> paths;
    string sql = "SELECT SourcePath FROM Paths WHERE JobId = " + to_string(jobId) + ";";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        return paths;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        string path = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        paths.push_back(path);
    }

    sqlite3_finalize(stmt);
    return paths;
}
