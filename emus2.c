#include "shell.h"

/**
 * _history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _history(info_t *info)
{
	prntlist(info->history);
	return (0);
}

/**
 * _unst_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int _unst_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _strichra(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias),
		_getnode_index(info->alias, node_strtwith(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * _st_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int _st_alias(info_t *info, char *str)
{
	char *p;

	p = _strichra(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (_unst_alias(info, str));

	_unst_alias(info, str);
	return (add_a_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * _prnt_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int _prnt_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strichra(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _alias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			_prnt_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _strichra(info->argv[i], '=');
		if (p)
			_st_alias(info, info->argv[i]);
		else
			_prnt_alias(node_strtwith(info->alias, info->argv[i], '='));
	}

	return (0);
}
