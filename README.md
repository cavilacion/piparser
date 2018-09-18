
# PI Parser 

## WARNING!!!
The software is extermeley unsafe to use, and memory errors are likely to happen! The provided inputs are safe to use, but precaution is adviced!

## Compiling
To compile the example, navigate to the src directory and enter the following command:
```
$ make trios
```
## Model checking
Executing the command 
```
./trios 1.in
```
will parse the content of the file 1.in
```
(new x y) x<true> . y(a) . 0 | y(a) . ( 0 | 0 )
```
