#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void printFiles(char *path) {
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (dir == NULL) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            char pathName[1024];
            snprintf(pathName, sizeof(pathName), "%s/%s", path, entry->d_name);
            printFiles(pathName);
        } else {
            printf("%s/%s\n", path, entry->d_name);
        }
    }

    closedir(dir);
}

int main() {
    char path[1024];
    // printf("Enter the path to the directory: ");
    // scanf("%s", path);
    printFiles("/home/kiarash/test");
    return 0;
}