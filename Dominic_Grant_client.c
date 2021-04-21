//Dominic Grant
//UH ID 1510819
//Assignment 2
//Ignore the commented out code, I went through a lot of fiddling to make sure
//the code was working as intended
//list of sources I used in the process of making this at bottom of code
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[128];
    /*if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }*/
    printf("Enter the server host name: ");
	char host[64];
	scanf("%s", host);
	server = gethostbyname(host);
    if (server == NULL) //server name input
    {
        printf("ERROR, no such host\n");
        exit(1);
    }
    printf("Enter the server port number: ");
	scanf("%d", &portno);
    if (portno < 1024)//server port input
	{
        printf("ERROR, invalid port number provided\n");
		exit(1);
	} 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    {
        error("ERROR connecting");
    }
    printf("Enter a city name: ");//city input
    bzero(buffer,128);
    int a;
    int i = 0;
    int j = 0;
    while ((a = getchar()) != '\n' && (a != EOF));
    fgets(buffer,127,stdin);
    buffer[strlen(buffer) - 1] = '\0';
    n = write(sockfd,buffer,strlen(buffer));//sends the city name over to the server
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,128);
    n = read(sockfd,buffer,127);//get results back
    if (n < 0) 
         error("ERROR reading from socket");
    //printf("%s\n",buffer);
    char check1[strlen(buffer)];//start of the separation of the results
    char check2[strlen(buffer)];
    //char checking = buffer[0];
    //printf("This is whats checking: %c",checking);
    if(buffer[0] == '0')//if nothing then say No Data
    {
        printf("No Data\n");
    }
    else
    {
        int split = atoi(&buffer[0]);//splits result up in temperature and weather description
        //printf("%d splitnum\n",split);
        for(j=0;j<split;j++)
        {
            check1[j]=buffer[j+2];
            //printf("%s for check1\n",check1);
        }
        check1[j+1]='\0';
        //printf("%s for check1\n",check1);
        for(i=0;i<strlen(buffer);i++)
        {
            check2[i]=buffer[i+j+3];
            //printf("%s for check2\n",check2);
        }
        printf("Tomorrow's maximum temperature is %s F\n",check1);
        printf("Tomorrow's sky condition is %s \n",check2);//print out and end of program
    }
    close(sockfd);
    return 0;
}
/*
https://cboard.cprogramming.com/c-programming/101121-passing-arg-1-%60atoi-makes-pointer-integer-without-cast-please-help.html
https://stackoverflow.com/questions/15577124/strange-character-after-an-array-of-characters
https://stackoverflow.com/questions/1559487/how-to-empty-a-char-array
https://www.codeproject.com/Questions/1266041/My-C-program-returns-error-Warning-initialization
https://www.programiz.com/c-programming/library-function/string.h/strlen
https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
https://stackoverflow.com/questions/4955198/what-does-dereferencing-a-pointer-mean
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/
http://www.cplusplus.com/reference/cstring/
https://stackoverflow.com/questions/18797943/format-s-expects-argument-of-type-char-but-argument-2-has-type-char
https://stackoverflow.com/questions/27345922/concatenating-two-2d-char-arrays
https://stackoverflow.com/questions/19641597/what-is-segmentation-fault-core-dumped
http://www.cplusplus.com/reference/cstring/strcmp/
https://www.educative.io/edpresso/what-is-the-toupper-function-in-c
https://stackoverflow.com/questions/308695/how-do-i-concatenate-const-literal-strings-in-c
https://www.tutorialspoint.com/learn_c_by_examples/program_to_print_array_in_c.htm
https://stackoverflow.com/questions/15192221/concatenating-array-of-integers-in-c
https://overiq.com/c-programming-101/array-of-strings-in-c/
https://www.programiz.com/c-programming/library-function/string.h/strcat
https://www.programiz.com/c-programming/c-strings
https://fresh2refresh.com/c-programming/c-printf-and-scanf/
https://www.geeksforgeeks.org/eof-and-feof-in-c/
https://www.geeksforgeeks.org/arrays-in-c-cpp/
https://stackoverflow.com/questions/5791117/how-to-detect-a-new-line-of-any-file-using-c-program
https://en.cppreference.com/w/c/io/fgetc
http://www.cplusplus.com/doc/tutorial/files/
https://www.dreamincode.net/forums/topic/26805-how-to-sort-lines-taken-from-a-textfile/
*/