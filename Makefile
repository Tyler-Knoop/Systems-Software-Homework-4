# $Id: Makefile,v 1.48 2023/11/15 17:00:21 leavens Exp leavens $
# Makefile for PL/0 compiler and code generation in COP 3402

# Add .exe to the end of target to get that suffix in the rules
COMPILER = compiler
LEXER = ./compiler -l
UNPARSER = ./compiler -u
VM = vm
DISASM = $(VM)/disasm
RUNVM = $(VM)/$(VM)

# Tools used
CC = gcc
# on Linux, the following can be used with gcc:
# CFLAGS = -fsanitize=address -static-libasan -g -std=c17 -Wall
CFLAGS = -g -std=c17 -Wall
YACC = bison
YACCFLAGS = -Wall --locations -d -v
LEX = flex
LEXFLAGS =
# Unix command names
MV = mv
RM = rm -f
SUBMISSIONZIPFILE = submission.zip
ZIP = zip -9
PL0 = pl0
SUF = pl0
# The GTESTS are a graduated series of tests, good to start with
GTESTS = hw4-gtest0.pl0 hw4-gtest1.pl0 hw4-gtest2.pl0 hw4-gtest3.pl0 \
	 hw4-gtest5.pl0 hw4-gtest6.pl0 hw4-gtest7.pl0 hw4-gtest8.pl0 \
	 hw4-gtest9.pl0 hw4-gtestA.pl0 hw4-gtestB.pl0 hw4-gtestC.pl0 \
	 hw4-gtestD.pl0 hw4-gtestE.pl0 hw4-gtestF.pl0 hw4-gtestG.pl0 \
	 hw4-gtestH.pl0 hw4-gtestI.pl0 hw4-gtestJ.pl0 hw4-gtestK.pl0 \
	 hw4-gtestL.pl0 hw4-gtestM.pl0 hw4-gtestN.pl0 hw4-gtestO.pl0 \
	 hw4-gtestP.pl0
# The READTESTS need input from stdin
READTESTS = hw4-read-test0.pl0 hw4-read-test1.pl0
# the VM tests are another set of tests that (somehwhat) increas in comlexity
VMTESTS = hw4-vmtest0.pl0 hw4-vmtest1.pl0 hw4-vmtest2.pl0 hw4-vmtest3.pl0 \
	hw4-vmtest4.pl0 hw4-vmtest5.pl0 hw4-vmtest6.pl0 hw4-vmtest7.pl0 \
	hw4-vmtest8.pl0 hw4-vmtest9.pl0 hw4-vmtestA.pl0 hw4-vmtestB.pl0 \
	hw4-vmtestC.pl0
# you can add your own tests to alltests
ALLTESTS = $(GTESTS) $(READTESTS) $(VMTESTS)
EXPECTEDOUTPUTS = $(ALLTESTS:.pl0=.out)
STUDENTTESTOUTPUTS = $(ALLTESTS:.pl0=.myo)

# Add the names of your own files with a .o suffix to link them in the program
# Feel free to edit the following definition of COMPILER_OBJECTS
COMPILER_OBJECTS = $(COMPILER)_main.o $(PL0)_lexer.o lexer_utilities.o \
		machine_types.o parser.o regname.o utilities.o \
		$(PL0).tab.o ast.o file_location.o unparser.o \
		scope.o scope_check.o symtab.o id_use.o id_attrs.o \
		instruction.o bof.o code.o \
		gen_code.o literal_table.o

# create the VM executable
.PRECIOUS: $(VM)/$(VM)
$(VM): $(VM)/$(VM)

$(PL0).tab.o: $(PL0).tab.c $(PL0).tab.h
	$(CC) $(CFLAGS) -c $<

$(PL0).tab.c $(PL0).tab.h: $(PL0).y ast.h parser_types.h machine_types.h 
	$(YACC) $(YACCFLAGS) $(PL0).y

.PRECIOUS: $(PL0)_lexer.c
$(PL0)_lexer.c: $(PL0)_lexer.l $(PL0).tab.h
	$(LEX) $(LEXFLAGS) $<

