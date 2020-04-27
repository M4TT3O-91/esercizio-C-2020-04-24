/*scrivere un programma in C:

 è data la stringa di caratteri:



 il processo padre crea un file output.txt (se il file esiste già, va troncato a zero e sovrascritto).

 il processo padre tiene aperto il file output.txt, salvare il descrittore in una variabile fd

 il processo padre crea un processo figlio: l'obiettivo finale è che il file output.txt contenga tutta la
 stringa text_to_write e che i due processi (processo padre e processo figlio) si dividano il lavoro di
 scrittura nel file. La scrittura sul file avviene in "parallelo".

 il processo padre scrive la prima metà di text_to_write nel file, cominciando a scrivere dalla posizione
 iniziale del file.

 il processo figlio scrive l'altra metà di text_to_write del file, cominciando a scrivere da dove il
 processo padre ha terminato di scrivere.

 il processo padre attende la conclusione del processo figlio e poi termina.*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

int main(int argc, char *argv[]) {
	off_t file_offset;
	char *text_to_write =
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. At ultrices mi tempus imperdiet nulla malesuada pellentesque elit. Non enim praesent elementum facilisis leo vel fringilla est ullamcorper. Non quam lacus suspendisse faucibus interdum. Metus vulputate eu scelerisque felis imperdiet. At ultrices mi tempus imperdiet nulla malesuada pellentesque. At tempor commodo ullamcorper a lacus vestibulum. Consequat semper viverra nam libero justo laoreet sit amet. Facilisis magna etiam tempor orci eu. Convallis tellus id interdum velit laoreet id donec ultrices. Interdum velit euismod in pellentesque massa placerat duis ultricies. Senectus et netus et malesuada fames. Pharetra vel turpis nunc eget lorem dolor. Nisi porta lorem mollis aliquam ut porttitor leo a. Euismod elementum nisi quis eleifend quam adipiscing vitae proin.";

	int len = strlen(text_to_write);
	int len1 = len / 2;
	int len2 = len - len1;

	char *s1 = malloc(len1 + 1);
	strncpy(s1, text_to_write, len1);
	strcat(s1, "\0");


	char *s2 = malloc(len2 + 1);
	strncpy(s2, text_to_write + len1, len2);
	strcat(s2, "\0");

	char *file_name;

	file_name = "output.txt";

	int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

	if (fd == -1) {
		perror("Error in open()");
		exit(EXIT_FAILURE);
	}

	switch (fork()) {
		case 0: {
			sleep(1);

			if ((file_offset = lseek(fd, 0, SEEK_END)) == -1) {
				perror("lseek()");
				exit(EXIT_FAILURE);
			}
			int res = write(fd, s2, len2);
			if (res == -1) {
				perror("write()");
				exit(EXIT_FAILURE);
			}
			printf("File offset: %ld\n", file_offset);
			printf("Figlio scrivo %d bytes\n", res);

			break;
		}
		default: {
			int res = write(fd, s1, len1);
			if (res == -1) {
				perror("write()");
				exit(EXIT_FAILURE);
			}
			printf("Padre scrivo %d bytes\n", res);
			wait(NULL);
			printf("Generato con successo '%s'\n",file_name);
			printf("BYE!!");
		}
	}

	free(s1);
	free(s2);

	return 0;
}
