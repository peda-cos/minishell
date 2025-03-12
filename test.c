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
	token	= ft_strtok(str, delimiter);
	printf("Token: %s\n", token);
	int i = strlen(token);
	int is_quote	= 0;
	while (token && argv[1][i])
	{
		// printf("I: %d\n", i);
		if (strchr(" \t\n", argv[1][i]))
		{
			i++;
			continue ;
		}
		if (argv[1][i] == '\''	|| argv[1][i] == '\"')
		{
			if (!is_quote)
			{
				is_quote = 1;
				delimiter = strdup(argv[1][i]	== '\'' ? "'" : "\"");
			}
			else
			{
				is_quote = 0;
				delimiter = strdup(argv[2]);
				i++;
				continue;
			}
			i++;
		}
		else 
			delimiter = strdup(argv[2]);
		// printf("Current char: %c\n", argv[1][i]);
		token	= ft_strtok(NULL, delimiter);
		if (token == NULL)
			break ;
		printf("Token: %s\n", token);
		i += strlen(token);
	}
	free(str);
	free(delimiter);
	return 0;
}

// set args "echo 123 \"'    '\" | echo '_  joao   _'" " " | cat -e