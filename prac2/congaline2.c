#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define oops(a,b)       { perror(a); exit(b); }

int counter=0;
#define LINE_MAXIMUM 256
size_t line_length;
// 250 characters on each line with a maximum of 100 command lines.
char cmds[100][LINE_MAXIMUM]; 
char *string_return;

char **a_p;
char *argvC[100][10];
char *ptr_of_cmd; //for parsing command line no more than 10 args

int main(int argc,char **argv)
{
int number_cmd;
int fds_Newest[2];
int fds_Older[2];
int pid;
//reads from stdin
// makes sure no more commands to read
while ((string_return=gets(cmds[counter]))!=NULL)
{
 
//parse the cmd into args
  for (a_p = argvC[counter],ptr_of_cmd=cmds[counter]; (*a_p = strsep(&ptr_of_cmd, " \t")) != NULL;)
    if (**a_p != '\0'){
      if (++a_p >= &argvC[counter][10])
          break;
  }
  counter++;
}
//for cmd in cmds
for(number_cmd = 0;number_cmd < counter;number_cmd++)
{
  if (number_cmd < counter-1)
        if ( pipe(fds_Newest) ==-1){// piping takes place
          oops("Cannot get a pipe",1);
        }
  if ((pid=fork()) ==-1)
         oops("Cannot fork",2);
  if (pid==0)
      {
      if (number_cmd > 0)
        {
        //if there is a previous cmd
         if( dup2(fds_Older[0], 0)==-1)
             oops("Could not redirect stdin",3);
             close(fds_Older[0]);
             close(fds_Older[1]);
         }
        if (number_cmd < counter-1)
        {
        //if there is a next cmd
            close(fds_Newest[0]);
         if(dup2(fds_Newest[1], 1) ==-1)
             oops("Could not dup stdout",4);
          close(fds_Newest[1]);
         }
        execvp( argvC[number_cmd][0],argvC[number_cmd]);
        oops(cmds[number_cmd],5);
       }
  else   
      {
        // checks if there was a previous cmd
      if (number_cmd > 0)
           {
            close(fds_Older[0]);close(fds_Older[1]);
            }
        //Checking if there is a next cmd
      if (number_cmd<
          counter-1)
           {
            fds_Older[0] = fds_Newest[0];
            fds_Older[1] = fds_Newest[1];
           }
      }
 }
//if there exixts multiple cmds
if (counter > 1)
{
    close(fds_Older[0]);close(fds_Older[1]);
}
return 0;
} // end of main function
