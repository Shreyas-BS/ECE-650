#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib>
#include <string.h>
#include "optionParser.h"

// Function to execute a program with given arguments
void executeProgram(const char *program, char *const args[])
{
    execvp(program, args);
    std::cerr << "Error: Executing " << program << std::endl;
    exit(1);
}

// Function to create a child process and return its PID
pid_t createChildProcess(const char *program, char *const args[], const int *readEnd, const int *writeEnd)
{
    pid_t childProcess = fork();
    if (childProcess == 0)
    {
        // Child process
        if (readEnd)
        {
            close(readEnd[1]);   // Close write end
            dup2(readEnd[0], 0); // Redirect stdin from the read end
        }
        if (writeEnd)
        {
            close(writeEnd[0]);   // Close read end
            dup2(writeEnd[1], 1); // Redirect stdout to the write end
        }
        executeProgram(program, args); // Execute the program
    }
    else if (childProcess < 0)
    {
        std::cerr << "Error: Invoking " << program << " fork()" << std::endl;
        exit(1);
    }
    return childProcess;
}

int main(int argc, char **argv)
{
    // vector<pid_t> masterpro;
    Options options = parseOptions(argc, argv);
    int status;

    char *rgenarg[] = {
        (char *)"./rgen",
        (char *)"-s",
        (char *)strdup(std::to_string(options.sValue).c_str()),
        (char *)"-n",
        (char *)strdup(std::to_string(options.nValue).c_str()),
        (char *)"-l",
        (char *)strdup(std::to_string(options.lValue).c_str()),
        (char *)"-c",
        (char *)strdup(std::to_string(options.cValue).c_str()),
        nullptr};

    char *pyarg[] = {
        (char *)"/usr/bin/python3",
        (char *)"./ece650-a1.py",
        nullptr
    };

    //***1.Parent process creating the pipe
    int cppToPyPipe[2];
    int pyToCppPipe[2];

    std::vector<pid_t> processList;
    // pid_t cprocess, pyprocess, c2process;

    if (pipe(cppToPyPipe) == -1 || pipe(pyToCppPipe) == -1)
    {
        std::cerr << "Error creating pipes." << std::endl;
        return 1;
    }

    // Create 1st child process
    processList.push_back(createChildProcess("./rgen", rgenarg, nullptr, cppToPyPipe));

    // Create 2nd child process
    processList.push_back(createChildProcess("/usr/bin/python3", pyarg, cppToPyPipe, pyToCppPipe));

    // Create 3rd child process
    processList.push_back(createChildProcess("./ece650-a2", nullptr, pyToCppPipe, nullptr));

    // masterpro.push_back(c2process);
    dup2(pyToCppPipe[1], 1);

    while (true)
    {
        std::string input;
        std::getline(std::cin, input);
        if (std::cin.eof())
        {
            break;
        }
        if (input.size() > 0)
            std::cout << input << std::endl;
        else
            std::cerr << "Input character length insufficient.\n";
    }

    close(cppToPyPipe[1]);
    close(pyToCppPipe[1]);


    for (pid_t childprocess : processList)
    {
        kill(childprocess, SIGTERM);
        waitpid(childprocess, &status, 0);
    }

    return 0;
}