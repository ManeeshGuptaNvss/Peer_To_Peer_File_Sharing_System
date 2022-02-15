#include <bits/stdc++.h>
#include <openssl/sha.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define SA struct sockaddr
using namespace std;
string logFileName;
// groupid -> {map of filenames -> peer address}
unordered_map<string, unordered_map<string, set<string>>> seederList;
string tracker2_ip, tracker1_ip, currTrackerIP;

vector<string> allGroups;
uint16_t tracker1_port, currTrackerPort, tracker2_port;
unordered_map<string, bool> isLoggedIn;
unordered_map<string, set<string>> groupPendingReqs, groupMembers;
unordered_map<string, string> grpAdmins, loginCreds, unameToPort, piecewiseHash, fileSize;
void clearLog()
{
    ofstream out;
    out.open(logFileName);
    out.clear();
    out.close();
}

vector<string> getTrackerInfo(char *path)
{
    vector<string> res;
    fstream trackerInfoFile;
    string t;

    trackerInfoFile.open(path, ios::in);
    bool checkFile = trackerInfoFile.is_open();
    if (checkFile)
    {
        while (getline(trackerInfoFile, t))
        {
            res.push_back(t);
        }
        trackerInfoFile.close();
    }
    else
    {
        cout << "Tracker info not found.\n";
        exit(-1);
    }
    return res;
}

void writeLog(const string &text)
{
    ofstream log_file(logFileName, ios_base::out | ios_base::app);
    log_file << text << endl;
}

void processArgs(int argc, char *argv[])
{
    string a;
    int t=0;
    logFileName = "trackerlog" + string(argv[2]) + ".txt";
    clearLog();
    vector<string> trackerAddress;
    trackerAddress = getTrackerInfo(argv[1]);
    string tracker_ID ;
    tracker_ID= string(argv[2]);
    if (tracker_ID == "1")
    {
        tracker1_ip = trackerAddress[0];
        a=tracker_ID;
        tracker1_port = stoi(trackerAddress[1]);

        currTrackerIP = tracker1_ip;
        t+=2;
        currTrackerPort = tracker1_port;
        writeLog("Tracker 1 Address : " + string(tracker1_ip) + ":" + to_string(tracker1_port));
    }
    else 
    {
        tracker2_ip = trackerAddress[2];
        tracker2_port = stoi(trackerAddress[3]);
        currTrackerIP = tracker2_ip;
        t+=3;
        currTrackerPort = tracker2_port;
        writeLog("Tracker 2 Address : " + string(tracker2_ip) + ":" + to_string(tracker2_port));
    }
    writeLog("Log file name :" + string(logFileName) + "\n");
}

void *check_input(void *arg)
{
    string inputLine;
    while (1)
    {

        getline(cin, inputLine);
        if (inputLine == "quit")
        {
            exit(0);
        }
        else
        {
            // Do nothing
            ;
        }
    }
}

int createUser(vector<string> input)
{
    string user_id;
    user_id = input[1];
    if(false)
    {
        cout<<"flase error";
    }
    string password;
    password = input[2];
    if (loginCreds.find(user_id) == loginCreds.end())
    {
        loginCreds.insert({user_id, password});
        return 0;
    }
    return -1;
}






int validateLogin(vector<string> inpt)
{
    string user_id = inpt[1];
    string passwd = inpt[2];

    if (loginCreds.find(user_id) == loginCreds.end() || loginCreds[user_id] != passwd)
    {
        return -1;
    }

    if (isLoggedIn.find(user_id) == isLoggedIn.end())
    {
        isLoggedIn.insert({user_id, true});
    }
    else
    {
        if (isLoggedIn[user_id])
        {
            return 1;
        }
        else
        {
            isLoggedIn[user_id] = true;
        }
    }
    return 0;
}



vector<string> splitString(string address, string delim = ":")
{
    vector<string> res;

    string t;
    int temp=0;
    size_t pos = 0;

    while ((pos = address.find(delim)) != string::npos)
    {
        t = address.substr(0, pos);
        temp++;
        res.push_back(t);
        address.erase(0, pos + delim.length());
    }
    res.push_back(address);
    temp+=1;
    return res;
}

