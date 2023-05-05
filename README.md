
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
  
The goal of this tool is to implement a threadpool mechanism and synchronization to improve the performance of an encryption algorithm. The tool takes advantage of multi-threading to utilize multiple cores and process the encryption or decryption tasks concurrently.

# CMD Syntax
Compile files first with ```make``` or ```make all``` in terminal.

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
**Example 1:**  
 ```./coder 2 -e < myfile.txt > encripted_file.txt```    
 ```./coder 2 -d < encripted_file.txt  > test.txt ```  
 ```cat encripted_file.txt | ./coder 2 -d > your_original_file.txt```  
   
  
Note: Replace 2 with the actual encryption key.  

**Example 2:**   
```./coder 20 -e < myfile.txt > encripted_file.txt```  
```./coder 20 -d < encripted_file.txt  > check.txt```  
```cat encripted_file.txt | ./coder 20 -d > your_original_file.txt```  


**Example 3:**  
 Encrypting the file and displaying the encrypted data on the console:     
 ```./coder 5 -e < myfile.txt```  
  Encrypting the file and saving the encrypted data to a file  
```./coder 5 -e < myfile.txt > encrypted_file.txt```  
Decrypting the encrypted file and displaying the decrypted data on the console  
```./coder 5 -d < encrypted_file.txt```  
Decrypting the encrypted file and saving the decrypted data to a file  
```./coder 5 -d < encrypted_file.txt > decrypted_file.txt```  

**Example 4:**  
Encrypting input from a pipe and saving the encrypted data to a file    
 ```echo "Hello, World!" | ./coder 3 -e > encrypted.txt```  
Decrypting input from a file and displaying the decrypted data on the console  
```./coder 3 -d < encrypted.txt```

**Example 5:**   
Encrypting input from a file and piping the encrypted data to another command  
```cat myfile.txt | ./coder 7 -e | gzip > encrypted.gz```  
Decrypting input from a compressed file and saving the decrypted data to a file    
```gunzip -c encrypted.gz | ./coder 7 -d > decrypted.txt```  


Please note that the encryption algorithm provided is for demonstration purposes only and may not be suitable for secure encryption requirements.

# Here's an overview of the files and their roles:

```libCodec.so```: This is a shared library (SO) compiled for x86 architecture. It contains two functions, "encode" and "decode", which are likely used for encryption and decryption operations.

```threadpool.c``` and ```threadpool.h```: These files contain the implementation and declarations of the threadpool mechanism. The threadpool is responsible for managing a pool of threads and assigning tasks to them in a parallel manner.

```task.c``` and ```task.h```: These may define the structure and functions related to tasks that can be executed by the threadpool. Tasks represent units of work that need to be performed in parallel.

```semaphore.c``` and ```semaphore.h```: These files provide the implementation and declarations of synchronization mechanisms, such as semaphores. These mechanisms are used to coordinate the execution of threads and ensure proper synchronization of shared resources.

```main.c```: This file may contain the main function of the CMD TOOL. It is responsible for parsing command-line arguments, setting up the encryption or decryption process, and utilizing the threadpool mechanism to parallelize the encryption/decryption operations.

```main.h``` header file defines the necessary data structures, function prototypes, and global variables to be used in the main.c file, which implements the logic for the CMD TOOL, including reading input, setting up encryption or decryption, utilizing the threadpool, and handling the output.

`myfile.txt` file with 1385 original characters
`encrypted_file.txt` Encrypted file of `myfile.txt` with key=2
`Tortoise_Hare.tx`t file with a story to further test
`test.txt` Encrypted file of  `Tortoise_Hare.txt` with key=2

# Features
Encryption and Decryption: The tool supports both encryption and decryption of data. You can specify the encryption or decryption mode using command-line flags.

Multi-threading: The tool utilizes a threadpool mechanism to distribute the encryption or decryption tasks across multiple threads. This allows for concurrent processing and improved performance on multi-core systems.

Input and Output Flexibility: The tool supports input from various sources, including file input, piping from other commands, and standard input. Similarly, the output can be redirected to files or piped to other commands for further processing.

Command-Line Interface: The tool provides a command-line interface with intuitive syntax and options for specifying encryption keys, input sources, and output destinations.

Error Handling: Robust error handling mechanisms are implemented to handle various scenarios, such as invalid input, file read/write errors, and memory allocation failures. Clear and informative error messages are provided to assist users in troubleshooting.

# Additional Information
To achieve optimal performance, make sure your system has multiple CPU cores available. The tool is designed to utilize multi-threading and distribute the encryption/decryption tasks across multiple threads, thereby maximizing CPU utilization.

# Testing - Files: 

 `cat Tortoise_Hare.txt | ./coder 2 -e > test.txt` :

![Encryption](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/Encryption.png)

 `cat test.txt | ./coder 2 -d > Tortoise_Hare.txt` :

![Decryption](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/Decryption.png)

 `cat Tortoise_Hare.txt ` :

![cat](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/cat.png)

 **check** :

![check](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/check.png)

# Testing - socket: 

The `nc -l 9090` command in one terminal and the `netstat -tuln` command in another terminal 

![connect](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/connect.png)

`./coder 2 -e < Tortoise_Hare.txt | nc localhost 9090`

![socket-dec](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/socket-enc.png)

`./coder 2 -d < test.txt | nc localhost 9090`

![socket-dec](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/socket-dec.png)

# Testing - pipe: 

`cat myfile.txt | ./coder 2 -e > encripted_file.txt`

![piping](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/piping.png)

`cat encripted_file.txt | ./coder 2 -d > your_original_file.txt`

![pipe](https://github.com/RazElbaz/ThreadPool/blob/main/pictures/pipe.png)
