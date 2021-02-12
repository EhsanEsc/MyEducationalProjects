#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/stat.h> // mkfifo
#include <fcntl.h> // open

#include <boost/filesystem.hpp>

#include "utils.h"

using namespace std;
using namespace boost::filesystem;

const int READ_PIPE = 0;
const int WRITE_PIPE = 1;

// print warning
void prw(bool cond, string warning)
{
    if(cond)
    {
        cerr << "Error: " << warning << endl;
        exit(1);
    }
}

string calc_pipe_address(string address)
{
    string res = "./tmp/";
    for (auto i : address)
        if(i != '/' && i!='.')
            res.push_back(i);
    return res;
}

void send_command_to_all_files(string files_path, string exec_file, string command)
{
    for (auto& child_file : directory_iterator(files_path))
    {
        string pr = child_file.path().c_str();

        int fds[2];
        prw(pipe(fds) == -1, "PipeCreation");

        int pid = fork();

        if (pid < 0) {
            prw(true, "Fork");
        } else if (pid == 0) {
            close(fds[WRITE_PIPE]);
    
            string fd_read_str =  to_string(fds[READ_PIPE]);

            char* argvv[3] = {
                (char*)exec_file.c_str(),
                (char*)fd_read_str.c_str(),
                NULL
                };

            prw(execvp(argvv[0], argvv) < 0, "execvp");
            exit(0);
        } else {
            usleep(10022);

            close(fds[READ_PIPE]);

            prw(write(fds[WRITE_PIPE], command.c_str(), strlen(command.c_str())+1) <= 0,
                "write command to unnamed pipe");
            
            usleep(10022);

            prw(write(fds[WRITE_PIPE], pr.c_str(), strlen(pr.c_str())+1) <= 0,
                "write path to unnamed pipe");
            
            close(fds[WRITE_PIPE]);
        }
    }
}

int get_cmd_res_from_all_files(string address, string cmd)
{
    // cmd_type: 0 MIN, 1 MAX
    int cmd_type = (cmd[2] == 'X') ? 1 : 0;

    int res = -1;
    for (auto& child : directory_iterator(address))
    {
        string child_address = child.path().c_str();
        string pipe_address = calc_pipe_address(child_address);
        
        mkfifo(pipe_address.c_str(), 0666);
        int fd = open(pipe_address.c_str(), O_RDONLY);
            
        char buffer[1024];
        prw(read(fd, buffer, 1024) <= 0, "Read child_res");
        
        close(fd);        

        int child_res = -1;
        child_res = atoi(buffer);

        // cout << "Get " << child_res << " From: " << pipe_address << endl;

        if (res == -1 ||
            (cmd_type == 0 && res > child_res && child_res != -1) ||
            (cmd_type == 1 && res < child_res && child_res != -1))
            res = child_res;
    }
    return res;
}

void send_res_to_parent(string address, int res)
{
    string pipe_address = calc_pipe_address(address);
        
    mkfifo(pipe_address.c_str(), 0666); 
    int fd = open(pipe_address.c_str(), O_WRONLY);

    string res_str = std::to_string(res);
    prw(write(fd, res_str.c_str(), strlen(res_str.c_str())+1) <= 0, "Write res to parent");

    close(fd);
}