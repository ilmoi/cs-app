#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int get_stat(char *argv) {
    struct stat sb;

   if (stat(argv, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

   printf("File type:                ");

   switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

   printf("I-node number:            %ld\n", (long) sb.st_ino);

   printf("Mode:                     %lo (octal)\n",
            (unsigned long) sb.st_mode);

   printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n",
            (long) sb.st_uid, (long) sb.st_gid);

   printf("Preferred I/O block size: %ld bytes\n",
            (long) sb.st_blksize);
    printf("File size:                %lld bytes\n",
            (long long) sb.st_size);
    printf("Blocks allocated:         %lld\n",
            (long long) sb.st_blocks);

   printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

   exit(EXIT_SUCCESS);
}

int print_contents(int with_details) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
      //lets also print file info
      //weird somehow the loop breaks just after one turn
      if (with_details > 0) {
        if (dir->d_namlen > 2) get_stat(dir->d_name);
      }
    }
    closedir(d);
  }
  return(0);
}

void myls(int argc, char *argv[], char *envp[]) {

    //print arg count (for debugging)
    printf("%d\n", argc);

    //print contents
    printf("\n\n");
    print_contents(0);

    //ls -la
    if (strcmp(argv[1], "-la") == 0) {
        printf("\n\n");
        print_contents(1);
    }
}

int main(int argc, char *argv[], char *envp[]) {
    printf("works\n");
    myls(argc, argv, envp);
}