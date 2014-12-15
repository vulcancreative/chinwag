#include "../include/chinwag.h"

char* chinwag(cw_t type, unsigned long min, unsigned long max, dict_t dict)
{
  char* result = NULL;

  if(min == 0 || max == 0)
  {
    char e[]="ERROR : chinwag requires max and min to be greater than 0; "
    "got min : %d, max : %d\n";
    fprintf(stderr, e, min, max);
    exit(EXIT_FAILURE);
  }

  if(max < min)
  {
    char e[]="ERROR : chinwag requires max greater than, or equal to, min; "
    "got min : %d, max : %d\n";
    fprintf(stderr, e, min, max);
    exit(EXIT_FAILURE);
  }

  if(type >= 4)
  {
    char e[]="ERROR : chinwag requires a valid output type\n";
    fprintf(stderr, e, min, max);
    exit(EXIT_FAILURE);
  }

  if(type == CW_LETTERS) result = ltr_rng(min, max, dict);
  else if(type == CW_WORDS) result = wrd_rng(min, max, dict);
  else if(type == CW_SENTENCES) result = snt_rng(min, max, dict);
  else if(type == CW_PARAGRAPHS) result = pgf_rng(min, max, dict);

  return result;
}

char* ltr_rng(unsigned long min, unsigned long max, dict_t dict)
{
  dict_t temp = open_dict();
  I32 amount = motherr(min,max), total = 0; U32 len = 0;
  char* s = (char*)malloc(SMALL_BUFFER); char* sample = NULL; 
  char* result = NULL; char* vowels = "aeiou";

  while(amount > 0)
  {
    if(amount == 2)
    {
      // SSWS : modifies destination, can't modify source, new string
      sample = sample_substring_with_size(vowels, 1);
      s = strcpy(s, sample); free(sample);
      len = 1; total += len;
      s[len] = '\0';
    }
    else
    {
      s = strcpy(s, sample_dict(dict));
      len = strlen(s); total += len; s[len] = '\0';
      if(len > amount || include(s, " ") || include(s, "-")) continue;
    }

    amount -= len;

    // postfixed alteration (append vowel chain/remove trailing character)
    if(amount + 1 == 0)
    {
      // SWS : modifies destination, can't modify source, new string
      sample = substring_with_size(s, 0, len - 2);
      s = strcpy(s, sample); free(sample);
      len -= 1; total -= 1; s[len] = '\0';

      amount += 1;
    }
    else if(amount - 1 == 0)
    {
      // SSWS : modifies destination, can't modify source, new string
      sample = sample_substring_with_size(vowels, 1);
      s = add_suffix(s, sample); free(sample);
      len += 1; total += 1; s[len] = '\0';

      amount -= 1;
    }
    else if(amount - 2 == 0)
    {
      // SSWS : modifies destination, can't modify source, new string
      sample = sample_substring_with_size(vowels, 2);
      s = add_suffix(s, sample); free(sample);
      len += 2; total += 2; s[len] = '\0';

      amount -= 2;
    }

    temp = place_word_in_dict(temp, s);
    if(amount > 0 && amount != 1) --amount;
  }

  // post-process dict (pass utility::capitalize function as parameter)
  temp = prune_dict(temp, false);
  temp = map_dict(temp, capitalize);
  result = join_dict(temp, " ");

  close_dict(temp);
  free(s); s = NULL;

  return result;
}

char* wrd_rng(unsigned long min, unsigned long max, dict_t dict)
{
  dict_t temp = open_dict();
  U32 amount = motherr(min, max), total = total_dict(dict);
  char* sample = NULL; char* result = NULL;
  bool invalid = true;

  // add words to dict
  for(U32 i = 0; i != amount; ++i)
  {
    while(invalid)
    {
      sample = sample_dict(dict);

      // valid if no space, hyphen, or duplicate (latter depends on size)
      if(exclude(sample, " ") && exclude(sample, "-"))
      {
        if(amount > total) invalid = false;
        else if(dict_exclude(temp, sample)) invalid = false;
      }
    }
    
    temp = place_word_in_dict(temp, sample);
    invalid = true;
  }

  // post-process dict (pass utility::capitalize function as parameter)
  temp = map_dict(temp, capitalize);
  result = join_dict(temp, " ");

  close_dict(temp);

  return result;
}

