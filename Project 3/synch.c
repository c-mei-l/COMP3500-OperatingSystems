/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>

////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	sem = kmalloc(sizeof(struct semaphore));
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	spl = splhigh();
	assert(thread_hassleepers(sem)==0);
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while (sem->count==0) {
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	spl = splhigh();
	sem->count++;
	assert(sem->count>0);
	thread_wakeup(sem);
	splx(spl);
}

////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));
	if (lock == NULL) {
		return NULL;
	}

	lock->name = kstrdup(name);
	if (lock->name == NULL) {
		kfree(lock);
		return NULL;
	}
	
	// add stuff here as needed
	lock->holder = NULL; 

	return lock;
}

void
lock_destroy(struct lock *lock)
{
	assert(lock != NULL);

	// add stuff here as needed
	
	kfree(lock->name);
	kfree(lock);
}

void
lock_acquire(struct lock *lock)
{
	// Write this
	int temp;
	temp = splhigh(); //turn off interrupts
	if (lock_do_i_hold(lock)) {
	panic("lock %s at %p: Deadlock.\n", lock->name, lock);
	}

	while (lock->holder != NULL) {
	thread_sleep(lock);
	}

	lock->holder = curthread;	
	splx(temp);

//	(void)lock;  // suppress warning until code gets written
}

void
lock_release(struct lock *lock)
{
	// Write this
	assert(lock != NULL);
	int temp;
	temp = splhigh();
	if (!lock_do_i_hold(lock)) {
	panic ("lock %s at %p Deadlock.\n", lock->name, lock);
	}	
	//Release lock
	lock -> holder = NULL;
	 
	thread_wakeone(lock);
	splx(temp);
//	(void)lock;  // suppress warning until code gets written
}

int
lock_do_i_hold(struct lock *lock)
{
	// Write this
	int temp, same;
	temp = splhigh(); 
	assert(lock != NULL);
	if (lock->holder == curthread) {
	same = 1;
	}
	else {
	same = 0;
	}

//	(void)lock;  // suppress warning until code gets written
	splx(temp);
	return same;   
}

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));
	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	
	kfree(cv->name);
	kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	// Write this
	//(void)cv;    // suppress warning until code gets written
	//(void)lock;  // suppress warning until code gets written


	assert(cv!=NULL);

	if (!lock_do_i_hold(lock)) {
	panic("lock %s at %p: Deadlock.\n", lock->name,lock);
	}	
	int temp;
	temp = splhigh();
	lock_release(lock);
	thread_sleep(cv);
	lock_acquire(lock);
	
	splx(temp);
}


void
cv_signal(struct cv *cv, struct lock *lock)
{
	// Write this
	//(void)cv;    // suppress warning until code gets written
	//(void)lock;  // suppress warning until code gets written
	assert(cv!=NULL);
	
	int temp;
	temp = splhigh();
	
	if (!lock_do_i_hold(lock)) {
	panic("cv_signal error: cv %s at %p, lock %s at %p. \n",
	 cv->name, cv, lock->name, lock);
	}	
thread_wakeone(cv);
splx(temp);
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	//(void)cv;    // suppress warning until code gets written
	//(void)lock;  // suppress warning until code gets written
	assert(cv!=NULL);
assert(cv!=NULL);
int temp;
temp = splhigh();

if(!lock_do_i_hold(lock)) {
panic("cv_signal arror: cv %s at %p. \n",
cv->name, cv, lock->name, lock);}
thread_wakeup(cv);
splx(temp);
}

