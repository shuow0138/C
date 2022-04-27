#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"event.h"
#include"calendar.h"

/*help functions prototypes*/
static void add(Event ** head, Event * new, Calendar * calendar);
static void print_events(Event * head, FILE * output_stream);

/*This function initializes a Calendar structure 
  based on the parameters provided.*/
int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar) {

   if (calendar == NULL || name == NULL || days < 1) {
      return FAILURE;
   }

   /*Allocate memory for Calendar and check if succeed. */
   *calendar = malloc(sizeof(Calendar));
   if (*calendar == NULL) {
      return FAILURE;
   }
   /*Allocate memory for string and check if succeed. */
   (*calendar)->name = malloc(strlen(name) + 1);
   if ((*calendar)->name == NULL) {
      return FAILURE;
   }
   /*Copy values to calendar. */
   strcpy((*calendar)->name, name);
   (*calendar)->days = days;
   (*calendar)->total_events = 0;
   /*Allocate memory for array and check if succeed. */
   (*calendar)->events = calloc(days, sizeof(Event *));
   if ((*calendar)->events == NULL) {
      return FAILURE;
   }
   /*Copy values to calendar. */
   (*calendar)->comp_func = comp_func;
   (*calendar)->free_info_func = free_info_func;

   return SUCCESS;
}

/*This function prints, to the designated output stream. 
  print_all decides if printing more infomation.*/
int print_calendar(Calendar * calendar, FILE * output_stream, int print_all) {
   /*i is for looping. */
   int i;

   if (calendar == NULL || output_stream == NULL) {
      return FAILURE;
   }
   /*If print_all is true, we print extra data. */
   if (print_all) {
      fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
      fprintf(output_stream, "Days: %d\n", calendar->days);
      fprintf(output_stream, "Total Events: %d\n", calendar->total_events);
      fprintf(output_stream, "\n");
   }

   fprintf(output_stream, "**** Events ****\n");
   if (calendar->total_events != 0) {
      /*Loop through the whole calendar. */
      for (i = 1; i <= calendar->days; i++) {
         fprintf(output_stream, "Day %d\n", i);
         /*If the event linkedlist is not empty,
            we print its value. */
         if (calendar->events[i - 1] != NULL) {
            print_events(calendar->events[i - 1], output_stream);
         }
      }
   }
   return SUCCESS;
}

/*This help function prints all elements in a linkedlist.*/
static void print_events(Event * head, FILE * output_stream) {
   /*If head is not NULL, we will print its elements. */
   if (head) {
      Event *current = head;
      while (current != NULL) {
         fprintf(output_stream,
                 "Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
                 current->name, current->start_time,
                 current->duration_minutes);
         /*Move to next element. */
         current = current->next;
      }
   }
}

/*This function adds the specified event to the list
  associated with the day parameter*/
int add_event(Calendar * calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
   /*new_item is a new event storing new data.
      current keeps track of current event.
      i is for looping. */
   Event *new_item, *current;
   int i;

   if (calendar == NULL || name == NULL || start_time < 0 ||
       start_time > 2400 || duration_minutes <= 0 || day < 1
       || day > calendar->days) {
      return FAILURE;
   }
   /*This for loop checks if the same event is 
      already in the calendar. */
   for (i = 1; i <= calendar->days; i++) {
      if (calendar->events[i - 1] != NULL) {
         current = calendar->events[i - 1];
         while (current != NULL) {
            /*If they have same name, they are considered 
               as the same event. */
            if (!strcmp(current->name, name)) {
               return FAILURE;
            }
            current = current->next;
         }
      }
   }
   /*Allocate memory for new_item and 
      its parameter and checks if succeed */
   new_item = malloc(sizeof(Event));
   if (new_item == NULL) {
      return FAILURE;
   }
   new_item->name = malloc(strlen(name) + 1);
   if (new_item->name == NULL) {
      return FAILURE;
   }
   strcpy(new_item->name, name);
   /*copy values to calendar. */
   new_item->start_time = start_time;
   new_item->duration_minutes = duration_minutes;
   new_item->info = info;
   new_item->next = NULL;
   /*add in increasing order. */
   add(&(calendar->events[day - 1]), new_item, calendar);
   /*increase event number. */
   calendar->total_events++;

   return SUCCESS;
}

/*This help func is for adding new event in increasing order.*/
static void add(Event ** head, Event * new, Calendar * calendar) {
   Event *current = *head, *prev = NULL;
   /*Keep looping until finding the correct position. */
   while (current != NULL && calendar->comp_func(current, new) <= 0) {
      prev = current;
      current = current->next;
   }
   /*make current as the next of new event. */
   new->next = current;
   /*If it is head, head points to new event.
      Otherwise, previous points to the new event. */
   if (prev == NULL) {
      *head = new;
   } else {
      prev->next = new;
   }
}

/*This function will return a pointer (event) 
  to the event with the specified name (if any). */
