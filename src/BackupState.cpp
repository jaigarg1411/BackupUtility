#include "BackupState.h"

BackupState::BackupState(int jobId) : jobId(jobId)
{
    this->dbUtil = new DBUtil();
    loadJobConfigFromDB(jobId);
}

BackupState::BackupState(string backupPath, char backupType, string startTime) : backupPath(backupPath), startTime(startTime)
{
    this->backupType = backupType == 'F' ? FULL : INCREMENTAL;
    this->dbUtil = new DBUtil();
    int lastJobId = this->dbUtil->getLastJobId();
    this->jobId = lastJobId == -1 ? 1 : lastJobId + 1;
}

BackupState::BackupState()
{
    this->dbUtil = new DBUtil();
}

void BackupState::loadJobConfigFromDB(int jobId)
{
    cout << "Loading job configuration from DB" << endl;
    
    // Load backup path
    this->backupPath = this->dbUtil->getBackupPath(jobId);
    
    // Load backup type
    char backupType = this->dbUtil->getJobType(jobId);
    this->backupType = backupType == 'F' ? FULL : INCREMENTAL;
    
    // Load start time
    this->startTime = this->dbUtil->getStartTime(jobId);
}

void BackupState::endBackup(string status)
{
    string strBackupType = backupType == FULL ? "Full" : "Inc";
    cout << endl << "Ending backup" << endl;
    cout << "Job ID: " << jobId << endl;
    cout << "Backup Path: " << backupPath << endl;
    cout << "Backup Type: " << strBackupType << endl;
    cout << "Status: " << status << endl;
    cout << endl;
    
    // Update job status
    this->dbUtil->insertJobEntry(jobId, strBackupType, startTime, Time::getCurrentEpochTime(), status, backupPath);
}

int BackupState::findPathsToBackup()
{
    cout << endl << "Finding paths to backup" << endl;
    
    if (!filesystem::exists(backupPath)) {
        cout << endl << "Backup path does not exist" << endl;
        return -1;
    }
    
    if (backupType == FULL)
    {
        for (const auto& entry : filesystem::recursive_directory_iterator(backupPath))
        {
            if (filesystem::is_regular_file(entry.path()))
            {
                pathsToBackup.push_back(entry.path());
            }
        }
    }
    else if (backupType == INCREMENTAL)
    {
        for (const auto& entry : filesystem::recursive_directory_iterator(backupPath))
        {
            string lastBackupTime = this->dbUtil->getLastBackupTimeFromPath(entry.path().string());
            lastBackupTime = lastBackupTime.empty() ? "0" : lastBackupTime;
            if (filesystem::is_regular_file(entry.path()) && Time::isTimeToBackup(entry.path(), lastBackupTime))
            {
                pathsToBackup.push_back(entry.path());
            }
        }
    }
    else
    {
        cout << endl << "Invalid backup type" << endl;
        return -1;
    }
    
    
    if (pathsToBackup.empty()) {
        cout << endl << "Nohting to backup." << endl;
        return 1;
    }
    
    return 0;
}

int BackupState::performBackup()
{
    cout << endl << "Performing backup" << endl;
    
    if (pathsToBackup.empty()) {
        cout << endl << "Nohting to backup." << endl;
        return 1;
    }
    
    string strBackupType = backupType == FULL ? "Full" : "Inc";
    string destinationDirName = to_string(jobId) + "_" + strBackupType + "_" + startTime;
    filesystem::path destinationDir = backupDirectory + "/" +  destinationDirName;
    
    if (!filesystem::create_directories(destinationDir)) 
    {
        cout << endl << "Failed to create destination directory" << endl;
        return -1;
    }
    
    for (auto& sourcePath : pathsToBackup) {
        // ensure source path exists
        if (!filesystem::exists(sourcePath)) {
            cout << endl << "Source path does not exist: " << sourcePath << endl;
            continue;
        }
        
        // Construct destination path
        filesystem::path relativePath = filesystem::relative(sourcePath, backupPath);
        filesystem::path destinationPath = destinationDir / relativePath;

        // Ensure directories leading to destination file exist
        if (!filesystem::exists(destinationPath.parent_path()) && !filesystem::create_directories(destinationPath.parent_path()))
        {
            cout << endl << "Failed to backup source path: " << sourcePath << endl;
            continue;
        }

        // Copy file from source to destination
        try {
            filesystem::copy(sourcePath, destinationPath, filesystem::copy_options::recursive);
            cout << endl << "Successfully backed up " << sourcePath << " to " << destinationPath << endl;
            
            this->dbUtil->insertPathEntry(jobId, sourcePath.string(), destinationPath.string(), strBackupType);
            
        } catch (const filesystem::filesystem_error& ex) {
            cout << endl << "Error copying " << sourcePath << " to " << destinationPath << ": " << ex.what() << endl;
            continue;
        }
    }
    
    return 0;
}

void BackupState::printAvailableJobs()
{
    vector<JobEntry> jobs = this->dbUtil->getAvailableJobs();
    
    if (jobs.empty())
    {
        cout << "No backups available" << endl;
        return;
    }
    
    int count = 1;
    cout << endl << "Following backups available: " << endl;
    cout << "-----------------------------" << endl;
    for(auto& job : jobs)
    {
        cout << count << ". Job ID: " << job.jobId << "  ";
        cout << "   Type: " << job.jobType << "  ";
        cout << "   Start Time: " << Time::getHumanReadableTime(job.startTime) << "  ";
        cout << "   End Time: " << Time::getHumanReadableTime(job.endTime) << "  ";
        cout << "   Status: " << job.status << "  ";
        cout << "   Path: " << job.path << "  ";
        cout << endl;
        count++;
    }
}

void BackupState::deleteJobAndPaths(int jobId)
{
    int ret = this->dbUtil->deleteJobAndPaths(jobId);
    if (ret == 0) {
        cout << "Job deleted successfully" << endl;
        
        // delete backup media for that job too
        auto it = find_if(filesystem::directory_iterator(backupDirectory), filesystem::directory_iterator(),
                           [&](const filesystem::directory_entry& entry) {
                               return filesystem::is_directory(entry) &&
                                      entry.path().filename().string().starts_with(to_string(jobId) + "_");
                           });
        if (it != filesystem::directory_iterator())
        {
            filesystem::remove_all(it->path());
        }
    }
    else {
        cout << "Failed to delete job" << endl;
    }
}

void BackupState::printPathsForJob(int jobId)
{
    vector<string> paths = this->dbUtil->getPathsForJob(jobId);
    
    if (paths.empty())
    {
        cout << "No paths backed up for job ID: " << jobId << endl;
        return;
    }
    
    int count = 1;
    cout << endl << "Following paths are backed up: " << endl;
    cout << "-----------------------------" << endl;
    for(auto& path : paths)
    {
        cout << count << ". " << path << endl;
        count++;
    }
}
