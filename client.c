// filename server_ipaddress portno
// argv[0] filename 
// argv[1] server_ipaddress
// argv[2] portno

//./server 9898  
//./client 127.0.0.1 9898

#include<stdio.h>   //fgets()
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>  //sockaddr_in
#include<netdb.h>   //define the hostent structure
#include<ctype.h>	//isspace

char * caeserCypher(char  *str,int key){
    // char str[maxlength];
    

    // printf("Enter Text: ");
    // fgets(str,maxlength,stdin);

    for(int i =0; i< strlen(str); i++)
    {
        if(str[i]==' ')
            continue;
        else if(str[i]>='a' && str[i] <='z')
        {
            if(str[i]+key > 'z')
                str[i] = str[i] +key - 26;
            
            else
                str[i] = str[i] + key;
        }
        else if(str[i]>='A' && str[i] <='Z')
        {
            if(str[i]+key > 'Z')
                str[i] = str[i] +key - 26;
            
            else
                str[i] = str[i] + key;
        }

        else if(str[i]>='0' && str[i]<='9')
        {
            if(str[i]+key>'9')
                str[i] = str[i] + key - 10;
            else
                str[i] = str[i]+ key;
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
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    if(argc < 3){
        fprintf(stderr,"usage %s host name port \n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("ERROR opening socket");
    }
    // # gethost
    server = gethostbyname(argv[1]);
    if(server == NULL)
    {
        fprintf(stderr,"Error, no such host");
    }
    bzero((char *)&serv_addr ,sizeof(serv_addr));
    serv_addr.sin_family  = AF_INET;
    bcopy((char *)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // connect
    if(connect(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr))< 0)
        error("Connection failed");
    
    bzero(buffer, 255);
	
	FILE *f;
	int words = 0,key;
	char c,ch,*encrypt;
    char file_name[]= "glad.txt";

    // printf("Enter the name of file you want to send to the server: ");
    // fgets(file_name,30,stdin);
    // printf("%s\n",file_name);

	f = fopen(file_name, "r");
    if(f == NULL)
    {
        perror("Error Opening file");
        return -1;
    }

    printf("enter the key for Encryption: ");
    scanf("%d", &key);
	while((c = getc(f)) != EOF)
	{
		fscanf(f,"%s",buffer);
		if(isspace(c) || c=='\t')
		words++;
	}
    words+=1;
	write(sockfd, &words, sizeof(int));
	rewind(f);  //to point the file pointer at the start of the file
	printf("\nThe encrypted string is: ");
	while(ch != EOF)
	{
		fscanf(f,"%s", buffer);
        encrypt = caeserCypher(buffer,key);
        printf(" %s ",encrypt);
		write(sockfd, encrypt, 255);
		ch = fgetc(f);
	}
	printf("\nThe file has been successfully sent.Thank you\n");

    bzero(buffer, 255);

    close(sockfd);
    return 0;
}