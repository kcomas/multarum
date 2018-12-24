
# Multarum 0.0 SPEC

## Data Types

### Int
### Float

#### Operations for Int And Float
Operations cannot be done between numeric types, must be casted first

= Equal

< Less then

<= Less then equal

\> Greater then

\>= Greater then equal

!= Not Equal

\+ Addition

\- Subtraction

\* Multiplication

/ Division

% Remainder

^ Exponent

###Str
"some words"

A chunked list of fixed buffers, utf-8

#### Operations
\#+ Concat

\#> Reverse

\#/ Split

\#r/body/flags Regex

### Vec
@[1, 2...]

Vector of values

#### Indexing
vec[number of var index]

#### Operations
@+ Concat

@< vec @< var: push, var @< vec pop

@> Reverse

@^ Raise

@l Length

@/ Split

@s Sort, sorting is stable

@m Map function of 3 args (value, index, original vec) @m vec

@f Filter fn of 3 args (value, index, original @m vec

@r Reduce fn of 4 args (amalgamation, value, index, original vec)

@d Delete, 1 @d vec deletes first element ((@l vec) - 1) @d vec, delete last element

@e Each, fn of 3 args (value, index, original) @e vec

@z Zip keys @z values, creates a new hash

### Hash
%[key:value,...]
Hashmap of key value pairs

#### Accessing
hash["key" or var string]

#### Operations
%+ Concat

%l Length

%k Keys as vec

%v Values as vec

%e Entries vec of 2 entry vec of key values

### Fn
(arg1,...) { arg1 + 1 }
Functions do not have access to the parent scope, this is done for threading

The ending bracket must be on the same line as the last statemet

#### Try Catch
?fn(args) to safe call a function this will return the error as a value

#### Self Calling
Due to the lack of parent scope use $ to call the current function

### Fd
Any file descriptor

#### Operations
&o Open

&c Close

&< Read

&> Write

&l Length

### Err
Errors are just strings tagged as an error

#### Operations
!! Throw Error, !! "Something Went Wrong" produces and new error

## Control Flow

### Assignment
:

Assign a var a value, myvalue: 1

### If
? { statement { body }.... { default } }

If statements are blocks of a comparison followed by a body

The last body without a comparison is the default else
