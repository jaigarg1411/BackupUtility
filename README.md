# BackupUtility

BackupUtility is a simple backup utility program designed for Unix filesystems.

It supports the following kinds of backups:

- **Full Backup:** Complete backup of files at a specified path, including sub-directories.
  
- **Incremental Backup:** Backup of files that have been modified since the last backup.

## Scope

- Uses SQLite DB for storing backup information.
  
- Capabilities:
  - Take full backups of a directory.
  - Perform incremental backups of a directory.
  - Display all backup jobs with details.
  - Display file paths backed up for a specific jobId.
  - Supports deletion of backup jobs (including associated data).

## Usage

### Building and Running BackupUtility

To build and run BackupUtility, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/jaigarg1411/BackupUtility.git
   cd BackupUtility

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   
3. Configure the build with cmake:
   ```bash
   cmake ..
   
4. Build the executable:
   ```bash
   make
   
5. Run backup utility:
   ```bash
   ./BackupUtility