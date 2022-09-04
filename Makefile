CSOURCES := $(shell find . -name "*.c")
CPPSOURCES := $(shell find . -name "*.cpp")
CFLAGS := -Wall -Wextra -Wfloat-equal -O -MMD -Woverloaded-virtual -Wold-style-cast -Wundef -Wshadow -Wcast-align -Wconversion -Wunreachable-code -ftrapv -pedantic -std=c++2a -Wformat=2 -Wformat-overflow=2 -Wreturn-type -Wdouble-promotion -Wstrict-overflow=5 -Wconversion

all: CFLAGS := $(CFLAGS) -O3
all: CXXFLAGS := $(CFLAGS)
debug: CFLAGS := $(CFLAGS) -g
debug: CXXFLAGS := $(CFLAGS)
test: CFLAGS := $(CFLAGS) -Werror -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=null -fsanitize=bounds-strict -fstack-protector-all
test: CXXFLAGS := $(CFLAGS)
suck: CFLAGS := -Wall -Wextra -Wfloat-equal -O -MMD -O3 -g
suck: CXXFLAGS := -Wall -Wextra -Wfloat-equal -O -MMD -O3 -g

CXXFLAGS := $(CFLAGS)

LDLIBS := -lm -lstdc++
test: LDFLAGS := -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=null -fsanitize=bounds-strict -fstack-protector-all

all: main
debug: main
test: main
suck: main

main: $(CSOURCES:%.c=%.o) $(CPPSOURCES:%.cpp=%.o)

DEPS := $(shell find -name "*.d")
-include $(DEPS)

clean:
	rm -f main
	rm -f *.o
	rm -f *.d
