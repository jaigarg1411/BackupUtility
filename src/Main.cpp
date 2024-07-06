#include "StdNamespace.h"
#include "BackupController.h"
#include "RestoreController.h"

int main() {
    
    cout << endl;
    cout << "-------------------" << endl;
    cout << "| Backup Utility |" << endl;
    cout << "-------------------" << endl;
    cout << endl;
    
    int option;
    while(1)
    {
        cout << endl << "Select an option:" << endl;
        cout << "1. Backup" << endl;
        cout << "2. Browse all backups" << endl;
        cout << "3. Browse backup items for a job" << endl;
        cout << "4. Delete Backup" << endl;
        cout << "5. Restore data" << endl;
        cout << "0. Exit" << endl;
        cout << endl;
        cin >> option;
        if(option == 1)
        {
            cout << endl << "Enter path for backup: ";
            string path;
            cin >> path;
            
            char type;
            while(1)
            {
                cout << "Full [F] or Incremental [I] backup ? ";
                cin >> type;
                
                if (type != 'F' && type != 'I') {
                    cout << "Invalid backup type" << endl;
                    continue;
                }
                break;
            }
            
            BackupController* backupController = new BackupController();
            if (backupController->performBackup(path, type) == -1) {
                cout << endl << "Failed to perform backup" << endl;
            }
        }
        else if(option == 2)
        {
            BackupController* backupController = new BackupController();
            backupController->printAvailableBackups();
        }
        else if(option == 3)
        {
            int jobId;
            cout << endl << "Enter jobId : ";
            cin >> jobId;
            BackupController* backupController = new BackupController();
            backupController->printPathsForJob(jobId);
        }
        else if(option == 4)
        {
            int jobId;
            cout << endl << "Enter jobId : ";
            cin >> jobId;
            BackupController* backupController = new BackupController();
            backupController->deleteJob(jobId);
        }
        else if(option == 5)
        {
            int jobId;
            cout << endl << "Enter jobId : ";
            cin >> jobId;
            
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clearing the newline character from buffer
            
            string destination = "";
            cout << endl << "Enter destination path (leave empty for in-place restore): ";
            getline(cin, destination);
            RestoreController* restoreController = new RestoreController();
            restoreController->restoreBackup(jobId, destination);
        }
        else if(option == 0)
        {
            cout << "Exit" << endl;
            break;
        }
        else
        {
            cout << "Invalid option" << endl;
        }
    }
    return 0;
}