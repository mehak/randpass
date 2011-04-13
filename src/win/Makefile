# Tools and options:
CC = gcc.exe
CFLAGS = -Wall -Wextra -pedantic -std=gnu99
BUGFLAGS = -g
LINK = -lm
RM = rm.exe -f

# Filenames:
OBJS = randpass.o version.o
EXEC = randpass.exe

# Primary Targets:
.PHONY: production testing clean
production: clean randpass

testing: clean debug

clean:
	$(RM) $(OBJS) randpass.exe drandpass.exe

# Rules to build prerequisites:
randpass debug: $(OBJS) 
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(LINK) 

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Target specific options:
debug: EXEC = drandpass.exe
debug: CFLAGS += $(BUGFLAGS)
