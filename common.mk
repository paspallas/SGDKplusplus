# Common definitions

BIN := $(GDKPLUS)/bin
LIB := $(GDKPLUS)/lib
XGCC:= $(GDKPLUS)/x68k-gcc

SRC_LIB 	 := $(GDKPLUS)/src
RES_LIB 	 := $(GDKPLUS)/res
INCLUDE_LIB  := $(GDKPLUS)/inc

MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR := $(subst \,/,$(MAKEFILE_DIR))

PREFIX 	:= m68k-elf
AR 		:= $(XGCC)/bin/$(PREFIX)-ar
CXX 	:= $(XGCC)/bin/$(PREFIX)-g++
CC 		:= $(XGCC)/bin/$(PREFIX)-gcc
LD 		:= $(XGCC)/bin/$(PREFIX)-ld
NM		:= $(XGCC)/bin/$(PREFIX)-nm
OBJCPY  := $(XGCC)/bin/$(PREFIX)-objcopy
LIBGCC 	:= $(XGCC)/libgcc.a
ASMZ80  := $(BIN)/sjasm
MACCER  := $(BIN)/mac68k
BINTOS  := $(BIN)/bintos

ifeq ($(OS),Windows_NT)
	# Native Windows
	SHELL 	:= $(BIN)/sh
	RM 		:= $(BIN)/rm
	CP 		:= $(BIN)/cp
	MKDIR 	:= $(BIN)/mkdir
	FIND 	:= $(BIN)/gfind
	VERSION := $(shell $(CC) -dumpversion)
	PLUGIN  := $(XGCC)/libexec/gcc/$(PREFIX)/$(VERSION)/liblto_plugin
else
	ifeq ($(SGDKPLUS_DOCKER),y)
		# Linux docker
		SHELL   := sh
		RM 	    := rm
		CP 	    := cp
		MKDIR   := mkdir
		FIND    := find
		VERSION := $(shell $(CC) -dumpversion)
		PLUGIN  := $(XGCC)/libexec/gcc/$(PREFIX)/$(VERSION)/liblto_plugin
	else
		# Native Linux
		SHELL   := sh
		RM 		:= rm
		CP 		:= cp
		MKDIR   := mkdir
		FIND 	:= find

		AR := $(PREFIX)-ar
		CXX:= $(PREFIX)-g++
		CC := $(PREFIX)-gcc
		LD := $(PREFIX)-ld
		NM := $(PREFIX)-nm
		OBJCPY := $(PREFIX)-objcopy
		ASMZ80 := sjasm
		MACCER := mac68k
		BINTOS := bintos
	    PLUGIN :=
	endif
endif

JAVA := java
ECHO := echo
SIZEBND := $(JAVA) -jar $(BIN)/sizebnd.jar
RESCOMP := $(JAVA) -jar $(BIN)/rescomp.jar
