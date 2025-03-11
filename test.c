#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ft_strtok(char *input, const char *delimiter)
{
	static char	*backup;
	char		*token;

	if (input != NULL)
		backup = input;
	if (backup == NULL)
		return (NULL);
	// Ignorar delimitadores iniciais
	while (*backup && strchr(delimiter, *backup))
		backup++;
	if (*backup == '\0')
		return (NULL);
	token = backup;
	// Encontrar o próximo delimitador
	while (*backup)
	{
		if (strchr(delimiter, *backup))
		{
			*backup = '\0';
			backup++;
			break ;
		}
		backup++;
	}
	// Se chegamos ao final da string, definir backup como NULL
	if (*backup == '\0')
		backup = NULL;
	return (token);
}

int main(int argc, char **argv)
{
	char *str;
	char *token;
	char *delimiter;

	if (argc != 3)
	{
		printf("Usage: %s <string> <delimiter>\n", argv[0]);
		return 1;
	}
	str = strdup(argv[1]);
	delimiter = strdup(argv[2]);
	token = strtok(str, delimiter);
	while (token != NULL)
	{
		printf("%s\n", token);
		token = strtok(NULL, delimiter);
	}
	free(str);
	free(delimiter);
	return 0;
}