/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : shell.c
 * Author        : ZengChao
 * Date          : 2018-01-24
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * 1.Date        : 2018-01-24
 *   Author      : ZengChao
 *   Modification: Created file

*************************************************************************************************************/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>


#include "shell.h"
#include "sal/appl/sal.h"
#include "sal/core/alloc.h"



int done=0,shell_debug=0;



#if 0

int cmd_help(char *line);

char cmd_monitor_usage[] =
	"Usages:\n\t"
	"monitor show [Slave_id] [Options]\n\t"
	"		- Monitor the given device.\n\t"
	"monitor poll\n\t"
	"		- Poll supported device and option.\n";
	

char cmd_debug_usage[] = 
	"Usages:\n\t"
	"debug on\n\t"
	"		- Open the debug information.\n\t"
	"debug off\n\t"
	"		- Close the debug information.\n\t"
	"debug show\n\t"
	"		- Show the current debug status.\n";

cmd_result_t
cmd_debug(char *line)
{
	int argc=0;
	char *argv[CMD_MAX_ARGS];
	
	argc = cmd_parse_line(line,argv);	

	SHELL_ASSERT(argc!=1,CMD_USAGE);
	
	if(!strcasecmp(argv[0],"on"))
	{
		shell_debug = 1;
		printf("Open the debug information\n");
	}
	else if(!strcasecmp(argv[0],"off"))
	{
		shell_debug = 0;
		printf("Close the debug information\n");
	}
	else if(!strcasecmp(argv[0],"show"))
	{
		printf("Debug is %s\n",shell_debug?"on":"off");
	}
	else
		return CMD_USAGE;

	return CMD_OK;
}


int
cmd_exit(char *line)
{
	done = 1;
	return 0;
}


COMMAND commands[] = {
  { "help", cmd_help, "Display this text",NULL},
  { "?", cmd_help,"Synonym for 'help'",NULL},
  { "exit", cmd_exit,"Exit BPU Shell",NULL},
  { "debug", cmd_debug,"Open/Close the debug information",cmd_debug_usage},
 
  /* Last entry, do not change */
  { (char *)NULL, (rl_icpfunc_t *)NULL, (char *)NULL,(char *)NULL }
};

int
cmd_help(char *line)
{
  register int i;
  int printed = 0;

  for (i = 0; commands[i].name; i++)
    {
      if (!*line || (strcmp (line, commands[i].name) == 0))
        {
          printf ("%s\t\t%s.\n", commands[i].name, commands[i].doc);
          printed++;
        }
    }

  if (!printed)
    {
      printf ("No commands match `%s'.  Possibilties are:\n", line);

      for (i = 0; commands[i].name; i++)
        {
          /* Print in six columns. */
          if (printed == 6)
            {
              printed = 0;
              printf ("\n");
            }

          printf ("%s\t", commands[i].name);
          printed++;
        }

      if (printed)
        printf ("\n");
    }
  return (0);
}


COMMAND *
find_command( char *name)
{
  register int i;

  for (i = 0; commands[i].name; i++)
    if (strcmp (name, commands[i].name) == 0)
      return (&commands[i]);

  return ((COMMAND *)NULL);
}


int
execute_line(char *line)
{
	register int i;
	COMMAND *command;
	char *word;
	cmd_result_t ret;
	
	i = 0;
	while (line[i] && whitespace (line[i]))
	i++;
	word = line + i;

	while (line[i] && !whitespace (line[i]))
	i++;

	if (line[i])
	line[i++] = '\0';

	command = find_command (word);

	if (!command)
	{
	  fprintf (stderr, "%s: No such command.\n", word);
	  return (-1);
	}

	while (whitespace (line[i]))
	i++;

	word = line + i;

	/* Call the function. */
	ret = (*(command->func)) (word);
	if(ret == CMD_USAGE)
	{
		if(command->usage!=NULL)
			printf("%s",command->usage);
	}

	return 0;
}


int 
cmd_shell_process(void)
{
	char prompt[32] = "BPU_SHELL> ";
	printf("%s\n",prompt);
#if 1
	shell_init();

	for( ; done == 0 ; )
	{
		char *line,*s;

		printf(__PURPLE__"%s"__NONE__,prompt);
		line = readline("");
		printf(__GREEN__"");
		
     	if (!line)
       		break;

		s = stripwhite (line);

	    if (*s)
	    {
			add_history (s);
			execute_line (s);			
	    }

		free (line);
		printf(__NONE__"");
	}
#endif

	return 0;
}

#endif

int     dyn_cmd_cnt = 0;
cmd_t * dyn_cmd_list = NULL;


//#define cli_out printf


DCL_CMD(sh_help_short,sh_help_short_usage)
DCL_CMD(sh_help,sh_help_usage)
DCL_CMD(sh_exit,sh_exit_usage)


