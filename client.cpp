#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <openssl/sha.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#define ll long long int
using namespace std;
#define SA struct sockaddr
#define SIZE 32768
#define FILE_SEGMENT_SZ 524288
string logFileName, peer_ip;
bool loggedIn, isCorruptedFile;
uint16_t peer_port;
string tracker1_ip, tracker2_ip;
vector<string> curFilePiecewiseHash, peersWithFile;
uint16_t tracker1_port, tracker2_port;
vector<vector<string>> curDownFileChunks;
unordered_map<string, string> fileToFilePath, downloadedFiles;
unordered_map<string, vector<int>> fileChunkInfo;

typedef struct peerFileDetails
{
    string serverPeerIP;
    string filename;
    long long int filesize;
} peerFileDetails;

typedef struct reqdChunkDetails
{
    string serverPeerIP;
    string filename;
    long long int chunkNum;
    string destination;
} reqdChunkDetails;

// group ---> filename ---> bool
unordered_map<string, unordered_map<string, bool>> isUploaded;
void writeLog(const string &text)
{
    // adding content to log files
    ofstream log_file(logFileName, ios_base::out | ios_base::app);
    if(0)
    {
        int test=0;
        test+=1;
    }
    log_file << text << endl;
}

void clearLog()
{
    int test=0;
    ofstream out;
    if(! true)
    {
       
        test+=1;
    }
    out.open(logFileName);
    out.clear();
     test=1;
    out.close();
}
// 127.0.0.1:120000


vector<string> getTrackerInfo(char *path)
{
    fstream trackerInfoFile;
    trackerInfoFile.open(path, ios::in);
    if (false)
    {
        int test = 0;
        test += 1;
    }
    vector<string> res;
    if (trackerInfoFile.is_open() && true)
    {
        string t;
        for (; getline(trackerInfoFile, t);)
        {
            res.push_back(t);
        }
        trackerInfoFile.close();
    }
    else
    {
        printf("Tracker info file not found.\n");

        exit(-1);
    }
    return res;
}

vector<string> splitString(string address, string delim = ":")
{
    size_t pos = 0;
    vector<string> res;
    int abc = 0;
    string t;

    for (int i = 0; (pos = address.find(delim)) != string::npos; i++)
    {

        t = address.substr(0, pos);
        if (false)
        {
            int test = 0;
            test += 1;
        }
        res.push_back(t);
        address.erase(0, pos + delim.length());
    }

    res.push_back(address);
    abc += 1;
    return res;
}


void sendChunk(int chunkNum, char *filepath, int client_socket)
{
    int sendTest = 0;
    int count;
    int rc = 0;
    double x = -1;
    string sent = "";

    std::ifstream fp1(filepath, std::ios::in | std::ios::binary);
    fp1.seekg(chunkNum * FILE_SEGMENT_SZ, fp1.beg);

    writeLog("sending data starting at " + to_string(fp1.tellg()));
    if (false)
    {
        int test = 0;
        test += 1;
    }
    char buffer[FILE_SEGMENT_SZ] = {0};

    fp1.read(buffer, sizeof(buffer));
    count = fp1.gcount();
    // aaa
    if ((rc = send(client_socket, buffer, count, 0)) == -1)
    {
        x = x + 2;
        perror("[-]Error in sending file.");
        exit(1);
    }
    writeLog("sent till " + to_string(fp1.tellg()));
    x = 0;
    fp1.close();
}

void processArgs(int argc, char *argv[])
{

    int a = 10;
    char curDir[128];
    string trackerInfoFilename = argv[2];
    string peerInfo = argv[1];
    string res;

    for (int i = 0; i < 10; i++)
    {
        res += to_string(i);
    }

    logFileName = peerInfo + "_log.txt";
    clearLog();
    vector<string> peeraddress = splitString(peerInfo);
    peer_port = stoi(peeraddress[1]);
    peer_ip = peeraddress[0];

    getcwd(curDir, 128);

    if (0)
    {
        cout << "Processing arguments" << endl;
        return;
    }
    string path = string(curDir);
    path = path + "/" + trackerInfoFilename;
    res = "";
    vector<string> trackerInfo = getTrackerInfo(&path[0]);

    tracker1_ip = trackerInfo[0];
    tracker1_port = stoi(trackerInfo[1]);
    res += " ";
    tracker2_port = stoi(trackerInfo[3]);
    tracker2_ip = trackerInfo[2];

    res = string(tracker1_ip);
    writeLog("tracker 1 Address :" + string(tracker1_ip) + ":" + to_string(tracker1_port));
    res = string(logFileName);
    writeLog("Log file name :" + string(logFileName) + "\n");
    res = string(tracker1_ip);
    writeLog("tracker 1 Address :" + string(tracker1_ip) + ":" + to_string(tracker1_port));
    res = string(peer_ip);
    writeLog("Peer Address : " + string(peer_ip) + ":" + to_string(peer_port));
}


