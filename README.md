## Introduction


Chinwag, other than being a funny word, is an open-source, developer toolset used for text-synthesis. The goal is to allow for developers and designers (with a little programming experience) to be able to rapidly prototype text-heavy interfaces, but in a fun manner.

It is primarily written in C99 (for the sake of speed and portability), but has many official language bindings, covering C, Ruby, Python, Swift, and Go.

The library universally features:

* Powerful dictionary type with accompanying routines
* Several embedded dictionaries
* Easy custom dictionary creation and use
* Easy output configuration
* C99-based for speed and portability
* Zero dependencies

## Installation


```shell
git clone https://github.com/vulcancreative/chinwag.git
cd chinwag
make && make install
```

## Versioning


When we make releases to the API, we strive for consistency across all of the various, language-flavors. Meaning &ndash; when we release an update to the core Chinwag API (in C99), we update all sister components. This should guarantee a consistent version release number across all equivalent libraries.

```c
// EXAMPLE IN
#include <chinwag.h>
puts(CW_VERSION);
// C doesn't support modules, hence a global constant
```

```sample
// EXAMPLE OUT
1.2.3
```
## Dictionaries


To generate output, you need to open a dictionary object. The dictionary can be blank, pulled from a custom token file, or loaded from one of Chinwag's embedded options &ndash; `Seussian` or `Latin`.


### Opening an Embedded Dictionary


```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t seuss = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
// Creates a named dictionary (recommended)
cwdict_t latin = cwdict_open_with_tokens(dict_latin, CW_DELIMITERS);
// Creates an unnamed dictionary
```

```sample
// EXAMPLE OUT
```


### Opening a Custom Dictionary

If you need a valid, custom dictionary to test against, we recommend our <a href="http://vulcanca.com/babble/docs/noise.dict" target="_blank">Noise dictionary</a>. It has several thousand entries, and will have no problem passing any and all internal validation procedures.


### Opening a Blank Dictionary

While having a blank dictionary is not particularly useful, you can append to it after the fact, gradually building a functional dictionary. Blank, unnamed dictionaries have no internal heap allocations, when first initialized.

```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t blank = cwdict_open();
```

```sample
// EXAMPLE OUT
```


### Examining Dictionaries

If there is ever a reason you need to visually debug a dictionary, each of our libraries supports a visualization component. This forces the dictionary instance to spill its guts, via the command-line.

```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t seuss = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
puts_cwdict(seuss);
```

```sample
// EXAMPLE OUT
```


### Dictionary Arithmetic


```c
// EXAMPLE IN
#include <stdlib.h>
#include <chinwag.h>
char** words = (char**)malloc(sizeof(char*) * 2);
words[0] = (char*)malloc(20); // setup first buffer
words[1] = (char*)malloc(20); // setup second buffer
strcpy(words[0], "quick"); strcpy(words[1], "test");
words[0][6] = '\0'; words[1][5] = '\0';
cwdict_t blank = cwdict_open();
blank = cwdict_place_word_strict(blank, "test");
blank = cwdict_place_words_strict(blank, words, 2);
// Performs a deep copy, so words buffer can
// be closed, as they are no longer needed
```

```sample
// EXAMPLE OUT
```


### Sorting and Pruning





### Duplication

As dictionaries are rooted as complex structs in C99, and require a variety of resources to initialize and close, duplication is a slightly complex procedure.

Nevertheless, we allow deep copies, via the library. Duplication will respect any sorting or pruning that has been done previously to the dictionary being copied, and will have a new address in memory.

```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t seuss = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
cwdict_t copy = cwdict_clone(seuss);
```

```sample
// EXAMPLE OUT
```


### In-Place Modification





### Closing a Dictionary


```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t blank = cwdict_close(seuss);
// Clears all of seuss' internal, dynamic memory,
// and resets it to a blank dictionary, which
// you are free to capture
```

```sample
// EXAMPLE OUT
```

## Validation and Errors



```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_e error;
cwdict_t blank = cwdict_open_with_name("Blank");

if(!cwdict_valid(blank, &error))
{
	switch(error)
	{
		case CWERROR_DICT_TOO_SMALL:
			// The dictionary had too few valid entries
			cwerror_warn(blank, error);
		case CWERROR_DICT_UNSORTABLE:
			// The dictionary couldn't sort entries by length
			cwerror_warn(blank, error);
		case default:
			// You received another error, unrelated to the library
			cwerror_fail(blank, error);
	}
}
```

```sample
// EXAMPLE OUT
CWERROR_DICT_TOO_SMALL : too few acceptable entries (0 of 300)
```

## Generation



```c
// EXAMPLE IN
#include <stdio.h>
#include <stdlib.h>
#include <chinwag.h>
cwdict_t seuss = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
char* output = chinwag(CW_WORDS, 10, 20, seuss); free(output);
// Generates ten to twenty words in Seussian
```

```sample
// EXAMPLE OUT
```

## Legal


Chinwag is available under the <a href="http://opensource.org/licenses/MIT" target="_blank">MIT License</a>.<br>
Use, abuse, and please don't bite the hand that feeds you.
