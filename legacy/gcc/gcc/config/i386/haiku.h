/* Definitions for Intel x86 running Haiku
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2004
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef I386
#include <i386/i386.h>	/* Base i386 target machine definitions */
#include <i386/att.h>	/* Use the i386 AT&T assembler syntax */
#include <svr4.h>	/* some common stuff */
#endif

/* /usr/ccs doesn't exist on Haiku, so we don't want to use it */
#undef MD_EXEC_PREFIX
#undef MD_STARTFILE_PREFIX

#undef TARGET_VERSION
#define TARGET_VERSION fprintf (stderr, " (i386 Haiku/ELF)");

/* Change debugging to Dwarf2.  */
#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* The SVR4 ABI for the i386 says that records and unions are returned
   in memory.  */
#undef DEFAULT_PCC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 1

#undef ASM_COMMENT_START
#define ASM_COMMENT_START " #"

/* This is how to output an element of a case-vector that is relative.
   This is only used for PIC code.  See comments by the `casesi' insn in
   i386.md for an explanation of the expression this outputs. */
#undef ASM_OUTPUT_ADDR_DIFF_ELT
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL) \
  fprintf (FILE, "\t.long _GLOBAL_OFFSET_TABLE_+[.-%s%d]\n", LPREFIX, VALUE)

/* Indicate that jump tables go in the text section.  This is
   necessary when compiling PIC code.  */
#define JUMP_TABLES_IN_TEXT_SECTION (flag_pic)

/* Copy this from the svr4 specifications... */
/* Define the register numbers to be used in Dwarf debugging information.
   The SVR4 reference port C compiler uses the following register numbers
   in its Dwarf output code:
	0 for %eax (gnu regno = 0)
	1 for %ecx (gnu regno = 2)
	2 for %edx (gnu regno = 1)
	3 for %ebx (gnu regno = 3)
	4 for %esp (gnu regno = 7)
	5 for %ebp (gnu regno = 6)
	6 for %esi (gnu regno = 4)
	7 for %edi (gnu regno = 5)
   The following three DWARF register numbers are never generated by
   the SVR4 C compiler or by the GNU compilers, but SDB on x86/svr4
   believes these numbers have these meanings.
	8  for %eip    (no gnu equivalent)
	9  for %eflags (no gnu equivalent)
	10 for %trapno (no gnu equivalent)
   It is not at all clear how we should number the FP stack registers
   for the x86 architecture.  If the version of SDB on x86/svr4 were
   a bit less brain dead with respect to floating-point then we would
   have a precedent to follow with respect to DWARF register numbers
   for x86 FP registers, but the SDB on x86/svr4 is so completely
   broken with respect to FP registers that it is hardly worth thinking
   of it as something to strive for compatibility with.
   The version of x86/svr4 SDB I have at the moment does (partially)
   seem to believe that DWARF register number 11 is associated with
   the x86 register %st(0), but that's about all.  Higher DWARF
   register numbers don't seem to be associated with anything in
   particular, and even for DWARF regno 11, SDB only seems to under-
   stand that it should say that a variable lives in %st(0) (when
   asked via an `=' command) if we said it was in DWARF regno 11,
   but SDB still prints garbage when asked for the value of the
   variable in question (via a `/' command).
   (Also note that the labels SDB prints for various FP stack regs
   when doing an `x' command are all wrong.)
   Note that these problems generally don't affect the native SVR4
   C compiler because it doesn't allow the use of -O with -g and
   because when it is *not* optimizing, it allocates a memory
   location for each floating-point variable, and the memory
   location is what gets described in the DWARF AT_location
   attribute for the variable in question.
   Regardless of the severe mental illness of the x86/svr4 SDB, we
   do something sensible here and we use the following DWARF
   register numbers.  Note that these are all stack-top-relative
   numbers.
	11 for %st(0) (gnu regno = 8)
	12 for %st(1) (gnu regno = 9)
	13 for %st(2) (gnu regno = 10)
	14 for %st(3) (gnu regno = 11)
	15 for %st(4) (gnu regno = 12)
	16 for %st(5) (gnu regno = 13)
	17 for %st(6) (gnu regno = 14)
	18 for %st(7) (gnu regno = 15)
*/
#undef DBX_REGISTER_NUMBER
#define DBX_REGISTER_NUMBER(n) \
((n) == 0 ? 0 \
 : (n) == 1 ? 2 \
 : (n) == 2 ? 1 \
 : (n) == 3 ? 3 \
 : (n) == 4 ? 6 \
 : (n) == 5 ? 7 \
 : (n) == 6 ? 5 \
 : (n) == 7 ? 4 \
 : ((n) >= FIRST_STACK_REG && (n) <= LAST_STACK_REG) ? (n)+3 \
 : (-1))

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.  */

