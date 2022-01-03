#include<stdio.h>   //fgets()
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>  //sockaddr_in

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Port no.not provided. Program terminated\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;   // socklen_t is a data type in socket.h it is 32 bit data type

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error Opening Socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));   //Typecasting from sockaddr_in to char
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);     //host to network short

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))< 0){
        error("Binding failed.");
    }

    listen(sockfd, 5);   //gives the maximum no. of client that can listen to the function
    clilen = sizeof(cli_addr);
 
    
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

    if(newsockfd<0){
        error("Error on Accept");
    }

    FILE *fp;

    int ch = 0;
    char file[] = "glad_received.txt";
    fp = fopen(file, "a");
    int words;

    read(newsockfd, &words, sizeof(int));

    while(ch!= words){
        read(newsockfd, buffer, 255);
        fprintf(fp, "%s ", buffer);
        ch++;
    }
    printf("\nThe file has been received successfully. It is saved by the name %s\n",file);



    close(newsockfd);
    close(sockfd);

    return 0;

}