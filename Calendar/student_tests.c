#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

typedef struct task_info {
   double cost;
   char *prog_language;
} Task_info;

static Task_info *create_task_info(double cost, const char *prog_language) {
   Task_info *task_info = malloc(sizeof(Task_info));

   if (task_info) {
      task_info->prog_language = malloc(strlen(prog_language) + 1);
      if (task_info->prog_language) {
         task_info->cost = cost;
         strcpy(task_info->prog_language, prog_language);
         return task_info;
      }
   }

   return NULL;
}

static void free_task_info(void *ptr) {
   Task_info *task_info = (Task_info *) ptr;

   free(task_info->prog_language);
   free(task_info);
}

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *) ptr1)->duration_minutes -
      ((Event *) ptr2)->duration_minutes;
}

static int comp_name(const void *ptr1, const void *ptr2) {
   return strcmp(((Event *) ptr1)->name, ((Event *) ptr2)->name);
}

/* Description here: This test checks if init_calendar, 
   print_calendar, destory_calendar works fine with correct values.*/
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*This test checks if add_event 
   including init_calendar, print_calendar and destory_calendar
   returns SUCCESS with all correct values.*/
static int test2() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);

   info = create_task_info(35000, "Java");
   if (add_event(calendar, "debugging", start_time_mil, duration_minutes,
                 info, activity_day) == SUCCESS) {

      info = create_task_info(10000, "JUnit");
      printf("the number of add is %d\n",
             add_event(calendar, "testing", 1600, 100, info, 1));
      destroy_calendar(calendar);
      return SUCCESS;
   } else {
      return FAILURE;
   }
}

/*This test checks if  add_event 
  including init_calendar, print_calendar and destory_calendar
    works when same event already exists(same name).*/
static int test3() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);

   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes,
             info, activity_day);

   /* info = create_task_info(10000, "JUnit"); */
   if (add_event(calendar, "debugging", 1600, 100, NULL, 1) == FAILURE) {
      destroy_calendar(calendar);
      return SUCCESS;
   } else {
      printf("fail\n");
      destroy_calendar(calendar);
      return FAILURE;
   }
}

/*This checks if remove_event works with all correct parameters
 and non-exist event.
 including init_calendar, print_calendar and destory_calendar*/
static int test4() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);
   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes,
             info, activity_day);
   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1);
   /*exist case */
   if (remove_event(calendar, "debugging") == SUCCESS) {
      /*non-exist case */
      if (remove_event(calendar, "debugging") == FAILURE) {
         destroy_calendar(calendar);
         return SUCCESS;
      }
   }
   destroy_calendar(calendar);
   return FAILURE;
}

/*This checks if find_events works for
  both situation(target exist and target not exist)
  including init_calendar, print_calendar and destory_calendar*/
static int test5() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;
   Event *event;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);
   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes,
             info, activity_day);
   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1);
   /*exist case */
   if (find_event(calendar, "debugging", &event) == SUCCESS) {
      if (remove_event(calendar, "debugging") == SUCCESS) {
         /*non-exist case */
         if (find_event(calendar, "debugging", &event) == FAILURE) {
            destroy_calendar(calendar);
            return SUCCESS;
         } else {
            destroy_calendar(calendar);
            return FAILURE;
         }
      }
   }

   return FAILURE;
}

/*This checks if find_events_in_day works for
  both situation(target exist and target not exist) 
  and for wrong day parameter.
including init_calendar, print_calendar and destory_calendar*/
static int test6() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;
   Event *event;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);
   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes,
             info, activity_day);
   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1);
   /*exist case */
   if (find_event_in_day(calendar, "debugging", 1, &event) == SUCCESS
       /*non-exist case */
       && find_event_in_day(calendar, "debugging", 2, &event) == FAILURE) {
      if (remove_event(calendar, "debugging") == SUCCESS) {
         /*wrong day case */
         if (find_event_in_day(calendar, "debugging", 1, &event) == FAILURE) {
            destroy_calendar(calendar);
            return SUCCESS;
         }
      }
   }
   destroy_calendar(calendar);
   return FAILURE;
}

/*This checks if clear_day and clear_calendar works 
  when have all correct parameters.
It also checks clear_day with wrong day.
including init_calendar, print_calendar and destory_calendar*/
static int test7() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);
   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes,
             info, activity_day);
   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1);
   info = create_task_info(10001, "JUnit2");
   add_event(calendar, "testing2", 1600, 100, info, 2);
   print_calendar(calendar, stdout, 1);
   /*normal case and wrong day case(clear_day) */
   if (clear_day(calendar, 1) == SUCCESS && clear_day(calendar, 4) == FAILURE) {
      printf("-------remove day1\n");
      print_calendar(calendar, stdout, 1);

      if (clear_calendar(calendar) == SUCCESS) {
         printf("-------remove all\n");
         print_calendar(calendar, stdout, 1);
         destroy_calendar(calendar);
         return SUCCESS;
      }
   }
   destroy_calendar(calendar);
   return FAILURE;
}

/*This checks if get_event_info works for
  both situation(target exist and target not exist)
  including init_calendar, print_calendar and destory_calendar */
static int test8() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;
   Event *event;
   void *p, *p2, *q;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);
   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes,
             info, activity_day);
   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1);
   /*exist case */
   p = get_event_info(calendar, "testing");
   /*non-exist case */
   p2 = get_event_info(calendar, "testing");
   q = info;
   /*check if the address is the same. */
   if (p == q) {
      if (p2 == NULL) {
         destroy_calendar(calendar);
         return SUCCESS;
      }
   }
   destroy_calendar(calendar);
   return FAILURE;
}
/*This checks if event is NULL and we find the target
 we still return SUCCESS.*/
static int test9() {
   int days = 3, start_time_mil = 800, duration_minutes = 300;
   int activity_day = 1;
   Calendar *calendar;
   Task_info *info;
   Event *event;

   init_calendar("Winter Calendar", days, comp_name, free_task_info,
                 &calendar);
   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes,
             info, activity_day);
   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 1);
   info = create_task_info(10050, "JUn");
   add_event(calendar, "test2", 1600, 100, info, 1);
   /*exist case */
   if (find_event_in_day(calendar, "debugging", 1, NULL) == SUCCESS) {
     destroy_calendar(calendar);
     return SUCCESS;
   }
   destroy_calendar(calendar);
   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   /* if (test1() == FAILURE)
      result = FAILURE;
   if (test2() == FAILURE)
      result = FAILURE;
   if (test3() == FAILURE)
      result = FAILURE;
   if (test4() == FAILURE)
      result = FAILURE;
   if (test5() == FAILURE)
      result = FAILURE;
   if (test6() == FAILURE)
      result = FAILURE;
   if (test7() == FAILURE)
      result = FAILURE;
   if (test8() == FAILURE)
      result = FAILURE;*/
   if (test9() == FAILURE)
      result = FAILURE;
   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/

   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
