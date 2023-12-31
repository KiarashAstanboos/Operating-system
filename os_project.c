/*

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 
#include <dirent.h>
#include <sys/stat.h>

/*

int main()
{
    FILE* fptr;

    fptr = fopen("test.txt", "r");
    char contet[1000];
    if(fptr != NULL)
    {
        printf("s\n"); 
        fgets(contet, 1000, fptr);
        printf("%s\n", contet);
    }else{
        printf("n\n");
    }

    return 0;
}

void * worekert(void * tid)
{
    long * myid = (long *) tid;
    printf("hjsfbjsfbf %ld\n", *myid);
}

int main()
{
    pthread_t tid;
    pthread_t tid1;
    pthread_t tid2;
    pthread_t * pthreads[] = {&tid,&tid1,&tid2};
    for (int i = 0; i < 3; i++)
    {
        pthread_create(pthreads[i], NULL, worekert, (void *)pthreads[i]);
    }
    
    pthread_exit(NULL);
    return 0;
}
*/
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


// find files in directory 
void printdir(char *dir, int depth, long int* maximum, long int* minimum, char* min_directory, char* max_directory, int* number_of_filse,int* text_filse,int* png_files,
int* jpg_filse,int* pdf_files,int* mp4_filse,int* zip_filse,int* pptx_filse)
{
    char s[400]; 
    struct stat st;
    long size;
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
            printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            printdir(entry->d_name,depth+4, maximum, minimum, min_directory, max_directory, number_of_filse, text_filse, png_files, jpg_filse, pdf_files, mp4_filse, zip_filse, pptx_filse);
        }
        else{
             
            printf("%*s%s\n",depth,"",entry->d_name);
            (*number_of_filse)++;
            //printf("Size of the file is %ld kilo bytes \n", (findSize(entry->d_name)/1024));
            //calculateSize(findSize(entry->d_name));
            if(findSize(entry->d_name) > *maximum)
            {
                *maximum = findSize(entry->d_name);
                //max_directory = NULL;
                memcpy(max_directory, getcwd(s, 400), strlen(getcwd(s, 400)));
                strcat(max_directory, "/");
                strcat(max_directory, entry->d_name);

            }
            if(findSize(entry->d_name) < *minimum)
            {
                *minimum = findSize(entry->d_name);
                //min_directory = NULL;
                memcpy(min_directory, getcwd(s, 400), strlen(getcwd(s, 400)));
                strcat(min_directory, "/");
                strcat(min_directory, entry->d_name);
            }
            if(strstr(entry->d_name, ".txt")!=NULL)
                (*text_filse)++;
            else if(strstr(entry->d_name, ".png")!=NULL)
                (*png_files)++;
            else if(strstr(entry->d_name, ".jpg")!=NULL)
                (*jpg_filse)++;
            else if(strstr(entry->d_name, ".pdf")!=NULL)
                (*pdf_files)++;
            else if(strstr(entry->d_name, ".mp4")!=NULL)
                (*mp4_filse)++;
            else if(strstr(entry->d_name, ".zip")!=NULL)
                (*zip_filse)++;
            else if(strstr(entry->d_name, ".pptx")!=NULL)
                (*pptx_filse)++;

        }
    }
    chdir("..");
    closedir(dp);
}

int main()
{
    char s[100]; 
    char check_directory[1024];
    char min_directory[1024];
    char max_directory[1024];
    long int maximum = 0;
    long int minimum = 429496729;
    int number_of_filse = 0;
    int text_filse = 0;
    int png_files = 0;
    int jpg_filse = 0;
    int pdf_files = 0;
    int mp4_filse = 0;
    int zip_filse = 0;
    int pptx_filse =0;
    // directory -> home
    while(s[1] == 'h')
    {
        chdir(".."); // -> back to home
        getcwd(s, 100); // ->  new s
    }
    
    //printf("%s\n", getcwd(s, 100));// -> getcwd -> get current directory
    scanf("%s", check_directory);
    printdir(check_directory,0, &maximum, &minimum, min_directory, max_directory, &number_of_filse, &text_filse, &png_files, &jpg_filse, &pdf_files, &mp4_filse, &zip_filse, &pptx_filse);
    //printf("%ld\n%ld", minimum,maximum);

    calculateSize(minimum);
    puts(min_directory);
    calculateSize(maximum);
    puts(max_directory);
    printf("the total number of filse :  %d\n", number_of_filse);
    printf("text_filse :  %d\n", text_filse);
    printf("png_files :  %d\n", png_files);
    printf("jpg_filse :  %d\n", jpg_filse);
    printf("pdf_files :  %d\n", pdf_files);
    printf("mp4_filse :  %d\n", mp4_filse);
    printf("zip_filse :  %d\n", zip_filse);
    printf("pptx_filse :  %d\n", pptx_filse);
   // printf("%s\n", getcwd(s, 100)); /home/amin/Desktop/Main"

///home/amin/Desktop/Main
    
    
    return 0; 
}