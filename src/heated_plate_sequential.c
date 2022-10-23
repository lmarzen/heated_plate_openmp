#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

/* Purpose:
 *
 *   MAIN is the main program for HEATED_PLATE_OPENMP.
 *
 * Discussion:
 *
 *   This code solves the steady state heat equation on a rectangular region.
 *
 *   The sequential version of this program needs approximately
 *   18/epsilon iterations to complete.
 *
 *
 *   The physical region, and the boundary conditions, are suggested
 *   by this diagram;
 *
 *                  W = 0
 *            +------------------+
 *            |                  |
 *   W = 100  |                  | W = 100
 *            |                  |
 *            +------------------+
 *                  W = 100
 *
 *   The region is covered with a grid of M by N nodes, and an N by N
 *   array W is used to record the temperature.  The correspondence between
 *   array indices and locations in the region is suggested by giving the
 *   indices of the four corners:
 *
 *                 I = 0
 *         [0][0]-------------[0][N-1]
 *            |                  |
 *     J = 0  |                  |  J = N-1
 *            |                  |
 *       [M-1][0]-----------[M-1][N-1]
 *                 I = M-1
 *
 *   The steady state solution to the discrete heat equation satisfies the
 *   following condition at an interior grid point:
 *
 *     W[Central] = (1/4) * ( W[North] + W[South] + W[East] + W[West] )
 *
 *   where "Central" is the index of the grid point, "North" is the index
 *   of its immediate neighbor to the "north", and so on.
 *
 *   Given an approximate solution of the steady state heat equation, a
 *   "better" solution is given by replacing each interior point by the
 *   average of its 4 neighbors - in other words, by using the condition
 *   as an ASSIGNMENT statement:
 *
 *     W[Central]  <=  (1/4) * ( W[North] + W[South] + W[East] + W[West] )
 *
 *   If this process is repeated often enough, the difference between successive
 *   estimates of the solution will go to zero.
 *
 *   This program carries out such an iteration, using a tolerance specified by
 *   the user, and writes the final estimate of the solution to a file that can
 *   be used for graphic processing.
 *
 * Licensing:
 *
 *   This code is distributed under the GNU LGPL license.
 *
 * Modified:
 *
 *   22 July 2008
 *
 * Author:
 *
 *   Original C version by Michael Quinn.
 *   Modifications by John Burkardt.
 *   Further Modifications by Luke Marzen.
 *
 * Reference:
 *
 *   Michael Quinn,
 *   Parallel Programming in C with MPI and OpenMP,
 *   McGraw-Hill, 2004,
 *   ISBN13: 978-0071232654,
 *   LC: QA76.73.C15.Q55.
 *
 * Parameters:
 *
 *   Commandline argument 1, double EPSILON, the error tolerance.
 *
 *   Commandline argument 2, char *OUTPUT_FILE, the name of the file into which
 *   the steady state solution is written when the program has completed.
 *
 * Local parameters:
 *
 *   Local, double DIFF, the norm of the change in the solution from one iteration
 *   to the next.
 *
 *   Local, double MEAN, the average of the boundary values, used to initialize
 *   the values of the solution in the interior.
 *
 *   Local, double U[M][N], the solution at the previous iteration.
 *
 *   Local, double W[M][N], the solution computed at the latest iteration.
 */
