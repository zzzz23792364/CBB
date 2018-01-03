

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "shell.h"


#ifndef whitespace
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#endif

typedef struct {
  char *name;			/* User printable name of the function. */
  rl_icpfunc_t *func;		/* Function to call to do the job. */
  char *doc;			/* Documentation for this function.  */
  char *usage;
} COMMAND;


int done=0,shell_debug=0;

int cmd_help(char *line);


char cmd_monitor_usage[] =
	"Usages:\n\t"
	"monitor show [Slave_id] [Options]\n\t"
	"		- Monitor the given device.\n\t"
	"monitor poll\n\t"
	"		- Poll supported device and option.\n";
	

int 
cmd_parse_line (char *pline,char **argv)
{
	int nargs = 0;

	while (nargs < CMD_MAX_ARGS) 
	{

		while ((*pline == ' ') || (*pline == '\t')) {
		++pline;
		}

		if (*pline == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return (nargs);
		}

		argv[nargs++] = pline;	/* begin of argument string	*/

		/* find end of string */
		while (*pline && (*pline != ' ') && (*pline != '\t')) {
			++pline;
		}

		if (*pline == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return (nargs);
		}
		*pline++ = '\0';		/* terminate current arg	 */

	}
	printf("Too many arguments (max. %d) \n", CMD_MAX_ARGS);
	return (nargs);
}


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

char *
dupstr(char *s)
{
  char *r;

  r = xmalloc (strlen (s) + 1);
  strcpy (r, s);
  return (r);
}

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



char *
command_generator(const char *text, int state)
{
  static int list_index, len;
  char *name;

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state)
    {
      list_index = 0;
      len = strlen (text);
    }

  /* Return the next name which partially matches from the command list. */
  while (name = commands[list_index].name)
    {
      list_index++;

      if (strncmp (name, text, len) == 0)
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

void
shell_init()
{
	 rl_readline_name = "BPU_SHELL";
	 rl_attempted_completion_function = fileman_completion;
}

char *
stripwhite(char *string)    
{
  register char *s, *t;

  for (s = string; whitespace (*s); s++)
  	;
    
  if (*s == 0)
    return (s);

  t = s + strlen (s) - 1;
  while (t > s && whitespace (*t))
    t--;
  *++t = '\0';

  return s;
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
shell(void)
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
