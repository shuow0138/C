#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"

int main(int argc, char *argv[]) {
   FILE *input;
   Document doc;
   const char *name = "main_document";
   char *temp, *temp2;
   /*command stores the command,
      value1 stores the 1st parameter
      value2 stores the 2nd parameter
      rest stores rest of the line. */
   char line[MAX_STR_SIZE + 1], command[MAX_STR_SIZE + 1],
      value1[MAX_STR_SIZE + 1], value2[MAX_STR_SIZE + 1],
      rest[MAX_STR_SIZE + 1] = "";
   /*star records if there is a '*'. */
   char star;
   /*index is array index. */
   int index = 0;

   init_document(&doc, name);
   /*if only 1 command_line argument, we make input standard input. */
   if (argc == 1) {
      input = stdin;
   } else if (argc == 2) {
      /*if 2 arguments, we open the file */
      input = fopen(argv[1], "r");
      if (input == NULL) {
         fprintf(stderr, "%s cannot be opened.\n", argv[1]);
         return EX_OSERR;
      }
   } else {
      /*If more than 2 arguments, we print error. */
      fprintf(stderr, "Usage: user_interface\n");
      fprintf(stderr, "Usage: user_interface <filename>\n");
      return EX_USAGE;
   }

   while (fgets(line, MAX_STR_SIZE + 1, input) != NULL) {
      if (input == stdin) {
         fprintf(stdout, "%s", "> ");
      }
      temp = line;
      /*Avoid fgets reads '\n' and prints a newline. */
      if (*temp && temp[strlen(temp) - 1] == '\n') {
         temp[strlen(temp) - 1] = '\0';
      }
      /*We check if the line is empty. */
      while (*temp == ' ' && *temp != '\0') {
         temp++;
      }
      /*If the first non-space character is #, then it is a comment.
         If the character is '\0', it is a empty line. */
      if (*temp != '#' && *temp != '\0') {
         /*we make each line end with '\0' */
         if (*line && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
         }
         /*We read the first string(command) and compare */
         sscanf(line, "%s", command /*, value1 */ );
         /*If command is add_paragraph_after */
         if (!strcmp(command, "add_paragraph_after")) {
            if (sscanf(line, "%s%s%s", command, value1, rest) != 2
                || atoi(value1) < 0) {
               fprintf(stdout, "Invalid Command\n");
               /*if atoi(value1) is 0 and the actual value is not 0,
                  then value1 is not a char of integer. */
            } else if (atoi(value1) == 0 && value1[0] != '0') {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (add_paragraph_after(&doc, atoi(value1)) == FAILURE) {
                  fprintf(stdout, "add_paragraph_after failed\n");
               }
            }
            /*If command is print_document */
         } else if (!strcmp(command, "print_document")) {
            if (sscanf(line, "%s%s", command, rest) != 1) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               print_document(&doc);
            }
            /*rest rest for next checking. */
            rest[0] = '\0';
            /*If command is add_line_after */
         } else if (!strcmp(command, "add_line_after")) {
            if (sscanf(line, "%s%s%s %c", command, value1, value2, &star) != 4
                || atoi(value1) <= 0 || atoi(value2) < 0 || star != '*') {
               fprintf(stdout, "Invalid Command\n");
               /*There also checks if atoi(value2) == 0 
                  means atoi retuning false */
            } else if (atoi(value2) == 0 && value2[0] != '0') {
               fprintf(stdout, "Invalid Command\n");
            } else {
               /*We get the temp points to the '*'. */
               temp = line;
               while (*temp != '*') {
                  temp++;
               }
               /*Add string starting with temp + 1(the 1st char after '*'). */
               if (add_line_after(&doc, atoi(value1), atoi(value2), temp + 1)
                   == FAILURE) {
                  fprintf(stdout, "add_line_after failed\n");
               }
            }
            /*If command is quit */
         } else if (!strcmp(command, "quit")) {
            if (sscanf(line, "%s%s", command, rest) != 1 || strlen(rest) != 0) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               fclose(input);
               return SUCCESS;
            }
            rest[0] = '\0';
            /*If command is exit */
         } else if (!strcmp(command, "exit")) {
            if (sscanf(line, "%s%s", command, rest) != 1 || strlen(rest) != 0) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               fclose(input);
               return SUCCESS;
            }
            rest[0] = '\0';
            /*If command is append_line */
         } else if (!strcmp(command, "append_line")) {
            if (sscanf(line, "%s%s %c%s", command, value1, &star, value2) != 4
                || atoi(value1) <= 0 || star != '*') {
               fprintf(stdout, "Invalid Command\n");
            } else {
               /*We get the temp points to the '*'. */
               temp = line;
               while (*temp != '*') {
                  temp++;
               }
               /*Add string starting with temp + 1(the 1st char after '*'). */
               if (append_line(&doc, atoi(value1), temp + 1) == FAILURE) {
                  fprintf(stdout, "append_line failed\n");
               }
            }
	    /*If command is remove_line*/
         } else if (!strcmp(command, "remove_line")) {
            if (sscanf(line, "%s%s%s%s", command, value1, value2, rest) != 3
                || atoi(value1) <= 0 || atoi(value2) <= 0 ||
                strlen(rest) != 0) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (remove_line(&doc, atoi(value1), atoi(value2)) == FAILURE) {
                  fprintf(stdout, "remove_line failed\n");
               }
            }
            /*reset rest */
            rest[0] = '\0';
	    /*If command is load_file*/
         } else if (!strcmp(command, "load_file")) {
            if (sscanf(line, "%s%s%s", command, value1, rest) != 2
                || strlen(rest) != 0) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (load_file(&doc, value1) == FAILURE) {
                  fprintf(stdout, "load_file failed\n");
               }
            }
            /*We reset rest to empty string for next appendix checking. */
            rest[0] = '\0';
	    /*If command is replace_text*/
         } else if (!strcmp(command, "replace_text")) {
            if (sscanf(line, "%s%s%s", command, value1, value2) != 3) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               index = 0;
               /*We set temp pointing to the 1st '"' */
               temp = strchr(line, '"');
               temp++;
               /*We set temp2 pointing to the 2nd '"' */
               temp2 = strchr(temp, '"');
               /*we copy all data between those pointers */
               while (temp < temp2) {
                  value1[index] = *temp;
                  temp++;
                  index++;
               }
               value1[index] = '\0';

               index = 0;
               temp2++;
	       /*We set temp pointing to the 1st '"'*/
               temp = strchr(temp2, '"');
               temp++;
	       /*We set temp2 pointing to the 2nd '"'*/
               temp2 = strchr(temp, '"');
	       /*we copy all data between those pointers*/
               while (temp < temp2) {
                  value2[index] = *temp;
                  temp++;
                  index++;
               }              
                 value2[index] = '\0';           
               /*Reset index */
               index = 0;

               if (strlen(value1) == 0) {
                  fprintf(stdout, "Invalid Command\n");
               } else {
                  if (replace_text(&doc, value1, value2) == FAILURE) {
                     fprintf(stdout, "replace_text failed\n");
                  }
               }
               /*reset value2 and value1 */
               value2[0] = '\0';
               value1[0] = '\0';
            }
	    /*If command is highlight_text*/
         } else if (!strcmp(command, "highlight_text")) {
            if (sscanf(line, "%s%s", command, rest) != 2) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               index = 0;
	       /*We set temp pointing to the 1st '"'*/
	       /*We set temp2 pointing to the 2nd '"'*/
	       /*we copy all data between those pointers*/
               temp = strchr(line, '"');
               temp++;
               temp2 = strchr(temp, '"');
               while (temp < temp2) {
                  value1[index] = *temp;
                  temp++;
                  index++;
               }
               value1[index] = '\0';
               /*reset index */
               index = 0;
               if (strlen(value1) == 0) {
                  fprintf(stdout, "Invalid Command\n");
               } else {
                  highlight_text(&doc, value1);
               }
               /*reset rest */
               rest[0] = '\0';
            }
	    /*If command is remove_text*/
         } else if (!strcmp(command, "remove_text")) {
            if (sscanf(line, "%s%s", command, rest) != 2) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               index = 0;
	       /*We set temp pointing to the 1st '"'*/
	       /*We set temp2 pointing to the 2nd '"'*/
	       /*we copy all data between those pointers*/
               temp = strchr(line, '"');
               temp++;
               temp2 = strchr(temp, '"');
               while (temp < temp2) {
                  value1[index] = *temp;
                  temp++;
                  index++;
               }
               value1[index] = '\0';
               index = 0;
               if (strlen(value1) == 0) {
                  fprintf(stdout, "Invalid Command\n");
               } else {
                  remove_text(&doc, value1);
               }
               rest[0] = '\0';
            }
	    /*If command is save_document*/
         } else if (!strcmp(command, "save_document")) {
            if (sscanf(line, "%s%s%s", command, value1, rest) != 2 ||
                strlen(rest) != 0) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (save_document(&doc, value1) == FAILURE) {
                  fprintf(stdout, "save_document failed\n");
               }
               /*reset rest */

            }
            rest[0] = '\0';
	    /*If command is reset_document*/
         } else if (!strcmp(command, "reset_document")) {
            if (sscanf(line, "%s%s", command, rest) != 1 || strlen(rest) != 0) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               reset_document(&doc);
            }
            /*reset rest */
            rest[0] = '\0';
         }

      }
   }
   fclose(input);
   return SUCCESS;
}
