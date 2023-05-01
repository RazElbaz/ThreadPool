
# Threaded Encryption/Decryption Tool
This is a CMD tool for encrypting and decrypting data using a provided encryption algorithm. The tool utilizes multi-threading to parallelize the encryption/decryption process and improve performance on multi-core systems.

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

# CMD Syntax
The tool can be used with the following syntax:

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

# Input/Output
The tool reads input data from stdin and writes the encrypted/decrypted data to stdout. You can use shell redirection and piping to provide input data and capture the output.

# Performance Considerations
The encryption algorithm provided in the shared library is not optimized and takes approximately 5 milliseconds to process each character. To maximize CPU utilization on multi-core systems, the tool utilizes a threadpool mechanism for parallel processing.

# Task Details   

All the required files for this task can be found in the ```threadPoolTask``` folder. The provided shared library (SO) includes two functions: encode and decode. Additionally, a simple basic_main is included to demonstrate the functionality of the library. The encryption algorithm is not optimized and can handle a maximum of 1k (1024) bytes of data. Anything beyond that will be ignored.

The goal of this task was to implement a CMD TOOL that uses the provided encryption algorithm. The tool reads input data from stdin and writes the encrypted/decrypted data to stdout. The -e and -d flags are used for encryption and decryption, respectively


My tools will take input in sdtIn and write the encrypted/decrypted data to stdOut.
"-e" and "-d" flags will be used for encryption and decryption accordingly.

# Example of use:
 ```./coder 2 -e < myfile.txt > encripted_file.txt```   
 ```./coder 2 -d < encripted_file.txt  > test.txt ```  
```./coder 2 -e < myfile.txt > encripted_file.txt```  
```./coder 2 -d < encripted_file.txt  > check.txt```  
```cat encripted_file.txt | ./coder 2 -d > your_original_file.txt```



