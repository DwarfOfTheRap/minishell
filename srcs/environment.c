#include "minishell.h"

static int	create_env_struct(char *keyvalue, t_env *env)
{
	char	*needle;

	if (!(needle = ft_strchr(keyvalue, '=')))
		return (0);
	env->key = ft_substr(keyvalue, 0, ft_strlen(keyvalue) - ft_strlen(needle));
	env->value = ft_strdup(needle + 1);
	return (1);
}

/*
** Create and store environment in a linked list of t_env from envp variable.
** All the links of the list are malloc() and should be free() once the program
** exit. If the first arg was malloc(), it can be free() after.
**
** @args:
**		char **envp a string table containing entries with the format KEY=VALUE
** @return:
**		t_list *: a pointer to the head of a list.
*/
t_list	*create_env_list(char **envp)
{
	t_env	*new;
	t_list	*env;
	t_list	*tmp_env;

	env = NULL;
	while (*envp != NULL)
	{
		if (!(new = (t_env*)malloc(sizeof(t_env))))
			return (NULL);
		create_env_struct(*envp, new);
		tmp_env = ft_lstnew(new);
		ft_lstadd_front(&env, tmp_env);
		envp++;
	}
	return (env);
}

/*
** Return a table similar to envp from a linked list of t_env.
** The table is malloc() and should be free() after use.
** The result should correspond to the return of the `env` builtin.
**
** @args:
**		t_list *env: a pointer to the head of a linked list.
** @return:
**		char **: a string table containing entries with the format KEY=VALUE
*/
char	**env_list_to_tab(t_list *env)
{
	size_t	count;
	t_list	*tmp_env;
	char	**new_env;
	char	*tmp_str;

	count = 1;
	tmp_env = env;
	while ((tmp_env = tmp_env->next))
		count++;
	if (!(new_env = (char **)malloc(sizeof(char *) * (count + 1))))
		return (NULL);
	new_env[count] = NULL;//setting last string of the array to NULL
	while (--count >= 0 && env != NULL)
	{
		tmp_str = ft_strjoin(ENV_KEY(env), "=");
		new_env[count] = ft_strjoin(tmp_str, ENV_VALUE(env));
		free(tmp_str);
		env = env->next;
	}
	return (new_env);
}

/*
** Create a new t_env from a string (format: KEY=VALUE) and push it on top of
** the linked list.
** The string is duplicated and can be free() after use. If the key already
** exist its old value is replaced by the new one.
**
** @args:
**		t_list **env: a pointer to the first link of the list.
**		char *var: a string with the format KEY=VALUE
*/
void	add_env_variable(t_list **env, char *var)
{
	t_env	*new;
	t_list	*tmp_env;

	if (!(new = (t_env*)malloc(sizeof(t_env))))
		return ;
	if (!create_env_struct(var, new))
			return ;
	tmp_env = *env;
	while (tmp_env)
	{
		if (!ft_strcmp(new->key, ENV_KEY(tmp_env)))
		{
			free(new->key);
			free(ENV_VALUE(tmp_env));
			ENV_VALUE(tmp_env) = new->value;
			free(new);
			break;
		}
		tmp_env = tmp_env->next;
	}
	if (tmp_env == NULL)
	{
		tmp_env = ft_lstnew(new);
		ft_lstadd_front(env, tmp_env);
	}
}

/*
** Will find a value if the key is stored inside a linked_list of t_env.
** The string is NOT duplicated and shouldn't be free() after use.
**
** @args:
**		t_list **head: a pointer to the first link of the list.
**		char *key: a string that will be compared against t_env->key
** @return:
**		char *: a string that correspond to the key that was sent.
*/
char	*find_env_value(t_list **env, char *key)
{
	t_list	*tmp_env;

	tmp_env = *env;
	while (tmp_env)
	{
		if (!ft_strcmp(key, ENV_KEY(tmp_env)))
			return (ENV_VALUE(tmp_env));
		tmp_env = tmp_env->next;
	}
	return (NULL);
}
