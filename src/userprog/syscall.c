#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/process.h"
#include "devices/shutdown.h"

static void
syscall_handler (struct intr_frame *);

/* Checks the validity of the user vaddr. Returns true if uaddr is not
 null, is not a pointer to kernel vitrual addr. space and is not a
 pointer to unmapped memory. */
static bool
is_uaddr_valid (void *uaddr)
{
  bool valid = false;

  if ((uaddr != NULL) && (is_user_vaddr (uaddr)))
    {
      if (pagedir_get_page (thread_current ()->pagedir, uaddr) != NULL)
	{
	  valid = true;
	}
    }

  return valid;
}

static struct file_descriptor *
get_open_file (int fd)
{
  struct list_elem *elem;
  struct file_descriptor *file_descriptor;
  elem = list_tail (&useropened_files);
  while ((elem = list_prev (elem)) != list_head (&useropened_files))
    {
      file_descriptor = list_entry (elem, struct file_descriptor, element);
      if (file_descriptor->descriptor_id == fd)
	{
	  return file_descriptor;
	}
    }
  return NULL;
}



void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void
syscall_halt (void)
{
  shutdown_power_off ();
}

void
syscall_exit (int status)
{

}

pid_t
syscall_exec (const char* cmd_line)
{
  //TODO: to implement
  return 0;
}

int
syscall_wait (pid_t pid)
{
  //TODO: to implement
  return 0;
}

bool
syscall_create (const char* file, unsigned initial_size)
{
  //TODO: to implement
  return false;
}

bool
syscall_remove (const char* file)
{
  //TODO: to implement
  return false;
}

int
syscall_open (const char* file)
{
  //TODO: to implement
  return 0;
}

int
syscall_filesize (int fd)
{
  //TODO: to implement
  return 0;
}

int
syscall_read (int fd, void* buffer, unsigned size)
{
  //TODO: to implement
  return 0;
}

int
syscall_write (int fd, const void *buffer, unsigned size)
{

  int status = 0;
  struct file_descriptor *file_descriptor;
  unsigned temp_buffer_size = size;
  void *temp_buffer = buffer;

  //valid memory check
  while (temp_buffer != NULL)
    {
      if (!is_uaddr_valid (temp_buffer))
	{
	  syscall_exit (-1);
	}
      else
	{
	  //termination condition
	  if (temp_buffer_size == 0)
	    {
	      temp_buffer = NULL;
	    }
	  else if (temp_buffer_size > PGSIZE)
	    {
	      temp_buffer = temp_buffer + PGSIZE;
	      temp_buffer = temp_buffer - PGSIZE;
	    }

	  else
	    {
	      temp_buffer = buffer + size - 1;
	      temp_buffer_size = 0;
	    }
	}
    }

  lock_acquire (&filesystem_lock);
  if (fd == STDIN_FILENO)
    {
      status = -1;
    }
  else if (fd == STDOUT_FILENO)
    {
      putbuf (buffer, size);
      status = size;
    }
  else
    {
      file_descriptor = get_open_file (fd);
      if (file_descriptor != NULL)
	{
	  status = file_write (file_descriptor->file, buffer, size);
	}
    }
  lock_release (&filesystem_lock);

  return status;
}

void
syscall_seek (int fd, unsigned position)
{
  //TODO: to implement
}

unsigned
syscall_tell (int fd)
{
  //TODO: to implement
  return 0;
}

void
syscall_close (int fd)
{
  //TODO: to implement
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  printf ("system call!\n");
  thread_exit ();
}
