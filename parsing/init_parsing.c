/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elchakir <elchakir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:20:21 by elchakir          #+#    #+#             */
/*   Updated: 2024/08/04 06:15:20 by elchakir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_command(Command *cmd)
{
	cmd->argv = (char **)malloc(MAX_TOKENS * sizeof(char *));
	cmd->redirection = NULL;
	cmd->heredoc = NULL;
	cmd->next = NULL;
}

Command	*create_command(void)
{
	Command	*cmd;

	cmd = (Command *)malloc(sizeof(Command));
	if (!cmd)
		return (NULL);
	init_command(cmd);
	return (cmd);
}

void	add_command_to_list(Command **head, Command *current)
{
	Command	*last;

	if (*head == NULL)
		*head = current;
	else
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = current;
	}
}

void	handle_word(Token *tokens, int *i, Command **current, int *argc)
{
	if (*current == NULL)
	{
		*current = create_command();
		*argc = 0;
	}
	if(tokens[*i + 1].type == TOKEN_AMBIGUOUS)
		(*current)->type = tokens[*i + 1].type;
	(*current)->argv[(*argc)++] = tokens[*i].value;
}
int is_token(const char *p)
{
    char *start;
    start = (char *)p;
    while(ft_isspace(*start))
        start++;
    if(*start == '\0' || *start == '|')
        return 1;
return 0;
}