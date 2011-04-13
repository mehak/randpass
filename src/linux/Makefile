#Copyright 2010 Nate Merlin
#    This file is part of randpass.
#
#    randpass is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    randpass is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

# Tools and options:
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=gnu99
LINK = -lm
BUGFLAGS = -g
RM = rm -f

# Filenames:
OBJS = randpass.o version.o
EXEC = randpass

# Primary Targets:
.PHONY: production testing clean

production: clean randpass

testing: clean debug

clean:
	$(RM) $(OBJS) randpass drandpass

# Rules to build prerequisites:
randpass debug: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $^ $(LINK)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Target specific options:
debug: EXEC = drandpass
debug: CFLAGS += $(BUGFLAGS)
