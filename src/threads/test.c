/* The main thread acquires a lock.  Then it creates two
   higher-priority threads that block acquiring the lock, causing
   them to donate their priorities to the main thread.  When the
   main thread releases the lock, the other threads should
   acquire it in priority order.

   Based on a test originally submitted for Stanford's CS 140 in
   winter 1999 by Matt Franklin <startled@leland.stanford.edu>,
   Greg Hutchins <gmh@leland.stanford.edu>, Yu Ping Hu
   <yph@cs.stanford.edu>.  Modified by arens. */

/*
run order:
pintos -v -k -T 60 --qemu  -- -q  run priority-donate-one

Acceptable output:
  (priority-donate-one) begin
  (priority-donate-one) This thread should have priority 32.  Actual priority: 32.
  (priority-donate-one) This thread should have priority 33.  Actual priority: 33.
  (priority-donate-one) acquire2: got the lock
  (priority-donate-one) acquire2: done
  (priority-donate-one) acquire1: got the lock
  (priority-donate-one) acquire1: done
  (priority-donate-one) acquire2, acquire1 must already have finished, in that order.
  (priority-donate-one) This should be the last line before finishing this test.
  (priority-donate-one) end
*/

#include <stdio.h>
#include "synch.h"
#include "thread.h"

static thread_func acquire1_thread_func;
static thread_func acquire2_thread_func;

int main() {
  struct lock lock;

  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  lock_init (&lock);
  lock_acquire (&lock);
  thread_create ("acquire1", PRI_DEFAULT + 1, acquire1_thread_func, &lock);
  printf ("This thread should have priority %d.  Actual priority: %d.",
       PRI_DEFAULT + 1, thread_get_priority ());
  thread_create ("acquire2", PRI_DEFAULT + 2, acquire2_thread_func, &lock);
  printf ("This thread should have priority %d.  Actual priority: %d.",
       PRI_DEFAULT + 2, thread_get_priority ());
  lock_release (&lock);
  printf ("acquire2, acquire1 must already have finished, in that order.");
  printf ("This should be the last line before finishing this test.");
}

static void
acquire1_thread_func (void *lock_)
{
  struct lock *lock = lock_;

  lock_acquire (lock);
  printf ("acquire1: got the lock");
  lock_release (lock);
  printf ("acquire1: done");
}

static void
acquire2_thread_func (void *lock_)
{
  struct lock *lock = lock_;

  lock_acquire (lock);
  printf ("acquire2: got the lock");
  lock_release (lock);
  printf ("acquire2: done");
}