int find_event(Calendar * calendar, const char *name, Event ** event) {
   /*i is for looping. */
   int i;

   if (calendar == NULL || name == NULL) {
      return FAILURE;
   }
   /*loop through the whole calendar. */
   for (i = 1; i <= calendar->days; i++) {
      /*If we find the target, we return SUCCESS.
         Otherwise, we keep looping. */
      if (find_event_in_day(calendar, name, i, event) == SUCCESS) {
         return SUCCESS;
      }
   }
   /*If not find the target, we return FAILURE. */
   return FAILURE;
}

/*This function will return a pointer (event) 
to the event with the specified name
in the specified day (if such event exist).*/
int find_event_in_day(Calendar * calendar, const char *name, int day,
                      Event ** event) {
   /*current keeps track of current event. */
   Event *current;

   if (calendar == NULL || name == NULL || day < 1 || day > calendar->days) {
      return FAILURE;
   }
   /*loop through the whole calendar. */
   if (calendar->events[day - 1] != NULL) {
      current = calendar->events[day - 1];
      while (current != NULL) {
         /*If find the same event, make the event pointer
            point to the current event */
         if (!strcmp(current->name, name)) {
            if (event != NULL) {
               *event = current;
            }
            return SUCCESS;
         }
         current = current->next;
      }
   }
   /*If not find the target, we return FAILURE. */
   return FAILURE;
}

/*This function will remove the specified event 
  from the calendar returning any memory allocated for the
  event.*/
int remove_event(Calendar * calendar, const char *name) {
   /*current keeps tracks of current event
      next keeps tracks of next event */
   Event *current, *prev = NULL;
   int i;

   if (calendar == NULL || name == NULL) {
      return FAILURE;
   }
   /*Loop throught whole calendar. */
   for (i = 1; i <= calendar->days; i++) {
      if (calendar->events[i - 1] != NULL) {
         current = calendar->events[i - 1];
         while (current != NULL) {
            /*If we find the target, 
               we conncet its previous and next event.
               Then remove the current event. */
            if (!strcmp(current->name, name)) {
               /*if it is the head, we make head point to its next.
                  Otherwise, previous point to its next. */
               if (prev == NULL) {
                  calendar->events[i - 1] = current->next;
               } else {
                  prev = current->next;
               }
               /*Free current. */
               if (current->info != NULL && calendar->free_info_func != NULL) {
                  calendar->free_info_func(current->info);
               }
               free(current->name);
               free(current);
               /*decrease event number. */
               calendar->total_events--;
               return SUCCESS;
            }
            /*move to next event. */
            prev = current;
            current = current->next;
         }
      }
   }
   /*if not find, return FAILURE. */
   return FAILURE;
}

/*This function returns the info pointer
  associated with the specified event. */
void *get_event_info(Calendar * calendar, const char *name) {
   /*current keeps track of current event
      i is for looping. */
   Event *current;
   int i;

   /*We loop through the whole calendar and 
      if we find the target we return the info. */
   for (i = 1; i <= calendar->days; i++) {
      if (calendar->events[i - 1] != NULL) {
         current = calendar->events[i - 1];
         while (current != NULL) {
            if (!strcmp(current->name, name)) {
               return current->info;
            }
            current = current->next;
         }
      }
   }
   /*if not find, we return NULL */
   return NULL;
}

/*This function will remove all the event lists
  associated with the calendar and set them to empty lists.*/
int clear_calendar(Calendar * calendar) {
   /*i is for looping */
   int i;

   if (calendar == NULL) {
      return FAILURE;
   }
   /*We loop through each day and clear them. */
   for (i = 1; i <= calendar->days; i++) {
      if (calendar->events[i - 1] != NULL) {
         clear_day(calendar, i);
      }
   }
   return SUCCESS;
}

/*This function will remove all the events for the specified day
 setting the event list to empty. 
The total number of events will be adjusted accordingly.*/
int clear_day(Calendar * calendar, int day) {
   /*current keeps tracks of current event
      next keeps tracks of next event */
   Event *current, *next;

   if (calendar == NULL || day < 1 || day > calendar->days) {
      return FAILURE;
   }

   if (calendar->events[day - 1] != NULL) {
      current = calendar->events[day - 1];
      while (current != NULL) {
         next = current->next;
         /*free memory */
         if (current->info != NULL && calendar->free_info_func != NULL) {
            calendar->free_info_func(current->info);
         }
         free(current->name);
         free(current);
         /*decrease event number. */
         calendar->total_events--;
         /*move to next element. */
         current = next;
      }
      /*setting the event list to empty */
      calendar->events[day - 1] = NULL;
   }

   return SUCCESS;
}

/*This function will return memory that was 
  dynamically-allocated for the calendar.*/
int destroy_calendar(Calendar * calendar) {

   if (calendar == NULL) {
      return FAILURE;
   }

   clear_calendar(calendar);
   free(calendar->events);
   free(calendar->name);
   free(calendar);
   return SUCCESS;
}
