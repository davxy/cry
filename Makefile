include config.mk

CC := gcc
AR := ar
AWK := awk
CP := cp
RM := rm -rf

source_dir := src
config := include/cry/config.h

#
# Get build name
#

# $(call normalstr,str)
build_name = $(shell $(CC) -dumpmachine)
# convert to lowercase
binary_dir = build/$(build_name)


# call $(src_to_bin_dir,list)
src_to_bin_dir = $(patsubst $(source_dir)%,$(binary_dir)%,$1)

target = $(binary_dir)/libcry.a

.SUFFIXES:

includes-y := -Iinclude -Isrc -include $(config)

cflags-y := -Wall -MMD -MP
cflags-$(CRY_COVERAGE) += --coverage
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
cflags-y += -O3 -funroll-loops
endif
endif


objects-y :=
paths-y	:=
objects_list :=

define include_subdir
$(shell mkdir -p $(call src_to_bin_dir,$1))
subdirs-y :=
current := $1
objects-y :=
include $1/subdir.mk
objects_list += $$(if $$(objects-y),$$(addprefix $1/,$$(objects-y)))
paths-y += $$(current)
subdirs-y := $$(addprefix $$(current)/, $$(subdirs-y))
$$(foreach subdir, $$(subdirs-y),$$(eval $$(call include_subdir,$$(subdir))))
endef

$(eval $(call include_subdir,src))

objects = $(call src_to_bin_dir,$(objects_list))
depends = $(patsubst %.o,%.d,$(objects))

CPPFLAGS = $(includes-y)
CFLAGS   = $(cflags-y)
AFLAGS   = $(aflags-y)
LDFLAGS  = $(lflags-y)

DATE := $(shell date +'%y%m%d')


.PHONY: all cry clean test testclean doc

all: cry

cry: $(target)

clean:
	@$(RM) $(binary_dir) $(config) *.a
	@$(RM) `find . -type f \( -name \*.gcda -o -name \*.gcno \)`

$(objects): Makefile config.mk $(config)

$(target): $(objects)
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

testclean:
	make -C test clean

doc:
	cd doc; doxygen Doxyfile.in

###############################################################################
# Code quality tools reports
###############################################################################

# Overwrite from command line with: `make SONAR_SCANNER=<path>`
SONAR_SCANNER := sonar-scanner

# Vera style checker rules to skip
VERA_SKIP := "T0(10|11|12|13|19)"

# CPPcheck: a general purpose static code checker
cppcheck:
	@echo "Running cppcheck ..."
	@cppcheck --language=c --std=c89 --force --enable=all --suppress=variableScope --suppress=unusedFunction --suppress=missingIncludeSystem --xml -I src -I include src 2> build/cppcheck-report.xml

# Vera++: static code checker focusing on code style issues
vera:
	@echo "Running vera++ ..."
	@find src include -type f -regextype sed -iregex ".*/*\.\(c\|h\)" -print | vera++ - -showrules -nodup 2> vera.tmp
	@cat vera.tmp | grep -v -E $(VERA_SKIP) | scripts/vera2report.perl > build/vera-report.xml
	@rm vera.tmp

# Valgrind: memory leaks and dynamic issues report
valgrind: test
	@echo "Running valgrind ..."
	@cd test; valgrind --xml=yes --xml-file=valgrind-report.xml --leak-check=full --show-leak-kinds=all --track-origins=yes ./test; cd ..
	@mv test/valgrind-report.xml build/valgrind-report.xml

# Gcovr: coverage report using unit tests (uses valgrind run)
coverage: valgrind
	@echo "Running gcovr ..."
	@gcovr --xml --root . > build/gcovr-report.xml

# Sonarqube: continuous inspection of code quality platform.
# Parse collected data and feed it into Sonarqube server.
sonar: cppcheck vera coverage valgrind
	@echo "Running sonar-scanner ..."
	@$(SONAR_SCANNER) -Dproject.settings=scripts/sonar-project.properties
