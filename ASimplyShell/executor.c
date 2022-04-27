/*UID: 116879538*/
/*Shuo Wang*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <fcntl.h>
#include <sysexits.h>
#include <sys/wait.h>
#include "command.h"
#include "executor.h"

/*prototypes:*/
/*static void print_tree(struct tree *t);*/
static int exec_none(struct tree *t, int p_input_fd, int p_output_fd);
static int execute_aux(struct tree *t, int p_input_fd, int p_output_fd);

/*It calls the execute_aux to resursively go through the tree.*/
int execute(struct tree *t) {
   return execute_aux(t, STDIN_FILENO, STDOUT_FILENO);
}

/*If the Node is NONE, we jump to this function.*/
static int exec_none(struct tree *t, int input_fd, int output_fd) {
   int status;
   pid_t pid;
   /*Check if the command is "cd" */
   if (!strcmp(t->argv[0], "cd")) {
      /*If there is an argument, we jump to the directory */
      if (t->argv[1] != NULL) {
         /*Check validity of the directory. */
         if (chdir(t->argv[1]) != 0) {
            perror(t->argv[1]);
         }
      } else {/*If there is no argument,
                we jump to HOME directory. */
         if (chdir(getenv("HOME")) != 0) {
            perror(getenv("HOME"));
         }
      }
      return 0;

   } else if (!strcmp(t->argv[0], "exit")) {
      /*If the command is exit, we exit successfully */
      exit(0);

   } else {
      /*NOT cd or exit */
      if ((pid = fork()) < 0) {
         perror("fork");
         return 1;
      }

      if (pid) {/*Parent process will wait for 
		  child process. */
         wait(&status);
         return status;
      } else {

         if (t->input) {/*If input is not NULL */
            if ((input_fd = open(t->input, O_RDONLY)) < 0) {
               perror("open input_fd error");
            }
            /*We redirect input */
            if (dup2(input_fd, STDIN_FILENO) < 0) {
               perror("dup2 (read) failed error");
            }
            /*Close the input */
            if (close(input_fd) < 0) {
               perror("close input_fd error");
            }
         }

         if (t->output) {       /*If output is not NULL */
            if ((output_fd = open(t->output, O_WRONLY | O_CREAT |
                                  O_TRUNC, 0664)) < 0) {
               perror("open output_fd error");
            }
            /*We redirect output */
            if (dup2(output_fd, STDOUT_FILENO) < 0) {
               perror("dup2 (write) failed error");
            }
            /*Close the output */
            if (close(output_fd) < 0) {
               perror("close output_fd error");
            }
         }
         /*using exec* to run the command */
         execvp(t->argv[0], (char *const *) t->argv);
         /*If error occurs, we print it to stderr. */
         fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
         fflush(stderr);
         exit(-1);
      }
   }
}

static int execute_aux(struct tree *t, int p_input_fd, int p_output_fd) {
   int status;
   pid_t pid;

   /*If the Node is AND */
   if (t->conjunction == AND) {

      if (t->input) {           /*If input is not NULL */
         if ((p_input_fd = open(t->input, O_RDONLY)) < 0) {
            perror("open input_fd");
         }
      }

      if (t->output) {          /*If output is not NULL */
         if ((p_output_fd = open(t->output, O_WRONLY | O_CREAT |
                                 O_TRUNC, 0664)) < 0) {
            perror("open output_fd");
         }
      }
      /*Process the left subtree using the parent input/output */
      status = execute_aux(t->left, p_input_fd, p_output_fd);
      if (status == 0) {
         /*If the left subtree execution is successful, process 
            the right subtree */
         execute_aux(t->right, p_input_fd, p_output_fd);
      }
      /*If the Node is NONE */
   } else if (t->conjunction == NONE) {
     return exec_none(t, p_input_fd, p_output_fd);
      /*If the Node is PIPE */
   } else if (t->conjunction == PIPE) {

      int pipe_fd[2];
      /*Ambiguous output redirect */
      if (t->left->output) {
	 printf("Ambiguous output redirect.\n");
	 fflush(stdout);
	 return 1;
      } else if (t->right->input) {
         /*Ambiguous input redirect */
         printf("Ambiguous input redirect.\n");
	 fflush(stdout);
	 return 1;
      } else {

         if (t->input) {        /*If input is not NULL */
            if ((p_input_fd = open(t->input, O_RDONLY)) < 0) {
               perror("open input_fd");
            }
         }
         if (t->output) {       /*If output is not NULL */
            if ((p_output_fd = open(t->output, O_WRONLY | O_CREAT |
                                    O_TRUNC, 0664)) < 0) {
               perror("open output_fd");
            }
         }
      }
      /*We Creat a pipe */
      if (pipe(pipe_fd)) {
         perror("pipe");
      }

      if ((pid = fork()) < 0) {
         perror("fork");
         return 1;
      }
      if (pid) {                /*Parent process. */
         /*Since we only read, we close write */
         close(pipe_fd[1]);
         /*redirect input with pipe */
         if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
           perror("dup2");
         }
         /*Move to next node */
         execute_aux(t->right, pipe_fd[0], p_output_fd);
         /*close read */
         close(pipe_fd[0]);
         /*wait for child process */
         wait(NULL);
      } else {                  /*Child process */
         /*Since we only write, we close read */
         close(pipe_fd[0]);
	 /*redirect output with pipe*/
         if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
         }
         /*Move to next(left) node */
         execute_aux(t->left, p_input_fd, pipe_fd[1]);
         close(pipe_fd[1]);
         exit(0);
      }
      /*If the Node is SUBSHELL */
   } else if (t->conjunction == SUBSHELL) {

      if ((pid = fork()) < 0) {
         perror("fork");
         return 1;
      }

      if (t->input) {           /*If input is not NULL */
         if ((p_input_fd = open(t->input, O_RDONLY)) < 0) {
            perror("open input_fd");
            return 1;
         }
      }
      if (t->output) {          /*If output is not NULL */
         if ((p_output_fd = open(t->output, O_WRONLY | O_CREAT |
                                 O_TRUNC, 0664)) < 0) {
            perror("open output_fd");
            return 1;
         }
      }

      if (pid) {                /*Parent process will wait for child process */
         wait(NULL);
      } else {                  /*Child process */
         /*We can only process left tree. */
         execute_aux(t->left, STDIN_FILENO, STDOUT_FILENO);
         exit(0);
      }
   }

   return 0;
}

/*
static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
   }*/
