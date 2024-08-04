/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_her_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yozainan <yozainan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:20:21 by elchakir          #+#    #+#             */
/*   Updated: 2024/08/03 16:59:06 by yozainan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void find_when_strat(char *str,t_data *data)
{
	int i;
	char *p;
	i = 0;
	p = ft_strchr(str,'$');
	p++;
	while (ft_isalnum(*p) || *p == '_')
		p++;
	printf("%s",expand_variable(p,str - p,data->env_list));
	//skdhfksjdfnskfnksdjnf$HOMEfmdlkngkldfg 	\n
}

void	check_limit_her_doc(Redirection *redir)
{
	Redirection	*tmp_redir;
	int			counter;

	counter = 0;
	tmp_redir = redir;
	while (tmp_redir)
	{
		if (tmp_redir->type == TOKEN_HERE_DOC)
			counter++;
		tmp_redir = tmp_redir->next;
	}
	if (counter > 16)
	{
		ft_putstr_fd(2, "bash: maximum here-document count exceeded\n");
		exit(2);
	}
}

void	handle_heredoc(t_data *data, Redirection *redir)
{
	char	*line;
	char	*expanded_line;
	char	*new_line;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		data->cmd->redir_erros = -1;
		return ;
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			break ;
		}
		// Remove single quotes from the line
		new_line = remove_single_quotes(redir->filename);
		// Compare the processed line with the limiter
		//              lim            lim
		if (ft_strcmp(line, new_line) == 0)
		{
			free(line);
			free(new_line);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			break ;
		}
		// Expand variables if needed
		if (redir->type == TOKEN_HERE_DOC)
		{
			expanded_line = find_when_strat(new_line, data);
			write(pipe_fd[1], expanded_line, strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(pipe_fd[1], new_line, strlen(new_line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		free(new_line);
	}
	close(pipe_fd[1]);
	data->cmd->fdin = pipe_fd[0];
}