#undef FUNCTION_PROFILER
#define FUNCTION_PROFILER(FILE, LABELNO)  \
{									\
  if (flag_pic)								\
    {									\
      fprintf (FILE, "\tleal %sP%d@GOTOFF(%%ebx),%%edx\n",		\
	       LPREFIX, (LABELNO));					\
      fprintf (FILE, "\tcall *mcount@GOT(%%ebx)\n");			\
    }									\
  else									\
    {									\
      fprintf (FILE, "\tmovl $%sP%d,%%edx\n", LPREFIX, (LABELNO));	\
      fprintf (FILE, "\tcall mcount\n");				\
    }									\
}

#undef SIZE_TYPE
#define SIZE_TYPE "long unsigned int"

#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"

#undef WCHAR_TYPE
#define WCHAR_TYPE "short unsigned int"

#undef WCHAR_UNSIGNED
#define WCHAR_UNSIGNED 1

#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 16

#undef CPP_PREDEFINES
#define CPP_PREDEFINES "-D__ELF__ -D__HAIKU__ -D__INTEL__ -D_X86_=1 \
-D__stdcall=__attribute__((__stdcall__)) \
-D__cdecl=__attribute__((__cdecl__)) \
-D__declspec(x)=__attribute__((x)) \
-Asystem(haiku)"

#undef CPP_SPEC
#define CPP_SPEC "%(cpp_cpu) %{!no-fPIC:%{!no-fpic:-D__PIC__ -D__pic__}}"

/* Haiku uses lots of multichars, so don't warn about them unless the
   user explicitly asks for the warnings with -Wmultichar.  Note that
   CC1_SPEC is used for both cc1 and cc1plus. */

#undef CC1_SPEC
#define CC1_SPEC "%{!no-fpic:%{!fPIC:-fpic}} %{!Wmultichar: -Wno-multichar} %(cc1_cpu) %{profile:-p}"

#undef CC1PLUS_SPEC
#define CC1PLUS_SPEC "%{!Wctor-dtor-privacy:-Wno-ctor-dtor-privacy}"

/* Provide a LINK_SPEC appropriate for BeOS.  Here we provide support
   for the special GCC options -static and -shared, which allow us to
   link things in one of these three modes by applying the appropriate
   combinations of options at link-time. */

/* If ELF is the default format, we should not use /lib/elf. */

#undef	LINK_SPEC
#define LINK_SPEC "%{!o*:-o %b} -m elf_i386_haiku -shared -Bsymbolic " \
	"%{nostart:-e 0} %{shared:-e 0} %{!shared: %{!nostart: --no-undefined}}"

/* Provide start and end file specs appropriate to glibc.  */

/* LIB_SPEC for Haiku */
#undef LIB_SPEC
#define LIB_SPEC "-lroot"

/* gcc runtime lib is built into libroot.so on Haiku */
/* ??? This is gonna be lovely when the next release of gcc has
   some new symbol in, so that links start failing.  */
#undef LIBGCC_SPEC
#define LIBGCC_SPEC ""

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crti.o%s crtbegin.o%s %{!nostart: %{!shared: start_dyn.o%s}} init_term_dyn.o%s %{p:i386-mcount.o%s}"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"

