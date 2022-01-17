#include<stdio.h>   //fgets()
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>  //sockaddr_in
#include<ctype.h>

char * Decrypter(char *str, int key)
{
    for(int i =0; i< strlen(str); i++)
    {
        if(str[i]>=' ' && str[i]<='/')
        {
            if(str[i]-key < ' ')
                str[i] = str[i] - key + 16;
            
            else
                str[i] = str[i] - key;

        }            
        else if(str[i]>='a' && str[i] <='z')
        {
            if(str[i]-key < 'a')
                str[i] = str[i] - key + 26;
            
            else
                str[i] = str[i] - key;
        }
        else if(str[i]>='A' && str[i] <='Z')
        {
            if(str[i]-key < 'A')
                str[i] = str[i] - key + 26;
            
            else
                str[i] = str[i] - key;
        }

        else if(str[i]>='0' && str[i]<='9')
        {
            if(str[i]-key<'0')
                str[i] = str[i] - key + 10;
            else
                str[i] = str[i] - key;
        }

    }
    // printf("\nEncrpted key is : %s\n",str);
    return str;


}

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
    int words=0,key;
    char tempChar,tempWord,*encrypt,*decrypt;

    fp = fopen(file, "a+");
    if(fp == NULL)
    {
        perror("Error Opening file");
        return -1;
    }

    read(newsockfd, &words, sizeof(int));
    read(newsockfd,&key,sizeof(int));

    while(ch!= words){
        read(newsockfd, buffer, 255);
        fprintf(fp, "%s", buffer);
        ch++;
    }
    rewind(fp);
    printf("\nThe file has been received successfully. It is saved by the name %s\n",file);


    FILE * df;
    df = fopen("decryptedFile.txt","a");
    if(df == NULL)
    {
        perror("Error Opening file");
        return -1;
    }

    // saving the recoded data
    words  = 0;
    while((tempChar = fgetc(fp)) != EOF)
	{
		fscanf(fp,"%s",buffer);
		if(isspace(tempChar) || tempChar=='\t')
		words++;
	}
    words+=1;
    rewind(fp);
    
    fprintf(df,"\nThe Decryped message is: ");
    // Decrypting the file onto another file

    while(tempWord != EOF)
	{
		fscanf(fp,"%s", buffer);
        decrypt = Decrypter(buffer,key);
        fprintf(df,"%s",decrypt);
        printf(" %s ",decrypt);
		tempWord = fgetc(fp);
	}    
    
    fclose(df);
    fclose(fp);
    close(newsockfd);
    close(sockfd);

    return 0;

}