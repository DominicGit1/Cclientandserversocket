//Dominic Grant
//UH ID 1510819
//Assignment 2
//Ignore the commented out code, I went through a lot of fiddling to make sure
//the code was working as intended
//sources i used all at bottom of code


/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
   gcc server2.c -lsocket
*/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

//void dostuff(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;
     FILE *weather;
     weather = fopen("weather20.txt", "r");
     if (weather == NULL)
	{
		fprintf(stderr, "ERROR, weather file does not exist, or is not in the same registry/folder as the server file\n");
		exit(1);
	}
    char c = getc(weather);
    char returnmess[64];
    int a = feof(weather);
    int numOfChar = 0;
    int highestNumOfChar = 0;
    int numOfLines = 0;
    while(c != EOF && a == 0 )//a initial read of the file to find where the highest number
    {//of uninterrupted characters is
        if(c == '\n')
        {
            numOfLines++;
        }
        if(c == ',')
        {
            if(highestNumOfChar < numOfChar)
            {
                highestNumOfChar = numOfChar;
            }
            numOfChar = 0;
        }
        else
        {
            numOfChar++;
        }
        c = getc(weather);
        a = feof(weather);
    }
    char arr1[numOfLines][highestNumOfChar];//initializing the 2d arrays where i will store the data
    char arr2[numOfLines][highestNumOfChar];
    char arr3[numOfLines][highestNumOfChar];
    fclose(weather);
    weather = fopen("weather20.txt", "r");//close the file than reopen it to go through it a second
    char d = getc(weather);//time where I save the data
    int e = feof(weather);
    int commacheck = 0;
    int lineCheck=0;
    numOfChar=0;
    int abom1[numOfLines];//arrays where I will store the length of the strings that will rest in
    int abom2[numOfLines];//the 2d arrays
    int abom3[numOfLines];
    while(d != EOF && e == 0)//double array that is sorted line by line
    {//while also saving a array of the length of the strings
        if(d==',')//I have code in here that adds anything to the character count except commas
        {//and enters and when the commas/enters happen I tally up the numofchars and reset the
            if (commacheck==0)//counters
            {
                arr1[lineCheck][numOfChar] = '\0';
                abom1[lineCheck] = numOfChar;
                //printf("abom1 %d \n",abom1[lineCheck]);
            }
            if (commacheck==1)
            {
                arr2[lineCheck][numOfChar] = '\0';
                abom2[lineCheck] = numOfChar;
                //printf("abom2 %d \n",abom2[lineCheck]);
            }
            if (commacheck==2)
            {
                arr3[lineCheck][numOfChar] = '\0';
                abom3[lineCheck] = numOfChar;
                //printf("%d ",abom3[lineCheck]);
            }
            
          commacheck++;
          numOfChar = 0;            
        }
        else if(d=='\n')
        {
            if (commacheck != 2)
            {
                printf("ERROR There is something wrong with the formatting of the text document.\n");
                printf("There should only be 3 sections of data separated by 2 commas, the city names, the temp, and the weather.\n");
                exit(1);
            }
            commacheck = 0;
            arr3[lineCheck][numOfChar] = '\0';
            abom3[lineCheck] = numOfChar;
            //printf("abom3 %d \n", abom3[lineCheck]);
            lineCheck++;
            numOfChar = 0;
        }
        else if(commacheck==0)
        {
            arr1[lineCheck][numOfChar] = d;
            numOfChar++;
        }
        else if(commacheck==1)
        {
            if (d==0 && e!='0')
            {
                printf("Error, characters found in temperature section, fix and try again.");
                exit(1);
            }
            arr2[lineCheck][numOfChar] = d;
            numOfChar++;
            if(3 < numOfChar)
            {
                printf("Error, unrealistic temperature detected. Only temperatures with 3 digits allowed.");
                exit(1);
            }
        }
        else if(commacheck==2)
        {
            arr3[lineCheck][numOfChar] = d;
            //printf("%c",arr3[lineCheck][numOfChar]);
            numOfChar++;
        }
        else
        {
            printf("Unknown error happened during the sorting of the data.");
            exit(1);
        }
        d = getc(weather);
        e = feof(weather);
    }
    fclose(weather);
    int i = 0, j=0,m=0,max=0;//testing code which I used to make sure the data was
    for(j=0;j<numOfLines;j++)//being inputted properly. uncomment to get the data printed out 
    {
     for (m = 0; m < sizeof(abom1); m++)
     {
        if (abom1[m] > max)
        { 
            max = abom1[m];
        }
     } 
     for(i = 0; i < max; i++)
    {
        //printf("%c", arr1[j][i]);
        if (arr1[j][i]=='\0')
        {
            break;
        }
    }
    max=0;
    for (m = 0; m < sizeof(abom2); m++)
     {
        if (abom2[m] > max)
        { 
            max = abom2[m];
        }
     } 
    //printf(",");
     for(i = 0; i < max; i++)
    {
        //printf("%c", arr2[j][i]);
        if (arr2[j][i]=='\0')
        {
            break;
        }
    }
    max=0;
    for (m = 0; m < sizeof(abom3); m++)
     {
        if (abom3[m] > max)
        { 
            max = abom3[m];
        }
     } 
    //printf(",");
     for(i = 0; i < max; i++)
    {
       //printf("%c", arr3[j][i]);
        if (arr3[j][i]=='\0')
        {
            break;
        }
    }
    //printf("\n");
    }
    printf("Enter server port number: ");//input server port num
	scanf("%d", &portno);
	if (portno < 1024)
	{
	    fprintf(stderr, "ERROR, invalid port number provided\n");
	    exit(1);
	} 
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
     {
        error("ERROR opening socket");
     }
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
        error("ERROR on binding");
     }
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) 
     {
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
         {
             error("ERROR on accept");
         }
        /* pid = fork();
         if (pid < 0)
         {
             error("ERROR on fork");
         }
         if (pid == 0)
         {
             close(sockfd);
         }*/
         int n;
         int o=0;
         char buffer[128];
      
         bzero(buffer,128);
         n = read(newsockfd,buffer,127);
         if (n < 0) 
             {
                 error("ERROR reading from socket");
             }
         printf("Weather report for %s\n",buffer);//input from client
         int lengthofbuf = strlen(buffer);
         //printf("%d\n",lengthofbuf);
         max=0;
         int i=0;//variables for counting
         int j=0;
         int m=0;
         int z=0;
         int g=0;
         //printf("%d\n",numOfLines);
         while(m<numOfLines)
         {
            //printf("%d a\n",numOfLines);
            if(abom1[m]==lengthofbuf)
            {
                 //printf("%d b\n",numOfLines);
                 while(i<lengthofbuf)
                 {
                    //printf("%d c\n",numOfLines);
                    if(buffer[i]==arr1[m][i])
                    {
                        //printf("%d e\n",numOfLines);
                        z++;
                    }
                    i++;
                 }
                 i=0;
                 if(z==lengthofbuf)
                 {
                     //printf("%d G\n",numOfLines);
                     g=1;
                     break;
                 }
                 else
                 {
                     z=0;
                 }
                 
            }
            m++;
         }
        bzero(returnmess,64);
        int leng=0;
        //printf("%d\n",m);
         if(g==1)//code that after seeing the input was a city name in the data that goes
         {//and puts the temp and weather into two different strings which are outputted
            char check11[abom2[m]];//variation of the previous code i did above
            for(i = 0; i < abom2[m]+1; i++)
            {
                //printf("%s\n",check11);
                if (arr2[m][i]=='\0')
                {
                    check11[i]='\0';
                    break;
                }
                check11[i]=arr2[m][i];
                //printf("%s\n",check11);
            }
            char check2[abom3[m]];
            for(i = 0; i < abom3[m]+1; i++)
            {
                //printf("%s\n",check2);
                if (arr3[m][i]=='\0')
                {
                    //printf("\n\t %s\n",check2);
                    check2[i]='\0';
                    break;
                }
                check2[i]=arr3[m][i];
                //printf("%s\n",check2);
            }
            leng = strlen(check11);
            int lengt = strlen(check2);
            bzero(returnmess,64);
            sprintf(returnmess,"%d.%s.%s",leng, check11, check2);
            printf("Tomorrow's maximum temperature is %s F\n",check11);
            printf("Tomorrow's sky condition is %s \n",check2);
            bzero(check2,lengt);
            bzero(check11,leng);
         }
         else//if name not found send no data message back
         {
             int hh = 0;
           sprintf(returnmess,"%d",hh);  
           printf("No Data\n");
         }
               
        n = write(newsockfd, returnmess, strlen(returnmess));
        close(newsockfd);
        //exit(0);
         /* end of while */
    }
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

