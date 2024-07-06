#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 4096

void print_current_directory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

int main(int argc, char *argv[]) {
    char current_dir[PATH_MAX];
    char *last_dir = getenv("OLDPWD");

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd() error");
        return 1;
    }

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "cd -") == 0) {
        if (last_dir == NULL) {
            fprintf(stderr, "No previous directory to change to.\n");
            return 1;
        }
        if (chdir(last_dir) == -1) {
            perror("chdir() error");
            return 1;
        }
        if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
            perror("getcwd() error");
            return 1;
        }
        print_current_directory();
    } else if (strncmp(argv[1], "cd ", 3) == 0) {
        const char *new_dir = argv[1] + 3;
        if (chdir(new_dir) == -1) {
            perror("chdir() error");
            return 1;
        }
        if (setenv("OLDPWD", current_dir, 1) == -1) {
            perror("setenv() error");
            return 1;
        }
        print_current_directory();
    } else if (strcmp(argv[1], "cd") == 0) {
        print_current_directory();
    } else {
        fprintf(stderr, "Invalid command.\n");
        return 1;
    }

    return 0;
}
