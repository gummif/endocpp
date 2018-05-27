# make && sudo make install
include Makefile.inc

CFLAGS += -Wall -Wextra -I$(INCDIR)
#LDFLAGS += -L$(LIBDIR) -l$(NAME)

# source files
SRC =
SRCXX = $(TARGET).cpp
OBJ = $(SRC:.c=.o) $(SRCXX:.cpp=.o)

DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

.PHONE: all install clean uninstall run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.c
%.o: %.c $(DEPDIR)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@
	$(POSTCOMPILE)

%.o: %.cpp
%.o: %.cpp $(DEPDIR)/%.d
	$(CXX) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

install:
	$(INSTALL) $(SRCDIR)/$(TARGET) $(OUTDIR)/$(TARGET)

clean:
	$(RM) $(OBJ) $(TARGET) $(DEPDIR)/*.d
	rmdir $(DEPDIR)

uninstall:
	$(RM) $(OUTDIR)/$(TARGET)

run:
	@./$(TARGET)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(OBJ)))
