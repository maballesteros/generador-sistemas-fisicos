PROJ	=GSFMS
DEBUG	=0
CC	=qcl
AS	=qcl
CFLAGS_G	= /AS /W1 /Ze 
CFLAGS_D	= /Zi /Zr /Gi$(PROJ).mdt /Od 
CFLAGS_R	= /O /Ot /DNDEBUG 
CFLAGS	=$(CFLAGS_G) $(CFLAGS_R)
AFLAGS_G	= /Cx /W1 /P1 
AFLAGS_D	= /Zi 
AFLAGS_R	= /DNDEBUG 
AFLAGS	=$(AFLAGS_G) $(AFLAGS_R)
LFLAGS_G	= /CP:0xfff /NOI /SE:0x80 /ST:0x800 
LFLAGS_D	= /CO 
LFLAGS_R	= 
LFLAGS	=$(LFLAGS_G) $(LFLAGS_R)
RUNFLAGS	=sis002.fsf /t
OBJS_EXT = 	
LIBS_EXT = 	

.asm.obj: ; $(AS) $(AFLAGS) -c $*.asm

all:	$(PROJ).EXE

gsfms_ma.obj:	gsfms_ma.c

gsfms_ga.obj:	gsfms_ga.c

gsfms_fi.obj:	gsfms_fi.c

gsfms_me.obj:	gsfms_me.c

gsfms_le.obj:	gsfms_le.c

gsfms_ms.obj:	gsfms_ms.c

$(PROJ).EXE:	gsfms_ma.obj gsfms_ga.obj gsfms_fi.obj gsfms_me.obj gsfms_le.obj gsfms_ms.obj $(OBJS_EXT)
	echo >NUL @<<$(PROJ).crf
gsfms_ma.obj +
gsfms_ga.obj +
gsfms_fi.obj +
gsfms_me.obj +
gsfms_le.obj +
gsfms_ms.obj +
$(OBJS_EXT)
$(PROJ).EXE

$(LIBS_EXT);
<<
	link $(LFLAGS) @$(PROJ).crf

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)

