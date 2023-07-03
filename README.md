# LoadBalancer
A multi-process system for calculating the md5 hash of files in parallel, using the Unix operating system as a basis.
## Binaries and Execution
To compile the project, a Makefile type file will be used. For this, the user must open a terminal at the root of the project, where they must do 2 commands.

```sh
make clean
make all
```

Additionally, you can perform an extra validation, running the PVS Studio, which is executed with the following command.

```sh
make check
```

Then, in the root directory of the project there will be 3 executables, which work together to perform the task.

- APPLICATION: It receives as a parameter in command line the files to be processed. It will manage the slave processes.
- SLAVE: It performs the md5 hashing of a series of files received by standard input from the parent process (APPLICATION).
- VIEW: It receives by command line parameter or by standard input (Prioritizing the first of both options) the number of files that are going to be processed. It will be in charge of printing the results of the md5 hash with the following format "(pid) (hash code) (filePath)".
  To be able to run the project, it can be done in one of two ways. The first option is by a pipe via command line, where the application process is first called with the respective parameters, followed by the view process.

```sh
./APPLICATION <file1> <file2> ... <fileN> | ./VIEW
```

Alternatively, you can run using 2 different terminals, where the APPLICATION process is first run and then the VIEW.

```sh
-- Terminal 1 --
 ./APPLICATION <file1> <file2> ... <fileN>
```

```sh
-- Terminal 2 --
 ./VIEW <numberOfFiles>
```