/* Get perform_* macros to build libgcc.a.  */
#include "i386/perform.h"

/* A C statement (sans semicolon) to output to the stdio stream
   FILE the assembler definition of uninitialized global DECL named
   NAME whose size is SIZE bytes and alignment is ALIGN bytes.
   Try to use asm_output_aligned_bss to implement this macro.  */

#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGN) \
  asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGN)

/* A C statement to output to the stdio stream FILE an assembler
   command to advance the location counter to a multiple of 1<<LOG
   bytes if it is within MAX_SKIP bytes.

   This is used to align code labels according to Intel recommendations.  */

#ifdef HAVE_GAS_MAX_SKIP_P2ALIGN
#define ASM_OUTPUT_MAX_SKIP_ALIGN(FILE,LOG,MAX_SKIP) \
  do {									\
    if ((LOG)!=0) \
      if ((MAX_SKIP)==0) fprintf ((FILE), "\t.p2align %d\n", (LOG));   \
      else fprintf ((FILE), "\t.p2align %d,,%d\n", (LOG), (MAX_SKIP)); \
  } while (0)
#endif

/*
 * Support for __declspec(dllimport) & __declspec(dllexport).
 */

/* We don't care about dllimport.  */
#define TARGET_NOP_FUN_DLLIMPORT 1

/* A C expression whose value is nonzero if IDENTIFIER with arguments ARGS
   is a valid machine specific attribute for DECL.
   The attributes in ATTRIBUTES have previously been assigned to DECL.  */

#undef VALID_MACHINE_DECL_ATTRIBUTE
#define VALID_MACHINE_DECL_ATTRIBUTE(DECL, ATTRIBUTES, IDENTIFIER, ARGS) \
  i386_pe_valid_decl_attribute_p (DECL, ATTRIBUTES, IDENTIFIER, ARGS)
extern int i386_pe_valid_decl_attribute_p ();

/* A C expression whose value is nonzero if IDENTIFIER with arguments ARGS
   is a valid machine specific attribute for TYPE.
   The attributes in ATTRIBUTES have previously been assigned to TYPE.  */

#undef VALID_MACHINE_TYPE_ATTRIBUTE
#define VALID_MACHINE_TYPE_ATTRIBUTE(TYPE, ATTRIBUTES, IDENTIFIER, ARGS) \
  i386_pe_valid_type_attribute_p (TYPE, ATTRIBUTES, IDENTIFIER, ARGS)
extern int i386_pe_valid_type_attribute_p ();

#define MERGE_MACHINE_DECL_ATTRIBUTES(OLD, NEW) \
  i386_pe_merge_decl_attributes ((OLD), (NEW))
extern union tree_node *i386_pe_merge_decl_attributes ();

/* Used to implement dllexport overriding dllimport semantics.  It's also used
   to handle vtables - the first pass won't do anything because
   DECL_CONTEXT (DECL) will be 0 so i386_pe_dll{ex,im}port_p will return 0.
   It's also used to handle dllimport override semantics.  */
#define REDO_SECTION_INFO_P(DECL) \
  ((DECL_MACHINE_ATTRIBUTES (DECL) != NULL_TREE) \
   || (TREE_CODE (DECL) == VAR_DECL && DECL_VIRTUAL_P (DECL)))

/* Used only here locally.  If the decl has been exported, emit the
   necessary assembly.  */

#define ASM_EXPORT_DECL(FILE, NAME, DECL)				\
  do {									\
    if ((DECL) && i386_pe_dllexport_p (DECL))				\
      {									\
	fprintf ((FILE), ".section .exports\n");			\
	fprintf ((FILE), "\t%s\t\"", STRING_ASM_OP);			\
        assemble_name (FILE, NAME);					\
	fprintf ((FILE), "\"\n.previous\n");				\
      }									\
  } while (0)

