#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include<time.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <signal.h>

#define DEFAULT_SERVER_PORT 8080 
#define LOCAL_HOST "127.0.0.1"
#define BROADCAST_ADDRESS "127.255.255.255"

#define MAX_PLAYER 8
#define MAX_GAME_SIZE 9
#define MAX_CELLS (MAX_PLAYER + 1) * (MAX_PLAYER + 1)

int server_sock;
int server_port;

int game_sock;
int game_port;
int game_size;
int number_of_moves;
int cell_count;
int player_count;
int player_id;

int is_score;
int turn;
int player_score[MAX_PLAYER];
int cell_fillng_edge[MAX_GAME_SIZE][MAX_GAME_SIZE][2];
char table[MAX_CELLS][MAX_CELLS];
char player_symbols[MAX_PLAYER] = ",@#$,,,,";

struct sockaddr_in addr;
int addrlen;

char print_buffer[1024];
int is_time_out;

void sig_handler(int signum)
{
    sprintf(print_buffer, "TimeOut! Turn is Over\n\n");
    write(1, print_buffer, strlen(print_buffer));
    is_time_out = 1;
}

void DieWithHonor(char* error_message)
{
    // perror(error_message);
    sprintf(print_buffer, "Error:%s \n\n", error_message);
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

void connect_to_server()
{
    struct sockaddr_in addr22; 
    char buffer[1024] = {0}; 

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        DieWithHonor("Socket Creation Failed"); 
    sprintf(print_buffer, "Socket Created Sucessfully\n");
    write(1, print_buffer, strlen(print_buffer));
   
    addr22.sin_family = AF_INET; 
    addr22.sin_port = htons(server_port); 
    if(inet_pton(AF_INET, LOCAL_HOST, &addr22.sin_addr)<=0)  
        DieWithHonor("Can not Convert Address to binary"); 
   
    if (connect(server_sock, (struct sockaddr *)&addr22, sizeof(addr22)) < 0) 
        DieWithHonor("Connection Failed"); 
    sprintf(print_buffer, "Clinet Connected to Server\n");
    write(1, print_buffer, strlen(print_buffer));

    if (read( server_sock , buffer, 1024) == 0)
        DieWithHonor("Can not Read from Server"); 
    sprintf(print_buffer, "Greeting Message from Server: %s\n\n", buffer);
    write(1, print_buffer, strlen(print_buffer)); 
}

void get_game_info()
{
    player_count = (rand()%3) + 2;
    player_count = 3;
    game_size = player_count + 1;
    number_of_moves = game_size * (game_size - 1) * 2;
    cell_count = (game_size - 1) * (game_size - 1);

    send_int_to_socket(server_sock, player_count);
    sprintf(print_buffer, "Player Count (%d Players Group) Send to Server\n\n", player_count);
    write(1, print_buffer, strlen(print_buffer)); 

    game_port = read_int_from_socket(server_sock);
    player_id = read_int_from_socket(server_sock);

    sprintf(print_buffer, "### Game Info ###\n");
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "PlayerCount: %d\n", player_count);
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "GamePort: %d\n", game_port);
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "Number Of total Moves: %d\n", number_of_moves);
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "PlayerID: %d\n", player_id);
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "PlayerSymbol: %c\n", player_symbols[player_id]);
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "###\n\n");
    write(1, print_buffer, strlen(print_buffer));
}

int is_valid(int ci, int cj, int dir)
{
    if (ci < 0 || ci >= game_size || 
        cj < 0 || cj >= game_size ||
        dir < 0 || dir > 1)
        return 0;
    if (ci == game_size-1 && dir == 1)
        return 0;
    if (cj == game_size-1 && dir == 0)
        return 0;
    if (cell_fillng_edge[ci][cj][dir] == 1)
        return 0;
    return 1;
}

void fill_edge(int player_id, int ci, int cj, int dir)
{
    if (is_valid(ci, cj, dir) == 0)
    {
        sprintf(print_buffer, "!@# Filling Edge Error ci:%d cj:%d dir:%d\n", ci, cj, dir);
        write(1, print_buffer, strlen(print_buffer));
        return;
    }
    cell_fillng_edge[ci][cj][dir] = 1; 
    if (dir == 0)
        table[ci*2][cj*2 + 1] = player_symbols[player_id];
    else if (dir == 1)
        table[ci*2 + 1][cj*2] = player_symbols[player_id];
}

