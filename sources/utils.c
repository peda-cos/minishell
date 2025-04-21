/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 15:00:20 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_parent_builtin(t_process_command_args *param)
{
	char	*command;
	char	**command_with_args;

	command = param->cmd->args[0];
	command_with_args = param->cmd->args;
	if (!ft_strcmp(command, "cd"))
		*param->last_exit = builtin_cd(command_with_args, param->env);
	else if (!ft_strcmp(command, "export"))
		*param->last_exit = builtin_export(command_with_args, param->env);
	else if (!ft_strcmp(command, "unset"))
		*param->last_exit = builtin_unset(command_with_args, param->env);
	else if (!ft_strcmp(command, "exit"))
	{
		*param->last_exit = builtin_exit(param);
		if (*param->last_exit != 1)
			exit_free(*param->last_exit, param->env, param->cmd, param->tokens);
	}
}

void	execute_command_line(t_command *cmd, char ***env, int *last_exit,
	t_token *tokens)
{
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	setup_execution_signals();
	execute_command(cmd, env, last_exit, tokens);
	setup_interactive_signals();
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
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
	t_process_command_args	args;

	if (!cmd)
		return ;
	args.cmd = cmd;
	args.env = env;
	args.tokens = tokens;
	args.last_exit = last_exit;
	args.has_fd_redirect_to_stderr = FALSE;
	setup_output_redirection(&args);
	if (cmd->args && !cmd->next && is_parent_builtin(cmd->args[0]))
		execute_parent_builtin(&args);
	else
		execute_command_line(cmd, env, last_exit, tokens);
	free_commands(cmd);
}

void	process_input(char *input, char ***env, int *last_exit)
{
	t_token		*tokens;
	t_command	*cmd;

	if (!input || !*input)
		return ;
	add_history(input);
	save_command_to_history(input);
	g_signal_received = 0;
	tokens = tokenize_input(input);
	if (process_tokens(&tokens, last_exit))
		return ;
	cmd = parse_tokens(tokens, *env, *last_exit);
	execute_parsed_commands(cmd, env, last_exit, tokens);
	free_tokens(tokens);
}
