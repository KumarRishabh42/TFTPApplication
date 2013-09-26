/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <arpa/inet.h>


int main()

{

	int sock, bytes_recieved;  
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;  

	host = gethostbyname("127.0.0.1");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 

	if (connect(sock, (struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}

	pid_t pid=fork();

	if (pid!=0)
	{
		while(1)
		{

			printf("\n ENTER COMMAND: ");
			char commandl[100];
			char aa[100];
			
			char *a;
			char *b;
			//	printf("dkjsfkl; ds \n");
			gets(commandl);
			strcpy(aa,commandl);
			//scanf("%[^\n]",command);
			//		printf("%s\n",command);

			a=strtok(commandl," ");
			if (strcmp(a,"q")==0 || strcmp(a,"Q")==0)
				break;


			//		printf("%s\n",a);
			if (strcmp(a,"FileUpload")==0)
			{
				//		printf("sdjkdsjfl\n");
				char *file_name;
				file_name=strtok(NULL," ");
				printf("%s\n",file_name);
				//	printf("\n Enter filename :");
				//	gets(file_name);

				send(sock,file_name,strlen(file_name), 0);

				//	send(sock,send_data,strlen(send_data), 0); 

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
						close(sock);
						break;
					}
					send(sock,send_data,xx, 0);
				}
				if (!ferror(fp))
					perror("Transfer status \n");
				//		close(sock);
			}
			//		kill(pid,1);
			else if (strcmp(a,"FileDownload")==0)
			{
				send(sock,a,strlen(a), 0);
				char *file_name;
				file_name=strtok(NULL," ");
				printf("Downloading file - %s\n",file_name);
				//	printf("\n Enter filename :");
				//	gets(file_name);

				send(sock,file_name,strlen(file_name), 0);

			}else{
				
				char ss[100]="parse";
				send(sock,ss,strlen(ss),0);
				send(sock,aa,strlen(aa),0);
				
			}
			break;
		}
	}
	else
	{
		while(1)
		{

			//			printf("hlkjflsd\n");
			char f_n[100];
			char file_name[100]="t_\0";
			char *fname;
			fname= (char *)malloc(sizeof(char)*100);
			bytes_recieved = recv(sock,f_n,1024,0);
			f_n[bytes_recieved]=0;
			//      printf("hjdkafs\n");

			if (strcmp(f_n,"FileDownload")==0)
			{
				//                      printf("hjdkafs\n");
				//                      break;
				bytes_recieved = recv(sock,f_n,1024,0);
				f_n[bytes_recieved]=0;

				printf("\n%s\n",f_n);
				send(sock,f_n,strlen(f_n), 0);
		
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
						close(sock);
						break;
					}
					send(sock,send_data,xx, 0);
				}
				if (!ferror(fp))
					perror("Transfer status \n");
				close(sock);

				break;

			}
			else if (strcmp(f_n,"FileUpload")!=0)
			{
				printf("came to this place\n");
				while(strcmp(f_n,"end")!=0)
				{
					printf("%s ",f_n);
					bytes_recieved = recv(sock,f_n,1024,0);

				}
				close(sock);
				
			}


			fname=strcat(file_name,f_n);
			printf("\nReceiving file - %s\n",fname);

			FILE *fp = fopen(fname,"wb");

			//      printf("%d\n",fp);



			while (1)
			{
				bytes_recieved = recv(sock,recv_data,1024,0);

				recv_data[bytes_recieved] = '\0';
				printf("\n RECEIVING DATA...");
				fwrite(recv_data,1,bytes_recieved,fp);
				//              write(fp,recv_data,bytes_recieved);
				fflush(stdout);
				if(bytes_recieved < 1024)
				{
					close(sock);
					break;
				}

			}
			printf("\nFile successfully received\n");
			break;

		}
	}
	kill(pid);
	return 0;
}
