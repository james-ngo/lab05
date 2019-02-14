#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "mypwd.h"

int main(int argc, char *argv[]) {
	char *path = (char*)malloc(PATH_MAX);
	pwd(".", path);
	printf("%s\n", path);
	free(path);
	return 0;
}

void pwd(char *name, char *path) {
	DIR *d;
	struct dirent *ent;
	struct stat *buf = malloc(sizeof(struct stat));
	dev_t dev_no;
	ino_t ino_no;
	if ((d = opendir(name)) < 0) {
		perror("opendir");
		exit(1);
	}
	if (-1 == stat(".", buf)) {
		perror("stat");
		exit(2);
	}
	dev_no = buf->st_dev;
	ino_no = buf->st_ino;
	closedir(d);
	chdir("..");
	d = opendir(".");
	while ((ent = readdir(d))) {
		if (-1 == stat(ent->d_name, buf)) {
			perror("stat");
			exit(2);
		}
		if (buf->st_dev == dev_no && buf->st_ino == ino_no) {
			if (!strcmp(ent->d_name, ".")) {
				break;
			}
			pwd(".", path);
			strcat(path, "/");
			strcat(path, ent->d_name);
			break;
		}
	}
	free(buf);
	closedir(d);
}
