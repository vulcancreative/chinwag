[![Build Status](https://travis-ci.org/vulcancreative/chinwag.svg)](https://travis-ci.org/vulcancreative/chinwag)

# Chinwag

A command-line application with a C/Swift language compatible integration library, used for mocking up various textual elements via synthesis. It is ideally paired with an application requiring a visual element, where the final copy is still undetermined. Likewise, designers may use either the command-line tool or C library, in order to prototype visual interfaces. A library, such as this, is ideal for embedding into a number of existing C-based design programs and plugins, for either Adobe, Corel, or [insert your creative suite of choice here].

Chinwag also contains a small library for converting compatible token files into an instance of the included dictionary structure - dict\_t. A valid dict\_t is used for synthesis, allowing for proper randomization, organic sentence structure and syntax, and punctuation. Chinwag currently includes two embedded dictionaries - Seussian (the default) and Latin - where Seussian is based on the collective works of Theodor "Dr. Seuss" Geisel, and Latin is reminiscent of classic Lorem Ipsum output. New, custom dictionaries can easily be embedded using the included compile\_dict tool, in utilties.

Chinwag supersedes existing Lorem Ipsum algorithms in its ability to naturally emulate sentences, in terms of punctuation, overall length, and word length. Furthermore, it excels in terms of speed and presence - generating thousands paragraphs with ease, and being available in numerous, multi-platform languages (as open-source software). It is also embedded in the popular, GUI-based design utility - Babble.

== Download and Installation

The latest version of Chinwag can be obtained from its Github repo:

    % git clone https://github.com/vulcancreative/chinwag.git

Upon obtaining the latest version, one can compile and run the tests with ease, prior to installation:

    % make && make test # builds dictionaries and native library
    % make install

Running the installation commands will result in the following installs

    chinwag executable -> /usr/local/bin/
    include/chinwag.h -> /usr/local/include/
    include/chinwag -> /usr/local/include/
    include/dicts -> /usr/local/include/
    libchinwag.* -> /usr/local/lib/

== Caveats

While Chinwag and Chinwag Ruby are heavily tested in multiple platforms, it is a C-based application, which could prove a bit odd in unknown environments. That said, it is built to be quite solid and resilient, using age-old standards, and hiccups should be far and few between (if at all).

The only other thing worth mentioning is the output cap. Chinwag is hard-wired to only be able to output up to 10,000 increments of the desired output type at a time. While this cannot be changed without modifying the source directly, it really is for the greater good - 10,000 :letters or :words takes only a mere fraction of a second, but 10,000 sentences can take ~3-5 seconds and 10,000 :paragraphs can take up to ~10 seconds on slower machines. That said, it's usually much faster to simply run the generate method a few times over, if a vast amount of placeholder text is needed, and concatenate the resulting String instances. (Bear in mind, the average novella has roughly 17,000 to 40,000 words, with novels having slightly more.)

== License

Chinwag, and its various language flavors, are released under the MIT license:

  http://www.opensource.org/licenses/MIT

== Contributing

Currently looking for help with i18n. If you have experience working with cross-platform Unicode strings in C, you are encouraged to contribute.