$(PL0)_lexer.o: $(PL0)_lexer.c ast.h utilities.h file_location.h
	$(CC) $(CFLAGS) -Wno-unused-function -Wno-unused-but-set-variable -c $(PL0)_lexer.c

$(PL0)_lexer.l: $(PL0).tab.h

# create the compiler executable
$(COMPILER): $(COMPILER_OBJECTS)
	$(CC) $(CFLAGS) -o $(COMPILER) $(COMPILER_OBJECTS)

# rule for compiling individual .c files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) *~ *.o '#'*
	$(RM) $(PL0)_lexer.c $(PL0)_lexer.h
	$(RM) $(PL0).tab.c $(PL0).tab.h $(PL0).output
	$(RM) $(COMPILER).exe $(COMPILER)
	$(RM) *.stackdump core
	$(RM) $(SUBMISSIONZIPFILE)

cleanall: clean
	$(RM) *.myo *.myto *.bof *.asm *.tout

# The .myo files are outputs of VM when run on compiled programs
# with no tracing in the VM (a high level output of the tests)
# The use of cat char-inputs.txt | allows read statments to not hang
.PRECIOUS: %.myo
%.myo: %.bof $(VM)
	cat char-inputs.txt | ./$(RUNVM) $< > $@ 2>&1

# The .myto files are outputs from running compiled programs
# with the VM's -t option (useful for debugging)
# The use of cat char-inputs.txt | allows read statments to not hang
.PRECIOUS: %.myto
%.myto: %.bof $(VM)
	cat char-inputs.txt | $(RUNVM) -t $< > $@ 2>&1

# The .bof files are the compiled binary object files.
.PRECIOUS: %.bof
%.bof: %.$(SUF) $(COMPILER)
	./$(COMPILER) $<

# The .asm files are disassembled binary object files.
# These are useful for debugging the code the compiler creates.
%.asm: %.bof $(DISASM)
	$(DISASM) $< > $@ 2>&1

# The .upo files are unparsed programs;
# these are useful for debugging the parser and understanding the parse.
%.upo: %.$(SUF) $(COMPILER)
	./$(UNPARSER) $< > $@ 2>&1

# main target for testing
.PHONY: check-outputs
check-outputs: $(COMPILER) $(VM)
	@DIFFS=0; \
	for f in `echo $(ALLTESTS) | sed -e 's/\\.$(SUF)//g'`; \
	do \
		echo running ./$(COMPILER) on "$$f.$(SUF)"; \
		$(RM) "$$f.bof"; \
		./$(COMPILER) "$$f.$(SUF)" ; \
		echo running $(RUNVM) on "$$f.bof"; \
		$(RM) "$$f.myo"; \
		cat char-inputs.txt | $(RUNVM) "$$f.bof" > "$$f.myo" 2>&1; \
		diff -w -B "$$f.out" "$$f.myo" && echo 'passed!' || DIFFS=1; \
	done; \
	if test 0 = $$DIFFS; \
	then \
		echo 'All output tests passed!'; \
	else \
		echo 'Some output test(s) failed!'; \
	fi

$(SUBMISSIONZIPFILE): *.c *.h $(STUDENTTESTOUTPUTS)
	$(ZIP) $(SUBMISSIONZIPFILE) $(PL0).y $(PL0)_lexer.l *.c *.h Makefile
	$(ZIP) $(SUBMISSIONZIPFILE) $(STUDENTTESTOUTPUTS) $(ALLTESTS) $(EXPECTEDOUTPUTS)

# developer's section below...

# The .out files are expected outputs from running the VM on
# the compiled program.
# The use of cat char-inputs.txt | allows read statments to not hang
.PRECIOUS: %.out
%.out: %.bof $(RUNVM)
	cat char-inputs.txt | $(RUNVM) $< >$@ 2>&1

%.tout: %.bof $(RUNVM)
	cat char-inputs.txt | $(RUNVM) -t $< >$@ 2>&1

