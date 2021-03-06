#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/process.h"
#include "threads/synch.h"

//Process Identifier ID
typedef int pid_t;

//lock for File-System allowing one thread access per time
struct lock filesystem_lock;

//list of files opened by users-progs
struct list useropened_files;

//file descriptor object
struct file_descriptor
{
  int descriptor_id;
  tid_t owner_id;
  struct file *file;
  struct list_elem element;
};

void
syscall_init (void);

//user implemented methods
void
syscall_halt (void);
void
syscall_exit (int status);
pid_t
syscall_exec (const char* cmd_line);
int
syscall_wait (pid_t pid);
bool
syscall_create (const char* file, unsigned initial_size);
bool
syscall_remove (const char* file);
int
syscall_open (const char* file);
int
syscall_filesize (int fd);
int
syscall_read (int fd, void* buffer, unsigned size);
int
syscall_write (int fd, const void *buffer, unsigned size);
void
syscall_seek (int fd, unsigned position);
unsigned
syscall_tell (int fd);
void
syscall_close (int fd);

#endif /* userprog/syscall.h */
