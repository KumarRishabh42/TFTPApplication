/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include<time.h>
#include <sys/types.h>
#include <regex.h>
#include <openssl/md5.h>


int sock, connected, bytes_recieved , true = 1;  

char resolved_path[1000];
char command[40][200];
int comlen=0;

typedef struct checkarr{

        char name[100];
        char checksum[200];





}checkarr;


checkarr check[100];
int countcheck=0;
void IndexGet_ShortList();
void IndexGet_LongList();
void IndexGet_RegEx();
void find_md5_hashvalue();
void findall_md5_hashvalue();


char compute_checksum[2*MD5_DIGEST_LENGTH+1];




////////////////////////****************functions start*************************************************/

void parseData(){
	printf("reached here atleast\n");
	int i;
	for(i=0;i<comlen;++i){
		printf("yaada yaada yaada %s\n",command[i]);
	}
	if(strcmp(command[0],"IndexGet") == 0 && strcmp(command[1],"ShortList") == 0)
	{
		printf("yaha bhi aaya\n");
//		strcpy(start_t , command[2]);
//		strcpy(end_t , command[3]);
		IndexGet_ShortList();
	}

	else if(strcmp(command[0],"IndexGet") == 0 && strcmp(command[1],"LongList") == 0)
	{
		printf("long list getting called from parsedata\n");
		IndexGet_LongList();
	}


	else if(strcmp(command[0],"IndexGet") == 0 && strcmp(command[1],"RegEx") == 0)
	{
	//	strcpy(start_t , command[2]);
		IndexGet_RegEx();
	}

	else if(strcmp(command[0],"FileHash") == 0 && strcmp(command[1],"Verify") == 0)
	{
	//	char file_name[100];
	//	strcpy(file_name , command[2]);
		find_md5_hashvalue();
	}

	else if(strcmp(command[0],"FileHash") == 0 && strcmp(command[1],"CheckAll") == 0)
	{
	//	flag = 1 ; 	
	//	char *file_na = NULL;
		findall_md5_hashvalue();
	//	flag = 0 ; 
	}	
	


}

void IndexGet_ShortList(){
	DIR *dp;
       	struct dirent *ep;
	struct stat     statbuf;
	char folder[100];
	char *j;
	char *outString = (char *)malloc(sizeof(char)*1000);
	printf("reached here \n");
	dp = opendir ("shared");
	chdir("shared");
	if (dp != NULL){
		
	   printf("came here too\n");
           while (ep = readdir (dp)){

	     int flag=0;
             j=ep->d_name;
	     stat(j, &statbuf);
	     char t[100]= "";	
	     strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
	     printf("last modified time is %s ",t);
	     if(strcmp(t,command[2])>=0 && strcmp(t,command[3])<=0){
		
	     size_t maxsize=100;
	     if(S_ISDIR(statbuf.st_mode)) 
		{
			/* Found a directory, but ignore . and .. */
			if(strcmp(".",j) == 0 || strcmp("..",j) == 0)
			{
				continue;
			}

			strcpy(outString,"");
			//strcat(outString," ");
			strcat(outString,j);
			//strcat(outString,"\n");
			strcat(outString," ");	
			printf("%s ",outString);
			send(connected,outString,strlen(outString), 0);

			printf("DIR ");
			send(connected,"DIR ",4, 0);


			flag=1;
			//longList(entry->d_name,depth+1);
			
		}
		else 
		{
			strcpy(outString,"");
			//char outString[100];
			strcat(outString,j);
			strcat(outString," ");
			send(connected,outString,strlen(outString), 0);


			//strcat(outString,"\n");
			printf("%s ",outString);
			printf("FILE ");
			send(connected,"FILE ",5, 0);


			//printf("%*s%s\n",spaces,"",entry->d_name);
		}
		if (stat(j, &statbuf)!=-1)
		{  
			//printf("came here\n");	
			strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
			//printf("last modified time is %s ",t);
			if(strcmp(t,command[2])>=0 && strcmp(t,command[3])<=0){
			//int ans=cmp("28/03/2013 10:50:56",t, "30/03/2013 00:22:52");	
		
					printf("Last modified date and time = %s ", t);
					char ssss[100];
					strcpy(ssss,"last modified string is ");
					strcat(ssss,t);
					strcat(ssss," ");
					send(connected,ssss,strlen(ssss), 0);
		

			}
			char sssss[100];
			int size = statbuf.st_size;
			char new[10];
			sprintf(new,"%d",size);
			strcpy(sssss,"Size in bytes is ");

			strcat(sssss,new);
			strcat(sssss,"\n");
			send(connected,sssss,strlen(sssss), 0);

		printf("Size is %d bytes ",size);


		}

		printf("\n");
		}
	     //if(!is_dir(j)){
			
	//		printf("%s\n",j);
	  //   }
             //sprintf(folder,"C:/folder/%s",j);
             //printf("%s\n",j);
           }

           (void) closedir (dp);
         }
       	else{
         perror ("Couldn't open the directory");
       	}
	send(connected,"end",3,0);	

}