/* Write the extra assembler code needed to declare a function properly.
   Some svr4 assemblers need to also have something extra said about the
   function's return value.  We allow for that here.  */

#undef ASM_DECLARE_FUNCTION_NAME
#define ASM_DECLARE_FUNCTION_NAME(FILE, NAME, DECL)			\
  do {									\
    ASM_EXPORT_DECL(FILE, NAME, DECL);					\
    fprintf (FILE, "\t%s\t ", TYPE_ASM_OP);				\
    assemble_name (FILE, NAME);						\
    putc (',', FILE);							\
    fprintf (FILE, TYPE_OPERAND_FMT, "function");			\
    putc ('\n', FILE);							\
    ASM_DECLARE_RESULT (FILE, DECL_RESULT (DECL));			\
    ASM_OUTPUT_LABEL(FILE, NAME);					\
  } while (0)

/* Write the extra assembler code needed to declare an object properly.  */

#undef ASM_DECLARE_OBJECT_NAME
#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL)			\
  do {									\
    ASM_EXPORT_DECL(FILE, NAME, DECL);					\
    fprintf (FILE, "\t%s\t ", TYPE_ASM_OP);				\
    assemble_name (FILE, NAME);						\
    putc (',', FILE);							\
    fprintf (FILE, TYPE_OPERAND_FMT, "object");				\
    putc ('\n', FILE);							\
    size_directive_output = 0;						\
    if (!flag_inhibit_size_directive && DECL_SIZE (DECL))		\
      {									\
        size_directive_output = 1;					\
        fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);				\
        assemble_name (FILE, NAME);					\
        putc (',', FILE);						\
        fprintf (FILE, HOST_WIDE_INT_PRINT_DEC,				\
                 int_size_in_bytes (TREE_TYPE (DECL)));			\
        fputc ('\n', FILE);						\
      }									\
    ASM_OUTPUT_LABEL(FILE, NAME);					\
  } while (0)

/* Similarly for COMMON symbols.  */

#undef ASM_OUTPUT_ALIGNED_COMMON
#undef ASM_OUTPUT_ALIGNED_DECL_COMMON
#define ASM_OUTPUT_ALIGNED_DECL_COMMON(FILE, DECL, NAME, SIZE, ALIGN)	\
  do {									\
    ASM_EXPORT_DECL(FILE, NAME, DECL);					\
    fprintf ((FILE), "\t%s\t", COMMON_ASM_OP);				\
    assemble_name ((FILE), (NAME));					\
    fprintf ((FILE), ",%u,%u\n", (SIZE), (ALIGN) / BITS_PER_UNIT);	\
  } while (0)

#undef ASM_OUTPUT_ALIGNED_LOCAL
#undef ASM_OUTPUT_ALIGNED_DECL_LOCAL
#define ASM_OUTPUT_ALIGNED_DECL_LOCAL(FILE, DECL, NAME, SIZE, ALIGN)	     \
  do {									     \
    fprintf ((FILE), "\t%s\t", LOCAL_ASM_OP);				     \
    assemble_name ((FILE), (NAME));					     \
    fprintf ((FILE), "\n");						     \
    ASM_OUTPUT_ALIGNED_DECL_COMMON((FILE), (DECL), (NAME), (SIZE), (ALIGN)); \
  } while (0)

/* Haiku's default stack size for a process' main thread is 16 MB. That's
   definitely not "small". */

#undef SMALL_STACK

/* For native compiler, use standard Haiku include file search paths
   rooted in /boot/develop/headers.  For a cross compiler, don't
   expect the host to use the Haiku directory scheme, and instead look
   for the Haiku include files relative to TOOL_INCLUDE_DIR.  Yes, we
   use ANSI string concatenation here (FIXME) */

