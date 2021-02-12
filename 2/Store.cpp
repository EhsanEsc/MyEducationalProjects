#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <unistd.h> // read

#include "utils.h"

using namespace std;

vector<string> split_str(string str, string delimiter)
{
    size_t pos = 0;
    vector<string> vv;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        vv.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    vv.push_back(str);  
    return vv;
}

int main(int argc, char** argv)
{
    char cmd[1024];
    char address[1024];
    int res_price = -1;

    int fdr = atoi(argv[1]);

    prw(read(fdr, cmd, 1024) <= 0, "Read command from parent");
    prw(read(fdr, address, 1024) <= 0, "Read address from parent");

    close(fdr);

    vector<string> command = split_str(cmd, " ");
    prw(command.size() != 4, "Command has Problems");

    string cmd_type = command[0];
    string cmd_product_id = command[1];
    string cmd_starting_date = command[2];
    string cmd_ending_data = command[3];

    fstream ff(address);
    string product;
    ff >> product;
    while(ff >> product)
    {
        vector<string> vv = split_str(product, ",");   

        prw(vv.size() != 3, "CSV has Problems");

        string data = vv[0];
        string product_id = vv[1];
        int price = stoi(vv[2]);

        if (product_id == cmd_product_id &&
            data >= cmd_starting_date &&
            data <= cmd_ending_data &&
            (
                (cmd_type == "MAX" && res_price < price) || 
                (cmd_type == "MIN" && (res_price > price || res_price == -1)) 
            )
            )
            res_price = price;
    }
    send_res_to_parent(address, res_price);

    return 0;
}