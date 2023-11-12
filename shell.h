#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* shell_loop.c */
int shell_loop(info_t *, char **);
int f_builtin(info_t *);
void f_cmd(info_t *);
void fork_shell_cmd(info_t *);

/* path.c */
int isit_command(info_t *, char *);
char *duplcated_chars(char *, int, int);
char *find_cmd_path(info_t *, char *, char *);

/* err_string_functions.c */
void _input_string(char *);
int writs_to_stderr(char);
int writs_to_c(char c, int fd);
int prnt_input_str(char *str, int fd);

/* string_functions.c */
int lenstr(char *);
int strcmpa(char *, char *);
char *start_wth(const char *, const char *);
char *_strconca(char *, char *);

/* string_functions2.c */
char *_stricopy(char *, char *);
char *_stridupli(const char *);
void _puts(char *);
int _putchar(char);

/* string_functions3.c */
char *_strncopy(char *, char *, int);
char *_strnconca(char *, char *, int);
char *_strichra(char *, char);

/* string_functions4.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory_functions */
char *_momset(char *, char, unsigned int);
void freestr(char **);
void *_realloc_bmom(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int pfree(void **);

/* more_functions.c */
int shell_interactive(info_t *);
int is_delimeter(char, char *);
int _is_alphab(int);
int _strtoin(char *);

/* more_functions2.c */
int _errstrtoin(char *);
void _error(info_t *, char *);
int print_decimal(int, int);
char *conv_num(long int, int, int);
void remv_comm(char *);

/* builtin_emulators.c */
int _exitshell(info_t *);
int custom_cd(info_t *);
int custom_help(info_t *);

/* builtin_emulators2.c */
int _history(info_t *);
int _alias(info_t *);

/* getline.c module */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void Sigint_Handler(int);

/* info.c module */
void clear_information(info_t *);
void set_information(info_t *, char **);
void free_information(info_t *, int);

/* env.c module */
char *_getenv(info_t *, const char *);
int _my_env(info_t *);
int _my_setenv(info_t *);
int _my_unsetenv(info_t *);
int populate_envlist(info_t *);

/* env2.c module */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* file_io_functions.c */
char *gt_histry_file(info_t *info);
int _write_histry(info_t *info);
int _read_histry(info_t *info);
int build_histrylist(info_t *info, char *buf, int linecount);
int re_number_histry(info_t *info);

/* liststr.c module */
list_t *add_a_node(list_t **, const char *, int);
list_t *add_a_node_end(list_t **, const char *, int);
size_t prnt_liststr(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* liststr2.c module */
size_t listlen(const list_t *);
char **listtostr(list_t *);
size_t prntlist(const list_t *);
list_t *node_strtwith(list_t *, char *, char);
ssize_t _getnode_index(list_t *, list_t *);

/* chain.c */
int isit_chain(info_t *, char *, size_t *);
void chck_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

#endif