int fill_cell(int ci, int cj, char c)
{
    if (ci < 0 || ci >= game_size-1 ||
        cj < 0 || cj >= game_size-1)
        return 0;
    if (cell_fillng_edge[ci][cj][0]==1 &&
        cell_fillng_edge[ci][cj][1]==1 &&
        cell_fillng_edge[ci+1][cj][0]==1 &&
        cell_fillng_edge[ci][cj+1][1]==1)
    {
        table[ci * 2 + 1][cj * 2 + 1] = c;
        return 1;
    }
    return 0;
}

int create_move(int player_id, int ci, int cj, int dir)
{
    int move = 0; 
    // ABCD-> A: Player ID, B: Cell Corner i, C: Cell Corner j, D : edge direction
    move = player_id;
    move *= 10;
    move += ci;
    move *= 10;
    move += cj;
    move *= 10;
    move += dir;
    return move;
}

int create_random_move(int player_id)
{
    int find_move = 0;
    int ci, cj;
    int dir;
    for (int i = 0; i < 1000 && (find_move == 0); i++)
    {
        ci = rand() % game_size;
        cj = rand() % game_size;
        dir = rand() % 2;
        if (is_valid(ci, cj, dir) != 0)
            find_move = 1;
    }
    if (find_move == 0)
        DieWithHonor("Cannot Find Proper Move");
    return create_move(player_id, ci, cj, dir);    
}

int find_first_possible_move(int player_id)
{
    for (int i=0; i<game_size; i++)
    {
        for (int j=0; j<game_size; j++)
        {
            for (int d=0; d<2; d++)
            {
                if (is_valid(i, j, d) != 0)
                    return create_move(player_id, i, j, d);
            }
        }
    }
    return 0;
}

int play_move(int move)
{
    int player_id, ci, cj, dir;
    dir = move%10;
    move/=10;
    cj = move%10;
    move/=10;
    ci = move%10;
    move/=10;
    player_id = move;

    fill_edge(player_id, ci, cj, dir);
    int score1 = fill_cell(ci, cj, '0' + player_id);
    int score2;
    if (dir==0)
        score2 = fill_cell(ci-1, cj, '0' + player_id);
    else
        score2 = fill_cell(ci, cj-1, '0' + player_id);

    if (score1 != 0 || score2 != 0)
    {
        is_score = 1;
        player_score[player_id] += score1 + score2;
    }
}