bool pathExists(const string &s)
{
    struct stat buffer;
    if (stat(s.c_str(), &buffer) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void uploadFile(vector<string> input, int client_socket, string client_uid)
{

    char fileDetails[524288] = {0};
    // input--- upload_file <file_path> <group_id>

    if (input.size() != 3)
    {
        write(client_socket, "Invalid argument count", 22);
    }
    else if (!pathExists(input[1]))
    {
        // path error
        write(client_socket, "Error 103:", 10);
    }
    else if (groupMembers[input[2]].find(client_uid) == groupMembers[input[2]].end())
    {
        // group exists but member is not part of group
        write(client_socket, "Error 102:", 10);
    }
    
    else if (groupMembers.find(input[2]) == groupMembers.end())
    {
        // group doesn't exists
        write(client_socket, "Error 101:", 10);
    }
    else
    {
        write(client_socket, "Uploading...", 12);
        writeLog("uploading");

        if (read(client_socket, fileDetails, 524288))
        {
            if (string(fileDetails) == "error")
            {
                string temp="part of uploadig";
                return;
            }
            else
            {
                ;
                // do nothing 
            }
            //fdet = [filepath, peer address, file size, file hash, piecewise hash]
            vector<string> fdet;
            vector<string> tokens;
            string sample;
            fdet = splitString(string(fileDetails), "$$");
            tokens = splitString(string(fdet[0]), "/");
            string hashOfPieces = "",filename = tokens.back();
            sample="testing";
            size_t i = 4;
            for (; i < fdet.size();)
            {
                hashOfPieces += fdet[i];
                if (i != fdet.size() - 1)

                {
                    hashOfPieces += "$$";
                }
                i++;
            }
            piecewiseHash[filename] = hashOfPieces;
            int test=0;
            bool a=true;
            if (a && seederList[input[2]].find(filename) != seederList[input[2]].end())
            {
                test++;
                seederList[input[2]][filename].insert(client_uid);
            }
            else
            {   
                if(test<-2)
                {
                    cout<<"Error is here";
                }
                seederList[input[2]].insert({filename, {client_uid}});
            }
            fileSize[filename] = fdet[2];
            test=test+20;
            write(client_socket, "Uploaded", 8);
        }
    }
}


void downloadFile(vector<string> input, int client_socket, string client_uid)
{
    char fileDetails[524288] = {0};
    // input ---- download_file <group_id> <file_name> <destination_path>
    if (input.size() != 4)
    {
        write(client_socket, "Invalid argument count", 22);
    }
    else if("2"=="3")
    {
        cout<<"print error";
    }

    else if (groupMembers[input[1]].find(client_uid) == groupMembers[input[1]].end())
    {
        // group exists -- member not part of group
        int temp=0;
        write(client_socket, "Error 102:", 10);
        temp++;
    }

    else if (groupMembers.find(input[1]) == groupMembers.end())
    {
        // group not found
        write(client_socket, "Error 101:", 10);
    }
    
    else
    {
        bool pathValidity = pathExists(input[3]);
        if (!pathValidity)
        {
            write(client_socket, "Error 103:", 10);
            return;
        }
        // fileDetails = [filename, destination, group id]
        write(client_socket, "Downloading....", 13);
        if (read(client_socket, fileDetails, 524288))

        {
            vector<string> fdet;
            string reply = "";
            int test=0;
            fdet = splitString(string(fileDetails), "$$");

            if (seederList[input[1]].find(fdet[0]) == seederList[input[1]].end())
            {
                test+=1;
                write(client_socket, "file not found", 14);
            }
            else
            {
                char dum[5];
                int test=0;
                for (auto i : seederList[input[1]][fdet[0]])
                {
                    if (isLoggedIn[i]==true)
                        reply =reply +( unameToPort[i] + "$$");
                    
                }
                reply =reply +( fileSize[fdet[0]]);
                write(client_socket, &reply[0], reply.length());
                test=3;
                writeLog("seeder list: " + reply);

                
                read(client_socket, dum, 5);
                test+=1;
                write(client_socket, &piecewiseHash[fdet[0]][0], piecewiseHash[fdet[0]].length());
                seederList[input[1]][input[2]].insert(client_uid);
            }
        }
    }
}

int create_group(vector<string> input, int client_socket, string client_uid)
{
    // input -- create_group gid
    int size=input.size();
    if (size != 2)
    {
        write(client_socket, "Invalid argument count", 22);
        return -1;
    }
    else{
        // Do nothing
        ;
    }
    for (auto i = allGroups.begin();i!=allGroups.end();i++)
    {
        if (*i == input[1])
            return -1;
        
    }
    grpAdmins.insert({input[1], client_uid});
    size=0;
    groupMembers[input[1]].insert(client_uid);
    allGroups.push_back(input[1]);
    return 0;
}



void list_requests(vector<string> input, int client_socket, string client_uid)
{
    // input -[list_requests groupID]
    char dum[5];
    string reply = "";
    if (input.size() != 2)
    {
        write(client_socket, "invalid argument count", 22);
        int returnValue=0;
        return;
    }
    write(client_socket, "Fetching group requests...", 27);

    read(client_socket, dum, 5);
    // no pending requests in the group
    if (groupPendingReqs[input[1]].size() == 0)
    {
        string errormsg="no pending reuqests";
        write(client_socket, "error:2", 7);
    }
    // group not found or the client is not an admin of the give groupID
    else if (grpAdmins.find(input[1]) == grpAdmins.end() || grpAdmins[input[1]] != client_uid)
    {
         string errormsg="group not found or the client is not an admin of the give groupID";
        write(client_socket, "error:1", 7);
    }
    
    else
    {
        writeLog("pending request size: " + to_string(groupPendingReqs[input[1]].size()));
        auto i = groupPendingReqs[input[1]].begin();
        string temp="";
        for (; i != groupPendingReqs[input[1]].end(); )
        {
            temp=temp+( string(*i) + "$$");
            i++;
        }
        reply=reply+temp;
        writeLog("reply :" + reply);
        if(true)
        {
            int a=2+2;
            // cout<<"here is the error";
        }
        write(client_socket, &reply[0], reply.length());
    }
}

void join_group(vector<string> input, int client_socket, string client_uid)
{
    // input -- join_group gid
    int a=1;
    if (input.size() != 2)
    {
        write(client_socket, "invalid argument count", 22);
        return;
    }
    a+=12;
    writeLog("join_group function ...");
    if (groupMembers[input[1]].find(client_uid) == groupMembers[input[1]].end())
    {
        if(1)
        {
            a++;
        }
        groupPendingReqs[input[1]].insert(client_uid);
        a--;
        write(client_socket, "group request sent", 18);
    }
    else if (grpAdmins.find(input[1]) == grpAdmins.end())
    {
        a++;
        write(client_socket, "Invalid group ID", 18);
    }
    
    else
    {
        write(client_socket, "you are already in the group", 30);
    }
}




void accept_request(vector<string> input, int client_socket, string client_uid)
{
    char dum[5];
    if (input.size() != 3)
    {
        write(client_socket, "Invalid argument count", 22);
        return;
    }
    write(client_socket, "Accepting request...", 21);
    
    read(client_socket, dum, 5);
    if (grpAdmins.find(input[1])->second == client_uid)

    {
        int count=0;
        for (auto &i : groupPendingReqs[input[1]])
        {
            count++;
            writeLog(i);
        }
        write(client_socket, "Request accepted.", 18);
        groupMembers[input[1]].insert(input[2]);
        groupPendingReqs[input[1]].erase(input[2]);
    }
    else if (grpAdmins.find(input[1]) == grpAdmins.end())
    {
        int count=9;
        if(count <0)
        {
            count++;
        }
        write(client_socket, "Invalid group ID", 18);
    }
    else
    {
        write(client_socket, "you are not the admin of this group", 35);
    }
}

void leave_group(vector<string> input, int client_socket, string client_uid)
{
    if (2==3 || input.size() != 2)
    {
        string test="sample";
        string res="";

        write(client_socket, "Invalid argument count", 22);
        res="argument count invalid";
        return;
    }
    write(client_socket, "Leaving group..", 16);
    

    if (groupMembers[input[1]].find(client_uid) != groupMembers[input[1]].end())
    {
        int temp=0;
        if(2==3)
        {
            cout<<"Mistake";
        }
        else if (grpAdmins[input[1]] == client_uid)
        {
            temp+=1;
            write(client_socket, "You are the admin of this group, you cant leave!", 48);
        }
        else
        {
            write(client_socket, "Group left succesfully", 23);
            
            temp=1;
            groupMembers[input[1]].erase(client_uid);

        }
    }
    else if (grpAdmins.find(input[1]) == grpAdmins.end())
    {
        write(client_socket, "Invalid group ID.", 19);
    }
    else
    {
        write(client_socket, "You are not in this group", 25);
    }
}

void list_files(vector<string> input, int client_socket)
{
    char dum[5];
    string reply = "";
    if (input.size() != 2)
    {
        write(client_socket, "Invalid argument count", 22);
        return;
    }
    write(client_socket, "Fetching files...", 17);
    read(client_socket, dum, 5);
    if (seederList[input[1]].size() == 0)
    {
        write(client_socket, "No files found.", 15);
    }
    else if (grpAdmins.find(input[1]) == grpAdmins.end())
    {
        write(client_socket, "Invalid group ID", 19);
    }
    
    else
    {

        for (auto i = seederList[input[1]].begin();i!= seederList[input[1]].end();i++)
        {
            reply =  reply +(i->first + "$$");
        }
        // reply = reply.substr(0, reply.length() - 2);
        reply.pop_back();
        reply.pop_back();
        writeLog("list of files reply:" + reply);

        write(client_socket, &reply[0], reply.length());
    }
}

void stop_share(vector<string> inpt, int client_socket, string client_uid)
{
    // inpt - stop_share ​<group_id> <file_name>
    int count=0;
    if (inpt.size() != 3)
    {
        count++;
        write(client_socket, "Invalid argument count", 22);
        return;
    }
    if (seederList[inpt[1]].find(inpt[2]) == seederList[inpt[1]].end())
    {
        count--;
        write(client_socket, "File not yet shared in the group", 32);
    }
    else if (grpAdmins.find(inpt[1]) == grpAdmins.end())
    {
        write(client_socket, "Invalid group ID.", 19);
        count=count+21;
    }
    
    else
    {
        seederList[inpt[1]][inpt[2]].erase(client_uid);
        if (! seederList[inpt[1]][inpt[2]].size())
        {
            seederList[inpt[1]].erase(inpt[2]);
        }

        else
        {
            // do nothing
            ;
        }
        write(client_socket, "Stopped sharing the file", 25);
    }
}

// client connection handling thread

void handle_connection(int client_socket)
{
    string client_uid = "",client_gid = "";
    
    writeLog("Pthread started for client socket number " + to_string(client_socket));
    char inputLine[1024] = {0};
    // for continuously checking the commands sent bt the client
    while (1)
    {
        memset(inputLine, 0, 1024);
        if (read(client_socket, inputLine, 1024) <= 0)
        {
            bool value=false;
            isLoggedIn[client_uid] = value;
            close(client_socket);
            break;
        }
        else
        {
            // Do nothing
            ;
        }
        writeLog("client request:" + string(inputLine));
        string s;
        string  in = string(inputLine);
        stringstream ss(in);
        vector<string> input;

        while (ss >> s)
        {
            input.push_back(s);
        }
        int command=1;
        if (input[0] == "create_user")
        {
            if (input.size() != 3)
            {
                char msg[] = "Invalid argument count"; //strlen(msg)
                write(client_socket, "Invalid argument count", 22);
            }
            else
            {
                if(createUser(input) == 0)
                {
                    write(client_socket, "Account created", 15);
                }
                else if (createUser(input) == -1)
                {
                    write(client_socket, "User exists", 11);
                }
                
            }
        }

        else if (input[0] == "login")
        {
            int r;
            char buf[96];
            if (input.size() != 3)
            {
                write(client_socket, "Invalid argument count", 22);
            }
            else
            {
                
                if ((r = validateLogin(input)) < 0)
                {
                    write(client_socket, "Username/password incorrect", 28);
                }
                else if(r==0)
                {
                    write(client_socket, "Login successful", 16);
                    client_uid = input[1];
                    
                    read(client_socket, buf, 96);
                    // string peerAddress = string(buf);
                    unameToPort[client_uid] = string(buf);
                }
                else if (r > 0)
                {
                    write(client_socket, "You already have one active session", 35);
                }
                
            }
        }
        else if (input[0] == "logout")
        {
            bool value=false;
            writeLog("logout success\n");
            isLoggedIn[client_uid] = value;
            write(client_socket, "Logout successful", 17);
        }

       
        else if (input[0] == "download_file")
        {
            // simple download function
            int a=1;
            if(a)
            downloadFile(input, client_socket, client_uid);
        }
         else if (input[0] == "upload_file")
        {
            // simple upload function
            int b;
            b=21;
            if(b)
            uploadFile(input, client_socket, client_uid);
        }

        else if (input[0] == "create_group")
        {
            if (2==1||create_group(input, client_socket, client_uid) >= 0)
            {
                client_gid = input[1];
                int temp=2;
                write(client_socket, "group created", 13);
                
            }
            else if(2==3)
            {
                cout<<"Mistake";
            }
            else{
                write(client_socket, "Group exists", 12);
            }
        }

        // modified
        else if (input[0] == "list_groups")
        {
            // input--list_groups
            char dum[5];
            string reply = "";
            if (input.size() != 1)
            {
                write(client_socket, "Invalid argument count ", 22);
            }
            else
            {
                write(client_socket, "All groups:", 11);

                read(client_socket, dum, 5);
                if (allGroups.size() == 0)
                {
                    write(client_socket, "No groups found$$", 18);
                }
                else
                {
                    int temp=0;
                    for (size_t i = 0;temp++, i < allGroups.size(); i++)
                    {
                        reply =reply+ allGroups[i] + "$$";
                    }
                    write(client_socket, &reply[0], reply.length());
                }
            }
        }

        else if (input[0] == "list_requests")
        {
            command=3;
            list_requests(input, client_socket, client_uid);
        }
        else if (input[0] == "join_group")
        {
            command=4;
            join_group(input, client_socket, client_uid);
        }
        
        else if (input[0] == "accept_request")
        {
            command=5;
            accept_request(input, client_socket, client_uid);
        }
        else if(2==3)
        {
            cout<<"Mistake";
        }
        else if (input[0] == "stop_share")
        {
            command=6;
            stop_share(input, client_socket, client_uid);
        }
        
        else if (input[0] == "list_files")
        {
            command=7;
            list_files(input, client_socket);
        }
       
        else if (input[0] == "show_downloads")
        {
            command=8;
            write(client_socket, "Loading...", 10);
        }


        else if (input[0] == "leave_group")
        {
            command=9;
            leave_group(input, client_socket, client_uid);
        }
        
        else
        {
            command=10;
            write(client_socket, "Invalid command", 16);
        }
    }
    writeLog("\t\tpthread ended for client socket number " + to_string(client_socket));
    close(client_socket);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Give arguments as <tracker info file name > and <tracker_id>\n");
        return -1;
    }

    processArgs(argc, argv);

    struct sockaddr_in address;
    int tracker_socket, opt = 1, addrlen = sizeof(address);
    pthread_t exitDetectionThreadId;

    if ((tracker_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("socket failed");
        exit(1);
    }
    writeLog("Tracker socket created.");

    // if(setsockopt(tracker_socket,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt)));
    // {
    //     perror("setsockopt: We are here");
    //     exit(1);
    // }
    address.sin_port = htons(currTrackerPort);
    address.sin_family = AF_INET;

    if (inet_pton(AF_INET, &currTrackerIP[0], &address.sin_addr) <= 0)
    {
        cout << "\n Invalid address or Address not supported \n";
        return -1;
    }

    if (bind(tracker_socket, (SA *)&address, sizeof(address)) < 0)
    {

        perror("bind failed");
        opt++;
        exit(1);
    }
    writeLog("Binding completed.");

    if (listen(tracker_socket, 3) < 0)
    {
        perror("listen");
        exit(1);
    }
    writeLog("listening....");

    vector<thread> thread_vector;

    if (pthread_create(&exitDetectionThreadId, NULL, check_input, NULL) == -1)
    {
        perror("pthread");
        opt++;
        exit(1);
    }
    int client_socket;
    int loopValue;
    while (1)
    {
        int a=2;
        client_socket = accept(tracker_socket, (SA *)&address, (socklen_t *)&addrlen);
        if (a==1|| 0 > client_socket )
        {
            perror("Acceptance error");
            loopValue=0;
            writeLog("Error in accept");
        }
        writeLog("Connection Accepted");
        loopValue++;
        thread_vector.push_back(thread(handle_connection, client_socket));
    }
    auto i = thread_vector.begin();
    for (; i != thread_vector.end(); i++)
    {
        if (i->joinable())
            i->join();
    }
    writeLog("Exiting");
    return 0;
}