char* snt_rng(unsigned long min, unsigned long max, dict_t dict)
{
  dict_t master = open_dict(), temp; drow_t selected;
  U32 word_amount = 0, last = 0, amount = motherr(min, max), now = 0, 
  len = 0, t_minus = 0; U8 comma = 0; I32 punct = 0;
  U32* no_dice = (U32*)malloc(sizeof(U32) * SMALL_BUFFER);
  char* sample = NULL; char* result = NULL; char* s = NULL;
  bool invalid = true;

  for(U32 i = 0; i != amount; ++i)
  {
    temp = open_dict();
    word_amount = motherr(SENTENCE_MIN_WORD_LENGTH, 
    SENTENCE_MAX_WORD_LENGTH);

    if(word_amount >= 2) comma = (U8)motherr(0, 1);

    // if comma, determine commma position after first word)
    if(word_amount >= 2 && comma == 1) comma = motherr(1, word_amount - 1);

    // determine sentence rhythm
    for(U32 j = 0; j != word_amount; ++j)
    {
      if(j == 0) now = motherr(5, 10);
      else if(j == word_amount - 1) now = motherr(3, 8);
      else if(t_minus > 0) { now = motherr(1, 10); --t_minus; }
      else if(last <= 10) now = motherr(1, dict.count - 1);
      else if(last > 10 || last <= 2) { now = motherr(6, 10); t_minus = 3; }

      selected = dict.drows[now];
      sample = sample_drow(selected);

      while(dict_include(temp, sample) && strlen(sample) != now)
      { sample = sample_dict(dict); }

      // add comma (if applicable)
      if(comma && j == comma - 1)
      {
        // get local copy of sample for modification
        len = strlen(sample);
        s = (char*)malloc(strlen(sample) + 1);

        strcpy(s, sample);
        s[len] = '\0';

        s = add_suffix(s, ",");
        temp = place_word_in_dict(temp, s);

        free(s);
      }
      else temp = place_word_in_dict(temp, sample);

      invalid = true;
      last = now;
    }

    // join temporary dict into a sentence; capitalize first word
    s = join_dict(temp, " ");
    s = capitalize(s);

    // determine punctuation; 1 - period, 2 - question, 3 - exclamation
    // based on a ratio of 64-21-15, sampled from Shakespeare's Hamlet
    punct = (I32)motherr(0, 99);

    if(punct >= 0 && punct <= 63) s = add_suffix(s, ".");
    else if(punct >= 64 && punct <= 84) s = add_suffix(s, "?");
    else if(punct >= 85 && punct <= 99) s = add_suffix(s, "!");

    // add sentence to master dict and cleanup
    master = place_word_in_dict(master, s);

    close_dict(temp);
    free(s);
  }

  result = join_dict(master, " ");
  close_dict(master);
  free(no_dice);

  return result;
}

char* pgf_rng(unsigned long min, unsigned long max, dict_t dict)
{
  char* result = NULL; char* sentences = NULL;
  U32 amount = motherr(min, max), sentence_amount = 0;
  dict_t master = open_dict();

  for(U32 i = 0; i != amount; ++i)
  {
    sentence_amount = motherr(PARAGRAPH_MIN_SENTENCE_LENGTH, 
    PARAGRAPH_MAX_SENTENCE_LENGTH);

    sentences = snt(sentence_amount, dict);
    master = place_word_in_dict(master, sentences);

    free(sentences);
  }

  result = join_dict(master, "\n\n");
  close_dict(master);

  return result;
}