cmd_t cbb_cmd_common[] = {
	{"?",				sh_help_short,			sh_help_short_usage, "Display list of commands"},
	{"??",				sh_help,				sh_help_usage,       "@help" },
	{"EXIT",			sh_exit,				sh_exit_usage, 		 "Exit the current shell (and possibly reset)" },
	{"Help",            sh_help,                sh_help_usage,       "Print this list OR usage for a specific command" },
};

int cbb_cmd_common_cnt = COUNTOF(cbb_cmd_common);


char *
dupstr(char *s)
{
  char *r;
  r = malloc (strlen (s) + 1);
  strcpy (r, s);
  return (r);
}


char *
command_generator(const char *text, int state)
{
	static int common_list_index,dyn_list_index,len;
	char *name;

  	/* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
	if (!state)
	{
		common_list_index = 0;
		dyn_list_index = 0;
		len = strlen (text);
	}

	/* Return the next name which partially matches from the command list. */
//	while (name = cbb_cmd_common[common_list_index].c_cmd)
//	{
//	
//		list_index++;

//		if (strncmp (name, text, len) == 0)
//			return (dupstr(name));
//	}

	while(common_list_index<cbb_cmd_common_cnt)
	{
		name = cbb_cmd_common[common_list_index].c_cmd;
		common_list_index ++;
		if (strncasecmp (name, text, len) == 0)
			return (dupstr(name));
	}

	while(dyn_list_index<dyn_cmd_cnt)
	{
		name = dyn_cmd_list[dyn_list_index].c_cmd;
		dyn_list_index ++;
		if (strncasecmp (name, text, len) == 0)
			return (dupstr(name));
	}

	/* If no names matched, then return NULL. */
	return ((char *)NULL);
}


char **
fileman_completion(const char *text, int start, int end)
{
  char **matches;

  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0)
    matches = rl_completion_matches (text, command_generator);

  return (matches);
}


char sh_help_short_usage[] =
    "Parameters: None\n\t"
    "Display a complete list of commands in a concise format.\n";

/*
 * Function:	 sh_help_short
 * Purpose:    Print list of all commands in command table.
 * Parameters:	  u - unit # (ignored)
 *		  a - args, if passed, turns into "??".
 * Returns:    CMD_OK
 */
cmd_result_t
sh_help_short(args_t *a)
{
	#define    CMD_PER_LINE    5
	int 	   i, words;
	
	if (0 != ARG_CNT(a)) 
	{
		return(sh_help(a));
	}
	
	cli_out("help: \"??\" or \"help\" for summary\n");
	cli_out("Commands common to all modes:\n");
	
	words = 0;
	
	for (i = 0; i < cbb_cmd_common_cnt; i++) 
	{
		if (cbb_cmd_common[i].c_help[0] != '.') 
		{
			cli_out("%-15s%s", cbb_cmd_common[i].c_cmd,
					(words % CMD_PER_LINE) == (CMD_PER_LINE - 1) ? "\n" : "");
			words++;
		}
	}
	
	if (words % CMD_PER_LINE) 
	{
		cli_out("\n");
	}

	if(dyn_cmd_cnt > 0) 
	{
		cli_out("Dynamic commands for all modes:\n");
		words = 0;
		for (i = 0; i < dyn_cmd_cnt; i++) 
		{
			if (dyn_cmd_list[i].c_help[0] != '.') 
			{
				cli_out("%-15s%s", dyn_cmd_list[i].c_cmd,
						(words % CMD_PER_LINE) == (CMD_PER_LINE - 1) ? "\n" : "");
				words++;
			}
		}
		
		if (words % CMD_PER_LINE) 
		{
			cli_out("\n");
		}
	}

	return(CMD_OK);
#undef CMD_PER_LINE
}



static void
print_help_list(cmd_t * cmd_list, int command_count, char *title)
{
    cmd_t      *cmd;

    if (command_count > 0) {
        cli_out("\n%s:\n", title);
        for (cmd = cmd_list; cmd < &cmd_list[command_count]; cmd++) {
            cli_out("\n\nCOMMAND: %s\tDescription: %s\n\n%s",
                    cmd->c_cmd,
                    cmd->c_help[0] == '.' ? &cmd->c_help[1] : cmd->c_help, cmd->c_usage);
        }
    }
}


char sh_help_usage[] =
    "Parameters: [command] ... \n\t"
    "Display usage information for the listed commands.\n\t"
    "If no parameters given, a list of all commands is printed with\n\t"
    "a short description of the purpose of the command.\n";

/*
 * Function:	 sh_help
 * Purpose:    Print usage messages for the listed commands
 * Parameters:	  u - unit number (not used)
 * Returns:    CMD_XXX
 */
