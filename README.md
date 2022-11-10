# Bit-Torrent: Peer-to-Peer File Sharing System

### Prerequisites
Socket Programming, SHA1 hash, Multi-threading in C++
### Goal
In this project, I build a group based file sharing system where users can share, download files from the group they belong to and files are downloaded parallely in multiple pieces from multiple peers.
### Approach:
- <b>File:</b> The file into logical pieces, where the size of each piece is 512KB.
     - Suppose the file size is 1024KB, then it is divide into two pieces of 512KB each
- <b>Data Integrity:</b> After downloading the complete file, the SHA1 value of the file is calculated and then is checked with the original SHA1 value available on the tracker side to verify the integrity of the file.
- <b>Authentication:</b> Every user should have an account inorder to download or share the files.

### Architecture Overview:
The Following entities will be present in the network:
1. Server/Tracker :
     Maintain information of clients with their files(shared by client) to assist the clients for the communication between peers<br/>
2. Clients:
     - User should create an account and register with tracker
     - Login using the user credentials
     - Create Group and hence will become owner of that group<
     - Fetch list of all Groups in server
     - Request to Join Group
     - Leave Group
     - Accept Group join requests (if owner)
     - Share file across group: Share the filename and SHA1 hash of the complete file with the tracker
     - Fetch list of all sharable files in a Group
     - Download file
          - Retrieve peer information from tracker for the file
          - Core Part: Download file from multiple peers (different pieces of file from different peers using piece selection algorithm) simultaneously and all the files which client downloads will be shareable to other users in the same group
     - Show downloads
     - Stop sharing file
     - Stop sharing all files(Logout)
     - Whenever client logins, all previously shared files before logout should automatically be on sharing mode

### How to compile project
1. go to ```client.cpp```
   * ```g++ client.cpp -o client -lssl -lcrypto -lpthread```
2. go to ```server.cpp```
   * ```g++ server.cpp -o server -lpthread```

### How to Run project
#### To run the Server
```
./server <my_server_ip> <my_server_port>
eg : ./server 127.0.0.1 5000
```
#### To run the Client
```
./client <CLIENT_IP> <CLIENT_PORT> <SERVER_IP> <SERVER_PORT>
```
* creating client1 on new terminal with socket : 127.0.0.1:6000 <br/>
eg : ```./client 127.0.0.1 6000 127.0.0.1 5000```

* creating client2 on another terminal with socket : 127.0.0.1:7000 <br/>
eg : ```./client 127.0.0.1 7000 127.0.0.1 5000```

#### Commands available for clients 
 **1. Create User Account :** 
 ```
 create_user <user_id> <passwd>
 ```
 **2. Login :**
 ```
 login <user_id> <passwd>
 ```
 **3. Create Group  :**
 ```
 create_group <group_id>
 ```
 **4. Join Group :**
 ```
 join_group <group_id>
 ```
 **5. Leave Group  :**
 ```
 leave_group <group_id>
 ```
 **6. List Pending Request :**
 ```
 requests list_requests <group_id>
 ```
 **7. Accept Group Joining Request :**
 ```
 accept_request <group_id> <user_id>
 ```
 **8. List All Group In Network :**
 ```
 list_groups
 ```
 **9. List All sharable Files In Group :**
 ```
 list_files <group_id>
 ```
 **10. Upload File :**
 ```
 upload_file <file_path> <group_id>
 eg.: upload_file /home/rishabh/F/video.mp4 my_grp
 ```
 **11. Download File :**
 ```
 download_file <group_id> <file_name> <destination_path>
 eg.: download_file my_grp video.mp4 /home/rishabh/D/
 ```
 **12. Logout :**
 ```
 logout
 ```
 **13. Show_downloads :**
 ```
 show_downloads
 ```
 <b>Output format:</b> </br>
<i>[D] [grp_id] filename </br>
[C] [grp_id] filename </i></br>
D(Downloading), C(Complete)

**14. Stop sharing :**
```
stop_share <group_id> <file_name>
```
 
#### Assumption
* Enter absolute path for files.
