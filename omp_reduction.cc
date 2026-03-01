/******************************************************************************
 * FILE: omp_reduction.c
 * DESCRIPTION:
 *   OpenMP Example - Combined Parallel Loop Reduction - C/C++ Version
 *   This example demonstrates a sum reduction within a combined parallel loop
 *   construct.  Notice that default data element scoping is assumed - there
 *   are no clauses specifying shared or private variables.  OpenMP will
 *   automatically make loop index variables private within team threads, and
 *   global variables shared.
 * AUTHOR: Blaise Barney  5/99
 * LAST REVISED: 04/06/05
 ******************************************************************************/

// modified by J. Tian

#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpp_helper.hh"

#define FACTOR 10

double reduction_sequential() {

  double sum = 0.0;
  size_t i;
  size_t n = 1024 * 1024 * FACTOR; // MiB-level
  double *a = (double *)malloc(n * sizeof(double));
  auto t_start = hires::now();
  auto t_end = hires::now();
  double time_elapsed;

  /* some initializations */
  for (i = 0; i < n; i++)
    a[i] = i * 1.0;

  /* computing */
  t_start = hires::now();

  for (i = 0; i < n; i++)
    sum += a[i];

  t_end = hires::now();

  /* summary and clean-up */
  time_elapsed = static_cast<duration_t>(t_end - t_start).count();
  printf("(sequential::chrono-timer) sum = %f, time elapsed = %lf\n", sum,
         time_elapsed);

  free(a);

  return time_elapsed;
}

double reduction_parfor_timer_chrono() {

  /* some initializations */
  double sum = 0.0;
  size_t n = 1024 * 1024 * FACTOR; // MiB-level
  double *a = (double *)malloc(n * sizeof(double));
  auto t_start = hires::now();
  auto t_end = hires::now();
  double time_elapsed;

  size_t i;
  for (i = 0; i < n; i++)
    a[i] = i * 1.0;

  /* computing */
#pragma omp parallel
  {

#pragma omp single
    t_start = hires::now();

#pragma omp for reduction(+ : sum)
    for (i = 0; i < n; i++)
      sum += a[i];

#pragma omp single
    {
      /* summary and clean-up */
      t_end = hires::now();
      time_elapsed = static_cast<duration_t>(t_end - t_start).count();
      printf("(parfor::chrono-timer)     sum = %f, time elapsed = %lf\n", sum,
             time_elapsed);
    } // end of omp-single

  } // end of omp-parallel

  /* clean-up */
  free(a);

  return time_elapsed;
}

double reduction_parfor_timer_omp() {

  /* some initializations */
  double sum = 0.0;
  size_t n = 1024 * 1024 * FACTOR; // MiB-level
  double *a = (double *)malloc(n * sizeof(double));

  double t_start, t_end;

  size_t i;
  for (i = 0; i < n; i++)
    a[i] = i * 1.0;

/* computing */
#pragma omp parallel
  {
#pragma omp single
    t_start = omp_get_wtime();

#pragma omp for reduction(+ : sum)
    for (i = 0; i < n; i++)
      sum += a[i];

#pragma omp single
    t_end = omp_get_wtime();
  }

  /* summary and clean-up */
  auto time_elapsed = t_end - t_start;
  printf("(parfor::omp-timer)        sum = %f, time elapsed = %lf\n", sum,
         time_elapsed);

  free(a);

  return time_elapsed;
}

int main(int argc, char *argv[]) {

  // report sequantial reference

  reduction_sequential();

  // report OMP-parallelized
  reduction_parfor_timer_omp();

  // report sequantial reference
  reduction_parfor_timer_chrono();

  return 0;
}