# printf_test
A simple main file to test printf.
This is the main file I have been using to test my printf project.

### How to use it?
compile the main with the provided libft.a and your project c files.
```console
cc -Wall -Wextra -Werror main.c libft.a <your files or library>
```
execute it by providing 2 arguments: 
* first is made of the specifiers you want to test (among `c` `s` `d`/`i` `u` `x` `X` `p` `%` + `m` for other tests)
* second can be one of
  * `short` for a test summary
  * `detail` for detailed output comparison with original printf.
  * `all` for both
#### Examples:
```console
./a.out c detail
./a.out csx all
./a.out % short
```
### Notes
Only covers the mandatory part (no bonus).

No guarantee that the test results are accurate.

Any suggestion is welcome to improve the code or the tests.
