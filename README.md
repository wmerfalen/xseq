# Overview
xseq attempts to be an extended form of the `seq` program from GNU's coreutils.
While seq mostly works with numbers, xseq can iterate over ranges of letters.
Some of seq's features are available in xseq.

# Motivation
seq is great. I often use it in bash scripts as an easy way to iterate over a range of numbers:
```sh
for num in $(seq 1 100); do
    echo "Fetching record ${num}..."
    curl -sSL "https://api.foobar.dev/api/v1/user/${num}" > ./users/"${num}.json"
done
```

But sometimes, it would be nice to have the same iteration over a range of letters:
```sh
for LETTER $(xseq A Z); do
    echo "Fetching users starting with letter: ${LETTER}..."
    curl -sSL "https://api.foobar.dev/api/v1/user?name_starts_with=${LETTER}" > ./users-"${LETTER}".json
done
```

Being woefully unsatisfied with invoking a scripting engine, I decided I'd just implement the features seq doesn't have.

#  Usage
```
Usage: xseq [OPTION]... <LAST>
 or:   xseq [OPTION]... <FIRST> <LAST>
 or:   xseq [OPTION]... <FIRST> <INCREMENT> <LAST>
Print numbers or letters from FIRST to LAST, in steps of INCREMENT.

  -a, --alpha       Treat FIRST and LAST as letters.
  -z, --zero        When LAST is the only argument, assume FIRST is zero
  -s, --separator=STRING   use STRING to separate numbers (default: \n)
  -h, --help        display this help and exit
  -v, --version     output version information and exit

 <INCREMENT> Works for both numbers and letters (when -a)

  If FIRST and LAST are letters, the range will be interpretted as if -a (or --alpha) was passed as an argument
  As a result, the following examples are all equivalent:
   xseq -a A Z
   xseq A Z
 When FIRST and LAST are letters, INCREMENT can be used without explicitly specifying -a or --alpha.
 The following invocation will skip print every 3rd letter between A and Z:
   xseq A 3 Z
```

# Looping over numbers
```sh
$ xseq 1 5
1
2
3
4
5
```

# automatic letter recognition
```sh
xseq A F
A
B
C
D
E
F
```

# INCREMENT works with letters and numbers
```sh
xseq A 3 M
A
D
G
J
M
```

# Automatic increment/decrement
```sh
xseq Z T
Z
Y
X
W
V
U
T
```

# Support for `-s STRING` and `--separator=STRING`
```sh
xseq -s , 0 10
0,1,2,3,4,5,6,7,8,9,10,

xseq --separator='}{' 0 10
0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}{
```
Admittedly, this output is a bit janky, but you get the idea.


# Building
You will need to have `g++` installed. A simple `make` will build the binary for you.
The code was written using widely supported C++ features. The code should compile just fine without having to tell your compiler which version of the c++ standard to use. Everything is written using well-established libraries. There are only three header files included.