void IndexGet_LongList(){
	printf("welcome to longlist\n");
	DIR *dp;
       	struct dirent *ep;
	struct stat     statbuf;
	char folder[100];
	char *j;
	char *outString = (char *)malloc(sizeof(char)*1000);
	printf("reached here \n");
	dp = opendir ("shared");
	chdir("shared");
	if (dp != NULL){
		
	   printf("came here too\n");
           while (ep = readdir (dp)){

	     int flag=0;
             j=ep->d_name;
	     printf("j is %s\n",j);
	     stat(j, &statbuf);
	     char t[100]= "";	
	     size_t maxsize=100;
	     if(S_ISDIR(statbuf.st_mode)) 
		{
			/* Found a directory, but ignore . and .. */
			if(strcmp(".",j) == 0 || strcmp("..",j) == 0)
			{
				continue;
			}

			strcpy(outString,"");
			//strcat(outString," ");
			strcat(outString,j);
			strcat(outString," ");	
			//strcat(outString,"\n");
			printf("%s ",outString);
			send(connected,outString,strlen(outString), 0);
			printf("DIR ");
			
			send(connected,"DIR ",4, 0);

			flag=1;
			//longList(entry->d_name,depth+1);
			
		}
		else 
		{
			strcpy(outString,"");
			//char outString[100];
			strcat(outString,j);
			strcat(outString," ");
			//strcat(outString,"\n");
			printf("%s ",outString);
			send(connected,outString,strlen(outString), 0);
		
			printf("FILE ");
			send(connected,"FILE ",5, 0);

			//printf("%*s%s\n",spaces,"",entry->d_name);
		}
		if (stat(j, &statbuf)!=-1)
		{  
			//printf("came here\n");	
			strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
			printf("last modified time is %s ",t);
			char ssss[100];
			strcpy(ssss,"last modified string is ");
			strcat(ssss,t);
			strcat(ssss," ");
			send(connected,ssss,strlen(ssss), 0);
			
			/*if(strcmp(t,command[2])>=0 && strcmp(t,command[3])<=0){
			//int ans=cmp("28/03/2013 10:50:56",t, "30/03/2013 00:22:52");	
		
					printf("Last modified date and time = %s ", t);

			}*/

			int size = statbuf.st_size;
			printf("Size is %d ",size);
			//char ssss[100];
			char sssss[100];
			strcpy(sssss,"Size in bytes is ");
			char new[10];
			
			sprintf(new,"%d",size);
			strcat(sssss,new);
			strcat(sssss,"\n");
			send(connected,sssss,strlen(sssss), 0);


		}

		printf("\n");
	     //if(!is_dir(j)){
			
	//		printf("%s\n",j);
	  //   }
             //sprintf(folder,"C:/folder/%s",j);
             //printf("%s\n",j);
           }

           (void) closedir (dp);
         }
       	else{
         perror ("Couldn't open the directory");
       	}
	send(connected,"end",3,0);	

}


