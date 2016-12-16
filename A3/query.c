#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"


int is_file (char *path,struct stat buffer)
	{
		if ((stat(path, &buffer)) == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

int main (int argc, char **argv)
	{
	FreqRecord * arry = malloc(50 * sizeof(FreqRecord));
    pid_t prtID = getpid();
    pid_t p_id;
    pid_t fd[10][15][2];
    int file_index = 0;


	char ch;
		char path[PATHLENGTH];
		char *startdir = ".";

		while((ch = getopt(argc, argv, "d:")) != -1) {
			switch (ch) {
				case 'd':
				startdir = optarg;
				break;
				default:
				fprintf(stderr, "Usage: query [-d DIRECTORY_NAME]\n");
				exit(1);
			}
		}
		// Open the directory provided by the user (or current working directory)

		DIR *dirp;
		if((dirp = opendir(startdir)) == NULL) {
			perror("opendir");
			exit(1);

		}

		struct dirent *dp;
		while((dp = readdir(dirp)) != NULL) {

			if(strcmp(dp->d_name, ".") == 0 ||
			   strcmp(dp->d_name, "..") == 0 ||
			   strcmp(dp->d_name, ".svn") == 0){
				continue;
			}
			strncpy(path, startdir, PATHLENGTH);
			strncat(path, "/", PATHLENGTH - strlen(path) - 1);
			strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);

			struct stat sbuf;
			if(stat(path, &sbuf) == -1) {
				//This should only fail if we got the path wrong
				// or we don't have permissions on this entry.
				perror("stat");
				exit(1);
			}

	        struct stat listbuf;
	        struct stat	namebuf;
	        char listfile[PATHLENGTH];
		    strncpy(listfile, path, PATHLENGTH);
		    strncat(listfile, "/", PATHLENGTH - strlen(listfile) - 1);
		    strncat(listfile, "index", PATHLENGTH - strlen(listfile) - 1);

		    char namefile[PATHLENGTH];
		    strncpy(namefile, path, PATHLENGTH);
		    strncat(namefile, "/", PATHLENGTH - strlen(namefile) - 1);
		    strncat(namefile, "index", PATHLENGTH - strlen(namefile) - 1);

	        int namefile_indicator = is_file(listfile,listbuf);
	        int listfile_indicator = is_file(namefile,namebuf);

	        if(S_ISDIR(sbuf.st_mode&& listfile_indicator == 1 && namefile_indicator == 1)) {

	        	pipe(fd[file_index ][0]);
	        	pipe(fd[file_index ][1]);
	        	if (p_id == 0)
	        	{
	                close(fd[file_index][0][1]);
	                close(fd[file_index][1][0]);
	                run_worker(path, fd[file_index][0][0], fd[file_index][1][1]);
	        	}
	        	else if (( p_id = fork() ) < 0)
	        	{
	                perror("ERROR:fork");
	                exit(1);

	        		}
	        	file_index++;


		}



		}


		 if (prtID == getpid())
		 {
			 while(1)
			 {
				 char word[MAXWORD];
				 int reader;
				 if ((reader = read(STDIN_FILENO, word, MAXWORD)) == 0)
						 {
					 	 int counter = 0;
					 	 	 while (counter < file_index)
					 	 	 {
				                    close(fd[counter][0][1]);
				                    close(fd[counter][1][0]);
				                    counter++;
					 	 	 }
					 	 	 int child  = 0;
					 	 	 int stat;

					 	 	 while( child < file_index)
					 	 	 {
					 	 		 wait(&stat);
					 	 		 child++;
					 	 	 }
					 	 	exit(0);
						 }
				 word[reader] = '\0';
			 int counter2 =0;
				FreqRecord *rcd = malloc(sizeof(FreqRecord));
	            while ( counter2 < file_index) {

	                close(fd[counter2][0][0]); // close all pipes not required
	                close(fd[counter2][1][1]);

	                int writer;
	                if ( ( writer = write(fd[counter2][0][1], word, MAXWORD) ) == -1) {
	                    perror("ERROR:write into fd");
	                    exit(1);
	                }

		            int reader2;
	                if ( ( reader2 = read(fd[counter2][1][0], rcd, sizeof(FreqRecord)) ) == -1) {
	                    perror("ERROR:read");
	                    exit(1);
	                }


	                counter2++;
		 }
	            int tracker =0;
	            while (rcd != NULL)
	            {
	            arry[tracker] = *rcd;
	            tracker++;
	            }


			 }
		 }
			return 0;
	}
