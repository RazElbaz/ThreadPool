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

# The CMD syntax:

```coder key -e < input_file > encrypted_file```  
```coder key -d < encrypted_file > output_file```  
```pipe | coder key -e > encrypted_file```  
```cat encrypted_file | coder key -d > output_file```  

**Here's an explanation of the different components:**

```coder```: The name of the executable program.  
```key```: A command-line argument representing the encryption or decryption key.  
```-e``` and ```-d```: Flags to indicate whether the program should perform encryption (-e) or decryption (-d).  
```< input_file```: Input redirection from a file. The contents of input_file will be provided as input to the program.  
```> encrypted_file```: Output redirection to a file. The encrypted data will be written to encrypted_file.  
```> output_file```: Output redirection to a file. The decrypted data will be written to output_file.  
```pipe |```: Input piped from another command. The output of the preceding command will be used as input for the encryption process.  

To use the program, you would replace key with the actual encryption/decryption key and specify the appropriate files for input, output, and piping, depending on your specific use case.


# The task in details:   
All the files given to me are in the ```threadPoolTask``` folder.  
I was given a shared library (SO), compiled for x86, with two functions: "encode" and "decode".
A simple basic_main is also included to demonstrate the functionality of the library.
Since the encryption algorithm is done by a novice student, it will take 5ms for each char, which is not
Capable of handling more than 1k (1024) bytes of data. More than that will be ignored.
I had to implement a CMD TOOL that would use the algorithm above.
By the cmd tool we mean, that my executable will receive its work data from stdIn, and put the output to stdOut.


My tools will take input in sdtIn and write the encrypted/decrypted data to stdOut.
"-e" and "-d" flags will be used for encryption and decryption accordingly.

# Example of use:
 ```./coder 2 -e < myfile.txt > encripted_file.txt```   
 ```./coder 2 -d < encripted_file.txt  > test.txt ```  
```./coder 2 -e < myfile.txt > encripted_file.txt```  
```./coder 2 -d < encripted_file.txt  > check.txt```  
```cat encripted_file.txt | ./coder 2 -d > your_original_file.txt```