void IndexGet_RegEx(){
	DIR *dp;
       	struct dirent *ep;
	struct stat     statbuf;
	char folder[100];
	char *j;
	char *outString = (char *)malloc(sizeof(char)*1000);
	printf("reached here \n");
	dp = opendir ("shared");
	chdir("shared");
	if (dp != NULL){
		
	   //printf("came here too\n");
           while (ep = readdir (dp)){

	     int flag=0;
             j=ep->d_name;
	     stat(j, &statbuf);
	     regex_t regex;
	     int reti;
	     char t[100]= "";
	     char tt[100];
	     strcpy(tt,".");
	     strcat(tt,command[2]);
	   
	     //printf("\n\nthe element is %s\n",j);
	     //printf("regex to be used is %s\n",tt);	
	     reti = regcomp(&regex,tt,0);
	     //printf("reti is %d \n",reti);
	     reti = regexec(&regex, j, 0, NULL,0);
	     //printf("reti now is %d\n",reti);
	     if(reti==0){
	     //strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
	     //printf("last modified time is %s ",t);
	     
	     //if(strcmp(t,command[2])>=0 && strcmp(t,command[3])<=0){
		
	     //size_t maxsize=100;
	     if(S_ISDIR(statbuf.st_mode)) 
		{
			/* Found a directory, but ignore . and .. */
			if(strcmp(".",j) == 0 || strcmp("..",j) == 0)
			{
				continue;
			}

			strcpy(outString,"");
			//strcat(outString," ");
			strcat(outString,j);
			strcat(outString," ");	
			send(connected,outString,strlen(outString), 0);
		

			//strcat(outString,"\n");
			printf("%s ",outString);
			printf("DIR ");
			send(connected,"DIR ",4, 0);


			flag=1;
			//longList(entry->d_name,depth+1);
			
		}
		else 
		{
			strcpy(outString,"");
			//char outString[100];
			strcat(outString,j);
			//strcat(outString,"\n");
			strcat(outString," ");
			//strcat(outString,"\n");
			printf("%s ",outString);
			send(connected,outString,strlen(outString), 0);
		
			//printf("%s ",outString);
			printf("FILE ");
			send(connected,"FILE ",5, 0);


			//printf("%*s%s\n",spaces,"",entry->d_name);
		}
		if (stat(j, &statbuf)!=-1)
		{  
			//printf("came here\n");	
			strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
			//printf("last modified time is %s ",t);
			//if(strcmp(t,command[2])>=0 && strcmp(t,command[3])<=0){
			//int ans=cmp("28/03/2013 10:50:56",t, "30/03/2013 00:22:52");	
		
					printf("Last modified date and time = %s ", t);
					char ssss[100];
					strcpy(ssss,"");
					strcpy(ssss,"last modified string is ");
					strcat(ssss,t);
					strcat(ssss," ");
					send(connected,ssss,strlen(ssss), 0);
		
			//}

			int size = statbuf.st_size;
			printf("Size is %d bytes ",size);
			char new[10];
			char sssss[100];
			sprintf(new,"%d",size);
			strcpy(sssss,"");
			strcpy(sssss,new);
			strcat(sssss,"\n");
			send(connected,sssss,strlen(sssss), 0);



		}

		printf("\n");
		}
	//	}
	     //if(!is_dir(j)){
			
	//		printf("%s\n",j);
	  //   }
             //sprintf(folder,"C:/folder/%s",j);
             //printf("%s\n",j);
           }

           (void) closedir (dp);
         }
       	else{
         perror ("Couldn't open the directory");
       	}
	send(connected,"end",3,0);	



}


