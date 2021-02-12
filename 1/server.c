#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

#define DEFAULT_SERVER_PORT 8080
#define LOCAL_HOST "127.0.0.1"
#define MAX_NUMBER_OF_CLIENTS 22

#define MIN_COUNT_OF_PLAYERS 2
#define MAX_COUNT_OF_PLAYERS 5

int server_socket;
int server_port;
int client_sockets[MAX_NUMBER_OF_CLIENTS];

int last_port = 10022;
int counter_group[MAX_COUNT_OF_PLAYERS];
int group[MAX_COUNT_OF_PLAYERS][MAX_COUNT_OF_PLAYERS];

char print_buffer[1024];

void DieWithHonor(char* error_message)
{
    sprintf(print_buffer, "!@# -> %s\n\n", error_message);
    write(1, print_buffer, strlen(print_buffer));
    exit(1);
}

void send_int_to_socket(int sock, int num)
{
    int rc = send(sock , &num, sizeof(num) , 0 ); 
    if (rc < 0)
        DieWithHonor("Sending int to Socket Failed");
}

int read_int_from_socket(int sock)
{
    int num;
    if (read(sock, &num, sizeof(num)) == 0)   
        DieWithHonor("Reading int from Socket Failed");
    return num;
}

void initialize_server()
{
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket <= 0)
       DieWithHonor("Server Can not Create Socket");
    sprintf(print_buffer, "Server Created Socket\n");
    write(1, print_buffer, strlen(print_buffer));

    addr.sin_family = AF_INET;
    if (inet_aton(LOCAL_HOST, &addr.sin_addr) == 0)
        DieWithHonor("Can not Convert IPAddress to binary");
    addr.sin_port = htons(server_port);
         
    if (bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)   
        DieWithHonor("Binding Socket is Failed");  
    sprintf(print_buffer, "Bind Port to Socket Is Completed\n");
    write(1, print_buffer, strlen(print_buffer));


    if (listen(server_socket, 22 ) < 0)
        DieWithHonor("Listen Failed!");
    sprintf(print_buffer, "Server is Listening\n\n");
    write(1, print_buffer, strlen(print_buffer));
}

void accept_new_client()
{
    int new_client_socket;
    struct sockaddr_in addr;
    int addrlen;
    
    if ((new_client_socket = accept(server_socket, (struct sockaddr*)&addr, (socklen_t*)&addrlen)) < 0)
        DieWithHonor("Accepting new client is Failed");
    sprintf(print_buffer, "New Client Accepted!\n");
    write(1, print_buffer, strlen(print_buffer));
 
    char* message = "Nothing else matters!";
    if(send(new_client_socket, message, strlen(message), 0) != strlen(message)) 
        DieWithHonor("Sending Greeting Message Failed");   
    sprintf(print_buffer, "Welcome Message Sent Successfully\n");
    write(1, print_buffer, strlen(print_buffer));

    int i;
    for (i = 0; i < MAX_NUMBER_OF_CLIENTS; i++)   
        if(client_sockets[i] == 0)   
        {   
            client_sockets[i] = new_client_socket;   
            sprintf(print_buffer, "Client ID: %d\n\n", i);
            write(1, print_buffer, strlen(print_buffer));
            break;   
        }   
}

void disconnect_client(int i)
{
    struct sockaddr_in addr;
    int addrlen;
    int sd = client_sockets[i];

    getpeername(sd, (struct sockaddr*)&addr, (socklen_t*)&addrlen);   
    sprintf(print_buffer, "Client Disconnected\n");
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "Info: ID %d , IP %s , PORT %d \n\n", i, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));   
    write(1, print_buffer, strlen(print_buffer));
    
    close(sd);   
    client_sockets[i] = 0;
}

void create_game(int num)
{
    sprintf(print_buffer, "#Group Created#\n");
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "Group Port: %d\n", last_port);
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "Number Of Players: %d\n", num);
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "Players: ");
    write(1, print_buffer, strlen(print_buffer));

    int j;
    for (j=0; j<num; j++)
    {
        int cs = client_sockets[group[num][j]];
        if (cs == 0)
        {
            sprintf(print_buffer, "\nPlayer Number %d Socket is Destoyed!\n", group[num][j]);
            write(1, print_buffer, strlen(print_buffer));
            sprintf(print_buffer, "Something Bad Happened. Ignoring Player...\n");
            write(1, print_buffer, strlen(print_buffer));
            continue;
        }
        int player_id = j + 1;
        send_int_to_socket(cs, last_port);
        send_int_to_socket(cs, player_id);
        sprintf(print_buffer, "%d ", group[num][j]);
        write(1, print_buffer, strlen(print_buffer));
        group[num][j] = -1;
    }
    sprintf(print_buffer, "\n\n");
    write(1, print_buffer, strlen(print_buffer));
    counter_group[num] = 0;
    last_port++;
}

int main(int argc , char* argv[])   
{   
    setvbuf(stdout, NULL, _IONBF, 0);
    server_port = DEFAULT_SERVER_PORT;
    if (argc == 2)
        server_port = atoi(argv[1]);

    int i;
    for (i = 0; i < MAX_NUMBER_OF_CLIENTS; i++) 
        client_sockets[i] = 0, counter_group[i] = 0;

    initialize_server();

    while (1)   
    {   
        fd_set fds_list;

        FD_ZERO(&fds_list);    
        FD_SET(server_socket, &fds_list);   
        int max_sock_num = server_socket;   
             
        for (i = 0; i < MAX_NUMBER_OF_CLIENTS; i++)   
        {   
            if(client_sockets[i] > 0)   
                FD_SET(client_sockets[i] , &fds_list);   
            if(client_sockets[i] > max_sock_num)   
                max_sock_num = client_sockets[i];   
        }   

        int event = select(max_sock_num+1, &fds_list ,NULL, NULL, NULL);   
       
        if ((event < 0) && (errno != EINTR))   
        {
            sprintf(print_buffer, "Select is Failed Successfully!\n");   
            write(1, print_buffer, strlen(print_buffer));;
        }

        if (FD_ISSET(server_socket, &fds_list))   
            accept_new_client();
           
        for (i = 0; i < MAX_NUMBER_OF_CLIENTS; i++)   
        {   
            int sd = client_sockets[i];   
                 
            if (FD_ISSET(sd, &fds_list))   
            {   
                int num, rc;
                if ((rc = read( sd , &num, sizeof(num))) == 0)   
                    disconnect_client(i);
                else 
                {   
                    if (num > MAX_COUNT_OF_PLAYERS-1 || num < MIN_COUNT_OF_PLAYERS)
                    {
                        sprintf(print_buffer, "Wrong Number sent by client %d.  Ignoring client...\n\n", i);
                        write(1, print_buffer, strlen(print_buffer));
                        send_int_to_socket(sd, -1);
                    }
                    else
                    {
                        sprintf(print_buffer, "Number %d sent by client %d\n\n", num, i);
                        write(1, print_buffer, strlen(print_buffer));
                        group[num][counter_group[num]] = i;
                        counter_group[num]++;
                        if(counter_group[num] == num)
                            create_game(num);
                    }
                }   
            }   
        }   
    }   

    return 0;
}