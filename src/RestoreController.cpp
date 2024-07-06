#include "RestoreController.h"

RestoreController::RestoreController()
{
    dbUtil = new DBUtil();
}

int RestoreController::restoreBackup(int jobId, string destination = "")
{
    string backupPath = dbUtil->getBackupPath(jobId);
    string backupType = dbUtil->getJobType(jobId);
    string startTime = dbUtil->getStartTime(jobId);
    
    string mediaPath = "/home/junix/Media/" + to_string(jobId) + "_" + backupType + "_" + startTime;
    
    if (!filesystem::exists(mediaPath)) {
        cout << "Backup media not found. Cannot peform restore." << endl;
        return -1;
    }
    
    destination = destination == "" ? backupPath : destination;
    filesystem::create_directories(destination); // Create destination directory if it does not exist
    
    for (const auto& entry : filesystem::recursive_directory_iterator(mediaPath)) {
        const auto& path = entry.path();
        auto relativePath = filesystem::relative(path, mediaPath);
        filesystem::path destPath = destination / relativePath;

        if (filesystem::is_directory(path)) {
            // Create directories in the destination path
            if (!filesystem::exists(destPath)) {
                filesystem::create_directory(destPath);
            }
        } else if (filesystem::is_regular_file(path)) {
            // Copy files to the destination path
            filesystem::copy_file(path, destPath, filesystem::copy_options::overwrite_existing);
        }
        cout << "Restored: " << relativePath << endl;
    }
    
    return 0;
}