void find_md5_hashvalue(){
	printf("came here to md5 fnc\n");
	chdir("shared");	
	FILE *inFile;
	inFile = fopen (command[2], "rb");
	unsigned char c[MD5_DIGEST_LENGTH];
	int i;
	char d[2*MD5_DIGEST_LENGTH+1];
	char buff[1000];
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	//unsigned char data[1024];
	if (inFile == NULL) 
	{
		printf ("%s can't be opened.\n", command[2]);
		return ;
	}  

	MD5_Init (&mdContext);
	while ((bytes = fread (data, 1, 1024, inFile)) != 0)
	{
		MD5_Update (&mdContext, data, bytes);
	}
	MD5_Final (c,&mdContext);

	for(i = 0; i < MD5_DIGEST_LENGTH; i++) 
	{
		sprintf(&d[i*2], "%02x", c[i]);
//		printf("%.02x", c[i]);
	}
	strcpy(compute_checksum,d);
	printf("%s\n",compute_checksum);
	char ssss[1024];
	strcpy(ssss,compute_checksum);
	strcat(ssss,"\n");
	printf("sex\n");
	//send(connected,"sex\n ",3,0);
	send(connected,ssss,strlen(ssss), 0);
	//send(connected,"end",3,0);	
	
	struct stat     statbuf;
	stat(command[2], &statbuf);
	//if (stat(command[2], &statbuf)!=-1){  
			char  t[100];
			strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
			printf("Last modified date and time = %s ", t);
			char sssss[100];
			strcpy(sssss,"");
			strcpy(sssss,"last modified string is ");
			strcat(sssss,t);
			strcat(sssss," ");
			strcat(sssss,"\n");
			send(connected,sssss,strlen(sssss), 0);
		
	//	}	

	send(connected,"end",3,0);	

	fclose (inFile);

}
	




void find_md5_hashvaluel(char *filename){
	//chdir("shared");	
	FILE *inFile;
	inFile = fopen (filename, "rb");
	unsigned char c[MD5_DIGEST_LENGTH];
	int i;
	char d[2*MD5_DIGEST_LENGTH+1];
	char buff[1000];
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	//unsigned char data[1024];
	if (inFile == NULL) 
	{
		printf ("%s can't be opened.\n", filename);
		return ;
	}  

	MD5_Init (&mdContext);
	while ((bytes = fread (data, 1, 1024, inFile)) != 0)
	{
		MD5_Update (&mdContext, data, bytes);
	}
	MD5_Final (c,&mdContext);

	for(i = 0; i < MD5_DIGEST_LENGTH; i++) 
	{
		sprintf(&d[i*2], "%02x", c[i]);
//		printf("%.02x", c[i]);
	}
	strcpy(compute_checksum,d);
	//printf("%s\n",compute_checksum);



	fclose (inFile);

}