void connect_to_game()
{
    if ((game_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        DieWithHonor("Game Socket Creation Failed"); 
    sprintf(print_buffer, "Game Socket Created Sucessfully\n");
    write(1, print_buffer, strlen(print_buffer));
   
    int broadcastEnable = 1, reuseEnable = 1;
    if (setsockopt(game_sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)) < 0)
        DieWithHonor("Can not enable broadcast");
    if (setsockopt(game_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseEnable, sizeof(reuseEnable)) < 0)
        DieWithHonor("Can not enable reuse");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(game_port); 
    addr.sin_addr.s_addr = inet_addr("127.255.255.255");
    addrlen = sizeof(addr);

    if (bind(game_sock, (struct sockaddr*)&addr, (socklen_t)addrlen) < 0)
        DieWithHonor("Binding Game Socket is Failed");  
    sprintf(print_buffer, "Bind Port to Game Socket Is Completed\n\n");
    write(1, print_buffer, strlen(print_buffer));
}

void print_table(int turn)
{
    turn += 1;
    sprintf(print_buffer, "## Game in Turn %d ##\n\n", turn);
    write(1, print_buffer, strlen(print_buffer));
    for (int i = 0; i < game_size*2 - 1; i++)
    {
        for (int j = 0; j < game_size*2 -1; j++)
        {
            sprintf(print_buffer, "%c", table[i][j]);
            write(1, print_buffer, strlen(print_buffer));
        }
        sprintf(print_buffer, "\n");
        write(1, print_buffer, strlen(print_buffer));
    }
    sprintf(print_buffer, "\n");
    write(1, print_buffer, strlen(print_buffer));
}

void init_table()
{
    for (int i = 0; i < game_size*2 - 1; i++)
    {
        for (int j = 0; j < game_size*2 -1; j++)
        {
            table[i][j] = ' ';
            if (i%2 == 0 && j%2 == 0)
                table[i][j] = '.';
        }
    }
}

void play_game()
{
    init_table();

    sprintf(print_buffer, "############## Game STARTED! ##############\n\n");
    write(1, print_buffer, strlen(print_buffer));
    int player_turn = 1;

    for(int i = 0; i < number_of_moves; i++)
    {
        is_score = 0;
        if (player_turn == player_id)
        {
            sleep(2);
            int move = create_random_move(player_turn);
            if (sendto(game_sock, &move, sizeof(move), 0, (struct sockaddr *)&addr, addrlen) != sizeof(move))
            {
                // perror("Cannot Send Move to BroadCast");
                sprintf(print_buffer, "Error: Cannot Send Move to BroadCast \n\n");
                write(1, print_buffer, strlen(print_buffer));
            }
            else
            {
                sprintf(print_buffer, "Move(%d) Sent to Group\n\n", move);
                write(1, print_buffer, strlen(print_buffer));
            }
        }
        sprintf(print_buffer, "Waiting to get Move\n");
        write(1, print_buffer, strlen(print_buffer));
        alarm(20);
        
        while(1)
        {
            fd_set fds_list;
            FD_ZERO(&fds_list);    
            FD_SET(game_sock, &fds_list);
            int max_sock_num = game_sock;

            int event = select(max_sock_num+1, &fds_list, NULL, NULL, NULL);
            
            if ((event < 0) && (errno != EINTR))   
            {
                sprintf(print_buffer, "Select is Failed Successfully!\n");   
                write(1, print_buffer, strlen(print_buffer));;
                continue;
            }

            if (is_time_out == 0 && FD_ISSET(game_sock, &fds_list) == 0)
                continue;
            
            int move;
            if (is_time_out == 1 || recv(game_sock, &move, sizeof(move), 0) == -1)
            {
                // perror("Cannnot Read From Broadcast\n");
                sprintf(print_buffer, "Error: Cannnot Read From Broadcast \n\n");
                write(1, print_buffer, strlen(print_buffer));

                move = find_first_possible_move(player_turn);
                sprintf(print_buffer, "Consider First Possible Move(%d)\n\n", move);
                write(1, print_buffer, strlen(print_buffer));
            }
            else
            {
                sprintf(print_buffer, "Move Read From BroadCast: %d\n\n", move);
                write(1, print_buffer, strlen(print_buffer));
            }
            play_move(move);
            is_time_out = 0;
            alarm(20);
            if (is_score == 0)
            {
                if (player_turn == player_count)
                    player_turn = 1;
                else
                    player_turn++;
            }

            print_table(i);
            break;
        }
    }
}

void print_results()
{
    sprintf(print_buffer, "#################### Game Finished! ####################\n\n");
    write(1, print_buffer, strlen(print_buffer));

    int winner = 0;
    int max_score = -1;

    sprintf(print_buffer, "Scores\n");
    write(1, print_buffer, strlen(print_buffer));
    for (int i = 1; i <= player_count; i++)
    {
        if (max_score < player_score[i])
        {
            max_score = player_score[i];
            winner = i;
        }
        sprintf(print_buffer, "Player %d: %d\n", i, player_score[i]);
        write(1, print_buffer, strlen(print_buffer));       
    }
    sprintf(print_buffer, "\n");
    write(1, print_buffer, strlen(print_buffer));
    sprintf(print_buffer, "*********** WINNER IS PLAYER %d ***********\n\n", winner);
    write(1, print_buffer, strlen(print_buffer));
}

int main(int argc, char const *argv[]) 
{ 
    setvbuf(stdout, NULL, _IONBF, 0);
    server_port = DEFAULT_SERVER_PORT;
    if (argc == 2)
        server_port = atoi(argv[1]);
    signal(SIGALRM, sig_handler);

    srand(time(NULL));

    connect_to_server();
    get_game_info();
    
    sleep(2);

    connect_to_game();

    sleep(2);

    play_game();

    sleep(2);

    print_results();

    return 0; 
} 