cmd_result_t
sh_help(args_t *a)
{
//	cmd_t	*cmd;
//	char	*c;
//	cmd_result_t ret = CMD_OK;
//	cmd_t		*clist; /* Current command list */
//	int 		ccnt;	/* Current count of commands */

	/* Provide easy method to print out all commands */
	if (ARG_CNT(a) == 1 && ! strcasecmp(_ARG_CUR(a), "print-manual")) 
	{
		ARG_NEXT(a); 	
		
		print_help_list(cbb_cmd_common, cbb_cmd_common_cnt,
			"Commands common to all modes");
		
		print_help_list(dyn_cmd_list, dyn_cmd_cnt,
			"Dynamic commands for all modes");
		return(CMD_OK);
	}

	if (ARG_CNT(a) == 0) /* Nothing listed */
	{		  
		int    i;
		cli_out("Help: Type help \"command\" for detailed command usage\n");
		cli_out("Help: Upper case letters signify minimal match\n");
		cli_out("\nCommands common to all modes:\n");
		for (i = 0; i < cbb_cmd_common_cnt; i++) 
		{
			if ((*cbb_cmd_common[i].c_help != '@') &&
				(*cbb_cmd_common[i].c_help != '.')) 
			{
				cli_out("\t%-20s%s\n",
						cbb_cmd_common[i].c_cmd, cbb_cmd_common[i].c_help);
			}
		}

		if(dyn_cmd_cnt > 0) 
		{
			cli_out("\nDynamic commands for all modes:\n");
			for (i = 0; i < dyn_cmd_cnt; i++) {
				if ((*dyn_cmd_list[i].c_help != '@') &&
					(*dyn_cmd_list[i].c_help != '.')) 
				{
					cli_out("\t%-20s%s\n",
							dyn_cmd_list[i].c_cmd, dyn_cmd_list[i].c_help);
				}
			}
		}
		
		cli_out("\nNumber Formats:\n"
				"\t[-]0x[0-9|A-F|a-f]+ -hex if number begins with \"0x\"\n"
				"\t[-][0-9]+		   -decimal integer\n"
				"\t[-]0[0-7]+		   -octal if number begins with \"0\"\n"
				"\t[-]0b[0-1]+		   -binary if number begins with \"0b\"\n\n"
				);
		
		return(CMD_OK);
	}

	return CMD_USAGE;
}



char sh_exit_usage[] =
    "Exit from the current shell\n\t";


/*
 * Function:     sh_exit
 * Purpose:    Exit from the current level of the shell.
 * Parameters:    None
 * Returns:    CMD_EXIT/CMD_USAGE
 */
cmd_result_t
sh_exit(args_t *a)
{
    int  arg_count;
//    char *arg_str;
//    cmd_result_t rv;

    arg_count = ARG_CNT(a);

    if (arg_count > 1) 
	{
        return(CMD_USAGE);
    }

	done = 1;

    return(CMD_OK);
}


char shell_self_info[] = {
	"*************************\n"
	"Author: zengchao@cqu.edu.cn\n"
	"Supported Command Type: \n"	
	"	1.shell cmd\n"
	"	2.self-defined cmd\n"
	"	3.function name with args\n"
	"*************************\n"
};


void
print_self_info(void)
{
	printf("%s",shell_self_info);
}


void
cmd_shell_init(char *prompt)
{
	 rl_readline_name = prompt;
	 rl_attempted_completion_function = fileman_completion;
}


typedef struct symbol_data_s
{
	unsigned long addr;
	char name[60];
	struct symbol_data_s *next;
}symbol_data_t;


symbol_data_t* symbol_data_list_head;


cmd_result_t 
api_shell_init()
{
	cmd_result_t ret;
	
	#define MAX_FILE_NAME_LEN  256

	char line[MAX_FILE_NAME_LEN];
	char file_name[MAX_FILE_NAME_LEN];
	char cmdline[MAX_FILE_NAME_LEN];
	char temp_file[] = "symbol.temp";
	
	int file_name_len = readlink("/proc/self/exe", file_name, MAX_FILE_NAME_LEN);
	file_name[file_name_len] = '\0';

	if(file_name_len>MAX_FILE_NAME_LEN)
		return CMD_FAIL;

	sprintf(cmdline,"nm -p %s > %s",file_name,temp_file);

	ret = sal_system(cmdline);
	if(ret<0)
		return ret;

	FILE *fp;
	fp = fopen(temp_file, "r");
	if (NULL == fp) 
    {
		return CMD_FAIL;
	}
	while (NULL != fgets(line, sizeof(line), fp)) 
	{
		char *next;

		next = strtok(line, " ");
		if (!next)
			continue;

		if (!isdigit(next[0]))
            continue;

		symbol_data_t* data = sal_alloc(sizeof(symbol_data_t),"symbol_data_t");

		data->addr = strtoul(next, NULL, 16);

		next = strtok(NULL, " ");
		if (!next)
		{
			sal_free(data);
			continue;
		}

		next = strtok(NULL, " ");
		if (!next)
		{
			sal_free(data);
			continue;
		}

		strcpy(data->name, next);
		
		data->next = symbol_data_list_head;
		symbol_data_list_head = data;
		
	}

	sprintf(cmdline,"rm %s",temp_file);
	sal_system(cmdline);

	return CMD_OK;
}


