#include"document.h"
#include<stdio.h>
#include<string.h>

/*prototypes:*/
static char *replace_in_line_const(char *orig, const char *target,
                                   const char *replacement, char *result);

/*We initialize the document with given string name
 and set its paragraph number to 0.*/
int init_document(Document * doc, const char *name) {

   if (doc == NULL || name == NULL || (int) strlen(name) > MAX_STR_SIZE) {
      return FAILURE;
   } else {
      strcpy(doc->name, name);
      doc->number_of_paragraphs = 0;
  
      return SUCCESS;
   }
}

/*We set the paragraph number back to 0.*/
int reset_document(Document * doc) {
   if (doc == NULL) {
      return FAILURE;
   } else {
      doc->number_of_paragraphs = 0;

      return SUCCESS;
   }
}

/*We print the document’s name, number of paragraphs, and paragraphs detail.*/
int print_document(Document * doc) {
   /*i is for looping through paragraphs.
      j is for looping through lines. */
   int i, j;
   if (doc == NULL) {
      return FAILURE;
   } else {
      printf("Document name: \"%s\"\n", doc->name);
      printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
      /*Looping through paragraphs which starts at 1. */
      for (i = 1; i <= doc->number_of_paragraphs; i++) {
         /*If current paragraph is not the first paragraph and it is not empty,
            we print a blank line to separate different paragraphs. */
         if (i != 1 && doc->paragraphs[i - 1].number_of_lines != 0) {
            printf("\n");
         }
         /*Looping through lines which starts at 1. */
         for (j = 1; j <= doc->paragraphs[i].number_of_lines; j++) {
            printf("%s\n", doc->paragraphs[i].lines[j]);
         }
      }
      return SUCCESS;
   }
}

/*We add a paragraph after the given paragraph number.*/
int add_paragraph_after(Document * doc, int paragraph_number) {
   if (doc == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS
       || paragraph_number >= MAX_PARAGRAPHS
       || paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   } else {
      /*If the given number is 0, 
         it means the paragraph is the first paragraph.
         We set the total paragraph number to 1.
         If it is not 0, it means there are at least
         "paragraph_number" of paragraphs. 
         Then we add one more to it. */
     /*We also initialize number_of_lines to 0.*/
      doc->number_of_paragraphs = paragraph_number + 1;
      doc->paragraphs[paragraph_number + 1].number_of_lines = 0;
      return SUCCESS;
   }
}

/*We add a line after the given paragraph number and line number.*/
int add_line_after(Document * doc, int paragraph_number, int line_number,
                   const char *new_line) {

   if (doc == NULL || paragraph_number > doc->number_of_paragraphs
       || paragraph_number >= MAX_PARAGRAPHS
       || paragraph_number > doc->number_of_paragraphs
       || doc->paragraphs[paragraph_number].number_of_lines >=
       MAX_PARAGRAPH_LINES 
       || doc->paragraphs[paragraph_number].number_of_lines * 100 <
       line_number || new_line == NULL) {

      return FAILURE;
   } else {
      /*We copy the value of new_line to the line_number +1 position.
         We do "+1", since we add a new line. */
      strcpy(doc->paragraphs[paragraph_number].lines[line_number + 1],
             new_line);
      /*We increase line number by 1. */
      doc->paragraphs[paragraph_number].number_of_lines++;

      return SUCCESS;
   }
}

/*It gives address of the number of lines in a paragraph
 to its parameter number_of_lines.*/
int get_number_lines_paragraph(Document * doc, int paragraph_number,
                               int *number_of_lines) {

   if (doc == NULL || number_of_lines == NULL ||
       paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   } else {
      /*We give the address to the pointer. */
      number_of_lines = &doc->paragraphs[paragraph_number].number_of_lines;

      return SUCCESS;
   }
}

/*We add a line to a paragraph.*/
int append_line(Document * doc, int paragraph_number, const char *new_line) {
   /*We get the current last_line number. */
   int line_number = doc->paragraphs[paragraph_number].number_of_lines;

   /*We add a new line behind the last_line number */
   return add_line_after(doc, paragraph_number, line_number, new_line);
}

/*Removes the specified line from the paragraph.
We actually copy the rest lines to 1 position ahead.*/
int remove_line(Document * doc, int paragraph_number, int line_number) {
   /*i is for looping through lines. */
   int i;

   if (doc == NULL || paragraph_number > doc->number_of_paragraphs
       || line_number > doc->paragraphs[paragraph_number].number_of_lines) {
      return FAILURE;
   } else {
      /*We start with specified line and move forward.
         We use "<" instead of "<=" beacuse we don't want
         lines[i + 1] go out of boundry.  */
      for (i = line_number;
           i < doc->paragraphs[paragraph_number].number_of_lines; i++) {
         /*We copy the next line to current line. */
         strcpy(doc->paragraphs[paragraph_number].lines[i],
                doc->paragraphs[paragraph_number].lines[i + 1]);
      }
      /*We decrease the line number by 1, since we removed a line. */
      doc->paragraphs[paragraph_number].number_of_lines--;
      return SUCCESS;
   }
}

