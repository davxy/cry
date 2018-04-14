include config.mk

CC := gcc
AR := ar
AWK := awk
CP := cp
RM := rm -rf

source_dir := src
config := $(source_dir)/config.h

#
# Get build name
#

# $(call normalstr,str)
build_name := $(shell uname -om)
space := $(empty) $(empty)
normalstr := $(subst _,-,$(subst /,-,$(subst $(space),-,$(build_name))))
# remove unwanted characters spaces
build_name := $(shell echo $(call normalstr,$(build_name)) | tr A-Z a-z)
# convert to lowercase
binary_dir := build/$(build_name)


# call $(src_to_bin_dir,list)
src_to_bin_dir = $(patsubst $(source_dir)%,$(binary_dir)%,$1)

target = $(binary_dir)/libcry.a

.SUFFIXES:

includes-y	:= -Iinclude -Isrc -include $(config)

cflags-y := -Wall -MMD -MP
cflags-y += -fprofile-arcs -ftest-coverage
cflags-$(CRY_SHORT_ENUMS) += -fshort-enums
cflags-$(CRY_OMIT_FRAME_POINTER) += -fomit-frame-pointer

ifeq ($(CRY_STACK_PROTECTOR),y)
cflags-y += -fstack-protector
else
cflags-y += -fno-stack-protector
endif

ifeq ($(CRY_DEBUG),y)
cflags-y += -O0 -g3 -fno-inline
else
cflags-y += -DNDEBUG
ifeq ($(CRY_SMALL_SIZE),y)
cflags-y += -Os
else
cflags-y += -O3
endif
endif


objs-y 	:=
paths-y	:=
objects_list :=

define include_subdir
$(shell mkdir -p $(call src_to_bin_dir,$1))
subdirs-y :=
current := $1
objs-y :=
include $1/subdir.mk
objects_list += $$(if $$(objs-y),$$(addprefix $1/,$$(objs-y)))
paths-y += $$(current)
subdirs-y := $$(addprefix $$(current)/, $$(subdirs-y))
$$(foreach subdir, $$(subdirs-y),$$(eval $$(call include_subdir,$$(subdir))))
endef

$(eval $(call include_subdir,src))

objects = $(call src_to_bin_dir,$(objects_list))
depends = $(patsubst %.o,%.d,$(objects))

CPPFLAGS =  $(includes-y)
CFLAGS  = 	$(cflags-y)
AFLAGS = 	$(aflags-y)
LDFLAGS = 	$(lflags-y)

DATE 	:= $(shell date +'%y%m%d')

.PHONY: all release clean test

all: $(target)

clean:
	$(RM) $(binary_dir) $(config) *.a

$(target): $(config) $(objects)
	$(AR) rcs $@ $^
	$(CP) $(target) .

$(config): config.mk
	@printf "/*\n * Automatically generated. Do not edit.\n */\n\n" > $(config)
	@$(AWK) -F= 'NF > 1 && $$1 !~ /^[# ]/ { print "#define", $$1; }' $< >> $(config)
	@echo "#define CRY_RELEASE $(DATE)" >> $(config)
	@echo "#define CRY_CFLAGS \"$(CFLAGS)\"" >> $(config)

$(binary_dir)/%.o: $(source_dir)/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

test: $(target)
	make -C test

cleantest:
	make -C test clean