#ifndef CROSS_COMPILE
#undef INCLUDE_DEFAULTS
#define INCLUDE_DEFAULTS \
    { \
    { GPLUSPLUS_INCLUDE_DIR, "G++", 1, 1 },\
    { GCC_INCLUDE_DIR, "GCC", 0, 0 },\
    { TOOL_INCLUDE_DIR, "BINUTILS", 0, 1}, \
    { "/boot/develop/headers/be/add-ons/graphics", 0, 0, 0 },\
    { "/boot/develop/headers/be/devel", 0, 0, 0 },\
    { "/boot/develop/headers/be/translation", 0, 0, 0 },\
    { "/boot/develop/headers/be/mail", 0, 0, 0 },\
    { "/boot/develop/headers/gnu", 0, 0, 0 },\
    { "/boot/develop/headers/be/drivers", 0, 0, 0 },\
    { "/boot/develop/headers/be/opengl", 0, 0, 0 },\
    { "/boot/develop/headers/be/game", 0, 0, 0 },\
    { "/boot/develop/headers/be/support", 0, 0, 0 },\
    { "/boot/develop/headers/be/storage", 0, 0, 0 },\
    { "/boot/develop/headers/be/kernel", 0, 0, 0 },\
    { "/boot/develop/headers/be/net", 0, 0, 0 },\
    { "/boot/develop/headers/be/midi", 0, 0, 0 },\
    { "/boot/develop/headers/be/midi2", 0, 0, 0 },\
    { "/boot/develop/headers/be/media", 0, 0, 0 },\
    { "/boot/develop/headers/be/interface", 0, 0, 0 },\
    { "/boot/develop/headers/be/device", 0, 0, 0 },\
    { "/boot/develop/headers/be/app", 0, 0, 0 },\
    { "/boot/develop/headers/be/precompiled", 0, 0, 0 },\
    { "/boot/develop/headers/be/add-ons/input_server", 0, 0, 0 },\
    { "/boot/develop/headers/be/add-ons/net_server", 0, 0, 0 },\
    { "/boot/develop/headers/be/add-ons/screen_saver", 0, 0, 0 },\
    { "/boot/develop/headers/be/add-ons/tracker", 0, 0, 0 },\
    { "/boot/develop/headers/be/be_apps/Deskbar", 0, 0, 0 },\
    { "/boot/develop/headers/be/be_apps/NetPositive", 0, 0, 0 },\
    { "/boot/develop/headers/be/be_apps/Tracker", 0, 0, 0 },\
    { "/boot/develop/headers/be/drivers/tty", 0, 0, 0 },\
    { "/boot/develop/headers/be/net/netinet", 0, 0, 0 },\
    { "/boot/develop/headers/be/storage", 0, 0, 0 },\
    { "/boot/develop/headers/be", 0, 0, 0 },\
    { "/boot/develop/headers/cpp", 0, 0, 0 },\
    { "/boot/develop/headers/posix", 0, 0, 0 },\
    { "/boot/develop/headers", 0, 0, 0 }, \
    { 0, 0, 0, 0 } \
    };
#else /* CROSS_COMPILE */
#undef	INCLUDE_DEFAULTS
#define INCLUDE_DEFAULTS				\
    { \
    { GPLUSPLUS_INCLUDE_DIR, "G++", 1, 1 },\
    { GCC_INCLUDE_DIR, "GCC", 0, 0 },\
    { TOOL_INCLUDE_DIR, "BINUTILS", 0, 1}, \
    { CROSS_INCLUDE_DIR "/be/add-ons/graphics", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/devel", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/translation", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/mail", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/gnu", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/drivers", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/opengl", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/game", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/support", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/storage", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/kernel", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/net", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/midi", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/midi2", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/media", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/interface", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/device", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/app", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/precompiled", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/add-ons/input_server", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/add-ons/net_server", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/add-ons/screen_saver", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/add-ons/tracker", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/be_apps/Deskbar", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/be_apps/NetPositive", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/be_apps/Tracker", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/drivers/tty", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/net/netinet", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be/storage", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/be", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/cpp", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR "/posix", 0, 0, 0 },\
    { CROSS_INCLUDE_DIR , 0, 0, 0 }, \
    { 0, 0, 0, 0 } \
    };