int main(int argc, char *argv[])
{
#ifndef M
#define M 500
#endif
#ifndef N
#define N 500
#endif

  double diff;
  FILE *fp;
  int i;
  int iterations;
  int iterations_print;
  int j;
  double mean;
  double u[M][N];
  double w[M][N];
  struct timeval start, end;
  double time_taken = 0;

  // default options
  double epsilon = 0.001;
  char *output_file = NULL; // NULL = no output
  int verbose = 1;
  int report_time = 0;

  // process option flags
  opterr = 0;
  int flag;
  while ((flag = getopt(argc, argv, "e:o:qt")) != -1)
  {
    switch (flag)
    {
    case 'e':
      epsilon = atof(optarg);
      if (epsilon <= 0)
      {
        printf("Illegal Input: Epsilon (error tolerance) must be greater than 0.\n");
        return 1;
      }
      break;
    case 'o':
      output_file = optarg;
      break;
    case 'q':
      verbose = 0;
      break;
    case 't':
      report_time = 1;
      break;
    case '?':
      if (optopt == 'e' || optopt == 'o')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option '-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
      return 1;
    default:
      return 1;
    }
  } // end options while-loop

  for (i = optind; i < argc; i++)
  {
    printf("Non-option argument %s\n", argv[i]);
  }

  diff = epsilon;

  if (verbose)
  {
    printf("\n");
    printf("HEATED_PLATE_SEQUENTIAL\n");
    printf("  C sequential version\n");
    printf("  A program to solve for the steady state temperature distribution\n");
    printf("  over a rectangular plate.\n");
    printf("\n");
    printf("  Spatial grid of %d by %d points.\n", M, N);
    printf("  The iteration will be repeated until the change is <= %e\n", epsilon);
    if (output_file != NULL)
    {
      printf("  The steady state solution will be written to '%s'.\n", output_file);
    }
  }

  gettimeofday(&start, NULL); // start timer

  // Set the boundary values, which don't change.
  for (i = 1; i < M - 1; i++)
  {
    w[i][0] = 100.0;
    w[i][N - 1] = 100.0;
  }
  for (j = 0; j < N; j++)
  {
    w[M - 1][j] = 100.0;
    w[0][j] = 0.0;
  }

  // Average the boundary values, to come up with a reasonable initial value for
  // the interior.
  mean = 0.0;
  for (i = 1; i < M - 1; i++)
  {
    mean += w[i][0] + w[i][N - 1];
  }
  for (j = 0; j < N; j++)
  {
    mean += w[M - 1][j] +  w[0][j];
  }
  mean = mean / (double)(2 * M + 2 * N - 4);
  if (verbose)
  {
    printf("\n");
    printf("  MEAN = %f\n", mean);
  }

  // Initialize the interior solution to the mean value.
  for (i = 1; i < M - 1; i++)
  {
    for (j = 1; j < N - 1; j++)
    {
      w[i][j] = mean;
    }
  }

  // iterate until the  new solution W differs from the old solution U by no
  // more than EPSILON.
  iterations = 0;
  iterations_print = 1;
  if (verbose)
  {
    printf("\n");
    printf("  Iteration Change\n");
    printf("\n");
  }

  gettimeofday(&start, NULL); // start timer

  while (epsilon <= diff)
  {
    // Save the old solution in U.
    for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
      {
        u[i][j] = w[i][j];
      }
    }

    // Determine the new estimate of the solution at the interior points. The
    // new solution W is the average of north, south, east and west neighbors.
    diff = 0.0;
    for (i = 1; i < M - 1; i++)
    {
      for (j = 1; j < N - 1; j++)
      {
        w[i][j] = (u[i - 1][j] + u[i + 1][j] + u[i][j - 1] + u[i][j + 1]) / 4.0;

        if (diff < fabs(w[i][j] - u[i][j]))
        {
          diff = fabs(w[i][j] - u[i][j]);
        }
      }
    }
    iterations++;
    if ((iterations == iterations_print) & verbose)
    {
      printf("  %8d  %f\n", iterations, diff);
      iterations_print = 2 * iterations_print;
    }
  } // end while-loop

  gettimeofday(&end, NULL); // stop timer

  if (verbose)
  {
    printf("\n");
    printf("  %8d  %f\n", iterations, diff);
    printf("\n");
    printf("  Error tolerance achieved.\n");
  }
  if (report_time | verbose)
  {
    time_taken = end.tv_sec + end.tv_usec / 1e6 -
                 start.tv_sec - start.tv_usec / 1e6; // in seconds
    printf("  Execution time = %fs\n", time_taken);
  }

  // Write the solution to the output file.
  if (output_file != NULL)
  {
    fp = fopen(output_file, "w");

    fprintf(fp, "%d\n", M);
    fprintf(fp, "%d\n", N);

    for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
      {
        fprintf(fp, "%6.2f ", w[i][j]);
      }
      fputc('\n', fp);
    }
    fclose(fp);

    printf("\n");
    printf("  Solution written to the output file '%s'\n", output_file);
  }

  if (verbose)
  {
    printf("\n");
    printf("HEATED_PLATE_SEQUENTIAL:\n");
    printf("  Normal end of execution.\n");
  }

  return 0;

#undef M
#undef N
}