//Ignore, test and useless code

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
/*void dostuff (int sock, int lines, int abom1[], int abom2[], int abom3[], char arr1[][],char arr2[][], char arr3[][])
{
   int n;
   int o=0;
   char buffer[128];
      
   bzero(buffer,128);
   n = read(sock,buffer,127);
   if (n < 0) error("ERROR reading from socket");
   printf("Weather report for %s\n",buffer);
   int i = 0, j=0,m=0,max=0;
    for(j=0;j<lines;j++)
    {
     for (m = 0; m < sizeof(abom1); m++)
     {
        if (abom1[m] > max)
        { 
            max = abom1[m];
        }
     }
     char check[max]; 
     for(i = 0; i < max; i++)
     {
        strcat(check, tolower(arr1[j][i]));
        if (arr1[j][i]=='\0')
        {
            break;
        }
     }
     if(strcmp(check,buffer)==0)
     {
        o=1;
        break;
     }
    }
    if(o==1)
    {
        max=0;
        for (m = 0; m < sizeof(abom2); m++)
        {
            if (abom2[m] > max)
            { 
                max = abom2[m];
            }
        }
        char check1[max]; 
        for(i = 0; i < max; i++)
        {
            printf("%c", arr2[j][i]);
            if (arr2[j][i]=='\0')
            {
                break;
            }
        }
        max=0;
        for (m = 0; m < sizeof(abom3); m++)
        {
            if (abom3[m] > max)
            { 
                max = abom3[m];
            }   
        }
        char check2[max]; 
        for(i = 0; i < max; i++)
        {
            printf("%c", arr3[j][i]);
            if (arr3[j][i]=='\0')
            {
                break;
            }
        }
        sprintf(buffer,"Tomorrow's maximum temperature is %s F\nTomorrow's sky condition is %s\n",check1,check2);
        n = write(sock,buffer,strlen(buffer));
        if (n < 0) 
        {
            error("ERROR writing to socket");
        }
    }
    else
    {
       n = write(sock,"No Data",7);
       if (n < 0) 
       {
            error("ERROR writing to socket");
       }
    }
}*/