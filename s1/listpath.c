// henriquecatani e Mateus_RW

// arguments: [progname] <path>
// lists the directory contents
// or the current path if invalid args

#include <stdio.h>
#include <stdlib.h> // null
#include <dirent.h> // directories
#include <unistd.h>
#include <limits.h> // PATH_MAX

void showCurrentDir () 
{
    char cwd[PATH_MAX]; //current working directory. PATH_MAX - default max length (4096)
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
        printf("%s\n", cwd);
    else 
    {
        perror("Erro ao obter o diretorio atual");
    }
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        showCurrentDir();
        return 0;
    }

    // try to open the directory from the argument
    DIR *dir = opendir(argv[1]);

    if (dir == NULL) 
        showCurrentDir(); // show path if invalid
    else 
    {   // list contents
        struct dirent *input;

        for (input = readdir(dir); input != NULL; input = readdir(dir)) {
            printf("%s\n", input->d_name);
        }
        closedir(dir);
    }

    return 0;
}
