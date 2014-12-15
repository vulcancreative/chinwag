#include <time.h>
#include <stdarg.h>

#include "seuss_dict_test.h"
#include "latin_dict_test.h"

#include "../include/chinwag.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define test(f, buf, cnt, ...) testf(f, #f, buf, cnt, __VA_ARGS__)

I32 result = EXIT_SUCCESS;
U32 passed = 0, failed = 0;

char* reset(char* buffer)
{
  passed = 0; failed = 0;
  buffer = memset(buffer, '\0', LARGE_BUFFER);

  return buffer;
}

char* detail_results(char* buffer, F32 time)
{
  U32 len = strlen(buffer), total = passed + failed;

  char r[] = "\n\n%s[%d/%d specs passed, %d/%d specs failed in "
  "%.3f seconds]%s\n";

  if(failed > 0)
    sprintf(&buffer[len], r, ANSI_COLOR_RED, passed, total, failed, 
    total, time, ANSI_COLOR_RESET);
  else
    sprintf(&buffer[len],r, ANSI_COLOR_GREEN, passed, total, failed, 
    total, time, ANSI_COLOR_RESET);
  
  return buffer;
}

void pass()
{
  fprintf(stdout, ANSI_COLOR_GREEN "." ANSI_COLOR_RESET); fflush(stdout);
  ++passed;
}

char* fail(const char* name, U32 expected, U32 got, char* buffer, char* s)
{
  char* sample = NULL;
  U32 len = strlen(s);

  if(len > 250)
  {
    sample = substring_with_size(s, 0, 250);
    sample = add_suffix(sample, "...");
  }
  else sample = substring_with_size(s, 0, len - 1);

  char e[] = "\n\n%sERROR : function %s failed! "
  "Expected %d units, got %d units\n\n\"%s\"%s";

  fprintf(stderr, ANSI_COLOR_RED "." ANSI_COLOR_RESET);
  sprintf(&buffer[strlen(buffer)], e, ANSI_COLOR_RED, name, expected, got, 
  sample, ANSI_COLOR_RESET); free(sample);
  result = EXIT_FAILURE;
  ++failed;

  return buffer;
}

char* testf(char* (*f)(unsigned long, unsigned long, dict_t), char* n, char* b, U32 c,...)
{
  dict_t d;
  va_list arguments;
  U32 anchor = 0, got = 0;
  char* splitters = NULL; char* r = NULL;

  if(include(n, "wrd_rng")) splitters = " ";
  else if(include(n, "snt_rng")) splitters = ".!?";
  else if(include(n, "pgf_rng")) splitters = "\r\n";

  va_start(arguments, c);

  for(U32 j = 0; j != c; ++j)
  {
    d = va_arg(arguments, dict_t);
    anchor = motherr(1500, 2500); r = (*f)(anchor, anchor, d);

    if(include(n, "ltr_rng")) got = strlen(r);
    else got = count(r, splitters);

    if(got != anchor)
    { 
      b = fail(n, anchor, got, b, r);
      free(r); va_end(arguments); return b;
    }

    free(r);
  }

  va_end(arguments);
  pass();

  return b;
}

U32 parse_amount(I32 argc, const char *argv[])
{
  U32 amount = 1;

  if(argc < 3) return amount;
  else if(include(argv[1], "-a") || include(argv[1], "--amount"))
  { amount = strtol(argv[2], NULL, 10); }

  return amount;
}

I32 main(I32 argc, const char *argv[])
{
  clock_t begin;
  U32 amount = parse_amount(argc, argv);
  char* buffer = (char*)malloc(LARGE_BUFFER);
  dict_t seuss = open_dict_with_tokens(dict_seuss_test, DELIMITERS);
  dict_t latin = open_dict_with_tokens(dict_latin_test, DELIMITERS);

  validate_dict(seuss, "main");
  validate_dict(latin, "main");

  for(U32 i = 0; i != amount; ++i)
  {
    begin = clock();
    buffer = test(ltr_rng, buffer, 2, seuss, latin);
    buffer = test(wrd_rng, buffer, 2, seuss, latin);
    buffer = test(snt_rng, buffer, 2, seuss, latin);
    buffer = test(pgf_rng, buffer, 2, seuss, latin);
    buffer = detail_results(buffer, (clock() - begin) / CLOCKS_PER_SEC);

    fprintf(stdout, "%s", buffer);
    buffer = reset(buffer);
  }

  close_dict(seuss);
  close_dict(latin);

  free(buffer);

  return result;
}
