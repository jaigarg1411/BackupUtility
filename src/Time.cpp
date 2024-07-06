#include "Time.h"

string Time::getCurrentEpochTime()
{
    chrono::time_point<chrono::system_clock> time = chrono::system_clock::now();
    time_t time_t = chrono::system_clock::to_time_t(time);
    return to_string(time_t);
}

bool Time::isTimeToBackup(const filesystem::path& filePath, const string& lastBackupTime)
{
    // Get the last write time of the file
    auto ftime = filesystem::last_write_time(filePath);

    // Convert to system_clock::time_point
    auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
                    ftime - filesystem::file_time_type::clock::now() + chrono::system_clock::now());

    // Convert to time_t
    auto epoch_time = chrono::system_clock::to_time_t(sctp);

    auto llLastWriteTime = static_cast<long long>(epoch_time);
    
    return llLastWriteTime > stoll(lastBackupTime);
}

string Time::getHumanReadableTime(const string& epochTime)
{
    time_t time_t = stoll(epochTime);
    tm* time = localtime(&time_t);
    
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", time);
    
    return string(buffer);
}