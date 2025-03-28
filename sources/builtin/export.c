/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:51 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/28 00:42:22 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	display_environment(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
// TODO: Nunca é chamado, identificar o motivo
// Possivelmente o problema está no find_env_index
static int	update_existing_entry(char ***env, char *new_entry, int idx)
{
	printf("Updating existing entry\n");
	free((*env)[idx]);
	(*env)[idx] = new_entry;
	return (0);
}

static int	add_new_entry(char ***env, char *new_entry)
{
	printf("Adding new entry\n");
	char	**new_env;
	int		count;
	int		j;

	count = 0;
	while ((*env)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	j = 0;
	while (j < count)
	{
		new_env[j] = (*env)[j];
		j++;
	}
	new_env[j] = new_entry;
	new_env[j + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

static int	process_export_arg(char ***env, char *arg)
{
	int		idx;
	char	*new_entry;

	new_entry = ft_strdup(arg);
	if (!new_entry)
		return (1);
	idx = find_env_index(arg, *env);
	if (idx >= 0)
		return (update_existing_entry(env, new_entry, idx));
	else
		return (add_new_entry(env, new_entry));
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;

	if (!*env)
		return (1);
	if (!args[1])
		return (display_environment(*env));
	i = 1;
	status = 0;
	while (args[i] && status == 0)
	{
		status = process_export_arg(env, args[i]);
		i++;
	}
	return (status);
}

// TODO: Ainda há vazamento de memória
// Desbravando  ainda o motivo
//
// $ cc -I . -I ../../includes -I ../../libft *.c ../../libft/libft.a
// $ valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./a.out 
//
// ==30739== 16 bytes in 1 blocks are definitely lost in loss record 1 of 1
// ==30739==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
// ==30739==    by 0x109696: add_new_entry (in /mnt/c/Users/jonnathan-ls/42-cursus/minishell/sources/builtin/a.out)
// ==30739==    by 0x1097C5: process_export_arg (in /mnt/c/Users/jonnathan-ls/42-cursus/minishell/sources/builtin/a.out)
// ==30739==    by 0x109848: builtin_export (in /mnt/c/Users/jonnathan-ls/42-cursus/minishell/sources/builtin/a.out)
// ==30739==    by 0x1098EF: main (in /mnt/c/Users/jonnathan-ls/42-cursus/minishell/sources/builtin/a.out)
//
// int main(int argc, char **argv, char **env)
// {
// 	char *args[] = { "export", "HELLO=world", "HELLO=world2", NULL };
// 	int last_exit = 0;
// 	char **env_copy = copy_env(env);
// 	builtin_export(args, &env_copy);
// 	while (*env_copy)
// 	{
// 		free(*env_copy);
// 		env_copy++;
// 	}
// 	return (0);
// }