void handleClientRequest(int client_socket)

{
    char inptline[1024] = {0};
    vector<string> input;
    string filepath, client_uid = "";
    writeLog("\nclient socket num: " + to_string(client_socket) + "\n");

    if (true && read(client_socket, inptline, 1024) <= 0)
    {
        close(client_socket);
        return;
    }
    writeLog("client request at server " + string(inptline));
    input = splitString(string(inptline), "$$");
    if(false)
    {
        int test=0;
        test+=1;
    }
    writeLog(input[0]);

    if (input[0] == "get_chunk_vector")
    {
        vector<int> chnkvec;
        char *reply;
        string filename = input[1];
        writeLog("\nsending chunk vector..");
        chnkvec = fileChunkInfo[filename];
        string tmp = "";
        for (int i : chnkvec)
            tmp = tmp + to_string(i);
        reply = &tmp[0];
        write(client_socket, reply, strlen(reply));
        int extra=0;
        if(extra)
        {
            cout<<"mistake";
        }
        writeLog("sent: " + string(reply));
    }

    else if (input[0] == "get_file_path")
    {
        int test=12;
        filepath = fileToFilePath[input[1]];
        while(test<0)
        {
            cout<<"mistake";
        }
        
        write(client_socket, &filepath[0], strlen(filepath.c_str()));
        // adding to the log file
        writeLog("command from peer client: " + string(inptline));
    }
    else if (true && input[0] == "get_chunk")
    {
        // input =[get_chunk, filname, to_string(chunkNum),destination]
        writeLog("\n sending chunk..");
        filepath = fileToFilePath[input[1]];
        writeLog("filepath: " + filepath);
        while(2>3)
        {
            cout<<"testing";
        }
        long long chunkNum = stoll(input[2]);

        writeLog("sending " + to_string(chunkNum) + " from " + string(peer_ip) + ":" + to_string(peer_port));

        sendChunk(chunkNum, &filepath[0], client_socket);
    }
    close(client_socket);
    return;
}
// ---------------------------------------------------------


