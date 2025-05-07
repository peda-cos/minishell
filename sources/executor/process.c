/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:19:21 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/07 14:05:02 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Handles the case when the command is empty (no arguments)
 * @param cmd The command structure to check
 * @return 1 if the command was empty and processed successfully,
 *         -1 if an error occurred during processing,
 *         0 if the command was not empty (had arguments).
 * @note If argc <= 0, this function reads from STDIN_FILENO and writes to
 *       STDOUT_FILENO, mimicking 'cat' behavior. Errors during read/write
 *       are reported via perror.
 */
static int	handle_empty_command(t_command *cmd)
{
	char	buffer[4096];
	ssize_t	bytes_read;
	ssize_t	bytes_written;

	if (cmd->argc <= 0)
	{
		while (TRUE)
		{
			bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
			if (bytes_read < 0)
			{
				perror("minishell: read error in empty command segment");
				return (-1);
			}
			if (bytes_read == 0)
				break ;
			bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
			if (bytes_written < 0)
			{
				perror("minishell: write error in empty command segment");
				return (-1);
			}
		}
		return (1);
	}
	return (0);
}

/**
 * @brief Sets the standard input file descriptor for the command
 * @param param The command arguments and environment variables
 * @return void
 * @note Closes the pipe file descriptors
 * and duplicates the read end of the pipe to stdin
 */
static void	set_pipefd_stdin(t_process_command_args *param)
{
	if (param->cmd->next && param->pipefd[0] > 0)
	{
		close(param->pipefd[1]);
		dup2(param->pipefd[0], STDIN_FILENO);
		close(param->pipefd[0]);
	}
}

/**
 * @brief Sets up input and output file descriptors for a child process.
 * @param arg Structure containing command, environment, pipe FDs, etc.
 * @return 0 on success, -1 on failure.
 * @note This function handles:

	*         1. Input redirection (from file or heredoc) via setup_file_input_redirection.
 *         2. Output redirection (to file) via setup_output_redirection.
 *         3. Output to a pipe if cmd->next exists and no file output occurred.
 *         4. Closing of all used/duplicated pipe FDs in the child.
 */
int	setup_child_io(t_process_command_args *arg)
{
	int	file_output_redirected_status;

	if (!arg || !arg->cmd)
		return (-1);
	if (setup_file_input_redirection(arg->cmd, *(arg->env), *(arg->last_exit)) < 0)
	{
		perror("minishell: input redirection setup failed");
		return (-1);
	}
	file_output_redirected_status = setup_output_redirection(arg);
	if (file_output_redirected_status < 0)
	{
		perror("minishell: output redirection setup failed");
		return (-1);
	}
	if (arg->cmd->next && file_output_redirected_status == 0)
	{
		if (arg->pipefd[1] < 0)
		{
			ft_putstr_fd("minishell: invalid pipe for output\n", STDERR_FILENO);
			return (-1);
		}
		if (dup2(arg->pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2 for pipe STDOUT failed");
			return (-1);
		}
	}
	if (arg->pipefd[0] >= 0)
		close(arg->pipefd[0]);
	if (arg->pipefd[1] >= 0)
		close(arg->pipefd[1]);
	return (0);
}

/**
 * @brief Handles the execution in a child process
 * @param param The command arguments and environment variables
 * @return void
 * @note Sets up signal handling, redirects I/O, and executes the command
 */
void	child_process(t_process_command_args *param)
{
	struct sigaction	sa;
	int					empty_cmd_status;

	if (!param->cmd || !param->env || !(param->last_exit))
		exit_free(1, param->env, param->cmd, param->tokens);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	if (setup_child_io(param) < 0)
		exit_free(1, param->env, param->cmd, param->tokens);
	empty_cmd_status = handle_empty_command(param->cmd);
	if (empty_cmd_status == 1)
		exit_free(0, param->env, param->cmd, param->tokens);
	else if (empty_cmd_status == -1)
		exit_free(1, param->env, param->cmd, param->tokens);
	if (is_builtin(param->cmd->args[0]))
		exit_free(execute_builtin(param), param->env, param->cmd,
			param->tokens);
	exit_free(execute_external(param->cmd, *param->env), param->env, param->cmd,
		param->tokens);
}

/**
 * @brief Handles the parent process after forking
 * @param param The command arguments and environment variables
 * @return void
 * @note Waits for the child process to finish and handles signals
 */
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

/**
 * @brief Processes the command and its arguments
 * @param cmd The command structure to be executed
 * @param envs Array of environment variables
 * @param last_exit Pointer to the last exit status
 * @param tokens Pointer to the token list for cleanup during exit
 * @return 0 on success, -1 on error
 * @note Sets up pipes and forks a child process for command execution
 */
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
