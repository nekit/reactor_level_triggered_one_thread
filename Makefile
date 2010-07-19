reactor_ltot := reactor_ltot
reactor_ltot_list := reactor_ltot.o parse_args.o mega_log.o reactor_ltot_work.o

CFLAGS += -O2 -Wall
LDFLAGS += -lpthread

all: $(reactor_ltot)

$(reactor_ltot): $(reactor_ltot_list)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CC) -c -MD $(CFLAGS) $<

include $(wildcard *.d)

clean:
	rm *.o *.d $(reactor_ltot)