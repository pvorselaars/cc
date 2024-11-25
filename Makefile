OBJECTS = cc.o lexer.o
CFLAGS  = -Wall -Wextra -g

TARGET  = cc

${TARGET}: $(OBJECTS)
	$(CC) $^ -o $@

%: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
