/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:19:21 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/02 20:10:23 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	handle_empty_command(t_command *cmd)
{
	char	buffer[4096];
	ssize_t	bytes_read;

	if (cmd->argc <= 0)
	{
		if (cmd->output_file_list[0] != NULL)
		{
			bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
			while (bytes_read > 0)
			{
				write(STDOUT_FILENO, buffer, bytes_read);
				bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
			}
		}
		return (1);
	}
	return (0);
}

static void	set_pipefd_stdin(t_process_command_args *param)
{
	if (param->cmd->next && param->pipefd[0] > 0)
	{
		close(param->pipefd[1]);
		dup2(param->pipefd[0], STDIN_FILENO);
		close(param->pipefd[0]);
	}
}

void	child_process(t_process_command_args *param)
{
	struct sigaction	sa;

	if (!param->cmd || !param->env || !(param->last_exit))
		exit_free(1, param->env, param->cmd, param->tokens);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	if (setup_child_io(param) < 0)
		exit_free(1, param->env, param->cmd, param->tokens);
	if (handle_empty_command(param->cmd))
		exit_free(0, param->env, param->cmd, param->tokens);
	if (is_builtin(param->cmd->args[0]))
		exit_free(execute_builtin(param), param->env, param->cmd,
			param->tokens);
	exit_free(execute_external(param->cmd, *param->env), param->env, param->cmd,
		param->tokens);
}

void	parent_process(t_process_command_args *param)
{
	int	status;
	int	exit_status;

	status = 0;
	if (!param->cmd || !(param->last_exit))
		return ;
	set_pipefd_stdin(param);
	if (!param->cmd->next)
	{
		waitpid(param->pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			exit_status = WTERMSIG(status) + 128;
			if (WTERMSIG(status) == SIGINT)
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		}
		else
			exit_status = WEXITSTATUS(status);
		*(param->last_exit) = exit_status;
	}
}

int	process_command(t_command *cmd, char ***envs, int *last_exit,
		t_token *tokens)
{
	int						pipefd[2];
	t_process_command_args	args;

	if (!cmd || !envs || !last_exit)
		return (-1);
	if (setup_pipe(cmd, pipefd) < 0)
		return (-1);
	args.cmd = cmd;
	args.env = envs;
	args.tokens = tokens;
	args.last_exit = last_exit;
	args.pid = fork();
	args.pipefd[0] = pipefd[0];
	args.pipefd[1] = pipefd[1];
	args.has_fd_redirect_to_stderr = FALSE;
	if (args.pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (args.pid == 0)
		child_process(&args);
	else
		parent_process(&args);
	return (0);
}
