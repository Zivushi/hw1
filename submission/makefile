CC = gcc
OBJS_ONE = event_manager.o date.o event.o member_set.o 
EXEC_ONE = event_manager
OBJST_TWO = priority_queue.o 
EXEC_TWO = priority_queue
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG


$(EXEC_ONE):$(OBJS_ONE)
	$(CC) $(COMP_FLAG) $(OBJS_ONE) tests/event_manager_tests.c -L. -lpriority_queue  -o event_manager

$(EXEC_TWO):$(OBJST_TWO)
	$(CC)  $(COMP_FLAG) $(OBJST_TWO)   tests/priority_queue_tests.c -o priority_queue

event_manager.o: event_manager.c event_manager.h priority_queue.h date.h member_set.h event.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
date.o: date.c date.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
event.o: event.c event.h date.h member_set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
member_set.o: member_set.c member_set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) -c  $(COMP_FLAG) $*.c

clean:
	rm -f $(EXEC_ONE) $(OBJS_ONE) $(EXEC_TWO) $(OBJST_TWO)
