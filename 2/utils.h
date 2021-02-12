#include <string>

void prw(bool, std::string);
void send_command_to_all_files(std::string, std::string, std::string);
int get_cmd_res_from_all_files(std::string, std::string);
void send_res_to_parent(std::string, int);