#endif

/* Whee.  LIBRARY_PATH is Haiku's LD_LIBRARY_PATH, which of course will
   cause nasty problems if we override it.  */
#define LIBRARY_PATH_ENV        "BELIBRARIES"

/* Haiku doesn't have a separate math library.  */
#define MATH_LIBRARY ""

/* Haiku headers are C++-aware (and often use C++).  */
#define NO_IMPLICIT_EXTERN_C

/* Define this macro if in some cases global symbols from one translation
   unit may not be bound to undefined symbols in another translation unit
   without user intervention.  For instance, under Microsoft Windows
   symbols must be explicitly imported from shared libraries (DLLs).  */
/* [zooey]:
	I believe BeOS doesn't actually require this, and *not* using it
   yields considerably smaller object files.
   That's why we do not follow the configuration from gnupro-0002224
   and go with a single symbol space instead: */
/* [zooey]:
	After more research, it turned out that with -O0, we require MULTIPLE
	SYMBOL_SPACES, but with active optimization, we don't.
	(We get link-errors about BView::SetHighColor(ui,ui,ui,ui) missing
	when optimization is off).
	So we do just that, we replace MULTIPLE_SYMBOL_SPACES with a
	test for optimization (see lex.c, line 4882ff and decl2.c, line 2701).
*/
/* [bonefish]:
    Review! */

#undef MULTIPLE_SYMBOL_SPACES

/* I'm not sure why this is defined this way, but pavel@be.com can
   probably provide some insight.  :-)  */
#define LIBSTDCXX "-lstdc++.r4"

/* A C statement to output assembler commands which will identify the object
   file as having been compiled with GNU CC.  This isn't needed for Haiku
   because we use DWARF and DWARF has an DW_AT_producer tag that does the
   same thing.  Gdb is probably the only debugger that uses this anyway,
   so the most likely ill effect of disabling this is that a BeOS gdb
   debugging an executable with no debug info would not see the gcc2_compiled
   label. */
/* [bonefish]:
    Review! */
#undef ASM_IDENTIFY_GCC
#define ASM_IDENTIFY_GCC(FILE)

/* required to compile winnt.c */
/* [bonefish]:
    Review! */
#define I386_PE_STRIP_ENCODING(SYM_NAME) \
  ((SYM_NAME) + ((SYM_NAME)[0] == '@' ? 3 : 0))

#define DRECTVE_SECTION_FUNCTION \
void									\
drectve_section ()							\
{									\
  if (in_section != in_drectve)						\
    {									\
      fprintf (asm_out_file, "%s\n", "\t.section .drectve\n");		\
      in_section = in_drectve;						\
    }									\
}

#undef EXTRA_SECTION_FUNCTIONS
#define EXTRA_SECTION_FUNCTIONS						\
  CONST_SECTION_FUNCTION						\
  CTORS_SECTION_FUNCTION						\
  DTORS_SECTION_FUNCTION						\
  DRECTVE_SECTION_FUNCTION

#undef EXTRA_SECTIONS
#define EXTRA_SECTIONS in_const, in_ctors, in_dtors, in_drectve

/* [zooey]:
   '#pragma interface' seems to trigger a bug which causes
   global constructors and destructors (keyed to the first function in
   a translation unit) not being generated, causing the link to fail.
   So we ignore '#pragma interface' for Haiku.
   All this applies only when multiple symbol spaces are active (i.e.
   if no optimization is done).
   */
#define WORK_AROUND_PRAGMA_INTERFACE_BUG 1

/* The origin of Haiku's gcc is egcs 1.1.x, and these version had a bug
   in mangled name builder. The APIs of Haiku are furnished to the
   developers as shared library of C++. And if we use fixed mangled
   names in newer gcc, some APIs are hidden with name problem. */
#define USE_EGCS_MANGLED_NAMES
