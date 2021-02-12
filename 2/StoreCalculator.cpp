#include <iostream>
#include <string>
#include <fstream>

#include <sys/wait.h> // wait

#include "utils.h"

using namespace std;

int main(int argc, char** argv)
{
    // StoreCalculator.out commands.txt assets
    if (argc != 3) 
    {
        cerr << "Wrong number of inputs" << endl;
        return 0;
    }
    string commands_address = argv[1]; 
    string assets_address = argv[2];

    std::fstream command_file(commands_address);
    
    string command;
    while(getline(command_file, command))
    {
        // cout << command << " STARTED " << endl;

        string provinces_path = "./" + assets_address;

        send_command_to_all_files(provinces_path, "./Province.out", command);
        int res = get_cmd_res_from_all_files(provinces_path, command);
        cout << res << endl;

        wait(0);
    }

    command_file.close();
    
    // cout << "END" << endl;

    return 0;
}