cmd_result_t
cmd_shell_process(args_t *a)
{
	cmd_result_t ret;
	char	*ac;
	cmd_t* cmd;

	ac = ARG_GET(a);

	if(!ac)
		return CMD_FAIL;

	cmd = (cmd_t*)parse_lookup(ac, cbb_cmd_common, sizeof(cmd_t), cbb_cmd_common_cnt);
//	if(cmd == NULL)
//	{
//		cmd = (cmd_t*)parse_lookup(ac, dyn_cmd_list, sizeof(cmd_t), dyn_cmd_cnt);
//	}

	if(cmd == NULL)
	{
		ARG_PREV(a);
		ret = CMD_NFND;
	}
	else
	{
		ret = cmd->c_f(a);

		if (ret == CMD_USAGE) /* Print usage is requested */
		{	
			uint32 help_enable = 1;

			if ((cmd->c_usage) && (help_enable)) 
			{
				cli_out("Usage (%s): %s", cmd->c_cmd, cmd->c_usage);
			} 
			else if (help_enable)
			{
				cli_out("Usage (%s): %s\n", cmd->c_cmd, cmd->c_help);
			}
		}

		if (ret == CMD_OK && ARG_CUR(a) != NULL) 
		{
			/*
			 * Warn about unconsumed arguments.  If a command doesn't need
			 * all its arguments, it should use ARG_DISCARD to eat them.
			 */
			cli_out("%s: WARNING: excess arguments ignored "
			        "beginning with '%s'\n",
			        cmd->c_cmd, ARG_CUR(a));
		}
	}

	return ret;
}

typedef int (*function)(int arg,...);

cmd_result_t
api_shell_process(args_t *a)
{
	char *ac;

#define STRTOUL(_a) ((ARG_CUR(_a)==NULL) ? 0 : strtoul(ARG_GET(_a),NULL,10))

	ac = ARG_GET(a);

	if(!ac)
		return CMD_FAIL;

	symbol_data_t* data = symbol_data_list_head;

	while(data!=NULL)
	{	
		if(!strncasecmp(ac,data->name,strlen(ac)))
		{
			int i,argv[4];
			
			for(i=0;i<a->a_argc;i++)
			{
				argv[i] = STRTOUL(a);
			}
			
			((function)data->addr)(argv[0],argv[1],argv[2],argv[3]);
			return CMD_OK;
		}

		data = data->next;
	}

	return CMD_FAIL;

#undef STRTOUL
}


cmd_result_t
bash_shell_process(char *str)
{
	return sal_system(str);
}


void
shell_cleanup()
{
	symbol_data_t *ptr = symbol_data_list_head;
	
	while(ptr!=NULL)
	{
		symbol_data_t *next = ptr->next;
		sal_free(ptr);
		ptr = next;
	}
}

void 
shell_test(int arg1,int arg2)
{
	printf("arg1=%d,arg2=%d\n",arg1,arg2);
}

int 
shell(char *prompt)
{
//	cmd_result_t  ret;
		
	print_self_info();

	cmd_shell_init(prompt);
	api_shell_init();

	for( ;done == 0; )
	{
		char *line,*next;
		
		line = readline(prompt);

		if(!line)
			continue;
		
		add_history (line);
		
		args_t  *a;
		cmd_result_t  ret;

		if ((a = sal_alloc(sizeof (args_t), "args_t")) == NULL) 
		{
			cli_out("sh_process_command: Out of memory\n");
			return CMD_FAIL;
		}

		next = line;

		while ((line = next) != NULL) 
		{
		
			memset(a,0,sizeof (args_t));
			
			while (isspace((unsigned) *line)) 
			{
			    line++;
			}

			if (diag_parse_args(line, &next, a))  /* Parses up to ; or EOL */
			{    
			    ret = CMD_FAIL;
			    break;
			}
			
			ret = cmd_shell_process(a);
			if(ret==CMD_OK)
				continue;

			ret = api_shell_process(a);
			if(ret<0)
				break;
		}

		sal_free(a);

		if(ret<0)
			bash_shell_process(line);
	}

	shell_cleanup();

	return CMD_OK;
}








