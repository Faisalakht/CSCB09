#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"
#include <fcntl.h>

/* The function get_word should be added to this file */


/* Print to standard output the frequency records for a word.
* Used for testing.
*/
void print_freq_records(FreqRecord *frp) {
	int i = 0;
	while(frp != NULL && frp[i].freq != 0) {
		printf("%d    %s\n", frp[i].freq, frp[i].filename);
		i++;
	}
}

FreqRecord *get_word(char *word,Node *head,char ** filenames)

{
	FreqRecord  *arry = malloc(50 * sizeof(FreqRecord));
	int arraycounter = 0;
	int counter = 0;

	if (head == NULL)
	{
		return NULL;
	}

	   while (head != NULL)
	   {
		   if (strcmp(head->word,word) == 0)
		   {
			  int len = sizeof(head->freq)/sizeof(int);
			  while ( counter < len)
			  {
				  if (head->freq[counter] > 0)
				  {
					  arry[arraycounter].freq = head->freq[counter];
					  strcpy(arry[arraycounter].filename,filenames[counter]);
					  arraycounter ++;

				  }
				  counter++;
			  }
		   }
		   head= head->next;
	   }
	   arry[arraycounter].freq = 0;
	   strcpy(arry[arraycounter].filename,"/0");
	   return arry;
}



/* run_worker
* - load the index found in dirname
* - read a word from the file descriptor "in"
* - find the word in the index list
* - write the frequency records to the file descriptor "out"
*/

void run_worker(char *dirname, int in, int out){
	Node *head = NULL;
	 char ** filenames = init_filenames();
	    char listfile[MAXWORD];
	    strncpy(listfile, dirname, MAXWORD);
	    strncat(listfile, "/", MAXWORD - strlen(listfile) - 1);
	    strncat(listfile, "index", MAXWORD - strlen(listfile) - 1);

	    char namefile[MAXWORD];
	    strncpy(namefile, dirname, MAXWORD);
	    strncat(namefile, "/", MAXWORD - strlen(namefile) - 1);
	    strncat(namefile, "index", MAXWORD - strlen(namefile) - 1);

	    read_list(listfile, namefile, &head, filenames);
	char word[MAXWORD];
	int reader=3;
	 FreqRecord *rcd;


		while (reader!= 0)
			{
	        if (( reader = read(in,word,MAXWORD)) < 0) {
	            perror("ERROR:Read in");
	            exit(1);

	        } else if (reader == 0) {
	            close(out);
	            exit(0);

	        } else {

	            // add a null character to the end
	            word[reader] = '\0';
	        }
	         rcd = get_word(word,head,filenames);
	         int i= 0;
	       	  while(rcd != NULL && rcd[i].freq != 0) {
	       	     if (write(out, &word[i], sizeof(FreqRecord)) != sizeof(FreqRecord)) {
	       	        	                perror("ERROR:write FreqRecord");
	       	        	                exit(1);
	       	     }
	       	  }
	       	if (write(out, &word[i], sizeof(FreqRecord)) != sizeof(FreqRecord)) {
	       		perror("ERROR:write FreqRecord");
	       		exit(1);

	       		       	}

	}


}

/*
int main()
{
	Node *head = NULL;
	  char **filenames = init_filenames();
	  char *listfile = "index";
	  char *namefile = "filenames";
	  read_list(listfile, namefile, &head, filenames);
	FreqRecord *k = get_word("well",head,filenames);
	print_freq_records(k);

	return 0;
}
*/

