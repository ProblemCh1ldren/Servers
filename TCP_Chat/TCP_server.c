#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main main(int argc, char** argv) {
  int sockfd, newsockfd, clilen, n;
  char line[1000];
  struct sockaddr_in servaddr, cliaddr;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(51000);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //Привязка сокета к локальному адресу.
  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  //Переводим сокет в ожидание.
  if (listen(sockfd, 1) < 0) 
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  clilen = sizeof(cliaddr);
  //Принимаем соединение на сокете.
  if ((newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) 
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  pid_t pid = fork();
  //сообщение ребенка
  if (pid == 0) 
  {
    while (1) 
    {
      printf("String =>");
      fflush(stdin);
      memset(line, 0, 1000);
      fgets(line, 1000, stdin);
      while ((n = write(newsockfd, line, strlen(line) + 1)) < 0) 
      {
        perror("Can\'t write\n");
        perror(NULL);
        close(sockfd);
        close(newsockfd);
        exit(1);
      }
    }
  } 
  else 
  { 
    //сообщение родителя
    while (1) 
    {
      memset(line, 0, 1000);
      while ((n = read(newsockfd, line, 999)) > 0) 
      {
        printf("\nReserved Message: %s\n", line);
      }
      if (n < 0) {
        perror(NULL);
        close(sockfd);
        close(newsockfd);
        exit(1);
      }
    }
  }
}
