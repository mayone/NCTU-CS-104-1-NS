#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int menu();
void game();
void leak();
void initmap();
void showmap();
void setmap();
void thread();
void handler();
void addItem();
int moveItem();
char **map;
float timeslot;
int speed;
int row;
int col;
int running;
int score;
char sh[3] = "sh";
char *dir[37] = {"phddaa","RSA","encrypt","plaintext",\
        "cipher","crypto","AES","key","symmetric","malware",\
        "DSNS","DES","random","RC4","SSL","TLS","WEP",\
        "WPA","NIST","ECB","integrity","attack","replay",\
        "DoS","Network","Security","overflow","HASH","XOR",\
        "MD5","SHA1","python","publicKey","MITM","Diffie",\
        "kerberos","vulerable"}; 

struct node {
    int valid;
    int x;
    int y;
    char name[20];
};

struct list {
    int maxIndex;
    struct node nodes[10];
};

struct list *itemlist;

void magic()
{
    printf("Congratulation! Here's your second flag\n");
    system("cat /home/magictype/flag2");
}

void Congrats()
{
    printf("Congrats! You beat Magic Type!\n");
    system("cat /home/magictype/flag1");
}

int main()
{
    setvbuf(stdout,0,2,0);
    printf("Welcome to the Magic Type!\n");
    while(1)
    {
        switch(menu())
        {
            case 1:
                game();
                break;
            case 2:
                leak();
                break;
            case 3:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("It's not a choice! Idoit!\n");
                break;
        }
    }

}

int menu()
{
    char choice[10];
    printf("=====Magic Type Menu=====\n");
    printf("1.Start a Game\n");
    printf("2.Get Information\n");
    printf("3.Exit\n");
    read(0, choice, 10);
    return atoi(choice);
}

/****************buffer overflow here****************/
void game()
{
    printf("Let's start a game!\n");
    printf("You have to type every words you see\n");
    printf("If any word drops to the floor, you will lose!\n");
    int i=0;
    char s[20];
    int ret;
    score = 0;
    pthread_t id,receiver;
    unsigned seed;
    seed = (unsigned) time(NULL);
    srand(seed);
    initmap();
    itemlist = (struct list*) malloc(sizeof(struct list));
    memset(itemlist,0,sizeof(struct list));
    itemlist->maxIndex = 10;
    printf("Choose the speed level(1-9):\n");
    scanf("%s",s);
    speed = atoi(s);
    if(speed < 1 || speed > 9)
        return;
    timeslot = 30000+(10-speed)*50000;
    running = 1;
    ret = pthread_create(&id, NULL, (void *)thread, NULL);
    if(ret != 0)
    {
        printf("create pthread error\n");
        return;
    }
    ret = pthread_create(&receiver, NULL, (void *)handler, NULL);
    if(ret != 0)
    {
        printf("create pthread error\n");
        return;
    }
    pthread_join(id, NULL);
    pthread_join(receiver, NULL);
    printf("Game over!\n");
    if(score >= 3000)
        Congrats();
    return;
}

void thread()
{
    int addNew = 7;
    while(running == 1){
        if(++addNew == 8)
            addItem();
        addNew %= 8;
        if(!moveItem()){
            running = 0;
            break;
        }
        setmap();
        printf("Score: %d\n", score);
        showmap();
        usleep(timeslot);
    }
}   

/****************compare the strings here****************/
void handler()
{
    while(running == 1)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        if(select(1, &read_fds, NULL, NULL, &timeout) == 1)
        {
            char buf[1024];
            int i;
            int index = 0;
            read(0, buf, 1024);
            if(strchr(buf,'\n'))
                index = strchr(buf,'\n') - buf;
            buf[index] = '\0';
            for(i=0 ; i<itemlist->maxIndex ; i++){
                if(itemlist->nodes[i].valid == 1){
                    if(!strncmp(buf,itemlist->nodes[i].name,3)){
                        itemlist->nodes[i].valid = 0;
                        if(speed == 9)
                            score += 100;
                        else if(speed == 8)
                            score += 85;
                        else
                            score += speed*strlen(buf);
                    }
                }
            }
        }
    }
}

void addItem()
{
    int i;
    for(i=0 ; i<itemlist->maxIndex ; i++)
    {
        if(itemlist->nodes[i].valid == 0)
        {
            itemlist->nodes[i].valid = 1;
            itemlist->nodes[i].y = -2;
            itemlist->nodes[i].x = (rand()%4)*10 + 4;
            int index = (int)rand()%36;
            strcpy(itemlist->nodes[i].name, dir[index]);
            break;
        }
    }
}

int moveItem()
{
    int i;
    for(i=0 ; i<itemlist->maxIndex ; i++)
        if(itemlist->nodes[i].valid == 1)
            if(itemlist->nodes[i].y++ == row-3)
                return 0;
    return 1;
}

void leak()
{
    int a;
    printf("What information do you want?\n");
    read(0, &a, 4);
    puts((char*)a);
}

void setmap()
{
    initmap();
    int i,j;
    for(j=0 ; j<itemlist->maxIndex ; j++)
    {
        if(itemlist->nodes[j].valid == 1)
        {
            int x = itemlist->nodes[j].x;
            int y = itemlist->nodes[j].y+1;
            char name[20];
            strcpy(name, itemlist->nodes[j].name);
            int len = strlen(name);
            if(y >= 0)
                for(i=x-1 ; i<x+len+1 ; i++)
                    map[y+1][i] = '-';
            if(y >= 1){
                for(i=x ; i<x+len ; i++)
                    map[y][i] = name[i-x];
                map[y][x-1] = '|';
                map[y][x+len] = '|';
            }
            if(y >= 2)
                for(i=x-1 ; i<x+len+1 ; i++)
                    map[y-1][i] = '-';
        }
    }
}

void initmap()
{
    row = 42;
    col = 48;
    int i,j;
    map = (char **)malloc(row*sizeof(char*));
    for(i=0 ; i<row ; i++)
    {
        *(map+i) = (char*)malloc(col*sizeof(char));
    }
    for(i=0 ; i<row ; i++)
    {
        for(j=0 ; j<col ; j++)
        {
            if(i == 0 || i == row-1)
                map[i][j] = '-';
            else if(j == 0 || j == col-1)
                map[i][j] = '|';
            else
                map[i][j] = ' ';
        }
    }
}

void showmap()
{
    int i,j,k;
    char *temp = (char *)malloc(row*(col+1));
    k=0;
    for(i=0 ; i<row ; i++)
    {
        for(j=0 ; j<col ; j++){
            temp[k] = map[i][j];
            k++;
        }
        temp[k] = '\n';
        k++;
    }
    puts(temp);
}
