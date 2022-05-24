IDIR =  inc/
SDIR =  src/
ODIR =	obj/
DDIR =	dep/
csrc =  $(shell find $(SDIR) -name "*.c")
ccsrc = $(shell find $(SDIR) -name "*.cpp")
exsrc = $(wildcard GL/*.c)
OBJ = 	$(subst $(SDIR), $(ODIR), $(csrc:.c=.o)) \
		$(subst $(SDIR), $(ODIR), $(ccsrc:.cpp=.o)) \
		$(subst ../, $(ODIR),$(exsrc:.c=.o))
DEP =	$(subst $(ODIR), $(DDIR), $(OBJ:.o=.d))

#preprocessor flags
CPPFLAGS = -g -DGLEW_STATIC -Iinc  -IGL -Iinc/engine -Iinc/engine/graphics -I/usr/include/freetype2
#linker flags (directories)
#LDFLAGS
#linker libraries
LDLIBS =-lm -lGL -lGLU -lglfw -lX11 -lfreetype -pthread


runeraider: $(OBJ)
	$(CXX) -o $@ $^ $(LDLIBS)

-include $(DEP)							#include all dep files in the makefile

.PHONY: clean
clean:
	rm -f $(OBJ) runeraider
	rm -f $(DEP)


%/ : 
	@mkdir -p $@

.SECONDEXPANSION:
$(DDIR)%.d: $(SDIR)%.cpp | $$(dir $$@)
	@$(CPP) $(CPPFLAGS) $< -MM -MT $(subst $(DDIR), $(ODIR), $(@:.d=.o)) >$@


$(ODIR)%.o : $(SDIR)%.c | $$(dir $$@)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(ODIR)%.o : ../%.c | $$(dir $$@)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(ODIR)%.o : $(SDIR)%.cpp | $$(dir $$@)
	$(CXX) $(CPPFLAGS) -c $< -o $@


