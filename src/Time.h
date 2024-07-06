#ifndef _TIME_H_
#define _TIME_H_

#include "StdNamespace.h"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <cstdio>

class Time
{
    std::chrono::time_point<std::chrono::system_clock> time;
    
    public:
        static string getCurrentEpochTime();
        static bool isTimeToBackup(const filesystem::path& filePath, const string& lastBackupTime);
        static string getHumanReadableTime(const string& epochTime);
};

#endif