/*It adds data to doc.*/
int load_document(Document * doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
   /*i is for looping through the data. */
   int i;

   if (doc == NULL || data == NULL || data_lines == 0) {
      return FAILURE;
   } else {
      /*We increase paragraph number by 1,
         since the function adds 1 paragraph to the first. */
      doc->number_of_paragraphs++;
      for (i = 1; i <= data_lines; i++) {
         if (strlen(data[i - 1]) == 0) {
            /*If the line is an empty string,
               we create a new paragraph. */
            doc->number_of_paragraphs++;
         } else {
            /*If the line is not an empty string,
               we create a new line and
               copy values of data to doc. */
            doc->paragraphs[doc->number_of_paragraphs].number_of_lines++;
            strcpy(doc->paragraphs[doc->number_of_paragraphs].
		   lines[doc-> paragraphs[doc->number_of_paragraphs].
			 number_of_lines], data[i - 1]);
         }
      }
      return SUCCESS;
   }
}

/*We replace the text target with the text replacement*/
int replace_text(Document * doc, const char *target, const char *replacement) {
   /*i is for looping through paragraphs.
      j is for looping through lines.
      result is for saving changed string. */
   int i, j;
   char result[MAX_STR_SIZE + 1];

   if (doc == NULL || target == NULL || replacement == NULL) {
      return FAILURE;
   } else {
      /*We loop through the whole doc */
      for (i = 1; i <= doc->number_of_paragraphs; i++) {
         for (j = 1; j <= doc->paragraphs[i].number_of_lines; j++) {
            /*We copy the changed string to the old string. */
            strcpy(doc->paragraphs[i].lines[j],
                   replace_in_line_const(doc->paragraphs[i].lines[j], target,
                                         replacement, result));
         }
      }
      return SUCCESS;
   }
}

/*We replace all target in a line.*/
static char *replace_in_line_const(char *orig, const char *target,
                                   const char *replacement, char *result) {
   /*target_position points to target.
      tmp points current target position. */
   char *target_position, *tmp;
   int len_target = strlen(target), len_replacement = strlen(replacement);
   /*i is for looping. */
   /*len_front is the distance between front to next target. */
   /*count is the number of target.
      size_result tracking the index of result */
   int i, len_front, count = 0, size_result = 0;

   /*Check if there is target in orig.
      If there is, we increase count and 
      move target_position to the next position of target. */
   target_position = orig;
   while (strstr(target_position, target) != NULL) {
      tmp = strstr(target_position, target);
      count++;
      /*We move target_position to the next position of target. */
      target_position = tmp + len_target;
   }

   while (count--) {
      /*target_position points to the first char of target.
         len_front is the distance between target and the front part.
       */
      target_position = strstr(orig, target);
      len_front = target_position - orig;
      /*We copy all characters before target to result. */
      for (i = 0; i < len_front; i++) {
         result[size_result] = orig[i];
         size_result++;
      }
      /*We copy the replacement to the result. */
      for (i = 0; i < len_replacement; i++) {
         result[size_result] = replacement[i];
         size_result++;
      }
      /*We move orig to the end of the target. */
      orig += len_front + len_target;
   }

   /*We copy the rest in orig to result. */
   for (i = 0; i < (int) strlen(orig); i++) {
      result[size_result] = orig[i];
      size_result++;
   }
   /*We add '\0' at the end.*/
   result[size_result] = '\0';

   return result;
}

/*We add HIGHLIGHT START STR and HIGHLIGHT END STR around target.*/
int highlight_text(Document * doc, const char *target) {
   /*replacement is HIGHLIGHT START STR and HIGHLIGHT END STR around target.
      result is for saving changed string.
      i is for looping through paragraphs.
      j is for looping through lines. */
   char replacement[MAX_STR_SIZE + 1];
   char result[MAX_STR_SIZE + 1];
   int i, j;

   if (doc == NULL || target == NULL) {
      return FAILURE;
   } else {
      /*We give values to replacement */
      strcpy(replacement, HIGHLIGHT_START_STR);
      strcat(replacement, target);
      strcat(replacement, HIGHLIGHT_END_STR);
      /*We loop through the doc. */
      for (i = 1; i <= doc->number_of_paragraphs; i++) {
         for (j = 1; j <= doc->paragraphs[i].number_of_lines; j++) {
/*We copy the changed string to the old string.*/
            strcpy(doc->paragraphs[i].lines[j],
                   replace_in_line_const(doc->paragraphs[i].lines[j],
                                         target, replacement, result));
         }
      }
      return SUCCESS;
   }
}

/*We remove target value. It equals to replace target with "". */
int remove_text(Document * doc, const char *target) {
   /*replacement is "".
      result is for saving changed string.
      i is for looping through paragraphs.
      j is for looping through lines. */
   char replacement[MAX_STR_SIZE + 1];
   char result[MAX_STR_SIZE + 1];
   int i, j;

   if (doc == NULL || target == NULL) {
      return FAILURE;
   } else {
/*We give values to replacement*/
      strcpy(replacement, "");
/*We loop through the doc.*/
      for (i = 1; i <= doc->number_of_paragraphs; i++) {
         for (j = 1; j <= doc->paragraphs[i].number_of_lines; j++) {
/*We copy the changed string to the old string.*/
            strcpy(doc->paragraphs[i].lines[j],
                   replace_in_line_const(doc->paragraphs[i].lines[j],
                                         target, replacement, result));
         }
      }
      return SUCCESS;
   }
}
