/*
** Roll Number: 2018201101
	Name: Suchismith Roy

*/



#include "filelisting.h"
#include "normalmode.h"
using namespace std;



int visit=0;               		
int trajectory_index=0;
int visit_flag=0;
int offset=0;
int upper_end=1;
int lower_end=0;
int total_rows=0;
int current_pos=1;
int overflow_flag=0;
int firstVisit=0;
int current_ind=0;
int debug=0;
char currentDir[PATH_MAX]="";
char homeDir[PATH_MAX]="";
char leftPath[PATH_MAX]="";
vector<string> trajectory;
vector<char*> filePaths;
char fullPath[PATH_MAX];
struct fileAttributes{

        char user_grp_others[12];
        string user_name;
        string group_name;
        string last_modified_time;
        char b;
        size_t file_size;
        char* dirname;
};
vector<fileAttributes> directories;






void resetCursor()
{
	current_pos=1;
	offset=0;
	int upper_end=1;
	int lower_end=0;

}

void clearScr()
{
	printf("\x1b[2J");
	printf("\x1b[1;1H");
}


void redraw()
{
printf("\x1b[1;1H");
}
void setCursorAtEnd()
{
	printf("\x1b[%d;1H",lower_end);
}

void printFileAttributes(int start,int end){

	struct fileAttributes fileattr;

	
	if(start>=0 && end<=directories.size()-1)
	{
	for(int i=start;i<=end;i++)
	{

		fileattr=directories[i];

		cout << fileattr.user_grp_others << " "<< fileattr.user_name << " " << fileattr.group_name << " " << 
		fileattr.file_size <<fileattr.b<<" "<< fileattr.last_modified_time<<" "<<fileattr.dirname <<"\n";

	}
	
	
	}	


}

char* handleDirectoryName(char* dir)
{

        
		if(dir[0]=='/')		
        	return dir;
        else
        {

        	char dir2[255];
        	int i;	
        	dir2[0]='/';
        	for( i=1;dir[i-1]!='\0';i++)
        		dir2[i]=dir[i-1];
        	dir2[i]=dir[i];
        	strcpy(dir,dir2);


        	//filePaths.push_back(dir);

        	strcat(currentDir,dir2);
        	//strcat(leftPath,currentDir);

        	string str(currentDir);
        	trajectory.push_back(str);
        	trajectory_index+=1;
        	return currentDir;


        }



}

void move_up(int dis)
{
	
		
		if(current_pos>=upper_end )
		{
		current_pos-=dis;
		printf("\033[%d;1H",current_pos);
		}
		else
		{	
				
		if(offset && overflow_flag){
			clearScr();
			printFileAttributes(offset-1,lower_end+offset-1);
			redraw();
			offset-=1;
		}

		}
		

}


void move_down(int dis)
{
	
	
	if(current_pos<lower_end)
	{	
		current_pos+=dis;
		
		printf("\033[%d;1H",current_pos);
		
		
	}
	else
	{
		if(overflow_flag){
		
		clearScr();
		if(lower_end+offset-1<directories.size()){
			printFileAttributes((upper_end-1)+offset,lower_end+offset-1);
			setCursorAtEnd();
			offset+=1;
		}
		else
			printFileAttributes((upper_end-1)+offset,lower_end+offset-2);
		//int sum=lower_end+offset;
		cout<<current_pos<<" "<<(upper_end-1)+offset<<" "<<lower_end+offset<<" "<<directories.size();
		}

	}






	
}

void move_left()
{


	if(visit_flag)
	{	


		
		trajectory_index>0?trajectory_index-=1:trajectory_index=0;
		string visited_dir=trajectory[trajectory_index];
		int length=visited_dir.length();
		char path[length+1];
		strcpy(path,visited_dir.c_str());
		strcpy(currentDir,path);
		directories.erase(directories.begin(),directories.end());
		listFile(path);




	}

}
void move_right()
{


	if(visit_flag)
	{	


		trajectory_index<trajectory.size()-1?trajectory_index+=1:trajectory_index=trajectory.size()-1;
		string visited_dir=trajectory[trajectory_index];
		int length=visited_dir.length();
		char path[length+1];
		strcpy(path,visited_dir.c_str());
		strcpy(currentDir,path);
		directories.erase(directories.begin(),directories.end());
		listFile(path);


	}



}








