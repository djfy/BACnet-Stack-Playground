#ifndef PIPE_H
#define PIPE_H

/// @brief Creates and opens a named pipe.
void ConstructPipe();

/// @brief Writes a byte to the named pipe.
void WriteBytePipe();

/// @brief Closes the named pipe created by ConstructPipe.
void DestroyPipe();

#endif // PIPE_H
