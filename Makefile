CXX := g++

CFLAGS := -Wall -fpermissive
CFLAGS += -std=c++20
ifneq ($(OS), 'Windows_NT')
    CFLAGS += -D_POSIX_C_SOURCE
endif

ifeq ($(shell uname -s), Darwin)
LIBS := -lGLEW `pkg-config --libs glfw3` -lpthread -ldl -lfreetype -pthread -lm
else
LIBS := -lGL -lGLU -lGLEW `pkg-config --libs glfw3` -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lstdc++fs -lfreetype -pthread -lm
endif

# Choose one according to your needs:
# CFLAGS += -DQGL_GLAD
CFLAGS += -DQGL_GLAD_LOCAL
# -------------------------------

LDFLAGS := -L/usr/local/lib
SANITIZERFLAGS := -fsanitize=address -fsanitize=undefined

SRCDIR := src
OBJDIR := obj
BINDIR := bin
INCDIR := include
DEPDIR := deps

TARGET := test

SOURCES += $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))


debug: CFLAGS += -Og -g3 -fno-omit-frame-pointer $(SANITIZERFLAGS)
debug: LDFLAGS += $(SANITIZERFLAGS)
debug: $(BINDIR)/$(TARGET)

release: CFLAGS += -O3 -g0 -DNDEBUG
release: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEPDIR)
	@$(CXX) -I$(INCDIR) -c $(CFLAGS) -o $@ \
	    -MT $@ -MMD -MP \
	    -MF $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $@) \
	    $<

$(BINDIR) $(OBJDIR) $(DEPDIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJDIR) $(DEPDIR) $(BINDIR)/$(TARGET)

.PHONY: debug release clean

-include $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $(SOURCES))