void handleOutput(){
    
     while(1){
     	int c = getchar();
		if(c==ESC)
		{

			char brac=getchar();

			if(brac=='[')
			{

				switch (getchar()) {
            		case 'A':
						move_up(1);
                		break;
            		case 'B':
                		move_down(1);
                		break;
            		case 'C':
                		move_right();
                		break;
            		case 'D':
                		move_left();
                		break;
            		
        	}	

		}


		}
		else if(c==ENTER)
		{	
			
			
			
			 struct fileAttributes fileattributes=directories[current_pos+offset-1];
			 char* selectedDir=directories[current_pos+offset-1].dirname;
			  if(visit>1)
			  	visit_flag=1;

			  

			 if(fileattributes.user_grp_others[0]=='d')

			 {
			 	

			  	if((strcmp(selectedDir,"..")==0 || strcmp(selectedDir,".")==0) && visit==1){
			
			  		strcpy(currentDir,homeDir);
			  		}
				 else{
				 	visit+=1;		 	
				 	selectedDir=handleDirectoryName(selectedDir);
				 	//cout<<selectedDir;
				 	trajectory.push_back(selectedDir);
				 	trajectory_index+=1;
				 	 
				 	}
					//strc(currentDir,selectedDir);
					//cout<<selectedDir;
			 		directories.erase(directories.begin(),directories.end());	

			 		listFile(selectedDir);

			  }
			  else if(fileattributes.user_grp_others[0]=='-'){
			  	int pid;
			   	pid = fork();

			   	char path[PATH_MAX];
			   	strcpy(path,currentDir);
			   	strcat(path,"/");
			   	strcat(path,selectedDir);
				if (pid == 0) {
  					execl("/usr/bin/xdg-open", "xdg-open", path, (char *)0);
  					exit(1);
				 }
			  	}
			  	

		}
		else if(c==HOME)
		{	
			strcpy(currentDir,homeDir);
			visit=1;
			directories.erase(directories.begin(),directories.end());
			listFile(homeDir);


		}
		else if(c==BACK)
		{


			if(strcmp(currentDir,homeDir)){
	
						string lastVisitedDir=trajectory[trajectory_index];
						lastVisitedDir=lastVisitedDir.substr(0,lastVisitedDir.find_last_of("/"));
						int length=lastVisitedDir.length();
						char path[length+1]="";
						strcpy(path,lastVisitedDir.c_str());
						strcpy(currentDir,path);
						trajectory.push_back(lastVisitedDir);
						trajectory_index+=1;//cout<<lastVisitedDir;
						directories.erase(directories.begin(),directories.end());
						listFile(currentDir);
			}



		}

	}

}





void errorHandler(int code){


	switch(code){

		case 11:cout<<"please enter file name";
				break;

		case 12:cout<<"please enter a valid filename/ directory";
				break;
		case 13:cout<<"Terminal Error";
				break;




	}
}



void FileExplorer()
{


	//char pwd[PATH_MAX];

	if(getcwd(currentDir, sizeof(currentDir)) == NULL) 
	{
		cout<<"error in fetching current directory";
	}
	
	if(getcwd(homeDir, sizeof(homeDir)) == NULL) 
	{
		cout<<"error in fetching home directory";
	}
	filePaths.push_back(homeDir);
	trajectory.push_back(homeDir);

	visit=1;
	listFile(currentDir);



}







void listFile(char *dirname)
{
		DIR *dp;

		struct dirent *dirp;
		struct stat fileDetails;
		struct group *group_details;
		struct passwd *user_details;
		struct fileAttributes fileattributes;
		char ugo[11];
		
		if(strcmp(dirname,"..")==0)

			

		if (ioctl(0, TIOCGWINSZ, (char *) &size) < 0)
			errorHandler(13);
		total_rows=size.ws_row-2;

		if((dp = opendir(dirname))==NULL){
			cout<<"unable to open"<<dirname;
			errorHandler(12);


		}
		else
		{

			while ((dirp = readdir(dp)) != NULL)
			{	

					if(visit==1)
		    		stat(dirp->d_name,&fileDetails);
		    		else
		    		{


					    	char dname[PATH_MAX]="";
							strcpy(dname,currentDir);
							strcat(dname,"/");
							strcat(dname,dirp->d_name);
						
							stat(dname,&fileDetails);
		    		}

			    mode_t permission=fileDetails.st_mode;
		    	ugo[0]=permission & S_IFDIR ? 'd' : '-';
		   		ugo[1]=permission & S_IRUSR ? 'r' : '-';
		   		ugo[2]=permission & S_IWUSR ? 'w' : '-';
		   		ugo[3]=permission & S_IXUSR ? 'x' : '-';
		   		ugo[4]=permission & S_IRGRP ? 'r' : '-';
		   		ugo[5]=permission & S_IWGRP ? 'w' : '-';
		   		ugo[6]=permission & S_IXGRP ? 'x' : '-';
		   		ugo[7]=permission & S_IROTH ? 'r' : '-';
		   		ugo[8]=permission & S_IWOTH ? 'w' : '-';
		   		ugo[9]=permission & S_IXOTH ? 'x' : '-';
		   		ugo[10]='\0';
		   		
		   		size_t fileSize=(fileDetails.st_size/1024);
		   		char t=(fileDetails.st_size/1024>1)?'K':'B';
		    	group_details=getgrgid(fileDetails.st_gid);
		    	user_details= getpwuid(fileDetails.st_uid);
		    	string user_name=user_details->pw_name;
		    	string group_name=group_details->gr_name;    
		    	string last_modified_time=ctime(&fileDetails.st_mtime);
		    	if(last_modified_time[last_modified_time.length()-1]=='\n')
		    		last_modified_time[last_modified_time.length()-1]='\0';
		    	strcpy(fileattributes.user_grp_others, ugo);		   		
		   		fileattributes.file_size=fileSize;
		    	fileattributes.user_name=user_name;
		    	fileattributes.group_name=group_name;
				fileattributes.last_modified_time=last_modified_time;
		    	fileattributes.dirname=dirp->d_name;
		    	fileattributes.b=t;
			    directories.push_back(fileattributes);		    	
	    	}

	    	if(total_rows<=directories.size())
				overflow_flag=1;
			lower_end= (overflow_flag)?total_rows:directories.size();
			resetCursor();
			clearScr();
			//sort(directories.begin(),directories.end());
	    	printFileAttributes(upper_end-1,lower_end-1);
	    	redraw();
	    	handleOutput();

   	
   		closedir(dp);
   }
   
}
































		
