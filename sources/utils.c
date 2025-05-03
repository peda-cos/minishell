/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/02 18:58:43 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_last_arg_without_pipe_executed(
	t_token *tokens, t_command *cmd, char ***envs)
{
	t_token	*current_token;

	if (!tokens || !cmd || !envs)
		return ;
	current_token = tokens;
	while (current_token && current_token->type != PIPE)
		current_token = current_token->next;
	if (current_token && current_token->type == PIPE)
		return ;
	set_underscore_arg_value(cmd, envs);
}

void	execute_parent_builtin(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	t_process_command_args	args;
	char					*command;
	char					**command_with_args;

	args.cmd = cmd;
	args.env = env;
	args.tokens = tokens;
	args.last_exit = last_exit;
	args.has_fd_redirect_to_stderr = FALSE;
	setup_output_redirection(&args);
	command = cmd->args[0];
	command_with_args = cmd->args;
	if (!ft_strcmp(command, "cd"))
		*last_exit = builtin_cd(command_with_args, env);
	else if (!ft_strcmp(command, "export"))
		*last_exit = builtin_export(command_with_args, env);
	else if (!ft_strcmp(command, "unset"))
		*last_exit = builtin_unset(command_with_args, env);
	else if (!ft_strcmp(command, "exit"))
	{
		*last_exit = builtin_exit(&args);
		if (*last_exit != 1)
			exit_free(*last_exit, env, cmd, tokens);
	}
}

int	process_tokens(t_token **tokens, int *last_exit)
{
	if (!*tokens)
		return (1);
	if (validate_tokens(tokens))
	{
		*last_exit = STDERR_FILENO;
		free_tokens(*tokens);
		return (1);
	}
	return (0);
}

void	execute_parsed_commands(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	setup_execution_signals();
	if (!cmd)
		return ;
	if (cmd->args && !cmd->next && is_parent_builtin(cmd->args[0]))
		execute_parent_builtin(cmd, env, last_exit, tokens);
	else
		execute_command(cmd, env, last_exit, tokens);
	setup_interactive_signals();
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdout);
	close(saved_stderr);
	close(saved_stdin);
	free_commands(cmd);
}

void	process_invalid_command(t_command *cmd,
	int *last_exit, t_token *tokens)
{
	if (cmd && cmd->args && *cmd->args[0] == '\0')
	{
		ft_putendl_fd("Minishell: Command  '' not found", STDERR_FILENO);
		*last_exit = 127;
	}
	else
		*last_exit = 1;
	free_commands(cmd);
	free_tokens(tokens);
}