.PHONY: create-outputs
create-outputs: $(COMPILER) $(VM) $(ALLTESTS)
	@if test '$(IMTHEINSTRUCTOR)' != true ; \
	then \
		echo 'Students should use the target check-compiler-outputs,' ; \
		echo 'as using this target (create-compiler-outputs) will invalidate the tests' ; \
		exit 1; \
	fi; \
	for f in `ls $(ALLTESTS) | sed -e 's/\\.$(SUF)//g'`; \
	do \
		echo running ./$(COMPILER) on "$$f.$(SUF)"; \
		$(RM) "$$f.bof"; \
		./$(COMPILER) "$$f.$(SUF)" ; \
		echo running $(RUNVM) on "$$f.bof"; \
		$(RM) "$$f.out" ; \
		cat char-inputs.txt | $(RUNVM) "$$f.bof" > "$$f.out" 2>&1; \
	done; \
	echo done creating compiler test outputs!

.PHONY: digest
digest: digest.txt

digest.txt: create-outputs
	for f in `ls $(ALLTESTS) | sed -e 's/\\.$(SUF)//g'` ; \
	do \
		cat $$f.$(SUF); \
		echo " "; cat $$f.out; \
		echo " "; echo " "; \
	done > digest.txt

# don't use outputs-clean unless you want to regenerate the expected outputs!
.PHONY: develop-clean outputs-clean clean-outputs
outputs-clean clean-outputs:
	@if test '$(IMTHEINSTRUCTOR)' != true ; \
	then \
		echo 'Students should use the targets clean or cleanall,' ; \
		echo 'as using this target (outputs-clean) will invalidate the tests'; \
		exit 1; \
	fi
	$(RM) *.out

develop-clean: clean
	$(RM) *.upo *.asm *.bof
	$(RM) digest.txt
	cd $(VM); make develop-clean

TESTSZIPFILE = ~/temp/hw4-tests.zip
GENERATEDFILES = $(PL0).tab.h $(PL0).tab.c $(PL0)_lexer.c $(PL0)_lexer.h
PROVIDEDFILES = compiler_main.c pl0_lexer.l lexer_utilities.[ch] lexer.[ch] \
		machine_types.[ch] parser.[ch] regname.[ch] utilities.[ch] \
		pl0.y parser_types.h ast.[ch] file_location.[ch] \
		unparser.[ch] scope.[ch] scope_check.[ch] symtab.[ch] \
		id_use.[ch] id_attrs.[ch] instruction.[ch] bof.[ch] code.[ch] \
		lexical_address.[ch] \
		gen_code.h literal_table.h \
		char-inputs.txt $(GENERATEDFILES)

.PHONY:hw4-tests.zip
hw4-tests.zip: create-outputs $(TESTSZIPFILE)

$(TESTSZIPFILE): $(ALLTESTS) Makefile $(PROVIDEDFILES)
	$(RM) $(TESTSZIPFILE)
	chmod 444 $(ALLTESTS) $(EXPECTEDOUTPUTS)
	chmod 744 Makefile $(PROVIDEDFILES)
	$(ZIP) $(TESTSZIPFILE) $(ALLTESTS) $(EXPECTEDOUTPUTS) Makefile $(PROVIDEDFILES)
	$(ZIP) -r $(TESTSZIPFILE) $(VM) -x 'vm/RCS/' 'vm/RCS/*,v' 'vm/*.o' 'vm/*.exe'

.PHONY: $(OVERALLZIPFILE)
$(OVERALLZIPFILE) ~/temp/$(OVERALLZIPFILE): Makefile *.[chly] *.pdf $(ALLTESTS)
	$(MAKE) clean create-outputs clean
	$(ZIP) ~/temp/$(OVERALLZIPFILE) $^ $(EXPECTEDOUTPUTS)

.PHONY: check-separately
check-separately: $(COMPILER_OBJECTS)

$(VM)/$(VM):
	cd $(VM); make vm

$(VM)/disasm:
	cd $(VM); make disasm

$(VM)/asm:
	cd $(VM); make asm

check-separately:
	$(CC) $(CFLAGS) -c *.c

all: $(COMPILER) $(VM)/$(VM) $(VM)/asm $(VM)/disasm