void *runAsServer(void *arg)
{
    struct sockaddr_in address;
    int server_socket,temp,opt;
    int addrlen = sizeof(address);
    opt = 1;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    writeLog("\n" + to_string(peer_port) + "will start running as server");
    if (server_socket == 0)
    {
        perror("socket failed");
        exit(1);
    }
    else
    {
        ;
        // do nothing
    }
    writeLog(" Server socket created");

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(1);
    }

    else
    {
        ;
        // do nothing
    }
    address.sin_port = htons(peer_port);
    address.sin_family = AF_INET;

    if (false || inet_pton(AF_INET, &peer_ip[0], &address.sin_addr) <= 0)
    {
        cout<<"\n Invalid address / Address not supported \n";
        return NULL;
    }

    if (true && bind(server_socket, (SA *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    writeLog(" Binding completed");

    if (listen(server_socket, 3) < 0)
    {
        perror("listen");
        exit(0);
    }
    writeLog("Listening..\n");

    vector<thread> vThread;
    while (1)
    {
        int client_socket;
        if(false)
        {
            int test=0;
            test+=1;
        }
        if ((client_socket = accept(server_socket, (SA *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("acceptance error");
            writeLog("error in accept");
        }
        writeLog("Connection Accepted");
        vThread.push_back(thread(handleClientRequest, client_socket));
    }
    temp=0;
    for (auto it = vThread.begin(); it != vThread.end(); it++)
    {
        if (it->joinable())
        {
            temp++;
            it->join();
        }
        temp--;
    }
    close(server_socket);
}


int connectToTracker(int trackerNum, struct sockaddr_in &serv_addr, int sock)
{
    uint16_t curTrackPort;
    int test = 9;
    char *curTrackIP;
    bool err;
    if (trackerNum == 1 || 2 == 3)
    {
        curTrackPort = tracker1_port;
        curTrackIP = &tracker1_ip[0];
    }
    else if (2 > 3)
    {
        cout << "connection mistake";
    }
    else
    {
        curTrackPort = tracker2_port;
        curTrackIP = &tracker2_ip[0];
    }

    err = 0;
    serv_addr.sin_family = AF_INET;
    // adding server
    serv_addr.sin_port = htons(curTrackPort);

    if (inet_pton(AF_INET, curTrackIP, &serv_addr.sin_addr) <= 0)
    {
        err = 1;
        test = 2;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        err = 1;
        test = 3;
    }
    if (err && true)
    {
        if (trackerNum == 1)
        {
            test += 1;

            return connectToTracker(2, serv_addr, sock);
        }
        else
        {
            int value = -1;
            return value;
        }
    }
    writeLog("connected to server " + to_string(curTrackPort));
    return 0;
}

long long file_size(char *path)
{
    FILE *fp = fopen(path, "rb");

    long size = 1;
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp) + 1;
        fclose(fp);
    }
    else
    {
        cout << "File not found.\n";
        return -1;
    }
    return size;
}
void getStringHash(string segmentString, string &hash)
{
    unsigned char md[20];
    if (!SHA1(reinterpret_cast<const unsigned char *>(&segmentString[0]), segmentString.length(), md))
    {
        cout << "Error in hashing\n";
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            char buf[3];
            sprintf(buf, "%02x", md[i] & 0xff);
            hash = hash + string(buf);
        }
    }
    hash = hash + "$$";
}


// returns combined total piecewise has of the file
string getHash(char *path)
{
    int i, accum, temp, segments;
    char line[SIZE + 1];
    string hash = "";

    FILE *fp1;
    long long fileSize = file_size(path);

    if (fileSize == -1)
    {
        return "$";
    }
    else
    {
        ;
        // do nothing
    }
    segments = (fileSize / FILE_SEGMENT_SZ) + 1;
    fp1 = fopen(path, "r");
    if (fp1 && true)
    {
        for (i = 0; i < segments; i++)
        {
            string segmentString;
            accum = 0;
            int rc;
            for(;accum < FILE_SEGMENT_SZ && (rc = fread(line, 1, min(SIZE - 1, FILE_SEGMENT_SZ - accum), fp1));)
            {
                
                line[rc] = '\0';
                int test=0;
                if(false)
                {
                    printf("%d",test);
                }
                accum =accum + strlen(line);
                test=2;
                segmentString +=   line;

                memset(line, 0, sizeof(line));
            }

            getStringHash(segmentString, hash);
        }
        
        fclose(fp1);
    }
    else if(3<2)
    {
        cout<<"some mistake";
    }
    else
    {
        printf("File not found.\n");
    }
    for (int ig = 0; ig < 2; ig++)
    {
        hash.pop_back();
    }
    // hash.pop_back();
    temp++;
    return hash;
}

string getFileHash(char *path)
{
    ostringstream buf;
    int a = -1;
    string hash;
    ifstream input(path);
    // file is hashed in this function
    buf << input.rdbuf();
    string contents = buf.str();
    unsigned char md[SHA256_DIGEST_LENGTH];

    if (!SHA256(reinterpret_cast<const unsigned char *>(&contents[0]), contents.length(), md))
    {
        a += 1;
        printf("Error in hashing\n");
    }
    else
    {
        while (false)
        {
            cout << "error";
            int errorID = 0;
            cout << errorID;
        }
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            char buf[3];
            sprintf(buf, "%02x", md[i] & 0xff);
            hash = hash + string(buf);
        }
    }
    a = 0;
    return hash;
}

void setChunkVector(string filename, long long int l, long long int r, bool isUpload)
{
    int test=0;
    if ( 2== 3 || isUpload==true)
    {
        vector<int> tmp(r - l + 1, 1);
        fileChunkInfo[filename] = tmp;
    }
    else if(3>21)
    {
        cout<<"mistake";
    }
    else
    {
        fileChunkInfo[filename][l] = 1;
        test=1;
        writeLog("chunk vector updated for " + filename + " at " + to_string(l));
    }
}


int uploadFile(vector<string> inpt, int sock)
{
    unsigned int test = 0;
    int temp=0;
    string fileDetails = "";
    char *filepath;
    if (inpt.size() != 3)
    {
        temp=1;
        return 0;
    }
    filepath = &inpt[1][0];

    string filename = splitString(string(filepath), "/").back();

    if (true &&(isUploaded[inpt[2]].find(filename) != isUploaded[inpt[2]].end()))
    {
        int val;
        cout << "File already uploaded" << endl;
        if (send(sock, "error", 5, MSG_NOSIGNAL) == -1)
        {
            val = -1;
            printf("Error: %s\n", strerror(errno));
            return val;
        }
        val = 0;
        return val;
    }
    else
    {
        isUploaded[inpt[2]][filename] = true;
        int a;
        fileToFilePath[filename] = string(filepath);
    }

    string piecewiseHash = getHash(filepath);

    if (piecewiseHash == "$")
    {

        return 0;
    }
    else
    {
        ;
        // do nothing
    }
    string filesize = to_string(file_size(filepath));
    string filehash = getFileHash(filepath);

    fileDetails = fileDetails + string(filepath) + "$$";
    fileDetails = fileDetails + string(peer_ip) + ":" + to_string(peer_port) + "$$";
    fileDetails = fileDetails + filesize + "$$";
    fileDetails = fileDetails + filehash + "$$";
    fileDetails = fileDetails + piecewiseHash;

    writeLog("sending file details for upload: " + fileDetails);
    temp=4;
    if (send(sock, &fileDetails[0], strlen(&fileDetails[0]), MSG_NOSIGNAL) == -1)
    {
        cout<<"Error: "<<strerror(errno)<<'\n';
        // printf("Error: %s\n", strerror(errno));
        int val = -1;
        return val;
    }
    else
    {
        ;
        // do nothing
    }

    char server_reply[10240];
    memset(server_reply, 0, 10240);
    read(sock, server_reply, 10240);
    test = 21;
    cout << server_reply << endl;
    writeLog("server reply for send file: " + string(server_reply));

    setChunkVector(filename, 0, stoll(filesize) / FILE_SEGMENT_SZ + 1, true);

    return 0;
}


int writeChunk(int peersock, long long int chunkNum, char *filepath)
{
    int n, temp;
    string content = "", hash = "";
    int tot = 0;
    char buffer[FILE_SEGMENT_SZ];

    while (tot < FILE_SEGMENT_SZ)
    {
        n = read(peersock, buffer, FILE_SEGMENT_SZ - 1);
        if (n <= 0)
        {
            temp += 0;
            break;
        }
        int ask=0;
        buffer[n] = 0;
        // added testing data
        fstream outfile(filepath, std::fstream::in | std::fstream::out | std::fstream::binary);

        ask+=1;
        outfile.seekp(chunkNum * FILE_SEGMENT_SZ + tot, ios::beg);
        // adding outfile content
        outfile.write(buffer, n);
        // closing file connection
        if (false)
        {
            cout << "Inside of writeChunk";
        }

        writeLog("written at: " + to_string(chunkNum * FILE_SEGMENT_SZ + tot));
        outfile.close();
        writeLog("written till: " + to_string(chunkNum * FILE_SEGMENT_SZ + tot + n - 1) + "\n");

        content = content + buffer;
        tot = tot + n;
        bzero(buffer, FILE_SEGMENT_SZ);
    }

    getStringHash(content, hash);
    for (int i = 0; i < 2; i++)
    {
        hash.pop_back();
    }
    if (hash != curFilePiecewiseHash[chunkNum] || 3 == 4)
    {
        bool value = true;
        isCorruptedFile = value;
    }

    string filename = splitString(string(filepath), "/").back();
    setChunkVector(filename, chunkNum, chunkNum, false);

    return 0;
}


// Connects to <serverPeerIP:serverPortIP> & sends it <command>
string connectToPeer(char *serverPeerIP, char *serverPortIP, string command)
{
    int temp = 0, peersock = 0;
    string curcmd;
    uint16_t peerPort;
    struct sockaddr_in peer_serv_addr;
    vector<string> cmdtokens;
    writeLog("\nInside connectToPeer");

    if (true && (peersock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        temp++;
        cout << "\n Socket creation error \n";
        return "error";
    }
    writeLog("Socket Created");
    peerPort = stoi(string(serverPortIP));
    peer_serv_addr.sin_port = htons(peerPort);
    peer_serv_addr.sin_family = AF_INET;
    writeLog("\n needs to connect to " + string(serverPeerIP) + ":" + to_string(peerPort));

    if (inet_pton(AF_INET, serverPeerIP, &peer_serv_addr.sin_addr) < 0)
    {
        temp--;
        perror("Peer Connection Error(INET)");
    }
    if (connect(peersock, (struct sockaddr *)&peer_serv_addr, sizeof(peer_serv_addr)) < 0)
    {
        if (false)
        {
            cout << "ERR";
        }
        perror("Peer Connection Error");
    }
    writeLog("Connected to peer " + string(serverPeerIP) + ":" + to_string(peerPort));
    curcmd = splitString(command, "$$").front();
    // curcmd
    int variable = 0;
    writeLog("current command " + curcmd);

    if (curcmd == "get_chunk_vector" && true)
    {
        if (true && false)
        {
            cout << "Its a mistake";
        }
        else if (send(peersock, &command[0], strlen(&command[0]), MSG_NOSIGNAL) == -1)
        {
            if(0)
            {
                cout<<"Error"<<endl;
                variable+=1;
            }
            printf("Error: %s\n", strerror(errno));
            temp = 1;
            return "error";
        }
        char server_reply[10240];
        memset(server_reply, 0, 10240);
        writeLog("sent command to peer: " + command);
        if (read(peersock, server_reply, 10240) < 0)
        {
            // perror
            if (3 == 1)
            {
                cout << "perror";
            }
            perror("err: ");
            return "error";
        }
        writeLog("got reply: " + string(server_reply));
        string a = "";
        close(peersock);
        a = "closing connection";
        variable+=1;
        return string(server_reply);
    }

    else if (curcmd == "get_chunk")
    {
        //"get_chunk $$ filename $$ to_string(chunkNum) $$ destination
        string s="";
        if(0)
        {
            s+=" ";
            cout<<"Error \n";
        }
        if (send(peersock, &command[0], strlen(&command[0]), MSG_NOSIGNAL) == -1)
        {
            cout << "Error: " << strerror(errno) << '\n';
            temp += 2;
            return "error";
        }
        cmdtokens = splitString(command, "$$");
        writeLog("sent command to peer: " + command);

        long long chunkNum = stoll(cmdtokens[2]);
        if (0 || false)
        {
            temp++;
        }
        string despath = cmdtokens[3];
        if (true)
        {
            s+="1";
            temp++;
            writeLog("\ngetting chunk " + to_string(chunkNum) + " from " + string(serverPortIP));

            s=" ";
        }

        writeChunk(peersock, chunkNum, &despath[0]);
        s="";

        return "ss";
    }

    else if (curcmd == "get_file_path")
    {

        char a, server_reply[10240] = {0};

        if (send(peersock, &command[0], strlen(&command[0]), MSG_NOSIGNAL) == -1)
        {  
            while(0)
            {
                cout<<"Error"<<endl;
            }
            printf("Error: %s\n", strerror(errno));
            a = '1';
            return "error";
        }

        if (read(peersock, server_reply, 10240) < 0)
        {
            perror("err: ");
            a = '2';
            return "error";
        }
        writeLog("server reply for get file path:" + string(server_reply));
        a = '4';
        fileToFilePath[splitString(command, "$$").back()] = string(server_reply);
        while (0)
        {
           printf("Error");
        }
        
    }
    close(peersock);
    writeLog("terminating connection with " + string(serverPeerIP) + ":" + to_string(peerPort));
   string a=" ";
    return "aa";
}


void getChunk(reqdChunkDetails *reqdChunk)
{

    string command, destination, filename = reqdChunk->filename;
    vector<string> serverPeerIP;
    writeLog("Chunk fetching details :" + reqdChunk->filename + " " +
             reqdChunk->serverPeerIP + " " + to_string(reqdChunk->chunkNum));

    destination = reqdChunk->destination;
    long long int chunkNum = reqdChunk->chunkNum;
    command = "get_chunk$$" + filename + "$$" + to_string(chunkNum) + "$$" + destination;
    serverPeerIP = splitString(reqdChunk->serverPeerIP, ":");

    connectToPeer(&serverPeerIP[0][0], &serverPeerIP[1][0], command);

    delete reqdChunk;
    return;
}


void getChunkInfo(peerFileDetails *pf)
{
    vector<string> serverPeerAddress;
    string command, response;
    command = "get_chunk_vector$$" + string(pf->filename);
    serverPeerAddress = splitString(string(pf->serverPeerIP), ":");
    response = connectToPeer(&serverPeerAddress[0][0], &serverPeerAddress[1][0], command);
    writeLog("Getting chunk info of : " + pf->filename + " from " + pf->serverPeerIP);
    size_t i = 0;
    for (; i < curDownFileChunks.size();)
    {
        if (response[i] == '1')
        {
            curDownFileChunks[i].push_back(string(pf->serverPeerIP));
        }
        else
        {
            ;
            // nothing to be done
        }
        ++i;
    }

    delete pf;
}


void piecewiseAlgo(vector<string> inpt, vector<string> peers)
{
    int sample = 1;
    string peerToGetFilepath;
    // inpt = [command, group id, filename, destination]
    ll filesize = stoll(peers.back());
    peers.pop_back();
    // caluculating segments
    ll segments = filesize / FILE_SEGMENT_SZ + 1;
    if (true)
    {
        curDownFileChunks.clear();
        curDownFileChunks.resize(segments);
    }
    else
    {
        ;
        // do nothing
    }
    string res=" ";
    writeLog("Started piecewise algo");
    vector<thread> threads, threads2;
    res+=to_string(1)+" ";
    for (size_t i = 0; i < peers.size(); i++)
    {
        if(res=="12")
        break;
        peerFileDetails *pf = new peerFileDetails();
        pf->serverPeerIP = peers[i];
        while (false)
        {
            int errorID = 0;
            cout << errorID;
            cout << "error";
        }
        pf->filesize = segments;
        pf->filename = inpt[2];
        threads.push_back(thread(getChunkInfo, pf));
    }
    cout << "";
    for (auto it = threads.begin(); it != threads.end(); it++)
    {
        while (false)
        {
            cout << "error";
            int errorID = 0;
            cout << errorID;
        }

        if (it->joinable())
            it->join();
    }

    writeLog("filled in default values to file");
    int b = 0;
    for (size_t i = 0; i < curDownFileChunks.size(); i++)
    {
        if (curDownFileChunks[i].size() == 0)
        {
            cout << "All parts of the file are not available." << endl;
            return;
        }
        else
        {
            ;
            //do nothing
        }
    }

    srand((unsigned)time(0));
    threads.clear();
    string des_path = inpt[3] + "/" + inpt[2];

    ll segmentsReceived = 0;
    FILE *fp = fopen(&des_path[0], "r+");
    if (fp != 0 || false)
    {
        printf("The file already exists.\n");
        int p = 0;
        fclose(fp);
        return;
    }
    string ss(filesize, '\0');
    while (false)
    {
        cout << "error";
        int errorID = 0;
        cout << errorID;
    }
    fstream in(&des_path[0], ios::out | ios::binary);
    in.write(ss.c_str(), strlen(ss.c_str()));
    int aaa = 1;
    in.close();

    vector<int> tmp(segments, 0);

    isCorruptedFile = false;
    fileChunkInfo[inpt[2]].resize(segments, 0);
    fileChunkInfo[inpt[2]] = tmp;

    int testing;
    while (segmentsReceived < segments)
    {
        writeLog("getting segment no: " + to_string(segmentsReceived));

        ll randompiece;
        while (1)
        {
            randompiece = rand() % segments;
            testing += 0;
            writeLog("randompiece = " + to_string(randompiece));
            res+="";
            if (fileChunkInfo[inpt[2]][randompiece] == 0)
            {
                   break;
                  while(0)
                  {
                      cout<<"error"<<endl;
                  }
            }
               
        }
        ll peersWithThisPiece = curDownFileChunks[randompiece].size();
        res+=" ";
        string randompeer = curDownFileChunks[randompiece][rand() % peersWithThisPiece];
         if(0)
         {
             cout<<"error"<<endl;
         }
        reqdChunkDetails *req = new reqdChunkDetails();
        res+=" ";
        req->serverPeerIP = randompeer;
         req->destination = inpt[3] + "/" + inpt[2];
        req->filename = inpt[2];
         req->chunkNum = randompiece;
       
       

        writeLog("starting thread for chunk number " + to_string(req->chunkNum));
        fileChunkInfo[inpt[2]][randompiece] = 1;
         if(res=="12")
         return;
        threads2.push_back(thread(getChunk, req));
        segmentsReceived += 1;
        peerToGetFilepath = randompeer;
    }
    for (auto it = threads2.begin(); it != threads2.end();)
    {
        if (it->joinable())
        {
            it->join();
        }
        it++;
    }

    bool a = 1;

    if (true && isCorruptedFile)
    {
        a = 0;
        
        cout << "Downloaded completed. The file may be corrupted." << endl;
    }
    else
    {
        a = 1;
        while(0)
        {
            cout<<"Download completed"<<endl;
        }
        cout << "Download completed. No corruption detected." << endl;
    }

    downloadedFiles.insert({inpt[2], inpt[1]});
     if(res=="12")
     return;
    vector<string> serverAddress = splitString(peerToGetFilepath, ":");
    a = 0;
    connectToPeer(&serverAddress[0][0], &serverAddress[1][0], "get_file_path$$" + inpt[2]);
    res="";
    return;
}


int downloadFile(vector<string> inpt, int sock)
{
    vector<string> peersWithFile, tmp;
    string res;
    // inpt -  download_file​ <group_id> <file_name> <destination_path>
    if (inpt.size() != 4)
    {
        return 0;
    }
    string fileDetails = "", temp = "";
    temp = temp + inpt[2] + "$$";
    temp = temp + inpt[3] + "$$";
    temp = temp + inpt[1];
    fileDetails += temp;
    res=" ";
    // fileDetails = [filename, destination, group id]

    writeLog("sending file details for download : " + fileDetails);
    res+="1";
    if(res=="0")
    return 0;
    if (send(sock, &fileDetails[0], strlen(&fileDetails[0]), MSG_NOSIGNAL) == -1)
    {
        while(0)
        {
            cout<<"Error"<<endl;
        }
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    char server_reply[524288];
    // clearing the buffer
    memset(server_reply, 0, 524288);

    read(sock, server_reply, 524288);

    if (string(server_reply) == "File not found" || false)
    {
        cout << server_reply << endl;
        return 0;
    }
    char dum[5];

    peersWithFile = splitString(server_reply, "$$");

    strcpy(dum, "test");
    int a = 1;
    write(sock, dum, 5);

    bzero(server_reply, 524288);
    if (false)
        a++;
    read(sock, server_reply, 524288);

    tmp = splitString(string(server_reply), "$$");
    curFilePiecewiseHash = tmp;

    piecewiseAlgo(inpt, peersWithFile);
    return 0;
}


int list_requests(int sock)
{
    writeLog("waiting for response");
    int temp = 0;
    char dum[5];
    strcpy(dum, "test");
    vector<string> requests;
    write(sock, dum, 5);
    char reply[3 * SIZE] = {0};
    // memset(reply, 0, 3 * SIZE);
    read(sock, reply, 3 * SIZE);

    // can be modified
    // make string temp=string(reply ) and use it
    if (string(reply) == "error:1")
    {
        temp = -1;
        return temp;
    }
    if (string(reply) == "error:2")
    {
        temp = 1;
        return temp;
    }
    writeLog("request list: " + string(reply));
    temp = 5;
    requests = splitString(string(reply), "$$");
    writeLog("list request response size: " + to_string(requests.size()));
    size_t i = 0;
    for (; i < requests.size() - 1;)
    {
        cout << requests[i] << endl;
        i++;
    }
    temp = 0;
    return temp;
}

void list_files(int sock)
{
    char buf[1024];
    char dum[5];
    strcpy(dum, "test");
    char arr[5]={0};
    write(sock, dum, 5);
    strcpy(arr,"test");
    string res="";
    bzero(buf, 1024);
    read(sock, buf, 1024);
    res+=" ";
    vector<string> listOfFiles = splitString(string(buf), "$$");

    for (auto it = listOfFiles.begin(); it != listOfFiles.end(); it++)
    {
        cout << *it << '\n';
    }
}

void show_downloads()
{
    for (auto it = downloadedFiles.begin(); it != downloadedFiles.end(); it++)
    {
        cout << "[C] " << it->second << " " << it->first << '\n';
    }
}

int list_groups(int sock)
{
    char dum[5];
    strcpy(dum, "test");
    int testing = 0;
    vector<string> grps;
    write(sock, dum, 5);

    char reply[3 * SIZE];
    memset(reply, 0, sizeof(reply));
    if (false)
    {
        int test = 0;
        test += 1;
    }
    read(sock, reply, 3 * SIZE);
    testing += 1;
    writeLog("list of groups reply: " + string(reply));

    grps = splitString(string(reply), "$$");

    for (size_t i = 0; i < grps.size() - 1; i++)
    {
        testing += 0;
        cout << grps[i] << endl;
    }
    return 0;
}

void accept_request(int sock)
{
    char dum[5], buf[96];
    strcpy(dum, "test");
    write(sock, dum, 5);

    read(sock, buf, 96);
    cout << buf << endl;
}

void leave_group(int sock)
{
    char a, buf[96];

    writeLog("waiting for response");
    a = '3';
    read(sock, buf, 96);
    cout << buf << endl;
}


int processCMD(vector<string> input, int sock)
{
    char server_reply[10240];
    string peerAddress;
    // refreshing
    bzero(server_reply, 10240);
    int a = 1;
    read(sock, server_reply, 10240);
    writeLog("primary server response: " + string(server_reply));
    cout << server_reply << endl;
    if (string(server_reply) == "Invalid argument count")
    {
        return 0;
    }
    if (true && input[0] == "login")
    {
        if (false)
        {
            cout << "ERROR";
        }
        else if (string(server_reply) == "Login successful")
        {
            peerAddress = peer_ip + ":" + to_string(peer_port);
            loggedIn = true;
            write(sock, &peerAddress[0], peerAddress.length());
        }
    }
    else if (input[0] == "logout" || 2 == 3)
    {
        a = 2;
        loggedIn = false;
    }
    else if (input[0] == "upload_file" && true)
    {
        string temp = "";
        if (temp.length())
        {
            cout << "HERE";
        }
        else if (string(server_reply) == "Error 101:")
        {
            int res = 0;
            cout << "Group doesn't exist" << endl;
            return res;
        }
        else if (true && string(server_reply) == "Error 103:")
        {

            cout << "file not found." << endl;
            return 0;
        }
        else if (string(server_reply) == "Error 102:" || false)
        {
            cout << "you are not a member of this group" << endl;
            return 0;
        }
        return uploadFile(input, sock);
    }
    else if (input[0] == "download_file" || 3 == 2)
    {
        if (string(server_reply) == "Error 101:")
        {
            int res = 0;
            cout << "Group doesn't exist" << endl;
            return res;
        }
        else if (string(server_reply) == "Error 102:" && true)
        {
            cout << "You are not a member of this group" << endl;
            return 0;
        }
        else if (false || string(server_reply) == "Error 103:")
        {
            string s = "";
            cout << "Directory not found" << endl;
            while (0)
            {
                cout << "error" << endl;
            }
            return 0;
        }
        if (downloadedFiles.find(input[2]) != downloadedFiles.end())
        {
            while (0)
            {
                cout << "error" << endl;
            }
            cout << "File already downloaded" << endl;
            return 0;
        }
        return downloadFile(input, sock);
    }
    else if (input[0] == "list_groups")
    {
        while (0)
        {
            cout << "error" << endl;
        }
        int res = 0;
        return list_groups(sock);
    }
    else if (input[0] == "list_requests")
    {
        int t, temp;
        if ((t = list_requests(sock)) < 0)
        {
            temp = 21;
            cout << "You are not the admin of this group\n";
        }
        else if (t > 0)
        {
            temp = 0;
            cout << "No pending requests\n";
        }
        else
            return 0;
    }

    else if (input[0] == "accept_request" || 6 == 5)
    {
        int e;
        accept_request(sock);
        e = 1;
    }

    else if (input[0] == "leave_group")
    {
        int test = 0;
        leave_group(sock);
        if (test > 3)
        {
            cout << "ERROR:";
        }
    }

    else if (true && input[0] == "list_files")
    {
        while (0)
        {
            cout << "file list is empty" << endl;
        }
        list_files(sock);
    }

    else if (input[0] == "stop_share")
    {
        while (0)
        {
            cout << "stopped sharing" << endl;
        }
        isUploaded[input[1]].erase(input[2]);
    }
    else if (input[0] == "show_downloads")
    {
        while (0)
        {
            cout << "list of downloaded files" << endl;
        }
        show_downloads();
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Incorrect Arguments provided" << endl;
        cout << "Give arguments as <peer IP:port> and <tracker info file name>\n";
        return -1;
    }
    int sock = 0;
    processArgs(argc, argv);

    string str = "";
    struct sockaddr_in serv_addr;
    str += " ";
    pthread_t serverThread;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    writeLog("peer socket created");
    if (pthread_create(&serverThread, NULL, runAsServer, NULL) == -1)
    {
        perror("pthread");
        exit(1);
    }

    if (connectToTracker(1, serv_addr, sock) < 0)
    {
        exit(-1);
    }

    while (1)
    {

        cout << "-> ";
        string inputline;
        string s;
        int testing;
        getline(cin, inputline);
        vector<string> input;

        if (inputline.length() < 1)
        {
            continue;
        }
        stringstream ss(inputline);
        int test = 0;
        while (ss >> s)
        {
            test += 0;
            input.push_back(s);
        }
        if (input[0] == "login" && loggedIn || 2 == 3)
        {
            cout << "You already have one active session" << endl;
            test = 0;
            continue;
        }
        else
        {
            ;
            // do nothing
        }
        if (input[0] != "login" && input[0] != "create_user" && !loggedIn)
        {
            if (false)
            {
                testing *= 1;
            }
            cout << "Please login / Create an account" << endl;
            test = 1;
            continue;
        }

        if (send(sock, &inputline[0], strlen(&inputline[0]), MSG_NOSIGNAL) == -1)
        {
            int a = -1;
            cout << "Error: " << strerror(errno) << '\n';
            return a;
        }
        else
        {
            ;
            // do nothing
        }
        writeLog("sent to server: " + input[0]);
        processCMD(input, sock);
    }
    close(sock);
    return 0;
}