void findall_md5_hashvalue(){
	printf("welcome to longlist\n");
	DIR *dp;
       	struct dirent *ep;
	struct stat     statbuf;
	char folder[100];
	char *j;
	char *outString = (char *)malloc(sizeof(char)*1000);
	printf("reached here \n");
	dp = opendir ("shared");
	chdir("shared");
	if (dp != NULL){
		
	   printf("came here too\n");
           while (ep = readdir (dp)){

	     int flag=0;
             j=ep->d_name;
	     stat(j, &statbuf);
	     char t[100]= "";	
	     size_t maxsize=100;
	     if(S_ISDIR(statbuf.st_mode)) 
		{
			/* Found a directory, but ignore . and .. */
			//longList(entry->d_name,depth+1);
			
		}
		else 
		{

			int ii;
			int found=0;
			for(ii=0;ii<countcheck;++ii){
				//printf("came here with element %s \n",j);
				//printf("ele being currently checked is %s\n",check[ii].name);	
				if(strcmp(check[ii].name,j)==0){
						find_md5_hashvaluel(j);
						if(strcmp(check[ii].checksum,compute_checksum)!=0){
								printf("file changed is %s and checksumnew is %s\n",check[ii].name,check[ii].checksum);
								strcpy(check[ii].checksum,compute_checksum);
								found=1;

								char ssss[1024];
								strcpy(ssss,check[ii].name);
								strcat(ssss," ");
								strcat(ssss,compute_checksum);
								strcat(ssss,"\n");
								//printf("sex\n");
								//send(connected,"sex\n ",3,0);
								send(connected,ssss,strlen(ssss), 0);
								//send(connected,"end",3,0);	
	
								struct stat     statbuf;
								stat(check[ii].name, &statbuf);
								//if (stat(command[2], &statbuf)!=-1){  
								char  t[100];
								strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
								printf("Last modified date and time = %s ", t);
								char sssss[100];
								strcpy(sssss,"");
								strcpy(sssss,"last modified string is ");
								strcat(sssss,t);
								strcat(sssss," ");
								strcat(sssss,"\n");
								send(connected,sssss,strlen(sssss), 0);
		









								break;
							
						}	else{

								found=1;
								break;
						}
				}
			}

			if(found==0){
					find_md5_hashvaluel(j);
					
					strcpy(check[countcheck].checksum,compute_checksum);
					strcpy(check[countcheck].name,j);
					char ssss[1024];
					strcpy(ssss,check[countcheck].name);
					strcat(ssss," ");
					strcat(ssss,compute_checksum);
					strcat(ssss,"\n");
								//printf("sex\n");
								//send(connected,"sex\n ",3,0);
					send(connected,ssss,strlen(ssss), 0);
								//send(connected,"end",3,0);	
	
					struct stat     statbuf;
					stat(check[countcheck].name, &statbuf);
								//if (stat(command[2], &statbuf)!=-1){  
					char  t[100];
					strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
					printf("Last modified date and time = %s ", t);
					char sssss[100];
					strcpy(sssss,"");
					strcpy(sssss,"last modified string is ");
					strcat(sssss,t);
					strcat(sssss," ");
					strcat(sssss,"\n");
					send(connected,sssss,strlen(sssss), 0);
		
					






					printf("file changed is %s and checksumnew is %s\n",check[countcheck].name,check[countcheck].checksum);
					countcheck++;


			}
			//printf("%*s%s\n",spaces,"",entry->d_name);
		}
		/*if (stat(j, &statbuf)!=-1)
		{  
			//printf("came here\n");	
			strftime(t,100,"%Y/%m/%d-%H:%M:%S", localtime( &statbuf.st_mtime));
			printf("last modified time is %s ",t);
			if(strcmp(t,command[2])>=0 && strcmp(t,command[3])<=0){
			//int ans=cmp("28/03/2013 10:50:56",t, "30/03/2013 00:22:52");	
		
					printf("Last modified date and time = %s ", t);

			}

			int size = statbuf.st_size;
			printf("Size is %d ",size);


		}

		printf("\n");*/
	     //if(!is_dir(j)){			
	//		printf("%s\n",j);
	  //   }
             //sprintf(folder,"C:/folder/%s",j);
             //printf("%s\n",j);
           }

           (void) closedir (dp);
         }
       	else{
         perror ("Couldn't open the directory");
       	}
	send(connected,"end",3,0);	



}

































/////////////////******************functions end***************************************************************/




