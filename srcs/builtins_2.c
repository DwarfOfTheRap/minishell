#include "minishell.h"

static int	echo_n_parser(char *str)
{
	if (*str != '-')
		return (1);
	while (*(str + 1))
	{
		if (*(str + 1) != 'n')
			return (1);
		str++;
	}
	return (0);
}

int			echo_builtin(t_command *cmd)
{
	int		flag;
	int		i;

	i = 0;
	if (!cmd->command[i + 1])
	{
		ft_putchar_fd('\n', cmd->fd[1]);
		return (0);
	}
	flag = 0;
	while (!echo_n_parser(cmd->command[++i]))
		flag = 1;
	while (cmd->command[i] != NULL)
	{
		ft_putstr_fd(cmd->command[i], cmd->fd[1]);
		ft_putchar_fd(' ', cmd->fd[1]);
		i++;
	}
	if (!flag)
		ft_putchar_fd('\n', cmd->fd[1]);
	return (0);
}

int		pwd_builtin(t_command *cmd)
{
	char	*stored;

	stored = getcwd(NULL, 0);
	ft_putstr_fd(stored, cmd->fd[1]);
	ft_putchar_fd('\n', cmd->fd[1]);
	free(stored);
	return (0);
}

/*
** If the index equals the length of the first argument, it means it is and
** intire numerical argument, so we can check the second argument, if any.
**
** 1. Checking if there is no second argument, if so, multiplies and module
** divides the errno so we can have a correct exit status (and exit minishell).
** 2. Otherwise, if there is a 2nd parameter, output the correct error message.
*/
int		exit_arg(t_command *cmd, size_t i)
{
	if (i == ft_strlen(cmd->command[1]))
	{
		if (!cmd->command[2])
		{
			errno = ft_atoi(cmd->command[1]);
			errno += 256;
			errno %= 256;
		//	return (errno);
			exit(errno);
		}
		else if (cmd->command[2])
		{
			error_msg("bash", cmd, NULL, "too many arguments");
			errno = 1;
			return (errno);
		}
	}
	return (0);
}

/*
** exit <nb less than long long>
** output: exit\n
** echo $? = if nb > 255 < longlong, it starts over again to count from 0 to 255
**
** exit <any string or number bigger than a long long>
** output: exit\nbash: exit: <argv>: numeric argument required\n
** echo $? = 2
**
** exit <str or nb less than long long> <str or nb less than long long>
** output: exit\nbash: exit: too many arguments\n
** IT DOES EXIT THE SHELL
** echo $? = 1
**
** If <argv> is omitted, the exit status is that of the last command executed.
**
**
** If there is one argument:
**		1. if it's a + or -, increase the endex (ignore it).
**		2. while the argument is a number, increase the index.
**		3. if the index is different form the length of the first argument, then
**		it means that this argument does not contains only numberm an so, it's
**		not a intire numeric argument. Then, output the correct error messsage.
*/
int		exit_builtin(t_command *cmd)
{
	size_t	i;

	i = 0;
	errno = 0;
	ft_putstr_fd("exit\n", 2);
	if (cmd->command[1] == NULL)
		return (0);
	//	exit(0);
	else if (cmd->command[1])
	{
		if (cmd->command[1][0] == '+' || cmd->command[1][0] == '-')
			i++;
		while (ft_isdigit((char)cmd->command[1][i]) == 1)
			i++;
		if (i != ft_strlen(cmd->command[1]))
		{
			error_msg("bas", cmd, cmd->command[1], "numeric argument required");
			errno = 2;
			return (errno);
		//	exit(2);
		}
		exit_arg(cmd, i);
	}
	return (0);
}
