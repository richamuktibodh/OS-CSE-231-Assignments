#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <assert.h>
#include <pthread.h>

int checkValidInbuiltCmd(char* s){
	char* inblt_cmds[] = {"echo","pwd","cd","ls","cat","date","mkdir","touch","rm"};
	int len = sizeof(inblt_cmds)/sizeof(inblt_cmds[0]);
    int i;
    for (i=0; i<len; i++){
        if (strcmp(s,inblt_cmds[i]) == 0){
            return 1;
        }

    }
    return 0;

}

int checkValid(char *s){
    struct stat st;
    if (stat(s,&st) == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int getMyId(){
    struct stat st;
    stat(getenv("HOME"),&st);
    int ownerId = st.st_uid;
    return ownerId;
}

int isFile(char *s){
    struct stat st;
    stat(s,&st);
    if (S_ISREG(st.st_mode)) {
        return 1;  // its a file
        }
    else{
        return 0;
    }
}

int isDir(char *s){
    struct stat st;
    stat(s,&st);
    if (S_ISDIR(st.st_mode)) {
        return 1;  // its a dir
        }
    else{
        return 0;
    }
}

int getFileOwnerId(char *s){ // parameter is file or path
    struct stat st;
    stat(s,&st);
    int fileOwnerId = st.st_uid;
    return fileOwnerId;
}

int amIOwner(int myid, int fileownerid){
    if (myid == fileownerid){
        return 1;
    }
    else{
        return 0;
    }
}

// permissions
int doesOwnerHaveR(char *s){
    struct stat st;
    stat(s,&st);
    if (st.st_mode & S_IRUSR){
        return 1; // owner has read permission
    }
    return 0;
}

int doesOthersHaveR(char *s){
    struct stat st;
    stat(s,&st);
    if (st.st_mode & S_IROTH){
        return 1; // others has read permission
    }
    return 0;
}

int doesOthersHaveW(char *s){
    struct stat st;
    stat(s,&st);
    if (st.st_mode & S_IWOTH){
        return 1; 
    }
    return 0;
}

int doesOwnerHaveW(char *s){
    struct stat st;
    stat(s,&st);
    if (st.st_mode & S_IWUSR){
        return 1; 
    }
    return 0;
}

// for ls, date
void *myFunc(void *arg) {
    system((char* )arg);
    return NULL;
}

int main(){
	int i,count = 0;
	char **arr ; // array of strings ; hardcoded 10 in this case
	char string[50]; // string is the first input taken by kernel
	char cwd[15] = "Richa~"; // original prompt, value of cwd changes
    char curr_cwd[15] = "Richa~"; // value is constant
    char curr_actual_cwd[100] = ""; // stores only name of current working directory
    char orig_actual_cwd[100] = "";
	char *token;
    int res;
    int val; // if file/ directory exists
    int myId = getMyId();
    int isfile;
    int isdir;
    int fileOwnerId;
	int isOwnerMe;
    int flag1;
    int flag2;
    int useThread = 0;
    pthread_t p1;
    int rc;
    char temp[20];

    // assigning memory for each char* pointer in arr
    arr = (char **)malloc(sizeof(char *)*10);
	
    getcwd(curr_actual_cwd,sizeof(curr_actual_cwd));
    getcwd(orig_actual_cwd,sizeof(orig_actual_cwd));

	do{
        useThread = 0;
		i = 0; 
        count = 0; // count is number of split strings
		printf("%s$ ",cwd);
		// exit (0);

		fgets(string, sizeof(string), stdin);
        string[strcspn(string, "\n")] = 0; //removing newline character
		
        if (strlen(string) == 0){ // if arr is empty
            continue;
        }

		for (char *p = strtok(string," "); p != NULL; p = strtok(NULL, " "))
		{
  			arr[i] = malloc(sizeof(char) * (strlen(p)+1));
              strcpy(arr[i], p); 
			// puts(p); // prints each string in arr
			i++;
            count++;
		}

        // printf("count: %d\n",count);

        if (*(arr[0]) == 'q'){ // exiting loop 
			break;	
		}

        if (strcmp(arr[count-1], "&t") == 0){
            printf("Using threads\n");
            useThread = 1;
        }

        // add check if valid command entered or not
        res = checkValidInbuiltCmd(arr[0]);
        // printf("res %d\n",res);
        if (res == 0){
            printf("Invalid command\n");
            continue;
        }

		
		
        // echo
        if (strcmp(arr[0],"echo") == 0){  
			if ((count >1) && (strcmp(arr[1],"-n") == 0)){ // for only echo,element at index 1 is null
                for (i=2; i<count; i++){
                    printf("%s ",arr[i]);
                }
            }
            
            else{
                for (i=1; i<count; i++){
                    printf("%s ",arr[i]);
                }
                printf("\n");
            }	
		}

        // pwd
        if (strcmp(arr[0],"pwd") == 0){
            if (count == 1){
                printf("%s\n",curr_actual_cwd); 
            }
            else if ((count > 1) && (strcmp(arr[1],"-L") == 0)){
                printf("%s\n",curr_actual_cwd);
            }
            else if ((count > 1) && (strcmp(arr[1],"-P") == 0)){
                printf("%s\n",curr_actual_cwd);
            }
            else{
                printf("Invalid flag used\n");
            }
            

        }

        // cd
        if (strcmp(arr[0],"cd") == 0){ // didnt add cd ..
            if ((count > 1) && (strcmp(arr[1],"~") != 0) ){
                if (checkValid(arr[1])){
                    strcat(cwd,"/");
                    strcat(cwd, arr[1]);
                    strcat(curr_actual_cwd,"/");
                    strcat(curr_actual_cwd,arr[1]);
                    // printf("curr_cwd: %s\n",curr_actual_cwd);
                    // setenv("PWD",curr_actual_cwd,1);
                    chdir(curr_actual_cwd);
                    // printf("cwd: %s\n", cwd);
                }
                else{
                    printf("directory doesn't exist\n");
                }
            }
            else if ((count > 1) && (strcmp(arr[1],"~") == 0)){ // goes to home directory
                strcpy(cwd,curr_cwd); 
                chdir(orig_actual_cwd);
            }

            else if (count == 1){
                strcpy(cwd,curr_cwd); 
                chdir(orig_actual_cwd);
            }

            else{
                printf("Invalid flag used\n");
            }

        }

        // ls
        if ((strcmp(arr[0],"ls") == 0)){
            if (!useThread){
                if (count == 1){ // only ls
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) { 
                        char *myargs[2];
                        myargs[0] = "ls"; 
                        myargs[1] = NULL;
                        execvp("ls", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }
                }
                else if ((count == 2) && (strcmp(arr[1],"-l") == 0)){
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) { 
                        char *myargs[3];
                        myargs[0] = "ls"; 
                        myargs[1] = "-l";
                        myargs[2] = NULL;
                        execvp("ls", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }


                }
                else if ((count == 2) && (strcmp(arr[1],"-r") == 0)){
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) { 
                        char *myargs[3];
                        myargs[0] = "ls"; 
                        myargs[1] = "-r";
                        myargs[2] = NULL;
                        execvp("ls", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }

                }
                else{
                    printf("Invalid flag used\n");
                }
            }
            else{ // if its a thread
                if (count == 2){ // only ls
                    rc = pthread_create(&p1, NULL, myFunc, "ls"); 
                    assert(rc == 0); // if pthread successful, returns 0
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else if ((count == 3) && (strcmp(arr[1],"-l") == 0)){
                    rc = pthread_create(&p1, NULL, myFunc, "ls -l"); 
                    assert(rc == 0); 
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else if ((count == 3) && (strcmp(arr[1],"-r") == 0)){
                    rc = pthread_create(&p1, NULL, myFunc, "ls -r"); 
                    assert(rc == 0); 
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else{
                    printf("Invalid flag used\n");
                }
            }

        }

        // date
        if ((strcmp(arr[0],"date") == 0)){
            if (!useThread){
                if (count == 1){ // only date
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) { 
                        char *myargs[2];
                        myargs[0] = "date"; 
                        myargs[1] = NULL;
                        execvp("date", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }
                }

                else if ((count == 2) && (strcmp(arr[1],"-u") == 0)){
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) { 
                        char *myargs[3];
                        myargs[0] = "date"; 
                        myargs[1] = "-u";
                        myargs[2] = NULL;
                        execvp("date", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }
                }

                else if ((count == 2) && (strcmp(arr[1],"-R") == 0)){
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) { 
                        char *myargs[3];
                        myargs[0] = "date"; 
                        myargs[1] = "-R";
                        myargs[2] = NULL;
                        execvp("date", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }
                }
                else{
                    printf("Invalid flag used\n");
                }
            }
            else{
                if (count == 2){ // only ls
                    rc = pthread_create(&p1, NULL, myFunc, "date"); 
                    assert(rc == 0); // if pthread successful, returns 0
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else if ((count == 3) && (strcmp(arr[1],"-u") == 0)){
                    rc = pthread_create(&p1, NULL, myFunc, "date -u"); 
                    assert(rc == 0); 
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else if ((count == 3) && (strcmp(arr[1],"-R") == 0)){
                    rc = pthread_create(&p1, NULL, myFunc, "date -R"); 
                    assert(rc == 0); 
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else{
                    printf("Invalid flag used\n");
                }
            }
        }

        // mkdir
        if ((strcmp(arr[0],"mkdir") == 0)){ 
            if (!useThread){
                if (count == 1){
                    printf("Missing operands\n");
                }

                else if (count == 2){
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) { 
                        char *myargs[3];
                        myargs[0] = "mkdir"; 
                        myargs[1] = arr[1];
                        myargs[2] = NULL;
                        execvp("mkdir", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }
                    
                }

                else if ((count == 3) && (strcmp(arr[1],"-v") == 0)){
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) {
                        char *myargs[4];
                        myargs[0] = "mkdir"; 
                        myargs[1] = "-v";
                        myargs[2] = arr[2];
                        myargs[3] = NULL;
                        execvp("mkdir", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }
                }

                else if ((count == 3) && (strcmp(arr[1],"-p") == 0)){
                    rc = fork();
                    if (rc < 0) { 
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } 
                    else if (rc == 0) {
                        char *myargs[4];
                        myargs[0] = "mkdir"; 
                        myargs[1] = "-p";
                        myargs[2] = arr[2];
                        myargs[3] = NULL;
                        execvp("mkdir", myargs); 
                    } 
                    else { 
                        int wc = wait(NULL);
                    }
                }
                else{
                    printf("Invalid flag used\n");
                }
            }
            else{
                if (count == 2){
                    printf("Missing operands\n");
                }
                else if (count == 3){ // only one dir
                    strcpy(temp,arr[0]);
                    strcat(temp," ");
                    strcat(temp,arr[1]);
                    rc = pthread_create(&p1, NULL, myFunc, temp); 
                    assert(rc == 0); // if pthread successful, returns 0
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else if ((count == 4) && (strcmp(arr[1],"-v") == 0)){
                    strcpy(temp,arr[0]);
                    strcat(temp," ");
                    strcat(temp,arr[1]);
                    strcat(temp," ");
                    strcat(temp,arr[2]);
                    rc = pthread_create(&p1, NULL, myFunc, temp); 
                    assert(rc == 0); // if pthread successful, returns 0
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else if ((count == 4) && (strcmp(arr[1],"-p") == 0)){
                    strcpy(temp,arr[0]);
                    strcat(temp," ");
                    strcat(temp,arr[1]);
                    strcat(temp," ");
                    strcat(temp,arr[2]);
                    rc = pthread_create(&p1, NULL, myFunc, temp); 
                    assert(rc == 0); // if pthread successful, returns 0
                    rc = pthread_join(p1, NULL); 
                    assert(rc == 0);
                }
                else{
                    printf("Invalid flag used\n");
                }
            }
        }

        // touch
        if ((strcmp(arr[0],"touch") == 0)){
            rc = fork();
            if (rc < 0) { // fork failed; exit
                fprintf(stderr, "fork failed\n");
                exit(1);
            } 
            else if (rc == 0) { // child (new process)
                char *myargs[3];
                myargs[0] = "touch"; 
                myargs[1] = arr[1];
                myargs[2] = NULL;
                execvp("touch", myargs); 
            } 
            else { // parent goes down this path (main)
                int wc = wait(NULL);
            }
        }

        // cat
        if ((strcmp(arr[0],"cat") == 0)){
            if (!useThread){
                if (count == 1){
                    printf("Missing operands\n");
                }

                else if (count == 2){
                    val = checkValid(arr[1]);
                    if (val){
                        isfile = isFile(arr[1]);
                        if (isfile){
                            rc = fork();

                            if (rc < 0) { 
                                fprintf(stderr, "fork failed\n");
                                exit(1);
                            } 
                            else if (rc == 0) {
                                char *myargs[3];
                                myargs[0] = "cat"; 
                                myargs[1] = arr[1];
                                myargs[2] = NULL;
                                execvp("cat", myargs); 
                            } 
                            else { 
                                int wc = wait(NULL);
                            }
                        }
                        else{
                            printf("Argument is not a file, can't use cat on it\n");
                        }
                    }
                    else{
                        printf("file does not exist\n");
                    }
                }

                else if ((count == 3) && (strcmp(arr[1],"-n") == 0)){ 
                    val = checkValid(arr[2]);
                    if (val){
                        isfile = isFile(arr[2]);
                        if (isfile){
                            rc = fork();
                            if (rc < 0) { 
                                fprintf(stderr, "fork failed\n");
                                exit(1);
                            } 
                            else if (rc == 0) { 
                                char *myargs[4];
                                myargs[0] = "cat"; 
                                myargs[1] = arr[1]; 
                                myargs[2] = arr[2];
                                myargs[3] = NULL;
                                execvp("cat", myargs); 
                            } 
                            else { 
                                int wc = wait(NULL);
                            }
                        }
                        else{
                            printf("Argument is not a file, can't use cat on it\n");
                        }
                    }
                    else{
                        printf("file does not exist\n");
                    }
                }
                else if ((count == 3) && (strcmp(arr[1],"-b") == 0)){ 
                    val = checkValid(arr[2]);
                    if (val){
                        isfile = isFile(arr[2]);
                        if (isfile){
                            rc = fork();
                            if (rc < 0) { 
                                fprintf(stderr, "fork failed\n");
                                exit(1);
                            } 
                            else if (rc == 0) { 
                                char *myargs[4];
                                myargs[0] = "cat"; 
                                myargs[1] = arr[1]; 
                                myargs[2] = arr[2];
                                myargs[3] = NULL;
                                execvp("cat", myargs); 
                            } 
                            else { 
                                int wc = wait(NULL);
                            }
                        }
                        else{
                            printf("Argument is not a file, can't use cat on it\n");
                        }
                    }
                    else{
                        printf("file does not exist\n");
                    }
                }
                else{
                    printf("Invalid flag used\n");
                }
            }
            else{
                if (count == 2){
                    printf("Missing operands\n");
                    // start writing from here
                }
                else if (count == 3){
                    val = checkValid(arr[1]);
                    if (val){
                        isfile = isFile(arr[1]);
                        if (isfile){
                            strcpy(temp,arr[0]);
                            strcat(temp," ");
                            strcat(temp,arr[1]);
                            rc = pthread_create(&p1, NULL, myFunc, temp); 
                            assert(rc == 0); // if pthread successful, returns 0
                            rc = pthread_join(p1, NULL); 
                            assert(rc == 0);
                        }
                        else{
                            printf("Argument is not a file, can't use cat on it\n");
                        }
                    }
                    else{
                        printf("File doesn't exist\n");
                    }
                }
                else if ((count == 4) && (strcmp(arr[1],"-n") == 0)){
                    val = checkValid(arr[2]);
                    if (val){
                        isfile = isFile(arr[2]);
                        if (isfile){
                            strcpy(temp,arr[0]);
                            strcat(temp," ");
                            strcat(temp,arr[1]);
                            strcat(temp," ");
                            strcat(temp,arr[2]);
                            rc = pthread_create(&p1, NULL, myFunc, temp); 
                            assert(rc == 0); // if pthread successful, returns 0
                            rc = pthread_join(p1, NULL); 
                            assert(rc == 0);     
                        }
                        else{
                            printf("Argument is not a file, can't use cat on it\n");
                        }                   
                    }
                    else{
                        printf("File doesn't exist\n");
                    }                    
                }
                else if ((count == 4) && (strcmp(arr[1],"-b") == 0)){
                    val = checkValid(arr[2]);
                    if (val){
                        isfile = isFile(arr[2]);
                        if (isfile){
                            strcpy(temp,arr[0]);
                            strcat(temp," ");
                            strcat(temp,arr[1]);
                            strcat(temp," ");
                            strcat(temp,arr[2]);
                            rc = pthread_create(&p1, NULL, myFunc, temp); 
                            assert(rc == 0); // if pthread successful, returns 0
                            rc = pthread_join(p1, NULL); 
                            assert(rc == 0);  
                        }
                        else{
                            printf("Argument is not a file, can't use cat on it\n");
                        }                      
                    }
                    else{
                        printf("File doesn't exist\n");
                    }                    
                }
                else{
                    printf("Invalid flag used\n");
                }                
            }
        }

        // rm
        if ((strcmp(arr[0],"rm") == 0)){
            if (!useThread){
                if (count == 1){
                    printf("Missing operands\n");
                }

                else if (count == 2){
                    val = checkValid(arr[1]);
                    if (val){
                        isfile = isFile(arr[1]);
                        if (isfile){
                            rc = fork();

                            if (rc < 0) { 
                                fprintf(stderr, "fork failed\n");
                                exit(1);
                            } 
                            else if (rc == 0) { 
                                char *myargs[3];
                                myargs[0] = "rm"; 
                                myargs[1] = arr[1];
                                myargs[2] = NULL;
                                execvp("rm", myargs); 
                            } 
                            else { 
                                int wc = wait(NULL);
                            }
                        }
                        else{
                            printf("Argument is not a file, can't use rm on it\n");
                        }
                    }
                    else{
                        printf("file does not exist\n");
                    }
                }

                else if ((count == 3) && (strcmp(arr[1],"-v") == 0)){
                    val = checkValid(arr[2]);
                    if (val){
                        isfile = isFile(arr[2]);
                        if (isfile){
                            rc = fork();
                            if (rc < 0) { 
                                fprintf(stderr, "fork failed\n");
                                exit(1);
                            } 
                            else if (rc == 0) { 
                                char *myargs[4];
                                myargs[0] = "rm"; 
                                myargs[1] = arr[1]; 
                                myargs[2] = arr[2];
                                myargs[3] = NULL;
                                execvp("rm", myargs); 
                            } 
                            else { 
                                int wc = wait(NULL);
                            }
                        }
                        else{
                            printf("Argument is not a file, can't use rm -v on it\n");
                        }
                    }
                    else{
                        printf("file does not exist\n");
                    }
                }

                else if ((count == 3) && (strcmp(arr[1],"-r") == 0)){
                    val = checkValid(arr[2]);
                    if (val){
                        rc = fork();
                        if (rc < 0) { 
                            fprintf(stderr, "fork failed\n");
                            exit(1);
                        } 
                        else if (rc == 0) { 
                            char *myargs[4];
                            myargs[0] = "rm"; 
                            myargs[1] = arr[1]; 
                            myargs[2] = arr[2];
                            myargs[3] = NULL;
                            execvp("rm", myargs); 
                        } 
                        else { 
                            int wc = wait(NULL);
                        }
                    }
                    else{
                        printf("file / directory does not exist\n");
                    }
                }
                else{
                    printf("Invalid flag used\n");
                }
            }
            else{
                if (count == 2){
                    printf("Missing operands\n");
                }
                else if (count == 3){
                    val = checkValid(arr[1]);
                    if (val){
                        isfile = isFile(arr[1]);
                        if (isfile){
                            strcpy(temp,arr[0]);
                            strcat(temp," ");
                            strcat(temp,arr[1]);
                            rc = pthread_create(&p1, NULL, myFunc, temp); 
                            assert(rc == 0); 
                            rc = pthread_join(p1, NULL); 
                            assert(rc == 0);
                        }
                        else{
                            printf("Argument is not a file, can't use rm on it\n");
                        }
                    }
                    else{
                        printf("file does not exist\n");
                    }
                }
                else if ((count == 4) && (strcmp(arr[1],"-v") == 0)){
                    val = checkValid(arr[2]);
                    if (val){
                        isfile = isFile(arr[2]);
                        if (isfile){
                            strcpy(temp,arr[0]);
                            strcat(temp," ");
                            strcat(temp,arr[1]);
                            strcat(temp," ");
                            strcat(temp,arr[2]);
                            rc = pthread_create(&p1, NULL, myFunc, temp); 
                            assert(rc == 0); 
                            rc = pthread_join(p1, NULL); 
                            assert(rc == 0);
                        }
                        else{
                            printf("Argument is not a file, can't use rm -v on it\n");
                        }
                    }
                    else{
                        printf("file does not exist\n");
                    }
                }
                else if ((count == 4) && (strcmp(arr[1],"-r") == 0)){
                    val = checkValid(arr[2]);
                    if (val){
                        strcpy(temp,arr[0]);
                        strcat(temp," ");
                        strcat(temp,arr[1]);
                        strcat(temp," ");
                        strcat(temp,arr[2]);
                        rc = pthread_create(&p1, NULL, myFunc, temp); 
                        assert(rc == 0); 
                        rc = pthread_join(p1, NULL); 
                        assert(rc == 0);
                    }
                    else{
                        printf("file/directory does not exist\n");
                    }
                }
                else{
                    printf("Invalid flag used\n");
                }
            }
        }

	}
	while (1);
	
	return 0;
}
