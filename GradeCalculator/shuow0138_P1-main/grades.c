/*Since the max value of assignments is 50,
  we have MAX_NUM_OF_ASSIGNMENTS 50.
  Since the max score(including weight) of 
  assignments is 100, we have MAX_SCORE 100.
  Since the lowest weight of assignments is 0,
  we have LOWEST_WEIGHT 0. */
  
#include<stdio.h>
#include<math.h>
#define MAX_NUM_OF_ASSIGNMENTS 50
#define MAX_SCORE 100
#define LOWEST_WEIGHT 0

/*Prototypes*/
int *get_assignment_detail(int total_num_of_assignments,
                           int *assignment_detail);
void print_assignment_detail(int total_num_of_assignments,
                             int *assignment_detail);
double get_sum(int total_num_of_assignments, int *assignment_detail,
               int points_penalty);
double get_standard_deviation(int total_num_of_assignments,
                              int points_penalty, int *assignment_detail,
                              double mean);
double get_numeric_score(int assignments_dropped, int *assignment_detail,
                         int points_penalty, int total_num_of_assignments);
int weight_check(int *assignment_detail, int total_num_of_assignments);

/*We use array to contain the info of assignments.
 Since the max value of assignments is 50 and each assignment
 has 4 categories: Assignment Number, Score, Weight, Days Late, 
 then we have the size of array assignment_detail is
 4 * MAX_NUM_OF_ASSIGNMENT */

/*sum is the sum of assignments grade.
 mean is the mean of assignments grade*/
int main() {
   int points_penalty, assignments_dropped;
   char stats_info;
   int total_num_of_assignments;
   int assignment_detail[4 * MAX_NUM_OF_ASSIGNMENTS];
   double sum;
   double mean;
 
   scanf("%d %d %c", &points_penalty, &assignments_dropped, &stats_info);
   scanf("%d", &total_num_of_assignments);
   get_assignment_detail(total_num_of_assignments, assignment_detail);
   /*weight check.
     If it is weight_check is false,
     we terminate the program.*/
   if (!weight_check(assignment_detail, total_num_of_assignments)) {
      return 0;
   } else {
      printf("Numeric Score: %5.4f\n",
             get_numeric_score(assignments_dropped, assignment_detail,
                               points_penalty, total_num_of_assignments));
      printf("Points Penalty Per Day Late: %d\n", points_penalty);
      printf("Number of Assignments Dropped: %d\n", assignments_dropped);
      printf("Values Provided:\n");
      printf("Assignment, Score, Weight, Days Late\n");
      print_assignment_detail(total_num_of_assignments, assignment_detail);

      /*If stats equals y or Y, we show the statistical infomation. */
      if (stats_info == 'y' || stats_info == 'Y') {
         sum =
            get_sum(total_num_of_assignments, assignment_detail,
                    points_penalty);
         mean = sum / total_num_of_assignments;
         printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean,
                get_standard_deviation(total_num_of_assignments,
                                       points_penalty, assignment_detail,
                                       mean));
      }

      return 0;
   }
}

/*We reads from inputs and create array(in the parameter). 
For each assignment, there are 4 categories: 
Assignment Number, Score, Weight, Days Late.

It can be seen as we divide each 4 position for a assignment.
The 0 position is for assignment_number and 
1 for score and so on.*/
int *get_assignment_detail(int total_num_of_assignments,
                           int *assignment_detail) {
   int assignment_number, score, weight, days_late;
   /*i is for loops. */
   int i;

   for (i = 0; i < total_num_of_assignments; i++) {
      scanf("%d,%d,%d,%d", &assignment_number, &score, &weight, &days_late);

      /*The reason we do "4 * (assignment_number - 1)" is that
         we take each 4 position as a whole.

         Then for different assignment, the array increase by 4.
         We minus 1 because we already consider the 1st assignment by using 
         "0 + 4 * (assignment_number - 1)"  and "1+" and so on.

         In order to sort the assignments by their assignments number,
         we have "assignment_number - 1" instead of "i-1".

         In that case,
	 the 1st assignments will directly go to the 1st 4-positions
         and the third will directly go to the 3rd 4-positions 
         leaving the 2nd 4-positions unchanged. */
      assignment_detail[0 + 4 * (assignment_number - 1)] = assignment_number;
      assignment_detail[1 + 4 * (assignment_number - 1)] = score;
      assignment_detail[2 + 4 * (assignment_number - 1)] = weight;
      assignment_detail[3 + 4 * (assignment_number - 1)] = days_late;
   }
 
   return assignment_detail;
}

/*We print the array elements. Since 4 positions is a whole,
we will print '\n' for each 4 elements.*/
void print_assignment_detail(int total_num_of_assignments,
                             int *assignment_detail) {
   int j;

   for (j = 0; j < total_num_of_assignments * 4; j++) {
      if ((j + 1) % 4 == 0) {
/*if it is the 4th elements for a assignment, we print with '\n'*/
         printf("%d\n", assignment_detail[j]);;
      } else {
/*if it is not the 4th elements for a assignment, we just print with a ', '*/
         printf("%d, ", assignment_detail[j]);
      }
   }
}

