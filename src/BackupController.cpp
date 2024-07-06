#include "BackupController.h"
#include "BackupState.h"

int BackupController::performBackup(string path, char type)
{
    cout << endl << "Preparing backup" << endl;
    
    BackupState* backupState = new BackupState(path, type, Time::getCurrentEpochTime());
    
    int ret = backupState->findPathsToBackup();
    if (ret == -1) {
        backupState->endBackup("Failed");
        return -1;
    }
    else if (ret == 1) {
        backupState->endBackup("Completed");
        return 0;
    }
    
    ret = backupState->performBackup();
    if (ret == -1) {
        backupState->endBackup("Failed");
        return -1;
    }
    else if (ret == 1) {
        backupState->endBackup("Completed");
        return 0;
    }
    
    backupState->endBackup("Completed");
    
    return 0;
}

void BackupController::printAvailableBackups()
{
   BackupState* backupState = new BackupState();
   backupState->printAvailableJobs();
}

void BackupController::deleteJob(int jobId)
{
    BackupState* backupState = new BackupState();
    backupState->deleteJobAndPaths(jobId);
}

void BackupController::printPathsForJob(int jobId)
{
    BackupState* backupState = new BackupState();
    backupState->printPathsForJob(jobId);
}