#include "types.h"
#include "stat.h"
#include "user.h"

int line;
char buf[1];
char *filename;

//file open for read function
int fileOpen(){
	int fd = open(filename,0);
	if(fd<0){
		printf(1,"htac : failed open %s\n", filename);
		exit();
	}
	return fd;
}

//htac fuction
void htac(int fd){
	int n;
	//total line
        int lineCnt=0;

	//count total line
	while((n=read(fd,buf,sizeof(buf)))>0){
		if(buf[0]=='\n'){
			lineCnt++;
		}
	}
	close(fd);
	if(n<0){
		printf(1,"htac : read error");
		exit();
	}

	//if the line to be read is smaller than the file line, exit
	if(lineCnt<line){
		printf(1,"htac : %s don't have enough line",filename);
		exit();
	}

	//read n lines starting from the last line in order
	for(int i=0;i<line;i++){
		int lineIndex = 0;
		fd = fileOpen();
		
		while((n=read(fd,buf,sizeof(buf))) > 0){
			//if the line to be read matches the current line
			if(lineIndex == lineCnt-i){ //i line from last line
				if(write(1,buf,n) != n){
					printf(1,"htac : write fail");
					exit();
				}
			}
			//if next line , lineIndex++
			if(buf[0]=='\n'){
				lineIndex++;
			}
		}
		//if last line, add '\n' , because last line is EOF 
		if(i==0) {
			printf(1,"\n");
		}
		if(n<0){
			printf(1,"htac : read fail");
			exit();
		}
		close(fd);
	}
}

int main(int argc, char *argv[]){
	int fd,i;

	if(argc<=1){
		htac(0);
		exit();
	}

	line = atoi(argv[1]);

	for(i=2;i<argc;i++){

		filename = argv[i];
		fd = fileOpen(filename);
		htac(fd);
	}
	exit();
}
