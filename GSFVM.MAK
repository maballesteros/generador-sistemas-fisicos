PROJ	=GSFVM
DEBUG	=0
CC	=qcl
AS	=qcl
CFLAGS_G	= /AS /W1 /Ze 
CFLAGS_D	= /Zi /Zr /Gi$(PROJ).mdt /Od 
CFLAGS_R	= /O /Ot /DNDEBUG 
CFLAGS	=$(CFLAGS_G) $(CFLAGS_R)
AFLAGS_G	=/Cx /P1
AFLAGS_D	=/Zi
AFLAGS_R	=/DNDEBUG
AFLAGS	=$(AFLAGS_G) $(AFLAGS_R)
LFLAGS_G	= /CP:0xfff /NOI /SE:0x80 /ST:0x800 
LFLAGS_D	= /CO 
LFLAGS_R	= 
LFLAGS	=$(LFLAGS_G) $(LFLAGS_R)
RUNFLAGS	=sis001.fms -t -v
OBJS_EXT = 	
LIBS_EXT = 	

.asm.obj: ; $(AS) $(AFLAGS) -c $*.asm

all:	$(PROJ).EXE

gsfvm_ma.obj:	gsfvm_ma.c

gsfvm_ga.obj:	gsfvm_ga.c

gsfvm_fi.obj:	gsfvm_fi.c

gsfvm_me.obj:	gsfvm_me.c

gsfvm_le.obj:	gsfvm_le.c

gsfvm_rp.obj:	gsfvm_rp.c

$(PROJ).EXE:	gsfvm_ma.obj gsfvm_ga.obj gsfvm_fi.obj gsfvm_me.obj gsfvm_le.obj gsfvm_rp.obj $(OBJS_EXT)
	echo >NUL @<<$(PROJ).crf
gsfvm_ma.obj +
gsfvm_ga.obj +
gsfvm_fi.obj +
gsfvm_me.obj +
gsfvm_le.obj +
gsfvm_rp.obj +
$(OBJS_EXT)
$(PROJ).EXE

$(LIBS_EXT);
<<
	link $(LFLAGS) @$(PROJ).crf

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)

