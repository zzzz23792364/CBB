<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 314ffacdd6cf907e6a0d8e055d6ed217c9a8fed6



#ifndef _PARSE_H_H
#define _PARSE_H_H


#define CMD_MAX_ARGS		 20
#define CMD_MAX_CHAR		 128


#ifndef whitespace
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#endif




char* stripwhite(char *string);  

int parse_input_line(char *pline,char **argv);







#define ARGS_CNT        1024            /* Max argv entries */
#define ARGS_BUFFER     8192           /* # bytes total for arguments */


/*
 * Typedef:     parse_key_t
 * Purpose:     Type used as FIRST entry in structures passed to
 *              parse_lookup.
 */
typedef char *parse_key_t;



/*
 * Typedef:     args_t
 * Purpose:     argc/argv (sorta) structure for parsing arguments.
 *
 * If the macro ARG_GET is used to consume parameters, unused parameters
 * may be passed to lower levels by simply passing the args_t struct.
 */
typedef struct args_s {
    parse_key_t a_cmd;                  /* Initial string */
    char        *a_argv[ARGS_CNT];      /* argv pointers */
    char        a_buffer[ARGS_BUFFER];  /* Split up buffer */
    int         a_argc;                 /* Parsed arg counter */
    int         a_arg;                  /* Pointer to NEXT arg */
} args_t;


/*
 * Macro:       ARG_CNT
 * Purpose:     Return the number of unconsumed arguments.
 */
#define ARG_CNT(_a)     ((_a)->a_argc - (_a)->a_arg)

#define ARG_NEXT(_a)    (_a)->a_arg++

#define _ARG_CUR(_a)    ((_a)->a_argv[(_a)->a_arg])



















#endif






<<<<<<< HEAD
=======
=======



#ifndef _PARSE_H_H
#define _PARSE_H_H


#define CMD_MAX_ARGS		 20
#define CMD_MAX_CHAR		 128


#ifndef whitespace
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#endif




char* stripwhite(char *string);  

int parse_input_line(char *pline,char **argv);







#define ARGS_CNT        1024            /* Max argv entries */
#define ARGS_BUFFER     8192           /* # bytes total for arguments */


/*
 * Typedef:     parse_key_t
 * Purpose:     Type used as FIRST entry in structures passed to
 *              parse_lookup.
 */
typedef char *parse_key_t;



/*
 * Typedef:     args_t
 * Purpose:     argc/argv (sorta) structure for parsing arguments.
 *
 * If the macro ARG_GET is used to consume parameters, unused parameters
 * may be passed to lower levels by simply passing the args_t struct.
 */
typedef struct args_s {
    parse_key_t a_cmd;                  /* Initial string */
    char        *a_argv[ARGS_CNT];      /* argv pointers */
    char        a_buffer[ARGS_BUFFER];  /* Split up buffer */
    int         a_argc;                 /* Parsed arg counter */
    int         a_arg;                  /* Pointer to NEXT arg */
} args_t;


/*
 * Macro:       ARG_CNT
 * Purpose:     Return the number of unconsumed arguments.
 */
#define ARG_CNT(_a)     ((_a)->a_argc - (_a)->a_arg)

#define ARG_NEXT(_a)    (_a)->a_arg++

#define _ARG_CUR(_a)    ((_a)->a_argv[(_a)->a_arg])



















#endif






>>>>>>> 8621da6db479e63b9e2f02400623d3048c20a4cc
>>>>>>> 314ffacdd6cf907e6a0d8e055d6ed217c9a8fed6
