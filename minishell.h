/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/19 19:28:54 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>    // readline, add_history
# include <readline/history.h>     // rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay

# include <stdio.h>                // printf, perror
# include <stdlib.h>               // malloc, free, exit, getenv
# include <unistd.h>               // write, access, read, close, fork, getcwd, chdir, isatty, ttyname, ttyslot, dup, dup2, pipe
# include <fcntl.h>                // open
# include <sys/types.h>            // wait, waitpid, wait3, wait4, stat, lstat, fstat, opendir, readdir, closedir
# include <sys/stat.h>             // stat, lstat, fstat
# include <sys/wait.h>             // wait, waitpid, wait3, wait4
# include <signal.h>               // signal, sigaction, sigemptyset, sigaddset, kill
# include <dirent.h>               // opendir, readdir, closedir
# include <string.h>               // strerror
# include <sys/ioctl.h>            // ioctl
# include <termios.h>              // tcsetattr, tcgetattr
# include <curses.h>               // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <term.h>                 // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

#endif