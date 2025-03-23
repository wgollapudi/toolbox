TOOL=thanos
SRC=$(TOOL).sh

PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin

.PHONY : all install uninstall clean

all : $(TOOL)

$(TOOL) : $(SRC)
	@chmod +x $(SRC)
	@cp $(SRC) $(TOOL)

install : $(TOOL)
	@echo "Installing $(TOOL) to $(BINDIR)"
	@install -d $(BINDIR)
	@install -m 755 $(TOOL) $(BINDIR)/$(TOOL)
	@echo "Installation complete."

uninstall :
	@echo "Uninstalling $(TOOL) from $(BINDIR)
	@rm -f $(BINDIR)/$(TOOL)
	@echo "Uninstallation complete."

clean :
	@rm -f $(TOOL)
