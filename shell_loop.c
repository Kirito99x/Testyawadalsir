#include "shell.h"

/**
 * shell_loop - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_loop(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_information(info);
		if (shell_interactive(info))
			_puts("$ ");
		writs_to_stderr(BUF_FLUSH);
		r = get_input(info);
		if (r != -1)
		{
			set_information(info, av);
			builtin_ret = f_builtin(info);
			if (builtin_ret == -1)
				f_cmd(info);
		}
		else if (shell_interactive(info))
			_putchar('\n');
		free_information(info, 0);
	}
	_write_histry(info);
	free_information(info, 1);
	if (!shell_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * f_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int f_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _exitshell},
		{"env", _my_env},
		{"help", custom_help},
		{"history", _history},
		{"setenv", _my_setenv},
		{"unsetenv", _my_unsetenv},
		{"cd", custom_cd},
		{"alias", _alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (strcmpa(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * f_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void f_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delimeter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_cmd_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_shell_cmd(info);
	}
	else
	{
		if ((shell_interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && isit_command(info, info->argv[0]))
			fork_shell_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			_error(info, "not found\n");
		}
	}
}

/**
 * fork_shell_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_shell_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_information(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				_error(info, "Permission denied\n");
		}
	}
}
