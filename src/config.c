#include "../include/chinwag/config.h"

const unsigned MAJOR_VERSION = 1;
const unsigned MINOR_VERSION = 2;
const unsigned PATCH_VERSION = 0;

const char* const DATE_YEAR = "2015";
const char* const DATE_MONTH = "01";
const char* const DATE_DAY = "03";
const char* const REVISION = "914";

const unsigned SMALL_BUFFER = 1024;
const unsigned LARGE_BUFFER = 5120;

const unsigned MIN_DICT_SIZE = 300;

const char* const DELIMITERS = "\r\n,;:\034";

const unsigned CW_DEFAULT = 1; // == WORDS

const unsigned DEFAULT_MIN_OUTPUT_LENGTH = 1;
const unsigned DEFAULT_MAX_OUTPUT_LENGTH = 5;

const unsigned SENTENCE_MIN_WORD_LENGTH = 2;
const unsigned SENTENCE_MAX_WORD_LENGTH = 25;

const unsigned PARAGRAPH_MIN_SENTENCE_LENGTH = 4;
const unsigned PARAGRAPH_MAX_SENTENCE_LENGTH = 6;
