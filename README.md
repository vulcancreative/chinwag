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
# also installs the command-line application
# will be available via Homebrew and APT shortly!

$ git clone https://github.com/vulcancreative/chinwag.git
$ cd chinwag
$ make && sudo make install
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
seuss: {
	name: "Seussian",
	// is a function, not a property of the struct
	cwdict_length(): 1096,
	// is a function, not a property of the struct
	cwdict_valid(): true,
	sorted: true,
	_: [
		[I, a], [TV, am, an, as, at, be, ...
		[Mordecai Ali Van Allen O'Shea]
	]
}

latin: {
	name: "Latin",
	// is a function, not a property of the struct
	cwdict_length(): 35664,
	// is a function, not a property of the struct
	cwdict_valid(): true,
	sorted: true
	_: [
		[a, b, c, d, e, f, k, l, m, n, o, ...
		semicircumferentia, supersubstantialis, supertriparticular]
	]
}
```


### Opening a Custom Dictionary

If you need a valid, custom dictionary to test against, we recommend our <a href="http://vulcanca.com/babble/docs/noise.dict" target="_blank">Noise dictionary</a>. It has several thousand entries, and will have no problem passing any and all internal validation procedures.

```c
// EXAMPLE IN
#include <stdio.h>
#include <stdlib.h>
#include <chinwag.h>
char *result = NULL, *tokens = NULL;
cwdict_t noise = cwdict_open();
cwerror_t e;

FILE* fi = fopen("noise.dict", "r");

if(fi)
{
  stringify_file(&tokens, fi); fclose(fi);
  noise = cwdict_open_with_name_and_tokens("noise", tokens, CW_DELIMITERS);
  free(tokens); cwdict_close(noise);
}
else
{
	// handle IO error...
}
```

```sample
// EXAMPLE OUT
noise: {
	name: "noise",
	// is a function, not a property of the struct
	cwdict_length(): 18957,
	// is a function, not a property of the struct
	cwdict_valid(): true,
	sorted: true,
	_: [
		[g, s, u, z, l, h, i, a, m, v, o, q, ...
		pzhvbzvnsdozcuxpgldrwylvedosnbbktoyi]
	]
}
```


> Note : loading a custom dictionary does invoke quite a bit of IO overhead. It is best practice to load a dictionary and cache it for the entirety of its use cycle (often in a global variable).


### Opening a Blank Dictionary

While having a blank dictionary is not particularly useful, you can append to it after the fact, gradually building a functional dictionary. Blank, unnamed dictionaries have no internal heap allocations, when first initialized.

```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t blank = cwdict_open();
```

```sample
// EXAMPLE OUT
blank: {
	name: NULL,
	// is a function, not a property of the struct
	cwdict_length(): 0,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_: []
}
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
[[I, a], [TV, am, an, as, at, be, ...
[Dibble Dibble Dibble Dribble], [Mordecai Ali Van Allen O'Shea]]
```


### Dictionary Arithmetic

Whether using an embedded dictionary, or something custom, you can concatenate new entries in the form of strings. This is particularly useful if you have a blank dictionary, and gradually want to build upon it by adding in information dynamically.

```c
// EXAMPLE IN
#include <chinwag.h>
const char* const g[] = {"these", "are", "some", "test", "words"};
const char* const u[] = {"these", "words", "will", "be", "sorted"};
cwdict_t grouped = cwdict_open();
cwdict_t ungrouped = cwdict_open();
grouped = cwdict_place_words(grouped, g, 5);
ungrouped = cwdict_place_words_strict(ungrouped, u, 5);
```

```sample
// EXAMPLE OUT
ungrouped: {
	name: NULL,
	// is a function, not a property of the struct
	cwdict_length(): 1,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_: [
		[test]
	]
}

grouped: {
	name: NULL,
	// is a function, not a property of the struct
	cwdict_length(): 2,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_: [
		[quick], [test]
	]
}
```


### Sorting and Pruning

While generation requires a dictionary to be sorted by length, it is also best-practice to prune your dictionary of repeat elements. Cleaning both sorts and prunes.

```c
// EXAMPLE IN
#include <chinwag.h>
const char* const s[] = {"this", "is", "a", "quick", "test"};
const char* const p[] = {"something", "something", "another", "done"};
const char* const c[] = {"first", "second", "first", "second", "third"};
cwdict_t sorted = cwdict_open_with_name("Sorted");
cwdict_t pruned = cwdict_open_with_name("Pruned");
cwdict_t cleaned = cwdict_open_with_name("Cleaned");
sorted = cwdict_place_words_strict(sorted, s, 5);
pruned = cwdict_place_words_strict(pruned, p, 4);
cleaned = cwdict_place_words_strict(cleaned, c, 5);
sorted = cwdict_sort(sorted);
// orders by entry length,
// meeting generation requirements
pruned = cwdict_prune(pruned, false, true);
// removes duplicates, retains placement
// needs to be sorted before generating
cleaned = cwdict_clean(cleaned);
// removes duplicates and sorts,
// meeting generation requirements
```

```sample
// EXAMPLE OUT
sorted: {
	name: "Sorted",
	// is a function, not a property of the struct
	cwdict_length(): 5,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: true,
	_ : [
		[a], [is], [test, this], [quick]
	]
}

pruned: {
	name: "Pruned",
	// is a function, not a property of the struct
	cwdict_length(): 3,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_ : [
		[something], [another], [done]
	]
}

cleaned: {
	name: "Cleaned",
	// is a function, not a property of the struct
	cwdict_length(): 3,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: true,
	_: [
		[first, third], [second]
	]
}
```


### Duplication

As dictionaries are rooted as complex structs in C99, and require a variety of resources to initialize and close, duplication is a slightly complex procedure.

Nevertheless, we allow deep copies, via the library. Duplication will respect any sorting or pruning that has been done previously to the dictionary being copied, and will have a new address in memory.

```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t seuss = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
cwdict_t copy = cwdict_clone(seuss);
seuss = cwdict_close(seuss);
```

```sample
// EXAMPLE OUT
seuss: {
	name: "",
	// is a function, not a property of the struct
	cwdict_length(): 0,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_ : []
}

copy: {
	name: "Seussian",
	// is a function, not a property of the struct
	cwdict_length(): 1096,
	// is a function, not a property of the struct
	cwdict_valid(): true,
	sorted(): true,
	_: [
		[I, a], [TV, am, an, as, at, be, ...
		[Mordecai Ali Van Allen O'Shea]
	]
}
```


### In-Place Modification





### Closing a Dictionary

By default, when closing a dictionary, a blank dictionary is returned. This value can be ignored, if desired.

```c
// EXAMPLE IN
#include <chinwag.h>
cwdict_t seuss = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
cwdict_t latin = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
cwdict_close(latin);
cwdict_t blank = cwdict_close(seuss);
// Clears all of seuss' internal, dynamic memory,
// and resets it to a blank dictionary, which
// you are free to capture
```

```sample
// EXAMPLE OUT
seuss: {
	name: NULL,
	// is a function, not a property of the struct
	cwdict_length(): 0,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_: []
}

latin: {
	name: NULL,
	// is a function, not a property of the struct
	cwdict_length(): 0,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_: []
}

blank: {
	name: NULL,
	// is a function, not a property of the struct
	cwdict_length(): 0,
	// is a function, not a property of the struct
	cwdict_valid(): false,
	sorted: false,
	_: []
}
```

## Validation and Errors


Upon loading a foreign dictionary, it is crucial to test its validity, prior to use. This checks that the library will be capable of understanding the dictionary format properly, and, if so, ensures adequate randomization for our synthesis algorithms.

Embedded dictionaries have already been thoroughly tested, and need no further validation. This, in turn, grants the embedded resources an additional speed boost.

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


With a valid dictionary in-hand, generating output is an incredibly easy task. One needs to simply specify the `output type` and `output amount(s)`, passing the dictionary reference as an argument, and the library will handle the rest.

It is possible to allow for generation using only the defaults, and, subsequently, modifying the defaults, to allow for succinct operation.

```c
// EXAMPLE IN
#include <stdio.h>
#include <stdlib.h>
#include <chinwag.h>
cwerror_t err;
cwdict_t seuss = cwdict_open_with_name_and_tokens("Seussian",
dict_seuss, CW_DELIMITERS);
char* output = chinwag(CW_WORDS, 10, 20, seuss, &err);
puts(output); free(output);
// Prints ten to twenty words in Seussian
cw_default_type = CW_LETTERS;
cw_default_max_output = 30;
// Max must be set first if higher than min
cw_default_min_output = 30;
output = chinwag_defaults(seuss, &err);
puts(output); free(output); cwdict_close(seuss);
// Prints thirty letters using defaults
```

```sample
// EXAMPLE OUT
A With Monkeys Everywhere I Comes Stew Mostly Lasso Shout
Confused Congratulations When Blackbottomed

Wonderfully Her Amounts Feetae
```

## Legal


Chinwag is available under the <a href="http://opensource.org/licenses/MIT" target="_blank">MIT License</a>.<br>
Use, abuse, and please don't bite the hand that feeds you.