/*We go through the whole array and add every individual grades. */
double get_sum(int total_num_of_assignments, int *assignment_detail,
               int points_penalty) {
   /*j is for loops */
   int j;
   double sum;
   for (j = 0; j < total_num_of_assignments; j++) {
      /*Since only the 1st position(starting from 0) is grades,
        and only the 3rd position(starting from 0) is days past,
        we can only get the those two and calculate the grade. */
      sum += assignment_detail[1 + 4 * j]
         - points_penalty * assignment_detail[3 + 4 * j];
   }
   return sum;
}

/*We go through the whole array and add up the  difference between 
every individual grades and the mean. Then we squre the sum of difference, 
divide with the total number of assignments and take squre root. */
double get_standard_deviation(int total_num_of_assignments,
                              int points_penalty, int *assignment_detail,
                              double mean) {
   /*j is for loops.
      We initialize sum to 0 since we don't want sum has trash values. */
   int j;
   double sum = 0;
   for (j = 0; j < total_num_of_assignments; j++) {
      sum +=
         pow((assignment_detail[1 + 4 * j] -
              points_penalty * assignment_detail[3 + 4 * j] - mean), 2);

   }
   return sqrt(sum / total_num_of_assignments);
}

/*
We first calculate the score of each assignments with late penalty.
Then we remove lowest assignment one by one by storing its weight and then 
we make the lowest score to MAX_SCORE, 
(it would not influence us finding the 2nd lowest grade)
and set its weight to the LOWEST_WEIGHT=0
(it would not affect the total score).
After removing the lowest grade(s), we calculate the score using weight.
*/
double get_numeric_score(int assignments_dropped, int *assignment_detail,
                         int points_penalty, int total_num_of_assignments) {
   /*  i,j is for loops
      min_score is for comparing calues.
      num_of_min_score is for recoding the assignment number of lowest grade.
      weight_of_min_score is for recoding the weight of lowest grade.
    */
   int i, j, min_score = MAX_SCORE, num_of_min_score;

   double weight_of_min_score = 0.0, rest_weight = 1.0, numeric_score = 0.0;
   double grade = 0;
   int temp[4 * MAX_NUM_OF_ASSIGNMENTS];

   /*We copy values form assignment_detail to temp */
   for (i = 0; i < 4 * MAX_NUM_OF_ASSIGNMENTS; i++) {
      temp[i] = assignment_detail[i];
   }
   /*We set the min_score to MAX_SCORE and
     if current value is less than min_score,
     we update min_socre and record its weight and assignment number. */
   for (i = 0; i < assignments_dropped; i++) {
      for (j = 0; j < total_num_of_assignments; j++) {
         if (temp[1 + 4 * j] - points_penalty * temp[3 + 4 * j] <= min_score) {
            min_score = temp[1 + 4 * j] - points_penalty * temp[3 + 4 * j];
            weight_of_min_score = temp[2 + 4 * j];
            num_of_min_score = j;
         }
      }
      /*We make the lowest score to MAX_SCORE 
	(it would not influence us finding the 2nd lowest grade)
	and set its weight to the LOWEST_WEIGHT=0
	(it would not affect the total score). */
      temp[num_of_min_score * 4 + 1] = MAX_SCORE;
      temp[num_of_min_score * 4 + 2] = LOWEST_WEIGHT;
      /*When calculating the weight, 
	we will apply the percent by dividing 100. */
      rest_weight -= weight_of_min_score / 100;
      /*We set min_score back to MAX_SCORE,
	in case we need to find the second lowest grade. */
      min_score = MAX_SCORE;
   }
   /*We calculate the numeric grade. 
     Since the weight of the lowest grade is 0,
     it will not affect our result. */
   for (j = 0; j < total_num_of_assignments; j++) {
      grade = temp[1 + 4 * j] - points_penalty * temp[3 + 4 * j];
      /*If we remove any assignments, the total_weight is not 1(or 100) now.
         Then we divideconsider the rest weight as total weight by 
	 doing "individual_weight / rest_weight"
         Then we can get each grade based on new weight. */
      numeric_score += grade * temp[2 + 4 * j] / (rest_weight * 100);
   }

   return numeric_score;
}

/*We check weight by adding up all weights.
If it equals 100, we return 1 for true, 
otherwise, we return 0; */
int weight_check(int *assignment_detail, int total_num_of_assignments) {
  /*We initialize weight_check to 0,
then our result will not be affected by trash value.*/
  /*j is for looping.*/
  int weight_check = 0, j;
  /*We loop through all 2nd positions(starting with 0)*/
  for (j = 0; j < total_num_of_assignments; j++) {
    weight_check += assignment_detail[2 + 4 * j];
  }

  if (weight_check == 100) {
      return 1;
  } else {
    printf("ERROR: Invalid values provided");
    return 0;
  }
}
