#include "pipe.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

const char* ENV_PIPE_PATH = "PIPE_PATH";
static int pipe_fd = -1;

void PrintErrno(int err)
{
  fprintf(stderr, "Error (%d): %s", err, strerror(err));
}

void HandlerSIGPIPE(int sig)
{
  
}

/// @details Makes the named pipe if it does not already exist and sets the static file descriptor
///          variable. The pipe path is specified by the environment variable $PIPE_PATH.
void ConstructPipe()
{
  static bool call_once;
  if(!call_once)
  {
    const char* pipe_path = getenv(ENV_PIPE_PATH);
    if(mkfifo(pipe_path, 0666))
    {
      int err = errno;
      if(EEXIST != err)
      {
        PrintErrno(err);
        exit(EXIT_FAILURE);
      }
    }

    pipe_fd = open(pipe_path, O_WRONLY);
    if(-1 == pipe_fd)
    {
      PrintErrno(errno);
      exit(EXIT_FAILURE);
    }
    // Set up a handler for SIGPIPE in case we get this error when writing to the pipe.
    signal(SIGPIPE, HandlerSIGPIPE);
    call_once = true;
  }
}

/// @details Writes a single byte to the named pipe. The purpose is to notify the listener that
///          something has happened.
void WriteBytePipe()
{
  if(-1 != pipe_fd)
  {
    uint8_t b = 1;
    ssize_t size = write(pipe_fd, (void*)(&b), sizeof(b));
    if(-1 == size)
    {
      PrintErrno(errno);
    }
  }
}

/// @details Closes the named pipe with the file descriptor.
void DestroyPipe()
{
  close(pipe_fd);
}