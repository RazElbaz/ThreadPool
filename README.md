# ThreadPool
## written by ✨

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/RazElbaz"><br /><sub><b>Raz Elbaz</b></sub></a><br /> </td>
  </tr>
</table>

</table>

# Introduction:
  
The goal is to implement a threadpool mechanism, and implement syncronization.


# The task in details:   
All the files given to me are in the '''threadPoolTask''' folder.
I was given a shared library (SO), compiled for x86, with two functions: "encode" and "decode".
A simple basic_main is also included to demonstrate the functionality of the library.
Since the encryption algorithm is done by a novice student, it will take 5ms for each char, which is not
Capable of handling more than 1k (1024) bytes of data. More than that will be ignored.
I had to implement a CMD TOOL that would use the algorithm above.
By the cmd tool we mean, that my executable will receive its work data from stdIn, and put the output to stdOut.

As an option of stdIn reading, see the stdin_main example. Use it like this:  
cat readme.txt | ./tester 2  
where readme.txt is a data file (or some stream)  
./tester is the compiled name of the stdin example  
2 is the encription key  
Your tools will get some input on sdtIn, and write the encrypted/decrypted data to stdOut.  
“-e” and “-d” flags will be used for encryption and decryption accordingly.  
Usage example:  
coder key -e < my_original_file > encripted_file  
coder key -d < my_decripter_file > my_original_file  
or  
pipe | coder key -e > encripted_file  
cat encripted_file | coder key -d > your_original_file  
Note that ‘<’, ’|’ and ‘>’ are stand alone shell command for redirecting input/output/piping  
It’s expected that you use the most of the cpu available on the machine.  
