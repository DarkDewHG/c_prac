#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct list{
	int pid;
	int i;
	struct list * next;

} 
list;

int findcmd(char ** arr, int oldcmdpos){
	int i=0;
	while(arr[oldcmdpos+i]!=NULL){
		i++;
	}
	if (arr[oldcmdpos+1+i]==NULL)	
		return 0;
	else
		return oldcmdpos+i+1;
}

list * addtolist(list* head, int pid,int i){
	list* tmp = malloc(sizeof(*tmp));
	tmp->pid= pid;
	tmp->i =i;
	tmp->next= head;
	return tmp;
}

int findname(list* head, int pid){
	int key=0;
	list* tmp = head;
	printf("\n list pid: %d\n",tmp->pid);
	while(tmp!=NULL){

		if (tmp->pid == pid)
			key=tmp->i;
		tmp=tmp->next;
	}
	return key;
}


int main(int argc, char ** argv)
{
	int i,pid;
	list * head=NULL;
	int respid,lrespid;
	int status,lstatus;
	char ** arr = malloc((argc+2)*sizeof(*arr));
	for( i=0; i<argc;i++)
	{
		if (!strcmp ("---",argv[i]))
		{
			arr[i]= NULL;
		}
		else
		{
			arr[i]=argv[i];
		}	
	}
	arr[argc]=NULL;
	arr[argc+1]=NULL;
	i=1;
	do
	{	
		pid= fork();
		if(pid==0)
		{
			printf("\nn: %d \n",i);
			execvp(arr[i], &arr[i]);
			perror(arr[i]);
			fflush(stderr);
			_exit(1);
		}
		printf("\n pid : %d n:%d \n",pid,i);
		head=addtolist(head,pid,i);
		i=findcmd(arr,i);
       	
	}
	while(i);
	respid=-1;
	status=1;
	do
	{
		lrespid=respid;
		lstatus=status;
		respid = wait4(-1,&status,WNOHANG,NULL);
	}
	while(respid!=-1);
	printf("\n ourpid : %d \n",lrespid);
	i= findname(head,lrespid);
	printf("last cmd %s , status %d", arr[i],lstatus);
}
