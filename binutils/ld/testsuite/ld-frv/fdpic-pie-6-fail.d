#name: FRV uClinux PIC relocs to undefined symbols, pie linking
#source: fdpic6.s
#objdump: -DR -j .text -j .data -j .got -j .plt
#as: -mfdpic
#ld: -pie
#error: different segments
