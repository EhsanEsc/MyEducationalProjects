#include <iostream>
#include <unistd.h> // read

#include "utils.h"

using namespace std;

int main(int argc, char** argv)
{
    char cmd[1024];
    char address[1024];
    
    int fdr = atoi(argv[1]);

    prw(read(fdr, cmd, 1024) <= 0, "Read command from parent");
    prw(read(fdr, address, 1024) <= 0, "Read address from parent");

    close(fdr);

    send_command_to_all_files(address, "./City.out", cmd);    
    int res = get_cmd_res_from_all_files(address, cmd);
    send_res_to_parent(address, res);

    return 0;
}