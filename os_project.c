#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h>


// this arguments is information about file directory.
struct arg_function
{
    char dir[1024];
    int depth;
    long int root;
    long int maximum;
    long int minimum;
    char min_directory[1024];
    char max_directory[1024];
    int number_of_files;
    int text_filse;
    int png_files;
    int jpg_filse;
    int zip_filse;
    int mp4_filse;
    int pptx_filse;
    int pdf_files;
    int c_filse;
    int uknown_filse;
};

pthread_mutex_t lock1; // lock varible

// this function convert byte -> kilo byte -> mega byte -> giga byte.
void calculateSize(size_t size)
{   
  static const char *SIZES[] = { "B", "kB", "MB", "GB" };
    size_t div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1024);
        div++;   
        size /= 1024;
    }

    printf("%.2f %s\n", (float)size + (float)rem / 1024.0, SIZES[div]);
}

// this function return size of file.
long int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    fclose(fp); 
  
    return res; 
} 


void recurive_find_files(char *dir, struct arg_function *arguments)
{
    char s[1024]; //->for live direction
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {   // -> sure about write directory
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }

    chdir(dir);

    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;

            /* Recurse at a new indent level */

            recurive_find_files(entry->d_name, arguments);

        }
        else{ //we found file not folder

            // Update statistics using a lock to ensure synchronization
            pthread_mutex_lock(&lock1); // if we dont have somthing like this then the programm giving somthingg incorrect...
            // start critical section

            arguments->number_of_files++;   // total number of files ++ we file


            //update root size
            arguments->root += findSize(entry->d_name);

            //update maximum
            if(findSize(entry->d_name) > arguments->maximum)
            {
                arguments->maximum = findSize(entry->d_name);
                memcpy(arguments->max_directory, getcwd(s, 1024), strlen(getcwd(s, 1024)));
                strcat(arguments->max_directory, "/");
                strcat(arguments->max_directory, entry->d_name);
            }

            //update minimum
            if(findSize(entry->d_name) < arguments->minimum)
            {
                arguments->minimum = findSize(entry->d_name);
                memcpy(arguments->min_directory, getcwd(s, 1024), strlen(getcwd(s, 1024)));
                strcat(arguments->min_directory, "/");
                strcat(arguments->min_directory, entry->d_name);
            }

            //check what type of file we have.
            if(strstr(entry->d_name, ".txt")!=NULL)
                arguments->text_filse++;
            else if(strstr(entry->d_name, ".png")!=NULL)
                arguments->png_files++;
            else if(strstr(entry->d_name, ".jpg")!=NULL)
                arguments->jpg_filse++;
            else if(strstr(entry->d_name, ".pdf")!=NULL)
                arguments->pdf_files++;
            else if(strstr(entry->d_name, ".mp4")!=NULL)
                arguments->mp4_filse++;
            else if(strstr(entry->d_name, ".zip")!=NULL)
                arguments->zip_filse++;
            else if(strstr(entry->d_name, ".pptx")!=NULL)
                arguments->pptx_filse++;
            else if(strstr(entry->d_name, ".c")!=NULL)
                arguments->c_filse++;
            else
                arguments->uknown_filse++;
            
            pthread_mutex_unlock(&lock1); // exit critical section

        }
    }
    chdir("..");
    closedir(dp);
}

void *thread_func(void *arg) {

    char *path = (char *)arg;
    struct arg_function *arguments = malloc(sizeof(struct arg_function));
    memset(arguments, 0, sizeof(struct arg_function));
    arguments->maximum = 0;
    arguments->minimum = 429496729;
    arguments->root = 0;
    recurive_find_files(path, arguments);
    pthread_exit(arguments);
}

int main()
{
    char s[1024]; 
    char check_directory[1024];
    // start trying get input -> go back to home
    while(s[1] == 'h')
    {
        chdir(".."); // -> back to home
        getcwd(s, 1024); // ->  new s
    }
    // get input
    scanf("%s", check_directory);// -> get directory
    pthread_mutex_init(&lock1, NULL);

    pthread_t thread;

    if (pthread_create(&thread, NULL, thread_func, check_directory) != 0) { //create
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }
    struct arg_function *arguments;
    if (pthread_join(thread, (void **)&arguments) != 0) { //join
        perror("Error joining thread");
        exit(EXIT_FAILURE);
    }

    //show result program
    printf("root size is : \n");
    calculateSize(arguments->root);

    printf("\n");

    printf("file with the smallest size :\n");
    puts(arguments->min_directory);
    calculateSize(arguments->minimum);
    
    printf("\n");

    printf("file with the largest size :\n");
    puts(arguments->max_directory);
    calculateSize(arguments->maximum);

    printf("\n");

    printf("the total number of filse :  %d\n", arguments->number_of_files);
    printf("text_filse :  %d\n", arguments->text_filse);
    printf("png_files :  %d\n", arguments->png_files);
    printf("jpg_filse :  %d\n", arguments->jpg_filse);
    printf("pdf_files :  %d\n", arguments->pdf_files);
    printf("mp4_filse :  %d\n", arguments->mp4_filse);
    printf("zip_filse :  %d\n", arguments->zip_filse);
    printf("pptx_filse :  %d\n", arguments->pptx_filse);
    printf("c_files :  %d\n", arguments->c_filse);
    printf("uknown_filse :  %d\n", arguments->uknown_filse);

    free(arguments);

    pthread_mutex_destroy(&lock1);

//      /home/amin/Desktop/Main
    
    
    return 0; 
}