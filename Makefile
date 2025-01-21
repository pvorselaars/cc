OBJECTS = cc.o lexer.o parser.o emit.o
CFLAGS  = -Wall -Wextra -g

TARGET  = cc
TESTS   = $(wildcard tests/valid/*.c)

tests: $(TARGET)
	@for test in $(TESTS); do \
		if !(./$(TARGET) $$test > /dev/null); then \
			echo Failed test: $$test; \
		fi \
	done

${TARGET}: $(OBJECTS)
	$(CC) $^ -o $@

%: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