int main()
{
	char send_data [1024] , recv_data[1024];       

	struct sockaddr_in server_addr,client_addr;    
	int sin_size;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}


	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(5000);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1)
	{
		perror("Listen");
		exit(1);
	}

	printf("\nTCPServer Waiting for client on port 5000");
	fflush(stdout);



	sin_size = sizeof(struct sockaddr_in);

	connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

	printf("\n I got a connection from (%s , %d)",
			inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));


	pid_t pid=fork();



	if (pid!=0)
	{
		while(1)
		{
			char f_n[100];
			char file_name[100]="t_\0";
			char *fname;
			fname= (char *)malloc(sizeof(char)*100);
			bytes_recieved = recv(connected,f_n,100,0);
			f_n[bytes_recieved]=0;

			if (strcmp(f_n,"FileDownload")==0)
			{
				//			printf("hjdkafs\n");
				//			break;
				bytes_recieved = recv(connected,f_n,100,0);
				f_n[bytes_recieved]=0;

				printf("\n%s\n",f_n);
				send(connected,f_n,strlen(f_n), 0);
		


				sleep(1);
				//      send(sock,send_data,strlen(send_data), 0); 

				printf("\nSending data ...");

				FILE * fp = fopen(f_n,"rb");

				printf("\nData Send\n");

				int xx;
				while(1)
				{
					if(!feof(fp))
					{
						xx=fread(send_data,1,1024,fp);
						printf("length of data %d\n",xx);
					}
					else
					{
						close(connected);
						break;
					}
					send(connected,send_data,xx, 0);
				}
				if (!ferror(fp))
					perror("Transfer status \n");
				close(sock);

				break;

			}else if(strcmp(f_n,"parse")==0){
					bytes_recieved = recv(connected,f_n,100,0);
					comlen=0;
    
					char *pch;

					printf("f_n is %s\n",f_n);
        				pch=strtok(f_n," ");
        				//strcpy(command[comlen++],pch);
        				//printf("%s\n",command[comlen-1]);
        				while (pch != NULL){
                				//printf ("%s\n",pch);
                				strcpy(command[comlen++],pch);
                				//printf("%s\n",command[comlen-1]);
                				pch = strtok (NULL, " ");

        				}
					int i;
					for(i=0;i<comlen;++i)
					{
						printf("hello world%s\n",command[i]);

					}

					parseData();
					chdir("..");
					break;



			}
			fname=strcat(file_name,f_n);
			printf("\n Receiving file - %s\n",fname);

			FILE *fp = fopen(fname,"wb");

			//	printf("%d\n",fp);
			
			sleep(1);


			while (1)
			{
				bytes_recieved = recv(connected,recv_data,1024,0);

				recv_data[bytes_recieved] = '\0';
				printf("\n RECEIVING DATA...");
				fwrite(recv_data,1,bytes_recieved,fp);
				//		write(fp,recv_data,bytes_recieved);
				fflush(stdout);
				if(bytes_recieved < 1024)
				{
					//					close(connected);
					break;
				}

			}
			printf("\nFile succesfully received\n");

			break;

			//		close(sock);
		}
	}
	else
	{
		while(1)
		{
			printf("\n ENTER COMMAND: ");
			char command[100];
			char *a;
			char *b;
			//			printf("dkjsfkl; ds \n");
			//      gets(command);
			scanf("%[^\n]",command);
			//              printf("%s\n",command);

			a=strtok(command," ");
			if (strcmp(a,"q")==0 || strcmp(a,"Q")==0)
				break;


			//              printf("%s\n",a);
			if (strcmp(a,"FileUpload")==0)
			{
				//              printf("sdjkdsjfl\n");
				char *file_name;
				file_name=strtok(NULL," ");
				printf("%s\n",file_name);
				//      printf("\n Enter filename :");
				//      gets(file_name);

				send(connected,file_name,strlen(file_name), 0);

				//      send(sock,send_data,strlen(send_data), 0); 

				printf("\nSending data ...");

				FILE * fp = fopen(file_name,"rb");

				printf("\nData Send\n");

				int xx;
				while(1)
				{
					if(!feof(fp))
					{
						xx=fread(send_data,1,1024,fp);
						printf("length of data %d\n",xx);
					}
					else
					{
						close(connected);
						break;
					}
					send(connected,send_data,xx, 0);
				}
				if (!ferror(fp))
					perror("Transfer status \n");
				close(sock);
			}
			else if (strcmp(a,"FileDownload")==0)
			{   
				send(connected,a,strlen(a), 0); 
				char *file_name;
				file_name=strtok(NULL," ");
				printf("Downloading file - %s\n",file_name);
				//      printf("\n Enter filename :");
				//      gets(file_name);

				send(connected,file_name,strlen(file_name), 0); 

			}   

			break;
		}


	}
	return 0;
}
