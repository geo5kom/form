/*
  	#[ includes :
*/

#include "form3.h"
 
static KEYWORD formatoptions[] = {
	 {"c",				(TFUN)0,	CMODE,				0}
	,{"doublefortran",	(TFUN)0,	DOUBLEFORTRANMODE,	0}
	,{"float",			(TFUN)0,	0,					2}
	,{"fortran",		(TFUN)0,	FORTRANMODE,		0}
	,{"maple",			(TFUN)0,	MAPLEMODE,			0}
	,{"mathematica",	(TFUN)0,	MATHEMATICAMODE,	0}
	,{"nospaces",		(TFUN)0,	NOSPACEFORMAT,		3}
	,{"pfortran",		(TFUN)0,	PFORTRANMODE,		0}
	,{"rational",		(TFUN)0,	RATIONALMODE,		1}
	,{"reduce",			(TFUN)0,	REDUCEMODE,			0}
	,{"spaces",			(TFUN)0,	NORMALFORMAT,		3}
	,{"vortran",		(TFUN)0,	VORTRANMODE,		0}
};

static KEYWORD trace4options[] = {
	 {"contract",    (TFUN)0,	CHISHOLM,		0          }
	,{"nocontract",  (TFUN)0,	0,				CHISHOLM   }
	,{"nosymmetrize",(TFUN)0,	0,				ALSOREVERSE}
	,{"notrick",     (TFUN)0,	NOTRICK,		0          }
	,{"symmetrize",  (TFUN)0,	ALSOREVERSE,	0          }
	,{"trick",       (TFUN)0,	0,				NOTRICK    }
};

static KEYWORD chisoptions[] = {
	 {"nosymmetrize",(TFUN)0,	0,				ALSOREVERSE}
	,{"symmetrize",  (TFUN)0,	ALSOREVERSE,	0          }
};

static KEYWORD writeoptions[] = {
	 {"stats",			(TFUN)&(AC.StatsFlag),	1,		0}
	,{"statistics",		(TFUN)&(AC.StatsFlag),	1,		0}
	,{"shortstats",		(TFUN)&(AC.ShortStats),	1,		0}
	,{"shortstatistics",(TFUN)&(AC.ShortStats),	1,		0}
	,{"warnings",		(TFUN)&(AC.WarnFlag),	1,		0}
	,{"allwarnings",	(TFUN)&(AC.WarnFlag),	2,		0}
	,{"setup",			(TFUN)&(AC.SetupFlag),	1,		0}
	,{"names",			(TFUN)&(AC.NamesFlag),	1,		0}
	,{"allnames",		(TFUN)&(AC.NamesFlag),	2,		0}
	,{"codes",			(TFUN)&(AC.CodesFlag),	1,		0}
	,{"highfirst",		(TFUN)&(AC.SortType),	SORTHIGHFIRST,		SORTLOWFIRST}
	,{"lowfirst",		(TFUN)&(AC.SortType),	SORTLOWFIRST,		SORTHIGHFIRST}
	,{"powerfirst",		(TFUN)&(AC.SortType),	SORTPOWERFIRST,		SORTHIGHFIRST}
	,{"tokens",			(TFUN)&(AC.TokensWriteFlag),1,	0}
};

static KEYWORD onoffoptions[] = {
	 {"compress",       (TFUN)&(AC.NoCompress),  0,  1}
	,{"insidefirst",	(TFUN)&(AC.insidefirst), 1,  0}
	,{"propercount",    (TFUN)&(AC.BottomLevel), 1,  0}
	,{"stats",			(TFUN)&(AC.StatsFlag),	1,	0}
	,{"statistics",		(TFUN)&(AC.StatsFlag),	1,	0}
	,{"shortstats",		(TFUN)&(AC.ShortStats),	1,	0}
	,{"shortstatistics",(TFUN)&(AC.ShortStats),	1,	0}
	,{"names",			(TFUN)&(AC.NamesFlag),	1,	0}
	,{"allnames",		(TFUN)&(AC.NamesFlag),	2,	0}
	,{"warnings",		(TFUN)&(AC.WarnFlag),	1,	0}
	,{"allwarnings",	(TFUN)&(AC.WarnFlag),	2,	0}
	,{"highfirst",		(TFUN)&(AC.SortType),	SORTHIGHFIRST,	SORTLOWFIRST}
	,{"lowfirst",		(TFUN)&(AC.SortType),	SORTLOWFIRST,	SORTHIGHFIRST}
	,{"powerfirst",		(TFUN)&(AC.SortType),	SORTPOWERFIRST,	SORTHIGHFIRST}
	,{"setup",			(TFUN)&(AC.SetupFlag),	1,	0}
	,{"codes",			(TFUN)&(AC.CodesFlag),	1,	0}
	,{"tokens",		    (TFUN)&(AC.TokensWriteFlag),1,0}
	,{"properorder",    (TFUN)&(AC.properorderflag),1,0}
	,{"threadloadbalancing",(TFUN)&(AC.ThreadBalancing),1,	0}
	,{"threads",		(TFUN)&(AC.ThreadsFlag),1,	0}
	,{"threadsortfilesynch",(TFUN)&(AC.ThreadSortFileSynch),1,  0}
	,{"threadstats",	(TFUN)&(AC.ThreadStats),1,	0}
	,{"finalstats",	    (TFUN)&(AC.FinalStats),1,	0}
	/*[30jan2004 mt]:*/
	/*,{"parallel",	    (TFUN)&(AC.parallelflag),0,1}*/
	,{"parallel",	    (TFUN)&(AC.parallelflag),PARALLELFLAG,NOPARALLEL_USER}
	/*:[30jan2004 mt]*/
};

static WORD one = 1;

/*
  	#] includes : 
  	#[ CoCollect :

	Collect,functionname
*/

int
CoCollect ARG1(UBYTE *,s)
{
/*	--------------change 17-feb-2003 Added percentage */
	WORD numfun;
	int type,x = 0;
	UBYTE *t = SkipAName(s), *t1, *t2;
	AC.AltCollectFun = 0;
	if ( t == 0 ) goto syntaxerror;
	t1 = t; while ( *t1 == ',' || *t1 == ' ' || *t1 == '\t' ) t1++;
	*t = 0; t = t1;
	if ( *t1 && ( FG.cTable[*t1] == 0 || *t1 == '[' ) ) {
		t2 = SkipAName(t1);
		if ( t2 == 0 ) goto syntaxerror;
		t = t2;
		while ( *t == ',' || *t == ' ' || *t == '\t' ) t++;
		*t2 = 0;
	}
	else t1 = 0;
	if ( *t && FG.cTable[*t] == 1 ) {
		while ( *t >= '0' && *t <= '9' ) x = 10*x + *t++ - '0';
		if ( x > 100 ) x = 100;
		while ( *t == ',' || *t == ' ' || *t == '\t' ) t++;
		if ( *t ) goto syntaxerror;
	}
	else {
		if ( *t ) goto syntaxerror;
		x = 100;
	}
	if ( ( ( type = GetName(AC.varnames,s,&numfun,WITHAUTO) ) != CFUNCTION )
	|| ( functions[numfun].spec != 0 ) ) {
		MesPrint("&%s should be a regular function",s);
		if ( type < 0 ) {
			if ( GetName(AC.exprnames,s,&numfun,NOAUTO) == NAMENOTFOUND )
				AddFunction(s,0,0,0,0);
		}
		return(1);
	}
	AC.CollectFun = numfun+FUNCTION;
	AC.CollectPercentage = (WORD)x;
	if ( t1 ) {
		if ( ( ( type = GetName(AC.varnames,t1,&numfun,WITHAUTO) ) != CFUNCTION )
		|| ( functions[numfun].spec != 0 ) ) {
			MesPrint("&%s should be a regular function",t1);
			if ( type < 0 ) {
				if ( GetName(AC.exprnames,t1,&numfun,NOAUTO) == NAMENOTFOUND )
					AddFunction(t1,0,0,0,0);
			}
			return(1);
		}
		AC.AltCollectFun = numfun+FUNCTION;
	}
	return(0);
syntaxerror:
	MesPrint("&Collect statement needs one or two functions (and a percentage) for its argument(s)");
	return(1);
}

/*
  	#] CoCollect : 
  	#[ setonoff :
*/

int
setonoff ARG4(UBYTE *,s,int *,flag,int,onvalue,int,offvalue)
{
	if ( StrICmp(s,(UBYTE *)"on") == 0 ) *flag = onvalue;
	else if ( StrICmp(s,(UBYTE *)"off") == 0 ) *flag = offvalue;
	else {
		MesPrint("&Unknown option: %s, on or off expected",s);
		return(1);
	}
	return(0);
}

/*
  	#] setonoff : 
  	#[ CoCompress :
*/

int CoCompress ARG1(UBYTE *,s) {
	GETIDENTITY
	UBYTE *t, c;
	if ( StrICmp(s,(UBYTE *)"on") == 0 ) {
		AC.NoCompress = 0;
		AR.gzipCompress = GZIPDEFAULT;
	}
	else if ( StrICmp(s,(UBYTE *)"off") == 0 ) {
		AC.NoCompress = 1;
		AR.gzipCompress = 0;
	}
	else {
		t = s; while ( FG.cTable[*t] <= 1 ) t++;
		c = *t; *t = 0;
		if ( StrICmp(s,(UBYTE *)"gzip") == 0 ) {
#ifndef WITHZLIB
			Warning("gzip compression not supported on this platform");
#endif
			s = t; *s = c;
			if ( *s == 0 ) {
				AR.gzipCompress = GZIPDEFAULT;  /* Normally should be 6 */
				return(0);
			}
			while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
			t = s;
			if ( FG.cTable[*s] == 1 ) {
				AR.gzipCompress = *s - '0';
				s++;
				while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
				if ( *s == 0 ) return(0);
			}
			MesPrint("&Unknown gzip option: %s, a digit was expected",t);
			return(1);

		}
		else {
			MesPrint("&Unknown option: %s, on, off or gzip expected",s);
			return(1);
		}
	}
	return(0);
}

/*
  	#] CoCompress : 
  	#[ CoOff :
*/

int CoOff ARG1(UBYTE *,s)
{
	GETIDENTITY
	UBYTE *t, c;
	int i, num = sizeof(onoffoptions)/sizeof(KEYWORD);
	for (;;) {
		while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
		if ( *s == 0 ) return(0);
		if ( chartype[*s] != 0 ) {
			MesPrint("&Illegal character or option encountered in OFF statement");
			return(-1);
		}
		t = s;	while ( chartype[*s] == 0 ) s++;
		c = *s; *s = 0;
		for ( i = 0; i < num; i++ ) {
			if ( StrICont(t,(UBYTE *)(onoffoptions[i].name)) == 0 ) break;
		}
		if ( i >= num ) {
			MesPrint("&Unrecognized option in OFF statement: %s",t);
			*s = c; return(-1);
		}
		if ( StrICont(t,"compress") == 0 ) {
			AR.gzipCompress = 0;
		}
		if ( StrICont(t,"threads") == 0 ) {
			AS.MultiThreaded = 0;
		}
		*s = c;
	 	*((int *)(onoffoptions[i].func)) = onoffoptions[i].flags; 
	}
}

/*
  	#] CoOff : 
  	#[ CoOn :
*/

int CoOn ARG1(UBYTE *,s)
{
	GETIDENTITY
	UBYTE *t, c;
	int i, num = sizeof(onoffoptions)/sizeof(KEYWORD);
	for (;;) {
		while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
		if ( *s == 0 ) return(0);
		if ( chartype[*s] != 0 ) {
			MesPrint("&Illegal character or option encountered in ON statement");
			return(-1);
		}
		t = s;	while ( chartype[*s] == 0 ) s++;
		c = *s; *s = 0;
		for ( i = 0; i < num; i++ ) {
			if ( StrICont(t,(UBYTE *)(onoffoptions[i].name)) == 0 ) break;
		}
		if ( i >= num ) {
			MesPrint("&Unrecognized option in ON statement: %s",t);
			*s = c; return(-1);
		}
		if ( StrICont(t,"compress") == 0 ) {
			AR.gzipCompress = GZIPDEFAULT;
			*s = c;
			while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
			if ( *s ) {
			  t = s;
			  while ( FG.cTable[*s] <= 1 ) s++;
			  c = *s; *s = 0;
			  if ( StrICmp(t,"gzip") == 0 ) {}
			  else {
				MesPrint("&Unrecognized option in ON compress statement: %s",t);
				return(-1);
			  }
			  *s = c;
			  while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
#ifndef WITHZLIB
			  Warning("gzip compression not supported on this platform");
#endif
			  if ( FG.cTable[*s] == 1 ) {
				AR.gzipCompress = *s++ - '0';
				while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
				if ( *s ) {
					MesPrint("&Unrecognized option in ON compress gzip statement: %s",t);
					return(-1);
				}
			  }
			  else if ( *s ) {
				MesPrint("&Unrecognized option in ON compress gzip statement: %s, single digit expected",t);
				return(-1);
			  }
			}
		}
		else { *s = c; }
		if ( StrICont(t,"threads") == 0 ) {
			if ( AM.totalnumberofthreads > 1 ) AS.MultiThreaded = 1;
		}
	 	*((int *)(onoffoptions[i].func)) = onoffoptions[i].type; 
	}
}

/*
  	#] CoOn : 
  	#[ CoInsideFirst :
*/

int CoInsideFirst ARG1(UBYTE *,s) { return(setonoff(s,&AC.insidefirst,1,0)); }

/*
  	#] CoInsideFirst : 
  	#[ CoProperCount :
*/

int CoProperCount ARG1(UBYTE *,s) { return(setonoff(s,&AC.BottomLevel,1,0)); }

/*
  	#] CoProperCount : 
  	#[ CoDelete :
*/

int
CoDelete ARG1(UBYTE *,s)
{
	if ( StrICmp(s,(UBYTE *)"storage") == 0 ) {
		if ( DeleteStore(1) < 0 ) {
			MesPrint("&Cannot restart storage file");
			return(1);
		}
		return(0);
	}
	else {
		MesPrint("&Unknown option: %s",s);
		return(1);
	}
}

/*
  	#] CoDelete : 
  	#[ CoFormat :
*/

int
CoFormat ARG1(UBYTE *,s)
{
	int error = 0, x;
	KEYWORD *key;
	if ( FG.cTable[*s] == 1 ) {
		x = 0;
		while ( FG.cTable[*s] == 1 ) x = 10*x + *s++ - '0';
		if ( x <= 0 || x > 255 ) {
			x = 72;
			error = 1;
			MesPrint("&Illegal value for linesize: %d",x);
		}
		if ( x < 39 ) {
			MesPrint(" ... Too small value for linesize corrected to 39");
			x = 39;
		}
		AO.DoubleFlag = 0;
		AC.OutputMode = 0;
		AC.LineLength = x;
		if ( *s != 0 ) {
			error = 1;
			MesPrint("&Illegal linesize field in format statement");
		}
	}
	else {
		key = FindKeyWord(s,formatoptions,
			sizeof(formatoptions)/sizeof(KEYWORD));
		if ( key ) {
			if ( key->flags == 0 ) {
				AO.DoubleFlag = 0;
				AC.OutputMode = key->type & NODOUBLEMASK;
				if ( ( key->type & DOUBLEPRECISIONFLAG ) != 0 ) {
					AO.DoubleFlag = 1;
				}
			}
			else if ( key->flags == 1 ) AC.OutNumberType = key->type;
			else if ( key->flags == 2 ) {
				while ( FG.cTable[*s] == 0 ) s++;
				if ( *s == 0 ) AC.OutNumberType = 10;
				else if ( *s == ',' ) {
					s++;
					x = 0;
					while ( FG.cTable[*s] == 1 ) x = 10*x + *s++ - '0';
					if ( *s != 0 ) {
						error = 1;
						MesPrint("&Illegal float format specifier");
					}
					else {
						if ( x < 3 ) {
							x = 3;
							MesPrint("& ... float format value corrected to 3");
						}
						if ( x > 100 ) {
							x = 100;
							MesPrint("& ... float format value corrected to 100");
						}
						AC.OutNumberType = x;
					}
				}
			}
			else if ( key->flags == 3 ) {
				AC.OutputSpaces = key->type;
			}
		}
		else { MesPrint("&Unknown option: %s",s); error = 1; }
		AC.LineLength = 72;
	}
	return(error);
}

/*
  	#] CoFormat : 
  	#[ CoKeep :
*/

int
CoKeep ARG1(UBYTE *,s)
{
	if ( StrICmp(s,(UBYTE *)"brackets") == 0 ) AC.ComDefer = 1;
	else { MesPrint("&Unknown option: '%s'",s); return(1); }
	return(0);
}

/*
  	#] CoKeep : 
  	#[ CoFixIndex :
*/

int
CoFixIndex ARG1(UBYTE *,s)
{
	int x, y, error = 0;
	while ( *s ) {
		if ( FG.cTable[*s] != 1 ) {
proper:		MesPrint("&Proper syntax is: FixIndex,number:value[,number,value];");
			return(1);
		}
		ParseNumber(x,s)
		if ( *s != ':' ) goto proper;
		s++;
		if ( *s != '-' && *s != '+' && FG.cTable[*s] != 1 ) goto proper;
		ParseSignedNumber(y,s)
		if ( *s && *s != ',' ) goto proper;
		while ( *s == ',' ) s++;
		if ( x >= AM.OffsetIndex ) {
			MesPrint("&Fixed index out of allowed range. Change ConstIndex in setup file?");
			MesPrint("&Current value of ConstIndex = %d",AM.OffsetIndex-1);
			error = 1;
		}
		if ( y != (int)((WORD)y) ) {
			MesPrint("&Value of d_(%d,%d) outside range for this computer",x,x);
			error = 1;
		}
		if ( error == 0 ) AC.FixIndices[x] = y;
	}
	return(error);
}

/*
  	#] CoFixIndex : 
  	#[ CoMetric :
*/

int
CoMetric ARG1(UBYTE *,s)
{ MesPrint("&The metric statement does not do anything yet"); return(1); }

/*
  	#] CoMetric : 
  	#[ DoPrint :
*/

int
DoPrint ARG2(UBYTE *,s,int,par)
{
	int i, error = 0, numdol = 0, type;
	UBYTE *name, c, *t;
	EXPRESSIONS e;
	WORD numexpr, tofile = 0;
	CBUF *C = cbuf + AC.cbufnum;
	while ( *s == ',' ) s++;
	if ( s[-1] == '+' || s[-1] == '-' ) s--;
	if ( *s == '+' && ( s[1] == 'f' || s[1] == 'F' ) ) {
		t = s + 2; while ( *t == ' ' || *t == ',' ) t++;
		if ( *t == '"' ) {
			tofile = 1;
			s = t;
		}
	}
	if ( par == PRINTON && *s == '"' ) {
		WORD code;
		if ( tofile == 1 ) code = TYPEFPRINT;
		else code = TYPEPRINT;
		s++; name = s;
		while ( *s && *s != '"' ) {
			if ( *s == '\\' ) s++;
			if ( *s == '%' && s[1] == '$' ) numdol++;
			s++;
		}
		if ( *s != '"' ) {
			MesPrint("&String in print statement should be enclosed in \"");
			return(1);
		}
		*s = 0;
		AddComString(1,&code,name,1);
		*s++ = '"';
		while ( *s == ',' ) {
			s++;
			if ( *s == '$' ) {
				s++; name = s; while ( FG.cTable[*s] <= 1 ) s++;
				c = *s; *s = 0;
				type = GetName(AC.dollarnames,name,&numexpr,NOAUTO);
				if ( type == NAMENOTFOUND ) {
					MesPrint("&$ variable %s not (yet) defined",name);
					error = 1;
				}
				else {
					C->lhs[C->numlhs][1] += 2;
					*(C->Pointer)++ = DOLLAREXPRESSION;
					*(C->Pointer)++ = numexpr;
					numdol--;
				}
			}
			else {
				MesPrint("&Illegal object in print statement");
				error = 1;
				return(error);
			}
			*s = c;
		}
		if ( *s != 0 ) {
			MesPrint("&Illegal object in print statement");
			error = 1;
		}
		if ( numdol > 0 ) {
			MesPrint("&More $ variables asked for than provided");
			error = 1;
		}
		return(error);
	}
	if ( *s == 0 ) {	/* All active expressions */
AllExpr:
		for ( e = Expressions, i = NumExpressions; i > 0; i--, e++ ) {
            if ( e->status == LOCALEXPRESSION || e->status ==
            GLOBALEXPRESSION || e->status == UNHIDELEXPRESSION
			|| e->status == UNHIDEGEXPRESSION ) e->printflag = par;
        }
		return(error);
	}
	while ( *s ) {
		if ( *s == '+' ) {
			s++;
			if ( tolower(*s) == 'f' ) par |= PRINTLFILE;
			else if ( tolower(*s) == 's' ) {
				if ( ( par & 3 ) < 2 ) par |= PRINTONETERM;
			}
			else {
illeg:			MesPrint("&Illegal option in (n)print statement");
				error = 1;
			}
			s++;
			if ( *s == 0 ) goto AllExpr;
		}
		else if ( *s == '-' ) {
			s++;
			if ( tolower(*s) == 'f' ) par &= ~PRINTLFILE;
			else if ( tolower(*s) == 's' ) {
				if ( ( par & 3 ) < 0 ) par &= ~PRINTONETERM;
			}
			else goto illeg;
			s++;
			if ( *s == 0 ) goto AllExpr;
		}
		else if ( FG.cTable[*s] == 0 || *s == '[' ) {
			name = s;
			if ( ( s = SkipAName(s) ) == 0 ) {
				MesPrint("&Improper name in (n)print statement");
				return(1);
			}
			c = *s; *s = 0;
			if ( ( GetName(AC.exprnames,name,&numexpr,NOAUTO) == CEXPRESSION )
			&& ( Expressions[numexpr].status == LOCALEXPRESSION
			|| Expressions[numexpr].status == GLOBALEXPRESSION ) ) {
				if ( c == '[' && s[1] == ']' ) {
					Expressions[numexpr].printflag = par | PRINTCONTENTS;
					*s++ = c; c = *++s;
				}
				else
					Expressions[numexpr].printflag = par;
			}
			else {
				MesPrint("&%s is not the name of an active expression",name);
				error = 1;
			}
			*s++ = c;
			if ( c == 0 ) return(0);
		}
		else if ( *s == ',' ) s++;
		else {
			MesPrint("&Illegal object in (n)print statement");
			return(1);
		} 
	}
	return(0);
}

/*
  	#] DoPrint : 
  	#[ CoPrint :
*/

int
CoPrint ARG1(UBYTE *,s) { return(DoPrint(s,PRINTON)); }

/*
  	#] CoPrint : 
  	#[ CoPrintB :
*/

int
CoPrintB ARG1(UBYTE *,s) { return(DoPrint(s,PRINTCONTENT)); }

/*
  	#] CoPrintB : 
  	#[ CoNPrint :
*/

int
CoNPrint ARG1(UBYTE *,s) { return(DoPrint(s,PRINTOFF)); }

/*
  	#] CoNPrint : 
  	#[ CoPushHide :
*/

int
CoPushHide ARG1(UBYTE *,s)
{
	GETIDENTITY
	WORD *ScratchBuf;
	int i;
	if ( AR.Fscr[2].PObuffer == 0 ) {
		ScratchBuf = (WORD *)Malloc1(AM.ScratSize*sizeof(WORD),"hidesize");
		AR.Fscr[2].POsize = AM.ScratSize * sizeof(WORD);
		AR.Fscr[2].POfull = AR.Fscr[2].POfill = AR.Fscr[2].PObuffer = ScratchBuf;
		AR.Fscr[2].POstop = AR.Fscr[2].PObuffer + AM.ScratSize;
		PUTZERO(AR.Fscr[2].POposition);
	}
	while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
	AC.HideLevel += 2;
	if ( *s ) {
		MesPrint("&PushHide statement should have no arguments");
		return(-1);
	}
	for ( i = 0; i < NumExpressions; i++ ) {
		switch ( Expressions[i].status ) {
			case DROPLEXPRESSION:
	        case SKIPLEXPRESSION:
	        case LOCALEXPRESSION:
				Expressions[i].status = HIDELEXPRESSION;
				Expressions[i].hidelevel = AC.HideLevel-1;
	            break;
			case DROPGEXPRESSION:
	        case SKIPGEXPRESSION:
	        case GLOBALEXPRESSION:
				Expressions[i].status = HIDEGEXPRESSION;
				Expressions[i].hidelevel = AC.HideLevel-1;
	            break;
	        default:
	            break;
		}
	}
	return(0);
}

/*
  	#] CoPushHide : 
  	#[ CoPopHide :
*/

int
CoPopHide ARG1(UBYTE *,s)
{
	int i;
	while ( *s == ' ' || *s == ',' || *s == '\t' ) s++;
	if ( AC.HideLevel <= 0 ) {
		MesPrint("&PopHide statement without corresponding PushHide statement");
		return(-1);
	}
	AC.HideLevel -= 2;
	if ( *s ) {
		MesPrint("&PopHide statement should have no arguments");
		return(-1);
	}
	for ( i = 0; i < NumExpressions; i++ ) {
		switch ( Expressions[i].status ) {
	        case HIDDENLEXPRESSION:
				if ( Expressions[i].hidelevel > AC.HideLevel )
					Expressions[i].status = UNHIDELEXPRESSION;
	            break;
	        case HIDDENGEXPRESSION:
				if ( Expressions[i].hidelevel > AC.HideLevel )
					Expressions[i].status = UNHIDEGEXPRESSION;
	            break;
	        default:
	            break;
		}
	}
	return(0);
}

/*
  	#] CoPopHide : 
  	#[ SetExprCases :
*/

int
SetExprCases ARG3(int,par,int,setunset,int,val)
{
	switch ( par ) {
		case SKIP:
			switch ( val ) {
		        case SKIPLEXPRESSION:
					if ( !setunset ) val = LOCALEXPRESSION;
		            break;
		        case SKIPGEXPRESSION:
					if ( !setunset ) val = GLOBALEXPRESSION;
		            break;
		        case LOCALEXPRESSION:
					if ( setunset ) val = SKIPLEXPRESSION;
		            break;
		        case GLOBALEXPRESSION:
					if ( setunset ) val = SKIPGEXPRESSION;
		            break;
		        default:
		            break;
			}
			break;
		case DROP:
			switch ( val ) {
		        case SKIPLEXPRESSION:
		        case LOCALEXPRESSION:
		        case HIDELEXPRESSION:
					if ( setunset ) val = DROPLEXPRESSION;
		            break;
		        case DROPLEXPRESSION:
					if ( !setunset ) val = LOCALEXPRESSION;
		            break;
		        case SKIPGEXPRESSION:
		        case GLOBALEXPRESSION:
		        case HIDEGEXPRESSION:
					if ( setunset ) val = DROPGEXPRESSION;
		            break;
		        case DROPGEXPRESSION:
					if ( !setunset ) val = GLOBALEXPRESSION;
		            break;
		        case HIDDENLEXPRESSION:
				case UNHIDELEXPRESSION:
					if ( setunset ) val = DROPHLEXPRESSION;
		            break;
		        case HIDDENGEXPRESSION:
				case UNHIDEGEXPRESSION:
					if ( setunset ) val = DROPHGEXPRESSION;
		            break;
		        case DROPHLEXPRESSION:
					if ( !setunset ) val = HIDDENLEXPRESSION;
		            break;
		        case DROPHGEXPRESSION:
					if ( !setunset ) val = HIDDENGEXPRESSION;
		            break;
		        default:
		            break;
			}
			break;
		case HIDE:
			switch ( val ) {
				case DROPLEXPRESSION:
		        case SKIPLEXPRESSION:
		        case LOCALEXPRESSION:
					if ( setunset ) val = HIDELEXPRESSION;
		            break;
		        case HIDELEXPRESSION:
					if ( !setunset ) val = LOCALEXPRESSION;
		            break;
				case DROPGEXPRESSION:
		        case SKIPGEXPRESSION:
		        case GLOBALEXPRESSION:
					if ( setunset ) val = HIDEGEXPRESSION;
		            break;
		        case HIDEGEXPRESSION:
					if ( !setunset ) val = GLOBALEXPRESSION;
		            break;
		        default:
		            break;
			}
			break;
		case UNHIDE:
			switch ( val ) {
		        case HIDDENLEXPRESSION:
		        case DROPHLEXPRESSION:
					if ( setunset ) val = UNHIDELEXPRESSION;
		            break;
				case UNHIDELEXPRESSION:
					if ( !setunset ) val = HIDDENLEXPRESSION;
		            break;
		        case HIDDENGEXPRESSION:
		        case DROPHGEXPRESSION:
					if ( setunset ) val = UNHIDEGEXPRESSION;
		            break;
				case UNHIDEGEXPRESSION:
					if ( !setunset ) val = HIDDENGEXPRESSION;
		            break;
		        default:
		            break;
			}
		default:
			break;
	}
	return(val);
}

/*
  	#] SetExprCases : 
  	#[ SetExpr :
*/

int
SetExpr ARG3(UBYTE *,s,int,setunset,int,par)
{
	WORD *w, numexpr;
	int error = 0, type, i;
	UBYTE *name, c;
	if ( *s == 0 ) {
		for ( i = 0; i < NumExpressions; i++ ) {
			w = &(Expressions[i].status);
			*w = SetExprCases(par,setunset,*w);
			if ( par == HIDE && setunset == 1 )
				Expressions[i].hidelevel = AC.HideLevel;
		}
		return(0);
	}
	while ( *s ) {
		if ( *s == ',' ) { s++; continue; }
		name = s;
		if ( ( s = SkipAName(s) ) == 0 ) {
			MesPrint("&Improper name for an expression: '%s'",name);
			return(1);
		}
		c = *s; *s = 0;
		if ( ( type = GetName(AC.exprnames,name,&numexpr,NOAUTO) ) == CEXPRESSION ) {
			w = &(Expressions[numexpr].status);
			*w = SetExprCases(par,setunset,*w);
			if ( par == HIDE && setunset == 1 )
				Expressions[numexpr].hidelevel = AC.HideLevel;
		}
		else if ( ( type = GetName(AC.varnames,name,&numexpr,NOAUTO) ) != NAMENOTFOUND ) {
			MesPrint("&%s is not an expression",name);
			error = 1;
		}
		*s = c;
	}
	return(0);
}

/*
  	#] SetExpr : 
  	#[ CoDrop :
*/

int
CoDrop ARG1(UBYTE *,s) { return(SetExpr(s,1,DROP)); }

/*
  	#] CoDrop : 
  	#[ CoNoDrop :
*/

int
CoNoDrop ARG1(UBYTE *,s) { return(SetExpr(s,0,DROP)); }

/*
  	#] CoNoDrop : 
  	#[ CoSkip :
*/

int
CoSkip ARG1(UBYTE *,s) { return(SetExpr(s,1,SKIP)); }

/*
  	#] CoSkip : 
  	#[ CoNoSkip :
*/

int
CoNoSkip ARG1(UBYTE *,s) { return(SetExpr(s,0,SKIP)); }

/*
  	#] CoNoSkip : 
  	#[ CoHide :
*/

int CoHide ARG1(UBYTE *,inp) {
	GETIDENTITY
	WORD *ScratchBuf;
	if ( AR.Fscr[2].PObuffer == 0 ) {
		ScratchBuf = (WORD *)Malloc1(AM.ScratSize*sizeof(WORD),"hidesize");
		AR.Fscr[2].POsize = AM.ScratSize * sizeof(WORD);
		AR.Fscr[2].POfull = AR.Fscr[2].POfill = AR.Fscr[2].PObuffer = ScratchBuf;
		AR.Fscr[2].POstop = AR.Fscr[2].PObuffer + AM.ScratSize;
		PUTZERO(AR.Fscr[2].POposition);
	}
	return(SetExpr(inp,1,HIDE));
}

/*
  	#] CoHide : 
  	#[ CoNoHide :
*/

int CoNoHide ARG1(UBYTE *,inp) { return(SetExpr(inp,0,HIDE)); }

/*
  	#] CoNoHide : 
  	#[ CoUnHide :
*/

int CoUnHide ARG1(UBYTE *,inp) { return(SetExpr(inp,1,UNHIDE)); }

/*
  	#] CoUnHide : 
  	#[ CoNoUnHide :
*/

int CoNoUnHide ARG1(UBYTE *,inp) { return(SetExpr(inp,0,UNHIDE)); }

/*
  	#] CoNoUnHide : 
  	#[ AddToCom :
*/

void
AddToCom ARG2(int,n,WORD *,array)
{
	CBUF *C = cbuf+AC.cbufnum;
#ifdef COMPBUFDEBUG
	MesPrint("  %a",n,array);
#endif
	while ( C->Pointer+n >= C->Top ) DoubleCbuffer(AC.cbufnum,C->Pointer);
	while ( --n >= 0 ) *(C->Pointer)++ = *array++;
}

/*
  	#] AddToCom : 
  	#[ AddComString :
*/

int
AddComString ARG4(int,n,WORD *,array,UBYTE *,thestring,int,par)
{
	CBUF *C = cbuf+AC.cbufnum;
	UBYTE *s = thestring, *w;
	WORD *cc;
#ifdef COMPBUFDEBUG
	UBYTE *ww;
#endif
	int i, numchars = 0, size, zeroes;
	while ( *s ) {
		if ( *s == '\\' ) s++;
		else if ( par == 1 &&
		( ( *s == '%' && s[1] != 't' && s[1] != 'T' && s[1] != '$' &&
		 s[1] != 'w' && s[1] != 'W' && s[1] != 'r' && s[1] != 0 ) || *s == '#'
		|| *s == '@' || *s == '&' ) ) {
			numchars++;
		}
		s++; numchars++;
	}
	AddLHS(AC.cbufnum);
	size = numchars/sizeof(WORD)+1;
	while ( C->Pointer+size+n+1 >= C->Top ) DoubleCbuffer(AC.cbufnum,C->Pointer);
	cc = C->Pointer;
	*(C->Pointer)++ = array[0];
	*(C->Pointer)++ = size+n+2;
	for ( i = 1; i < n; i++ ) *(C->Pointer)++ = array[i];
	*(C->Pointer)++ = size;
#ifdef COMPBUFDEBUG
	ww =
#endif
	w = (UBYTE *)(C->Pointer);
	zeroes = size*sizeof(WORD)-numchars;
	s = thestring;
	while ( *s ) {
		if ( *s == '\\' ) s++;
		else if ( par == 1 && ( ( *s == '%' &&
		s[1] != 't' && s[1] != 'T' && s[1] != '$' &&
		s[1] != 'w' && s[1] != 'W' && s[1] != 'r' && s[1] != 0 ) || *s == '#'
		|| *s == '@' || *s == '&' ) ) {
			*w++ = '%';
		}
		*w++ = *s++;
	}
	while ( --zeroes >= 0 ) *w++ = 0;
	C->Pointer += size;
#ifdef COMPBUFDEBUG
	MesPrint("LH: %a",size+1+n,cc);
	MesPrint("        %s",thestring);
#endif
	return(0);
}

/*
  	#] AddComString : 
  	#[ Add2ComStrings :
*/

int
Add2ComStrings ARG4(int,n,WORD *,array,UBYTE *,string1,UBYTE *,string2)
{
	CBUF *C = cbuf+AC.cbufnum;
	UBYTE *s1 = string1, *s2 = string2, *w;
	int i, num1chars = 0, num2chars = 0, size1, size2, zeroes1, zeroes2;
	AddLHS(AC.cbufnum);
	while ( *s1 ) { s1++; num1chars++; }
	size1 = num1chars/sizeof(WORD)+1;
	if ( s2 ) {
		while ( *s2 ) { s2++; num2chars++; }
		size2 = num2chars/sizeof(WORD)+1;
	}
	else size2 = 0;
	while ( C->Pointer+size1+size2+n+3 >= C->Top ) DoubleCbuffer(AC.cbufnum,C->Pointer);
	*(C->Pointer)++ = array[0];
	*(C->Pointer)++ = size1+size2+n+3;
	for ( i = 1; i < n; i++ ) *(C->Pointer)++ = array[i];
	*(C->Pointer)++ = size1;
	w = (UBYTE *)(C->Pointer);
	zeroes1 = size1*sizeof(WORD)-num1chars;
	s1 = string1;
	while ( *s1 ) { *w++ = *s1++; }
	while ( --zeroes1 >= 0 ) *w++ = 0;
	C->Pointer += size1;
	*(C->Pointer)++ = size2;
	if ( size2 ) {
		w = (UBYTE *)(C->Pointer);
		zeroes2 = size2*sizeof(WORD)-num2chars;
		s2 = string2;
		while ( *s2 ) { *w++ = *s2++; }
		while ( --zeroes2 >= 0 ) *w++ = 0;
		C->Pointer += size2;
	}
	return(0);
}

/*
  	#] Add2ComStrings : 
  	#[ CoDiscard :
*/

int
CoDiscard ARG1(UBYTE *,s)
{
	if ( *s == 0 ) {
		Add2Com(TYPEDISCARD)
		return(0);
	}
	MesPrint("&Illegal argument in discard statement: '%s'",s);
	return(1);
}

/*
  	#] CoDiscard : 
  	#[ CoContract :

	Syntax:
		Contract
		Contract:#
		Contract #
		Contract:#,#
*/

static WORD ccarray[5] = { TYPEOPERATION,5,CONTRACT,0,0 };

int
CoContract ARG1(UBYTE *,s)
{
	int x;
	if ( *s == ':' ) {
		s++;
		ParseNumber(x,s)
		if ( *s != ',' && *s ) {
proper:		MesPrint("&Illegal number in contract statement");
			return(1);
		}
		if ( *s ) s++;
		ccarray[4] = x;
	}
	else ccarray[4] = 0;
	if ( FG.cTable[*s] == 1 ) {
		ParseNumber(x,s)
		if ( *s ) goto proper;
		ccarray[3] = x;
	}
	else if ( *s ) goto proper;
	else ccarray[3] = -1;
	return(AddNtoL(5,ccarray));
}

/*
  	#] CoContract : 
  	#[ CoGoTo :
*/

int
CoGoTo ARG1(UBYTE *,inp)
{
	UBYTE *s = inp;
	int x;
	while ( FG.cTable[*s] <= 1 ) s++;
	if ( *s ) {
		MesPrint("&Label should be an alpha-numeric string");
		return(1);
	}
	x = GetLabel(inp);
	Add3Com(TYPEGOTO,x);
	return(0);
}

/*
  	#] CoGoTo : 
  	#[ CoLabel :
*/

int
CoLabel ARG1(UBYTE *,inp)
{
	UBYTE *s = inp;
	int x;
	while ( FG.cTable[*s] <= 1 ) s++;
	if ( *s ) {
		MesPrint("&Label should be an alpha-numeric string");
		return(1);
	}
	x = GetLabel(inp);
	AC.Labels[x] = cbuf[AC.cbufnum].numlhs;
	return(0);
}

/*
  	#] CoLabel : 
  	#[ DoArgument :

	Layout:
		par,full size,numlhs(+1),par,scale
		scale is for normalize
*/

int
DoArgument ARG2(UBYTE *,s,int,par)
{
	GETIDENTITY
	UBYTE *name, *t, *v, c;
	WORD *oldworkpointer = AT.WorkPointer, *w, *ww, number, *scale;
	int error = 0, zeroflag, type, x;
	AC.lhdollarflag = 0;
	while ( *s == ',' ) s++;
	w = AT.WorkPointer;
	*w++ = par;
	w++;
	switch ( par ) {
		case TYPEARG:
	        if ( AC.arglevel >= MAXNEST ) {
    	        MesPrint("@Nesting of argument statements more than %d levels"
        	    ,(WORD)MAXNEST);
            	return(-1);
	        }
    	    AC.argsumcheck[AC.arglevel] = AC.IfLevel + AC.RepLevel + AC.insidelevel
				+ AC.termlevel;
        	AC.argstack[AC.arglevel] = cbuf[AC.cbufnum].Pointer
			                       - cbuf[AC.cbufnum].Buffer + 2;
			AC.arglevel++;
	        *w++ = cbuf[AC.cbufnum].numlhs;
			break;
		case TYPENORM:
		case TYPENORM4:
		case TYPESPLITARG:
		case TYPESPLITFIRSTARG:
		case TYPESPLITLASTARG:
		case TYPEFACTARG:
	        *w++ = cbuf[AC.cbufnum].numlhs+1;
			break;
    }
	*w++ = par;
	scale = w;
	*w++ = 1;
	*w++ = 0;
	if ( *s == '^' ) {
		s++; ParseSignedNumber(x,s)
		while ( *s == ',' ) s++;
		*scale = x;
	}
	if ( *s == '(' ) {
		t = s+1; SKIPBRA3(s)	/* We did check the brackets already */
		if ( par == TYPEARG ) {
			MesPrint("&Illegal () entry in argument statement");
			error = 1; s++; goto skipbracks;
		}
		else if ( par == TYPESPLITFIRSTARG ) {
			MesPrint("&Illegal () entry in splitfirstarg statement");
			error = 1; s++; goto skipbracks;
		}
		else if ( par == TYPESPLITLASTARG ) {
			MesPrint("&Illegal () entry in splitlastarg statement");
			error = 1; s++; goto skipbracks;
		}
		v = t;
		while ( v < s ) {
			if ( *v == '?' ) {
				MesPrint("&Wildcarding not allowed in this type of statement");
				error = 1; break;
			}
			v++;
		}
		v = s++;
		if ( *t == '(' && v[-1] == ')' ) {
			t++; v--;
			if ( par == TYPESPLITARG ) oldworkpointer[0] = TYPESPLITARG2;
			else if ( par == TYPEFACTARG ) oldworkpointer[0] = TYPEFACTARG2;
			else if ( par == TYPENORM4 ) oldworkpointer[0] = TYPENORM4;
			else if ( par == TYPENORM ) {
				if ( *t == '-' ) { oldworkpointer[0] = TYPENORM3; t++; }
				else             { oldworkpointer[0] = TYPENORM2; *scale = 0; }
			}
		}
		if ( error == 0 ) {
			CBUF *C = cbuf+AC.cbufnum;
			WORD oldnumrhs = C->numrhs, oldnumlhs = C->numlhs;
			WORD prototype[SUBEXPSIZE+40]; /* Up to 10 nested sums! */
			WORD *m, *mm;
			int i, retcode;
			LONG oldpointer = C->Pointer - C->Buffer;
			*v = 0;
			prototype[0] = SUBEXPRESSION;
			prototype[1] = SUBEXPSIZE;
			prototype[2] = C->numrhs+1;
			prototype[3] = 1;
			prototype[4] = AC.cbufnum;
			AT.WorkPointer += TYPEARGHEADSIZE+1;
			AddLHS(AC.cbufnum);
			if ( ( retcode = CompileAlgebra(t,LHSIDE,prototype) ) < 0 )
				error = 1;
			else {
				prototype[2] = retcode;
				ww = C->lhs[retcode];
				AC.lhdollarflag = 0;
				if ( *ww == 0 ) {
					*w++ = -2; *w++ = 0;
				}
				else if ( ww[ww[0]] != 0 ) {
					MesPrint("&There should be only one term between ()");
					error = 1;
				}
				else if ( NewSort() ) { if ( !error ) error = 1; }
				else if ( NewSort() ) {
					LowerSortLevel();
					if ( !error ) error = 1;
				}
				else {
					AN.RepPoint = AT.RepCount + 1;
			        m = AT.WorkPointer;
					mm = ww; i = *mm;
					while ( --i >= 0 ) *m++ = *mm++;
					mm = AT.WorkPointer; AT.WorkPointer = m;
					AR.Cnumlhs = C->numlhs;
					if ( Generator(BHEAD mm,C->numlhs) ) {
						LowerSortLevel(); error = 1;
					}
					else if ( EndSort(mm,0) < 0 ) {
						error = 1;
						AT.WorkPointer = mm;
					}
					else if ( *mm == 0 ) {
						*w++ = -2; *w++ = 0;
						AT.WorkPointer = mm;
					}
					else if ( mm[mm[0]] != 0 ) {
						error = 1;
						AT.WorkPointer = mm;
					}
					else {
						AT.WorkPointer = mm;
						m = mm+*mm;
						if ( par == TYPEFACTARG ) {
							if ( *mm != ABS(m[-1])+1 ) {
								*mm -= ABS(m[-1]);	/* Strip coefficient */
							}
							mm[-1] = -*mm-1; w += *mm+1;
						}
						else {
							*mm -= ABS(m[-1]);	/* Strip coefficient */
/*
							if ( *mm == 1 ) { *w++ = -2; *w++ = 0; }
							else
*/
							{ mm[-1] = -*mm-1; w += *mm+1; }
						}
						oldworkpointer[1] = w - oldworkpointer;
					}
					LowerSortLevel();
				}
				oldworkpointer[5] = AC.lhdollarflag;
			}
			*v = ')';
			C->numrhs = oldnumrhs;
			C->numlhs = oldnumlhs;
			C->Pointer = C->Buffer + oldpointer;
		}
	}
skipbracks:
	if ( *s == 0 ) { *w++ = 0; *w++ = 2; *w++ = 1; }
	else {
		do {
			if ( *s == ',' ) { s++; continue; }
			ww = w; *w++ = 0; w++;
			if ( FG.cTable[*s] > 1 && *s != '[' && *s != '{' ) {
				MesPrint("&Illegal parameters in statement");
				error = 1;
				break;
			}
			while ( FG.cTable[*s] == 0 || *s == '[' || *s == '{' ) {
				if ( *s == '{' ) {
					name = s+1;
					SKIPBRA2(s)
					c = *s; *s = 0;
					number = DoTempSet(name,s);
					name--; *s++ = c; c = *s; *s = 0;
					goto doset;
				}
				else {
					name = s;
					if ( ( s = SkipAName(s) ) == 0 ) {
						MesPrint("&Illegal name '%s'",name);
						return(1);
					}
					c = *s; *s = 0;
					if ( ( type = GetName(AC.varnames,name,&number,WITHAUTO) ) == CSET ) {
doset:					if ( Sets[number].type != CFUNCTION ) goto nofun;
						*w++ = CSET; *w++ = number;
					}
					else if ( type == CFUNCTION ) {
						*w++ = CFUNCTION; *w++ = number + FUNCTION;
					}
					else {
nofun:					MesPrint("&%s is not a function or a set of functions"
						,name);
						error = 1;
					}
				}
				*s = c;
				while ( *s == ',' ) s++;
			}
			ww[1] = w - ww;
			ww = w; w++; zeroflag = 0;
			while ( FG.cTable[*s] == 1 ) {
				ParseNumber(x,s)
				if ( *s && *s != ',' ) {
					MesPrint("&Illegal separator after number");
					error = 1;
					while ( *s && *s != ',' ) s++;
				}
				while ( *s == ',' ) s++;
				if ( x == 0 ) zeroflag = 1;
				if ( !zeroflag ) *w++ = (WORD)x;
			}
			*ww = w - ww;
		} while ( *s );
	}
	oldworkpointer[1] = w - oldworkpointer;
	if ( par == TYPEARG ) {  /* To make sure. The Pointer might move in the future */
       	AC.argstack[AC.arglevel-1] = cbuf[AC.cbufnum].Pointer
			                       - cbuf[AC.cbufnum].Buffer + 2;
	}
	AddNtoL(oldworkpointer[1],oldworkpointer);
	AT.WorkPointer = oldworkpointer;
	return(error);
}

/*
  	#] DoArgument :
  	#[ CoArgument :
*/

int
CoArgument ARG1(UBYTE *,s) { return(DoArgument(s,TYPEARG)); }

/*
  	#] CoArgument : 
  	#[ CoEndArgument :
*/

int
CoEndArgument ARG1(UBYTE *,s)
{
	CBUF *C = cbuf+AC.cbufnum;
	while ( *s == ',' ) s++;
	if ( *s ) {
		MesPrint("&Illegal syntax for EndArgument statement");
		return(1);
	}
	if ( AC.arglevel <= 0 ) {
		MesPrint("&EndArgument without corresponding Argument statement");
		return(1);
	}
	AC.arglevel--;
	cbuf[AC.cbufnum].Buffer[AC.argstack[AC.arglevel]] = C->numlhs;
	if ( AC.argsumcheck[AC.arglevel] != ( AC.IfLevel + AC.RepLevel + AC.insidelevel
		+ AC.termlevel ) ) {
		MesPrint("&Illegal nesting of if, repeat, argument");
		return(1);
	}
	return(0);
}

/*
  	#] CoEndArgument : 
  	#[ CoInside :
*/

int
CoInside ARG1(UBYTE *,s) { return(DoInside(s)); }

/*
  	#] CoInside : 
  	#[ CoEndInside :
*/

int
CoEndInside ARG1(UBYTE *,s)
{
	CBUF *C = cbuf+AC.cbufnum;
	while ( *s == ',' ) s++;
	if ( *s ) {
		MesPrint("&Illegal syntax for EndInside statement");
		return(1);
	}
	if ( AC.insidelevel <= 0 ) {
		MesPrint("&EndInside without corresponding Inside statement");
		return(1);
	}
	AC.insidelevel--;
	cbuf[AC.cbufnum].Buffer[AC.insidestack[AC.insidelevel]] = C->numlhs;
	if ( AC.insidesumcheck[AC.insidelevel] != ( AC.IfLevel + AC.RepLevel
		+ AC.arglevel + AC.termlevel ) ) {
		MesPrint("&Illegal nesting of if, repeat, argument, inside");
		return(1);
	}
	return(0);
}

/*
  	#] CoEndInside : 
  	#[ CoNormalize :
*/

int
CoNormalize ARG1(UBYTE *,s) { return(DoArgument(s,TYPENORM)); }

/*
  	#] CoNormalize : 
  	#[ CoMakeInteger :
*/

int
CoMakeInteger ARG1(UBYTE *,s) { return(DoArgument(s,TYPENORM4)); }

/*
  	#] CoMakeInteger :
  	#[ CoSplitArg :
*/

int
CoSplitArg ARG1(UBYTE *,s) { return(DoArgument(s,TYPESPLITARG)); }

/*
  	#] CoSplitArg : 
  	#[ CoSplitFirstArg :
*/

int
CoSplitFirstArg ARG1(UBYTE *,s) { return(DoArgument(s,TYPESPLITFIRSTARG)); }

/*
  	#] CoSplitFirstArg : 
  	#[ CoSplitLastArg :
*/

int
CoSplitLastArg ARG1(UBYTE *,s) { return(DoArgument(s,TYPESPLITLASTARG)); }

/*
  	#] CoSplitLastArg : 
  	#[ CoFactArg :
*/

int
CoFactArg ARG1(UBYTE *,s) { return(DoArgument(s,TYPEFACTARG)); }

/*
  	#] CoFactArg : 
  	#[ DoSymmetrize :

        Syntax:
        Symmetrize Fun[:[number]] [Fields]      -> par = 0;
        AntiSymmetrize Fun[:[number]] [Fields]  -> par = 1;
        CycleSymmetrize Fun[:[number]] [Fields] -> par = 2;
        RCycleSymmetrize Fun[:[number]] [Fields]-> par = 3;
*/

int
DoSymmetrize ARG2(UBYTE *,s,int,par)
{
	GETIDENTITY
	int extra = 0, error = 0, err, fix, x, groupsize, num, i;
	UBYTE *name, c;
	WORD funnum, *w, *ww, type;
	for(;;) {
		name = s;
		if ( ( s = SkipAName(s) ) == 0 ) {
			MesPrint("&Improper function name");
			return(1);
		}
		c = *s; *s = 0;
		if ( c != ',' || ( FG.cTable[s[1]] != 0 && s[1] != '[' ) ) break;
		if ( par <= 0 && StrICmp(name,(UBYTE *)"cyclic") == 0 ) extra = 2;
		else if ( par <= 0 && StrICmp(name,(UBYTE *)"rcyclic") == 0 ) extra = 6;
		else {
			MesPrint("&Illegal option: '%s'",name);
			error = 1;
		}
		*s++ = c;
	}
	if ( ( err = GetVar(name,&type,&funnum,CFUNCTION,WITHAUTO) ) == NAMENOTFOUND ) {
		MesPrint("&Undefined function: %s",name);
		AddFunction(name,0,0,0,0);
		*s++ = c;
		return(1);
	}
	funnum += FUNCTION;
	if ( err == -1 ) error = 1;
	*s = c;
	if ( *s == ':' ) {
		s++;
		if ( *s == ',' || *s == '(' || *s == 0 ) fix = -1;
		else if ( FG.cTable[*s] == 1 ) {
			ParseNumber(fix,s)
			if ( fix == 0 ) 
				Warning("Restriction to zero arguments removed");
		}
		else {
			MesPrint("&Illegal character after :");
			return(1);
		}
	}
	else fix = 0;
	w = AT.WorkPointer;
	*w++ = TYPEOPERATION;
	w++;
	*w++ = SYMMETRIZE;
	*w++ = par | extra;
	*w++ = funnum;
	*w++ = fix;
/*
	And now the argument lists. We have either ,#,#,... or (#,#,..,#),(#,...
*/
	w += 2; ww = w; groupsize = -1;
	while ( *s == ',' ) s++;
	while ( *s ) {
		if ( *s == '(' ) {
			s++; num = 0;
			while ( *s && *s != ')' ) {
				if ( *s == ',' ) { s++; continue; }
				if ( FG.cTable[*s] != 1 ) goto illarg;
				ParseNumber(x,s)
				if ( x <= 0 || ( fix > 0 && x > fix ) ) goto illnum;
				num++;
				*w++ = x-1;
			}
			if ( *s == 0 ) {
				MesPrint("&Improper termination of statement");
				return(1);
			}
			if ( groupsize < 0 ) groupsize = num;
			else if ( groupsize != num ) goto group;
			s++;
		}
		else if ( FG.cTable[*s] == 1 ) {
			if ( groupsize < 0 ) groupsize = 1;
			else if ( groupsize != 1 ) {
group:			MesPrint("&All groups should have the same number of arguments");
				return(1);
			}
			ParseNumber(x,s)
			if ( x <= 0 || ( fix > 0 && x > fix ) ) {
illnum:			MesPrint("&Illegal argument number: %d",x);
				return(1);
			}
			*w++ = x-1;
		}
		else {
illarg:		MesPrint("&Illegal argument");
			return(1);
		}
		while ( *s == ',' ) s++;
	}
/*
	Now the completion
*/
	if ( w == ww ) {
		ww[-1] = 1;
		ww[-2] = 0;
		if ( fix > 0 ) {
			for ( i = 0; i < fix; i++ ) *w++ = i;
			ww[-2] = fix; /* Bugfix 31-oct-2001. Reported by York Schroeder */
		}
	}
	else {
		ww[-1] = groupsize;
		ww[-2] = (w-ww)/groupsize;
	}
	AT.WorkPointer[1] = w - AT.WorkPointer;
	AddNtoL(AT.WorkPointer[1],AT.WorkPointer);
	return(error);
}

/*
  	#] DoSymmetrize : 
  	#[ CoSymmetrize :
*/

int
CoSymmetrize ARG1(UBYTE *,s) { return(DoSymmetrize(s,SYMMETRIC)); }

/*
  	#] CoSymmetrize : 
  	#[ CoAntiSymmetrize :
*/

int
CoAntiSymmetrize ARG1(UBYTE *,s) { return(DoSymmetrize(s,ANTISYMMETRIC)); }

/*
  	#] CoAntiSymmetrize : 
  	#[ CoCycleSymmetrize :
*/

int
CoCycleSymmetrize ARG1(UBYTE *,s) { return(DoSymmetrize(s,CYCLESYMMETRIC)); }

/*
  	#] CoCycleSymmetrize : 
  	#[ CoRCycleSymmetrize :
*/

int
CoRCycleSymmetrize ARG1(UBYTE *,s) { return(DoSymmetrize(s,RCYCLESYMMETRIC)); }

/*
  	#] CoRCycleSymmetrize : 
  	#[ CoWrite :
*/

int
CoWrite ARG1(UBYTE *,s)
{
	UBYTE *option;
	KEYWORD *key;
	option = s;
	if ( ( ( s = SkipAName(s) ) == 0 ) || *s != 0 ) {
		MesPrint("&Proper use of write statement is: write option");
		return(1);
	}
	key = FindInKeyWord(option,writeoptions,sizeof(writeoptions)/sizeof(KEYWORD));
	if ( key == 0 ) {
		MesPrint("&Unrecognized option in write statement");
		return(1);
	}
	*((int *)(key->func)) = key->type;
	return(0);
}

/*
  	#] CoWrite : 
  	#[ CoNWrite :
*/

int
CoNWrite ARG1(UBYTE *,s)
{
	UBYTE *option;
	KEYWORD *key;
	option = s;
	if ( ( ( s = SkipAName(s) ) == 0 ) || *s != 0 ) {
		MesPrint("&Proper use of nwrite statement is: nwrite option");
		return(1);
	}
	key = FindInKeyWord(option,writeoptions,sizeof(writeoptions)/sizeof(KEYWORD));
	if ( key == 0 ) {
		MesPrint("&Unrecognized option in nwrite statement");
		return(1);
	}
	*((int *)(key->func)) = key->flags;
	return(0);
}

/*
  	#] CoNWrite : 
  	#[ CoRatio :
*/

static WORD ratstring[6] = { TYPEOPERATION, 6, RATIO, 0, 0, 0 };

int
CoRatio ARG1(UBYTE *,s)
{
	UBYTE c, *t;
	int i, type, error = 0;
	WORD numsym, *rs;
	rs = ratstring+3;
	for ( i = 0; i < 3; i++ ) {
		if ( *s ) {
			t = s;
			s = SkipAName(s);
			c = *s; *s = 0;
			if ( ( ( type = GetName(AC.varnames,t,&numsym,WITHAUTO) ) != CSYMBOL )
			&& type != CDUBIOUS ) {
				MesPrint("&%s is not a symbol",t);
				error = 4;
				if ( type < 0 ) numsym = AddSymbol(t,-MAXPOWER,MAXPOWER,0);
			}
			*s = c;
			if ( *s == ',' ) s++;
		}
		else {
			if ( error == 0 )
				MesPrint("&The ratio statement needs three symbols for its arguments");
			error++;
			numsym = 0;
		}
		*rs++ = numsym;
	}
	AddNtoL(6,ratstring);
	return(error);
}

/*
  	#] CoRatio : 
  	#[ CoRedefine :

	We have a preprocessor variable and a (new) value for it.
	This value is inside a string that must be stored.
*/

int
CoRedefine ARG1(UBYTE *,s)
{
	UBYTE *name, c, *args = 0;
	int numprevar;
	WORD code[2];
	name = s;
	if ( FG.cTable[*s] || ( s = SkipAName(s) ) == 0 || s[-1] == '_' ) {
		MesPrint("&Illegal name for preprocessor variable in redefine statement");
		return(1);
	}
	c = *s; *s = 0;
	for ( numprevar = NumPre-1; numprevar >= 0; numprevar-- ) {
		if ( StrCmp(name,PreVar[numprevar].name) == 0 ) break;
	}
	if ( numprevar < 0 ) {
		MesPrint("&There is no preprocessor variable with the name `%s'",name);
		*s = c;
		return(1);
	}
	*s = c;
/*
	The next code worries about arguments.
	It is a direct copy of the code in TheDefine in the preprocessor.
*/
	if ( *s == '(' ) {	/* arguments. scan for correctness */
		s++; args = s;
		for (;;) {
			if ( chartype[*s] != 0 ) goto illarg;
			s++;
			while ( chartype[*s] <= 1 ) s++;
			while ( *s == ' ' || *s == '\t' ) s++;
			if ( *s == ')' ) break;
			if ( *s != ',' ) goto illargs;
			s++;
			while ( *s == ' ' || *s == '\t' ) s++;
		}
		*s++ = 0;
		while ( *s == ' ' || *s == '\t' ) s++;
	}
	while ( *s == ',' ) s++;
	if ( *s != '"' ) {
encl:	MesPrint("&Value for %s should be enclosed in double quotes"
		,PreVar[numprevar].name);
		return(1);
	}
	s++; name = s; /* actually name points to the new string */
	while ( *s && *s != '"' ) { if ( *s == '\\' ) s++; s++; }
	if ( *s != '"' ) goto encl;
	*s = 0;
	code[0] = TYPEREDEFPRE; code[1] = numprevar;
/*
	AddComString(2,code,name,0);
*/
	Add2ComStrings(2,code,name,args);
	*s = '"';
#ifdef WITHPTHREADS
/*
	Now we prepare the input numbering system for pthreads.
	We need a list of preprocessor variables that are redefined in this
	module.
*/
	{
	  int j;
	  WORD *newpf;
	  LONG *newin;
	  for ( j = 0; j < AC.numpfirstnum; j++ ) {
		if ( numprevar == AC.pfirstnum[j] ) break;
	  }
	  if ( j >= AC.numpfirstnum ) {  /* add to list */
		if ( j >= AC.sizepfirstnum ) {
			if ( AC.sizepfirstnum <= 0 ) { AC.sizepfirstnum = 10; }
			else { AC.sizepfirstnum = 2 * AC.sizepfirstnum; }
			newin = (LONG *)Malloc1(AC.sizepfirstnum*(sizeof(WORD)+sizeof(LONG)),"AC.pfirstnum");
			newpf = (WORD *)(newin+AC.sizepfirstnum);
			for ( j = 0; j < AC.numpfirstnum; j++ ) {
				newpf[j] = AC.pfirstnum[j];
				newin[j] = AC.inputnumbers[j];
			}
			if ( AC.inputnumbers ) M_free(AC.pfirstnum,"AC.pfirstnum");
			AC.inputnumbers = newin;
			AC.pfirstnum = newpf;
		}
		AC.pfirstnum[AC.numpfirstnum] = numprevar;
		AC.inputnumbers[AC.numpfirstnum] = -1;
		AC.numpfirstnum++;
	  }
	}
#endif
	return(0);
illarg:;
	MesPrint("&Illegally formed name in argument of redefine statement");
	return(1);
illargs:;
	MesPrint("&Illegally formed arguments in redefine statement");
	return(1);
}

/*
  	#] CoRedefine : 
  	#[ CoRenumber :

	renumber    or renumber,0     Only exchanges (n^2 until no improvement)
	renumber,1                    All permutations (could be slow)
*/

int
CoRenumber ARG1(UBYTE *,s)
{
	int x;
	UBYTE *inp;
	while ( *s == ',' ) s++;
	inp = s;
	if ( *s == 0 ) { x = 0; }
	else ParseNumber(x,s)
	if ( *s == 0 && x >= 0 && x <= 1 ) {
		Add3Com(TYPERENUMBER,x);
		return(0);
	}
	MesPrint("&Illegal argument in Renumber statement: '%s'",inp);
	return(1);
}

/*
  	#] CoRenumber : 
  	#[ CoSum :
*/

int
CoSum ARG1(UBYTE *,s)
{
	CBUF *C = cbuf+AC.cbufnum;
	UBYTE *ss = 0, c, *t;
	int error = 0, i = 0, type, x;
	WORD numindex,number;
	while ( *s ) {
		t = s;
		if ( *s == '$' ) {
			t++; s++; while ( FG.cTable[*s] < 2 ) s++;
			c = *s; *s = 0;
			if ( ( number = GetDollar(t) ) < 0 ) {
				MesPrint("&Undefined variable $%s",t);
				if ( !error ) error = 1;
				number = AddDollar(t,0,0,0);
			}
			numindex = -number;
		}
		else {
			if ( ( s = SkipAName(s) ) == 0 ) return(1);
			c = *s; *s = 0;
			if ( ( ( type = GetOName(AC.exprnames,t,&numindex,NOAUTO) ) != NAMENOTFOUND )
			|| ( ( type = GetOName(AC.varnames,t,&numindex,WITHAUTO) ) != CINDEX ) ) {
				if ( type != NAMENOTFOUND ) error = NameConflict(type,t);
				else {
					MesPrint("&%s should have been declared as an index",t);
					error = 1;
					numindex = AddIndex(s,AC.lDefDim,AC.lDefDim4) + AM.OffsetIndex;
				}
			}
		}
		Add3Com(TYPESUM,numindex);
		i = 3; *s = c;
		if ( *s == 0 ) break;
		if ( *s != ',' ) {
			MesPrint("&Illegal separator between objects in sum statement.");
			return(1);
		}
		s++;
		if ( FG.cTable[*s] == 0 || *s == '[' || *s == '$' ) {
			while ( FG.cTable[*s] == 0 || *s == '[' || *s == '$' ) {
				if ( *s == '$' ) {
					s++;
					ss = t = s;
					while ( FG.cTable[*s] < 2 ) s++;
					c = *s; *s = 0;
					if ( ( number = GetDollar(t) ) < 0 ) {
						MesPrint("&Undefined variable $%s",t);
						if ( !error ) error = 1;
						number = AddDollar(t,0,0,0);
					}
					numindex = -number;
				}
				else {
					ss = t = s;
					if ( ( s = SkipAName(s) ) == 0 ) return(1);
					c = *s; *s = 0;
					if ( ( ( type = GetOName(AC.exprnames,t,&numindex,NOAUTO) ) != NAMENOTFOUND )
					|| ( ( type = GetOName(AC.varnames,t,&numindex,WITHAUTO) ) != CINDEX ) ) {
						if ( type != NAMENOTFOUND ) error = NameConflict(type,t);
						else {
							MesPrint("&%s should have been declared as an index",t);
							error = 1;
							numindex = AddIndex(s,AC.lDefDim,AC.lDefDim4) + AM.OffsetIndex;
						}
					}
				}
				AddToCB(C,numindex)
				i++;
				C->Pointer[-i+1] = i;
				*s = c;
				if ( *s == 0 ) return(error);
				if ( *s != ',' ) {
					MesPrint("&Illegal separator between objects in sum statement.");
					return(1);
				}
				s++;
			}
			if ( FG.cTable[*s] == 1 ) {
				C->Pointer[-i+1]--; C->Pointer--; s = ss;
			}
		}
		else if ( FG.cTable[*s] == 1 ) {
			while ( FG.cTable[*s] == 1 ) {
				t = s;
				x = *s++ - '0';
				while( FG.cTable[*s] == 1 ) x = 10*x + *s++ - '0';
				if ( *s && *s != ',' ) {
					MesPrint("&%s is not a legal fixed index",t);
					return(1);
				}
				else if ( x >= AM.OffsetIndex ) {
					MesPrint("&%d is too large to be a fixed index",x);
					error = 1;
				}
				else {
					AddToCB(C,x)
					i++;
					C->Pointer[-i] = TYPESUMFIX;
					C->Pointer[-i+1] = i;
				}
				if ( *s == 0 ) break;
				s++;
			}
		}
		else {
			MesPrint("&Illegal object in sum statement");
			error = 1;
		}
	}
	return(error);
}

/*
  	#] CoSum : 
  	#[ CoToTensor :
*/

static WORD cttarray[7] = { TYPEOPERATION,7,TENVEC,0,0,1,0 };

int
CoToTensor ARG1(UBYTE *,s)
{
	UBYTE *t, c, *args[2], cc[2];
	int j, type, error = 0, ex = 0;
	WORD number, dol[2];
	cttarray[3] = cttarray[4] = 0; cttarray[5] = 1;
	dol[0] = dol[1] = 0;
	for ( j = 0; j < 2; j++ ) {
inloop:	args[j] = s;
		if ( ( s = SkipAName(s) ) == 0 ) {
proper:		MesPrint("&Syntax error in ToTensor statement");
			return(1);
		}
		cc[j] = *s;
		if ( *s == '!' ) {
			*s++ = 0;
			if ( FG.cTable[*s] == 0 || *s == '[' || *s == '_' || *s == '$' ) {
				if ( ( s = SkipAName(s) ) == 0 ) goto proper;
			}
			else if ( *s == '{' ) {
				SKIPBRA2(s)
				s++;
			}
			else goto proper;
			if ( *s == 0 ) break;
			if ( *s != ',' ) goto proper;
			*s = 0;
		}
		else {
			*s = 0;
			if ( cc[j] == 0 ) break;
			if ( cc[j] != ',' ) goto proper;
		}
		s++;
	}
	if ( cc[1] == ',' ) {
		if ( StrICmp(args[0],(UBYTE *)"nosquare") == 0 ) cttarray[5] |= 2;
		else if ( StrICmp(args[0],(UBYTE *)"functions") == 0 ) cttarray[5] |= 4;
		else {
			MesPrint("&Unrecognized option in ToTensor statement: '%s'",args[0]);
			error = 1;
		}
		args[0] = args[1]; args[1][-1] = cc[0]; cc[0] = cc[1];
		j = 1; goto inloop;
	}
	if ( cc[1] != '!' && cc[1] != 0 ) goto proper;
	for ( j = 0; j < 2; j++ ) {
		if ( args[j][0] == '$' ) {
			dol[j] = GetDollar(args[j]+1);
			if ( dol[j] < 0 ) dol[j] = AddDollar(args[j]+1,DOLUNDEFINED,0,0);
		}
		else if ( ( type = GetName(AC.varnames,args[j],&number,WITHAUTO) ) == CVECTOR ) {
			cttarray[4] = number + AM.OffsetVector;
			if ( j == 0 ) ex = 1;
		}
		else if ( type == CFUNCTION && ( functions[number].spec > 0 ) ) {
			cttarray[3] = number + FUNCTION;
			if ( j == 1 ) ex = 1;
		}
		else {
			MesPrint("&%s is not a vector or a tensor",args[j]);
			error = 1;
		}
	}
	if ( cttarray[3] == 0 || cttarray[4] == 0 ) {
	 	if ( dol[0] == 0 && dol[1] == 0 ) {
			MesPrint("&ToTensor statement needs a vector and a tensor");
			error = 1;
		}
		else if ( cttarray[3] ) {
			if ( dol[1] )        cttarray[4] = dol[1];
			else if ( dol[0] ) { cttarray[4] = dol[0]; ex = 1; }
			else {
				MesPrint("&ToTensor statement needs a vector and a tensor");
				error = 1;
			}
		}
		else if ( cttarray[4] ) {
			if ( dol[1] )    { cttarray[3] = -dol[1]; ex = 1; }
			else if ( dol[0] ) cttarray[3] = -dol[0];
			else {
				MesPrint("&ToTensor statement needs a vector and a tensor");
				error = 1;
			}
		}
		else {
			if ( dol[0] == 0 || dol[1] == 0 ) {
				MesPrint("&ToTensor statement needs a vector and a tensor");
				error = 1;
			}
			else {
				cttarray[3] = -dol[0]; cttarray[4] = dol[1];
			}
		}
	}
	if ( ex ) j = 0;
	else j = 1;
	if ( cc[j] == '!' ) {
		s = args[1-j]; while ( *s ) s++; *s = cc[1-j];
		s = args[j]; while ( *s ) s++; *s++ = cc[j];
		cttarray[5] |= 8;
		if ( FG.cTable[*s] == 0 || *s == '[' || *s == '_' ) {
			t = s;
			if ( ( s = SkipAName(s) ) == 0 ) goto proper;
			c = *s; *s = 0;
			if ( ( type = GetName(AC.varnames,t,&number,WITHAUTO) ) != CSET ) {
				MesPrint("&%s is not the name of a set",t);
				error = 1;
			}
			*s = c;
			cttarray[6] = number;
		}
		else if ( *s == '{' ) {
			s++; t = s; SKIPBRA2(s) *s = 0;
			cttarray[6] = DoTempSet(t,s);
			if ( cttarray[6] < 0 ) error = 1;
			*s++ = '}';
			if ( AC.wildflag ) {
				MesPrint("&Improper use of wildcard(s) in set specification");
				error = 1;
			}
		}
		if ( *s != 0 ) goto proper;
		cttarray[1] = 7;
		AddNtoL(7,cttarray);
	}
	else {
		args[1][-1] = cc[0]; s[-1] = cc[1];
		cttarray[1] = 6;
		AddNtoL(6,cttarray);
	}
	return(error);
}

/*
  	#] CoToTensor : 
  	#[ CoToVector :
*/

static WORD ctvarray[6] = { TYPEOPERATION,6,TENVEC,0,0,0 };

int
CoToVector ARG1(UBYTE *,s)
{
	UBYTE *t, c;
	int j, type, error = 0;
	WORD number, dol[2];
	dol[0] = dol[1] = 0;
	ctvarray[3] = ctvarray[4] = ctvarray[5] = 0;
	for ( j = 0; j < 2; j++ ) {
		t = s;
		if ( ( s = SkipAName(s) ) == 0 ) {
proper:		MesPrint("&Arguments of ToVector statement should be a vector and a tensor");
			return(1);
		}
		c = *s; *s = 0;
		if ( *t == '$' ) {
			dol[j] = GetDollar(t+1);
			if ( dol[j] < 0 ) dol[j] = AddDollar(t+1,DOLUNDEFINED,0,0);
		}
		else if ( ( type = GetName(AC.varnames,t,&number,WITHAUTO) ) == CVECTOR )
			ctvarray[4] = number + AM.OffsetVector;
		else if ( type == CFUNCTION && ( functions[number].spec > 0 ) )
			ctvarray[3] = number+FUNCTION;
		else {
			MesPrint("&%s is not a vector or a tensor",t);
			error = 1;
		}
		*s = c; if ( *s && *s != ',' ) goto proper;
		if ( *s ) s++;
	}
	if ( *s != 0 ) goto proper;
	if ( ctvarray[3] == 0 || ctvarray[4] == 0 ) {
	 	if ( dol[0] == 0 && dol[1] == 0 ) {
			MesPrint("&ToVector statement needs a vector and a tensor");
			error = 1;
		}
		else if ( ctvarray[3] ) {
			if ( dol[1] )      ctvarray[4] = dol[1];
			else if ( dol[0] ) ctvarray[4] = dol[0];
			else {
				MesPrint("&ToVector statement needs a vector and a tensor");
				error = 1;
			}
		}
		else if ( ctvarray[4] ) {
			if ( dol[1] )      ctvarray[3] = -dol[1];
			else if ( dol[0] ) ctvarray[3] = -dol[0];
			else {
				MesPrint("&ToVector statement needs a vector and a tensor");
				error = 1;
			}
		}
		else {
			if ( dol[0] == 0 || dol[1] == 0 ) {
				MesPrint("&ToVector statement needs a vector and a tensor");
				error = 1;
			}
			else {
				ctvarray[3] = -dol[0]; ctvarray[4] = dol[1];
			}
		}
	}
	AddNtoL(6,ctvarray);
	return(error);
}

/*
  	#] CoToVector : 
  	#[ CoTrace4 :
*/

int
CoTrace4 ARG1(UBYTE *,s)
{
	int error = 0, type, option = CHISHOLM;
	UBYTE *t, c;
	WORD numindex, one = 1;
	KEYWORD *key;
	for (;;) {
		t = s;
		if ( FG.cTable[*s] == 1 ) break;
		if ( ( s = SkipAName(s) ) == 0 ) {
proper:		MesPrint("&Proper syntax for Trace4 is 'Trace4[,options],index;'");
			return(1);
		}
		if ( *s == 0 ) break;
		c = *s; *s = 0;
		if ( ( key = FindKeyWord(t,trace4options,
			sizeof(trace4options)/sizeof(KEYWORD)) ) == 0 ) break;
		else {
			option |=  key->type;
			option &= ~key->flags;
		}
		if ( ( *s++ = c ) != ',' ) {
			MesPrint("&Illegal separator in Trace4 statement");
			return(1);
		}
		if ( *s == 0 ) goto proper;
	}
	s = t;
	if ( FG.cTable[*s] == 1 ) {
		ParseNumber(numindex,s)
		if ( *s != 0 ) {
			MesPrint("&Last argument of Trace4 should be an index");
			return(1);
		}
		if ( numindex >= AM.OffsetIndex ) {
			MesPrint("&fixed index >= %d. Change value of OffsetIndex in setup file"
			,AM.OffsetIndex);
			return(1);
		}
	}
	else if ( *s == '$' ) {
		if ( ( type = GetName(AC.dollarnames,s+1,&numindex,NOAUTO) ) == CDOLLAR )
			numindex = -numindex;
		else {
			MesPrint("&%s is undefined",s);
			numindex = AddDollar(s+1,DOLINDEX,&one,1);
			return(1);
		}
tests:	s = SkipAName(s);
		if ( *s != 0 ) {
			MesPrint("&Trace4 should have a single index or $variable for its argument");
			return(1);
		}
	}
	else if ( ( type = GetName(AC.varnames,s,&numindex,WITHAUTO) ) == CINDEX ) {
		numindex += AM.OffsetIndex;
		goto tests;
	}
	else if ( type != -1 ) {
		if ( type != CDUBIOUS ) {
			NameConflict(type,s);
			type = MakeDubious(AC.varnames,s,&numindex);
		}
		return(1);
	}
	else {
		MesPrint("&%s is not an index",s);
		numindex = AddIndex(s,AC.lDefDim,AC.lDefDim4) + AM.OffsetIndex;
		return(1);
	}
	if ( error ) return(error);
	if ( ( option & CHISHOLM ) != 0 )
		Add4Com(TYPECHISHOLM,numindex,(option & ALSOREVERSE));
	Add5Com(TYPEOPERATION,TAKETRACE,4 + (option & NOTRICK),numindex);
	return(0);
}

/*
  	#] CoTrace4 : 
  	#[ CoTraceN :
*/

int
CoTraceN ARG1(UBYTE *,s)
{
	WORD numindex, one = 1;
	int type;
	if ( FG.cTable[*s] == 1 ) {
		ParseNumber(numindex,s)
		if ( *s != 0 ) {
			MesPrint("&TraceN should have a single index for its argument");
			return(1);
		}
		if ( numindex >= AM.OffsetIndex ) {
			MesPrint("&fixed index >= %d. Change value of OffsetIndex in setup file"
			,AM.OffsetIndex);
			return(1);
		}
	}
	else if ( *s == '$' ) {
		if ( ( type = GetName(AC.dollarnames,s+1,&numindex,NOAUTO) ) == CDOLLAR )
			numindex = -numindex;
		else {
			MesPrint("&%s is undefined",s);
			numindex = AddDollar(s+1,DOLINDEX,&one,1);
			return(1);
		}
tests:	s = SkipAName(s);
		if ( *s != 0 ) {
			MesPrint("&TraceN should have a single index or $variable for its argument");
			return(1);
		}
	}
	else if ( ( type = GetName(AC.varnames,s,&numindex,WITHAUTO) ) == CINDEX ) {
		numindex += AM.OffsetIndex;
		goto tests;
	}
	else if ( type != -1 ) {
		if ( type != CDUBIOUS ) {
			NameConflict(type,s);
			type = MakeDubious(AC.varnames,s,&numindex);
		}
		return(1);
	}
	else {
		MesPrint("&%s is not an index",s);
		numindex = AddIndex(s,AC.lDefDim,AC.lDefDim4) + AM.OffsetIndex;
		return(1);
	}
	Add5Com(TYPEOPERATION,TAKETRACE,0,numindex);
	return(0);
}

/*
  	#] CoTraceN : 
  	#[ CoChisholm :
*/

int
CoChisholm ARG1(UBYTE *,s)
{
	int error = 0, type, option = CHISHOLM;
	UBYTE *t, c;
	WORD numindex, one = 1;
	KEYWORD *key;
	for (;;) {
		t = s;
		if ( FG.cTable[*s] == 1 ) break;
		if ( ( s = SkipAName(s) ) == 0 ) {
proper:		MesPrint("&Proper syntax for Chisholm is 'Chisholm[,options],index;'");
			return(1);
		}
		if ( *s == 0 ) break;
		c = *s; *s = 0;
		if ( ( key = FindKeyWord(t,chisoptions,
			sizeof(chisoptions)/sizeof(KEYWORD)) ) == 0 ) break;
		else {
			option |=  key->type;
			option &= ~key->flags;
		}
		if ( ( *s++ = c ) != ',' ) {
			MesPrint("&Illegal separator in Chisholm statement");
			return(1);
		}
		if ( *s == 0 ) goto proper;
	}
	s = t;
	if ( FG.cTable[*s] == 1 ) {
		ParseNumber(numindex,s)
		if ( *s != 0 ) {
			MesPrint("&Last argument of Chisholm should be an index");
			return(1);
		}
		if ( numindex >= AM.OffsetIndex ) {
			MesPrint("&fixed index >= %d. Change value of OffsetIndex in setup file"
			,AM.OffsetIndex);
			return(1);
		}
	}
	else if ( *s == '$' ) {
		if ( ( type = GetName(AC.dollarnames,s+1,&numindex,NOAUTO) ) == CDOLLAR )
			numindex = -numindex;
		else {
			MesPrint("&%s is undefined",s);
			numindex = AddDollar(s+1,DOLINDEX,&one,1);
			return(1);
		}
tests:	s = SkipAName(s);
		if ( *s != 0 ) {
			MesPrint("&Chisholm should have a single index or $variable for its argument");
			return(1);
		}
	}
	else if ( ( type = GetName(AC.varnames,s,&numindex,WITHAUTO) ) == CINDEX ) {
		numindex += AM.OffsetIndex;
		goto tests;
	}
	else if ( type != -1 ) {
		if ( type != CDUBIOUS ) {
			NameConflict(type,s);
			type = MakeDubious(AC.varnames,s,&numindex);
		}
		return(1);
	}
	else {
		MesPrint("&%s is not an index",s);
		numindex = AddIndex(s,AC.lDefDim,AC.lDefDim4) + AM.OffsetIndex;
		return(1);
	}
	if ( error ) return(error);
	Add4Com(TYPECHISHOLM,numindex,(option & ALSOREVERSE));
	return(0);
}

/*
  	#] CoChisholm : 
  	#[ DoChain :

	Syntax: Chainxx functionname;
*/

int
DoChain ARG2(UBYTE *,s,int,option)
{
	WORD numfunc,type;
	if ( *s == '$' ) {
		if ( ( type = GetName(AC.dollarnames,s+1,&numfunc,NOAUTO) ) == CDOLLAR )
			numfunc = -numfunc;
		else {
			MesPrint("&%s is undefined",s);
			numfunc = AddDollar(s+1,DOLINDEX,&one,1);
			return(1);
		}
tests:	s = SkipAName(s);
		if ( *s != 0 ) {
			MesPrint("&ChainIn/ChainOut should have a single function or $variable for its argument");
			return(1);
		}
	}
	else if ( ( type = GetName(AC.varnames,s,&numfunc,WITHAUTO) ) == CFUNCTION ) {
		numfunc += FUNCTION;
		goto tests;
	}
	else if ( type != -1 ) {
		if ( type != CDUBIOUS ) {
			NameConflict(type,s);
			type = MakeDubious(AC.varnames,s,&numfunc);
		}
		return(1);
	}
	else {
		MesPrint("&%s is not a function",s);
		numfunc = AddFunction(s,0,0,0,0) + FUNCTION;
		return(1);
	}
	Add3Com(option,numfunc);
	return(0);
}

/*
  	#] DoChain : 
  	#[ CoChainin :

	Syntax: Chainin functionname;
*/

int
CoChainin ARG1(UBYTE *,s)
{
	return(DoChain(s,TYPECHAININ));
}

/*
  	#] CoChainin : 
  	#[ CoChainout :

	Syntax: Chainout functionname;
*/

int
CoChainout ARG1(UBYTE *,s)
{
	return(DoChain(s,TYPECHAINOUT));
}

/*
  	#] CoChainout : 
  	#[ CoExit :
*/

int
CoExit ARG1(UBYTE *,s)
{
	UBYTE *name;
	WORD code = TYPEEXIT;
	while ( *s == ',' ) s++;
	if ( *s == 0 ) {
		Add3Com(TYPEEXIT,0);
		return(0);
	}
	name = s+1;
	s++;
	while ( *s ) { if ( *s == '\\' ) s++; s++; }
	if ( name[-1] != '"' || s[-1] != '"' ) {
		MesPrint("&Illegal syntax for exit statement");
		return(1);
	}
	s[-1] = 0;
	AddComString(1,&code,name,0);
	s[-1] = '"';
	return(0);
}

/*
  	#] CoExit : 
  	#[ CoInExpression :
*/

int
CoInExpression ARG1(UBYTE *,s)
{
	GETIDENTITY
	UBYTE *t, c;
	WORD *w, number;
	int error = 0;
	w = AT.WorkPointer;
	if ( AC.inexprlevel >= MAXNEST ) {
		MesPrint("@Nesting of inexpression statements more than %d levels",(WORD)MAXNEST);
		return(-1);
	}
	AC.inexprsumcheck[AC.inexprlevel] = AC.IfLevel + AC.RepLevel
				+ AC.arglevel + AC.termlevel + AC.insidelevel;
	AC.inexprstack[AC.inexprlevel] = cbuf[AC.cbufnum].Pointer
								 - cbuf[AC.cbufnum].Buffer + 2;
	AC.inexprlevel++;
	*w++ = TYPEINEXPRESSION;
	w++; w++;
	for(;;) {	/* Look for a (comma separated) list of variables */
		while ( *s == ',' ) s++;
		if ( *s == 0 ) break;
		if ( *s == '[' || FG.cTable[*s] == 0 ) {
			t = s;
			if ( ( s = SkipAName(s) ) == 0 ) {
				MesPrint("&Improper name for an expression: '%s'",t);
				return(1);
			}
			c = *s; *s = 0;
			if ( GetName(AC.exprnames,t,&number,NOAUTO) == CEXPRESSION ) {
				*w++ = number;
			}
			else if ( GetName(AC.varnames,t,&number,NOAUTO) != NAMENOTFOUND ) {
				MesPrint("&%s is not an expression",t);
				error = 1;
			}
			*s = c;
		}
		else {
			MesPrint("&Illegal object in InExpression statement");
			error = 1;
			while ( *s && *s != ',' ) s++;
			if ( *s == 0 ) break;
		}
	}
	AT.WorkPointer[1] = w - AT.WorkPointer;
	AddNtoL(AT.WorkPointer[1],AT.WorkPointer);
	return(error);
}

/*
  	#] CoInExpression : 
  	#[ CoEndInExpression :
*/

int
CoEndInExpression ARG1(UBYTE *,s)
{
	CBUF *C = cbuf+AC.cbufnum;
	while ( *s == ',' ) s++;
	if ( *s ) {
		MesPrint("&Illegal syntax for EndInExpression statement");
		return(1);
	}
	if ( AC.inexprlevel <= 0 ) {
		MesPrint("&EndInExpression without corresponding InExpression statement");
		return(1);
	}
	AC.inexprlevel--;
	cbuf[AC.cbufnum].Buffer[AC.inexprstack[AC.inexprlevel]] = C->numlhs;
	if ( AC.inexprsumcheck[AC.inexprlevel] != ( AC.IfLevel + AC.RepLevel
		+ AC.arglevel + AC.termlevel + AC.insidelevel ) ) {
		MesPrint("&Illegal nesting of if, repeat, argument, inside, inexpression");
		return(1);
	}
	return(0);
}

/*
  	#] CoEndInExpression : 
  	#[ CoSetExitFlag :
*/

int
CoSetExitFlag ARG1(UBYTE *,s)
{
	if ( *s ) {
		MesPrint("&Illegal syntax for the SetExitFlag statement");
		return(1);
	}
	Add2Com(TYPESETEXIT);
	return(0);
}

/*
  	#] CoSetExitFlag : 
  	#[ CoTryReplace :
*/

int CoTryReplace ARG1(UBYTE *,p)
{
	GETIDENTITY
	UBYTE *name, c;
	WORD *w, error = 0, i, which = -1, c1, minvec = 0;
	w = AT.WorkPointer;
	*w++ = TYPETRY;
	*w++ = 3;
	*w++ = 0;
	*w++ = REPLACEMENT;
	*w++ = FUNHEAD;
	FILLFUN(w)
/*
	Now we have to read a function argument for the replace_ function.
	Current arguments that we allow involve only single arguments
	that do not expand further. No brackets!
*/
	while ( *p ) {
/*
		No numbers yet
*/
		if ( *p == '-' && minvec == 0 && which == (CVECTOR+1) ) {
			minvec = 1; p++;
		}
		if ( *p == '[' || FG.cTable[*p] == 0 ) {
			name = p;
			if ( ( p = SkipAName(p) )  == 0 ) return(1);
			c = *p; *p = 0;
			i = GetName(AC.varnames,name,&c1,WITHAUTO);
			if ( which >= 0 && i >= 0 && i != CDUBIOUS && which != (i+1) ) {
				MesPrint("&Illegal combination of objects in TryReplace");
				error = 1;
			}
			else if ( minvec && i != CVECTOR && i != CDUBIOUS ) {
				MesPrint("&Currently a - sign can be used only with a vector in TryReplace");
				error = 1;
			}
			else switch ( i ) {
				case CSYMBOL: *w++ = -SYMBOL; *w++ = c1; break;
				case CVECTOR:
					if ( minvec ) *w++ = -MINVECTOR;
					else          *w++ = -VECTOR;
					*w++ = c1 + AM.OffsetVector;
					minvec = 0;
					break;
				case CINDEX: *w++ = -INDEX; *w++ = c1 + AM.OffsetIndex;
					if ( c1 >= AM.WilInd && c == '?' ) { *p++ = c; c = *p; }
					break;
				case CFUNCTION: *w++ = -c1-FUNCTION; break;
				case CDUBIOUS: minvec = 0; error = 1; break;
				default:
					MesPrint("&Illegal object type in TryReplace: %s",name);
					error = 1;
					i = 0;
					break;
			}
			if ( which < 0 ) which = i+1;
			else which = -1;
			*p = c;
			if ( *p == ',' ) p++;
			continue;
		}
		else {
			MesPrint("&Illegal object in TryReplace");
			error = 1;
			while ( *p && *p != ',' ) {
				if ( *p == '(' ) SKIPBRA3(p)
				else if ( *p == '{' ) SKIPBRA2(p)
				else if ( *p == '[' ) SKIPBRA1(p)
				else p++;
			}
		}
		if ( *p == ',' ) p++;
		if ( which < 0 ) which = 0;
		else which = -1;
	}
	if ( which >= 0 ) {
		MesPrint("&Odd number of arguments in TryReplace");
		error = 1;
	}
	i = w - AT.WorkPointer;
	AT.WorkPointer[1] = i;
	AT.WorkPointer[2] = i - 3;
	AT.WorkPointer[4] = i - 3;
	AddNtoL((int)i,AT.WorkPointer);
	return(error);
}

/*
  	#] CoTryReplace : 
  	#[ CoModulus :
*/

int CoModulus ARG1(UBYTE *,inp)
{
	UBYTE *p, c;
	WORD sign = 1, Retval;
	while ( *inp == '-' || *inp == '+' ) {
		if ( *inp == '-' ) sign = -sign;
		inp++;
	}
	p = inp;
	if ( FG.cTable[*inp] != 1 ) {
		MesPrint("&Invalid value for modulus:%s",inp);
		if ( AC.modpowers ) M_free(AC.modpowers,"AC.modpowers");
		AC.modpowers = 0;
		return(1);
	}
	do { inp++; } while ( FG.cTable[*inp] == 1 );
	c = *inp; *inp = 0;
	Retval = GetLong(p,(UWORD *)AC.cmod,&AC.ncmod);
	if ( sign < 0 ) AC.ncmod = -AC.ncmod;
	*p = c;
	if ( c == 0 ) return(Retval);
	else if ( c != ':' ) {
		MesPrint("&Illegal option for modulus %s",inp);
		if ( AC.modpowers ) M_free(AC.modpowers,"AC.modpowers");
		AC.modpowers = 0;
		return(1);
	}
	inp++;
	p = inp;
	while ( FG.cTable[*inp] == 1 ) inp++;
	if ( *inp ) {
		MesPrint("&Illegal character in option for modulus %s",inp);
		if ( AC.modpowers ) M_free(AC.modpowers,"AC.modpowers");
		AC.modpowers = 0;
		return(1);
	}
	if ( GetLong(p,(UWORD *)AC.powmod,&AC.npowmod) ) Retval = -1;
	if ( TakeModulus((UWORD *)AC.powmod,&AC.npowmod,AC.cmod,AC.ncmod,1) ) Retval = -1;
	if ( AC.npowmod == 0 ) {
		MesPrint("&Improper value for generator");
		Retval = -1;
	}
	if ( MakeModTable() ) Retval = -1;
	AC.DirtPow = 1;
	return(Retval);
}

/*
  	#] CoModulus : 
  	#[ CoRepeat :
*/

int CoRepeat ARG1(UBYTE *,inp)
{
	int error = 0;
	AC.RepSumCheck[AC.RepLevel] = AC.IfLevel + AC.arglevel + AC.insidelevel
		+ AC.termlevel;
	AC.RepLevel++;
	if ( AC.RepLevel > AM.RepMax ) {
		MesPrint("&Too many repeat levels. Maximum is %d",AM.RepMax);
		return(1);
	}
	Add3Com(TYPEREPEAT,-1)   /* Means indefinite */
	while ( *inp == ' ' || *inp == ',' || *inp == '\t' ) inp++;
	if ( *inp ) {
		error = CompileStatement(inp);
		if ( CoEndRepeat(inp) ) error = 1;
	}
	return(error);
}

/*
  	#] CoRepeat : 
  	#[ CoEndRepeat :
*/

int CoEndRepeat ARG1(UBYTE *,inp)
{
	CBUF *C = cbuf+AC.cbufnum;
	int level, error = 0, repeatlevel = 0;
	AC.RepLevel--;
	if ( AC.RepLevel < 0 ) {
		MesPrint("&EndRepeat without Repeat");
		AC.RepLevel = 0;
		return(1);
	}
	else if ( AC.RepSumCheck[AC.RepLevel] != AC.IfLevel + AC.arglevel
		+ AC.insidelevel + AC.termlevel ) {
		MesPrint("&Illegal nesting of if, repeat, argument");
		error = 1;
	}
	level = C->numlhs+1;
	while ( level > 0 ) {
		if ( C->lhs[--level][0] == TYPEREPEAT ) {
			if ( repeatlevel == 0 ) {
				Add3Com(TYPEENDREPEAT,level)
				return(error);
			}
			repeatlevel--;
		}
		else if ( C->lhs[level][0] == TYPEENDREPEAT ) repeatlevel++;
	}
	return(1);
}

/*
  	#] CoEndRepeat : 
  	#[ DoBrackets :

		Reads in the bracket information.
		Storage is in the form of a regular term.
		No subterms and arguments are allowed.
*/

int DoBrackets ARG2(UBYTE *,inp,int, par)
{
	GETIDENTITY
	UBYTE *p, *pp, c;
	WORD *to, i, type, *w, error = 0;
	WORD c1,c2, *WorkSave;
	int biflag;
	p = inp;
	WorkSave = to = AT.WorkPointer;
	to++;
	if ( AT.BrackBuf == 0 ) {
		AR.MaxBracket = 100;
		AT.BrackBuf = (WORD *)Malloc1(sizeof(WORD)*(AR.MaxBracket+1),"bracket buffer");
	}
	*AT.BrackBuf = 0;
	AR.BracketOn = 0;
	AC.bracketindexflag = 0;
	if ( p[-1] == ',' && *p ) p--;
	if ( p[-1] == '+' && *p ) { biflag = 1;  if ( *p != ',' ) { *--p = ','; } }
	else if ( p[-1] == '-' && *p ) { biflag = -1; if ( *p != ',' ) { *--p = ','; } }
	else biflag = 0;
	while ( *p == ',' ) {
redo:	AR.BracketOn++;
		while ( *p == ',' ) p++;
		if ( *p == 0 ) break;
		inp = pp = p;
		p = SkipAName(p);
		if ( p == 0 ) return(1);
		c = *p;
		*p = 0;
		type = GetName(AC.varnames,inp,&c1,WITHAUTO);
		if ( c == '.' ) {
			if ( type == CVECTOR || type == CDUBIOUS ) {
				*p++ = c;
				inp = p;
				p = SkipAName(p);
				if ( p == 0 ) return(1);
				c = *p;
				*p = 0;
				type = GetName(AC.varnames,inp,&c2,WITHAUTO);
				if ( type != CVECTOR && type != CDUBIOUS ) {
					MesPrint("&Not a vector in dotproduct in bracket statement: %s",inp);
					error = 1;
				}
				else type = CDOTPRODUCT;
			}
			else {
				MesPrint("&Illegal use of . after %s in bracket statement",inp);
				error = 1;
				*p++ = c;
				goto redo;
			}
		}
		switch ( type ) {
			case CSYMBOL :
				*to++ = SYMBOL; *to++ = 4; *to++ = c1; *to++ = 1; break;
			case CVECTOR :
				*to++ = INDEX; *to++ = 3; *to++ = AM.OffsetVector + c1; break;
			case CFUNCTION :
				*to++ = c1+FUNCTION; *to++ = FUNHEAD; *to++ = 0;
				FILLFUN3(to)
				break;
			case CDOTPRODUCT :
				*to++ = DOTPRODUCT; *to++ = 5; *to++ = c1 + AM.OffsetVector;
				*to++ = c2 + AM.OffsetVector; *to++ = 1; break;
			case CDELTA :
				*to++ = DELTA; *to++ = 4; *to++ = EMPTYINDEX; *to++ = EMPTYINDEX; break;
			default :
				MesPrint("&Illegal bracket request for %s",pp);
				error = 1; break;
		}
		*p = c;
	}
	if ( *p ) MesCerr("separator",p);
	*to++ = 1; *to++ = 1; *to++ = 3;
	*AT.WorkPointer = to - AT.WorkPointer;
	AT.WorkPointer = to;
	AC.BracketNormalize = 1;
	if ( Normalize(BHEAD WorkSave) ) { error = 1; AR.BracketOn = 0; }
	else {
		w = WorkSave;
		if ( *w == 4 || !*w ) { AR.BracketOn = 0; }
		else {
			i = *(w+*w-1);
			if ( i < 0 ) i = -i;
			*w -= i;
			i = *w;
			if ( i > AR.MaxBracket ) {
				WORD *newbuf;
				newbuf = (WORD *)Malloc1(sizeof(WORD)*(i+1),"bracket buffer");
				AR.MaxBracket = i;
				if ( AT.BrackBuf != 0 ) M_free(AT.BrackBuf,"bracket buffer");
				AT.BrackBuf = newbuf;
			}
			to = AT.BrackBuf;
			NCOPY(to,w,i);
		}
	}
	AC.BracketNormalize = 0;
	if ( par == 1 ) AR.BracketOn = -AR.BracketOn;
	if ( error == 0 ) AC.bracketindexflag = biflag;
	AT.WorkPointer = WorkSave;
	return(error);
}

/*
  	#] DoBrackets : 
  	#[ CoBracket :
*/

int CoBracket ARG1(UBYTE *,inp)
{ return(DoBrackets(inp,0)); }

/*
  	#] CoBracket : 
  	#[ CoAntiBracket :
*/

int CoAntiBracket ARG1(UBYTE *,inp)
{ return(DoBrackets(inp,1)); }

/*
  	#] CoAntiBracket : 
  	#[ CountComp :

		This routine reads the count statement. The syntax is:
		count minimum,object,size[,object,size]
		Objects can be:
			symbol
			dotproduct
			vector
			function
		Vectors can have the auxiliary flags:
			+v +f +d +?setname

		Output for the compiler:
		TYPECOUNT,size,minimum,objects
		with the objects:
		SYMBOL,4,number,size
		DOTPRODUCT,5,v1,v2,size
		FUNCTION,4,number,size
		VECTOR,5,number,bits,size or VECTOR,6,number,bits,setnumber,size

		Currently only used in the if statement
*/

WORD *CountComp ARG2(UBYTE *,inp, WORD *,to)
{
	GETIDENTITY
	UBYTE *p, c;
	WORD *w, mini = 0, type, c1, c2;
	int error = 0;
	p = inp;
	w = to;
	AR.Eside = 2;
	*w++ = TYPECOUNT;
	*w++ = 0;
	*w++ = 0;
	while ( *p == ',' ) {
		p++; inp = p;
		if ( *p == '[' || FG.cTable[*p] == 0 ) {
			if ( ( p = SkipAName(inp) ) == 0 ) return(0);
			c = *p; *p = 0;
			type = GetName(AC.varnames,inp,&c1,WITHAUTO);
			if ( c == '.' ) {
				if ( type == CVECTOR || type == CDUBIOUS ) {
					*p++ = c;
					inp = p;
					p = SkipAName(p);
					if ( p == 0 ) return(0);
					c = *p;
					*p = 0;
					type = GetName(AC.varnames,inp,&c2,WITHAUTO);
					if ( type != CVECTOR && type != CDUBIOUS ) {
						MesPrint("&Not a vector in dotproduct in bracket statement: %s",inp);
						error = 1;
					}
					else type = CDOTPRODUCT;
				}
				else {
					MesPrint("&Illegal use of . after %s in bracket statement",inp);
					if ( type == NAMENOTFOUND )
						MesPrint("&%s is not a properly declared variable",inp);
					error = 1;
					*p++ = c;
					while ( *p && *p != ')' && *p != ',' ) p++;
					if ( *p == ',' && FG.cTable[p[1]] == 1 ) {
						p++;
						while ( *p && *p != ')' && *p != ',' ) p++;
					}
					continue;
				}
			}
			*p = c;
			switch ( type ) {
				case CSYMBOL:
					*w++ = SYMBOL; *w++ = 4; *w++ = c1;
Sgetnum:			if ( *p != ',' ) {
						MesCerr("sequence",p);
						while ( *p && *p != ')' && *p != ',' ) p++;
						error = 1;
					}
					p++; inp = p;
					ParseSignedNumber(mini,p)
					if ( FG.cTable[p[-1]] != 1 || ( *p && *p != ')' && *p != ',' ) ) {
						while ( *p && *p != ')' && *p != ',' ) p++;
						error = 1;
						c = *p; *p = 0;
						MesPrint("&Improper value in count: %s",inp);
						*p = c;
						while ( *p && *p != ')' && *p != ',' ) p++;
					}
					*w++ = mini;
					break;
				case CFUNCTION:
					*w++ = FUNCTION; *w++ = 4; *w++ = c1+FUNCTION; goto Sgetnum;
				case CDOTPRODUCT:
					*w++ = DOTPRODUCT; *w++ = 5;
					*w++ = c2 + AM.OffsetVector;
					*w++ = c1 + AM.OffsetVector;
					goto Sgetnum;
				case CVECTOR:
					*w++ = VECTOR; *w++ = 5;
					*w++ = c1 + AM.OffsetVector;
					if ( *p == ',' ) {
						*w++ = VECTBIT | DOTPBIT | FUNBIT;
						goto Sgetnum;
					}
					else if ( *p == '+' ) {
						p++;
						*w = 0;
						while ( *p && *p != ',' ) {
							if ( *p == 'v' || *p == 'V' ) {
								*w |= VECTBIT; p++;
							}
							else if ( *p == 'd' || *p == 'D' ) {
								*w |= DOTPBIT; p++;
							}
							else if ( *p == 'f' || *p == 'F'
							|| *p == 't' || *p == 'T' ) {
								*w |= FUNBIT; p++;
							}
							else if ( *p == '?' ) {
								p++; inp = p;
								if ( *p == '{' ) { /* } */
									SKIPBRA2(p)
									if ( p == 0 ) return(0);
									if ( ( c1 = DoTempSet(inp+1,p) ) < 0 ) return(0);
									if ( Sets[c1].type != CFUNCTION ) {
										MesPrint("&set type conflict: Function expected");
										return(0);
									}
									type = CSET;
									c = *++p;
								}
								else {
									p = SkipAName(p);
									if ( p == 0 ) return(0);
									c = *p; *p = 0;
									type = GetName(AC.varnames,inp,&c1,WITHAUTO);
								}
								if ( type != CSET && type != CDUBIOUS ) {
									MesPrint("&%s is not a set",inp);
									error = 1;
								}
								w[-2] = 6;
								*w++ |= SETBIT;
								*w++ = c1;
								*p = c;
								goto Sgetnum;
							}
							else {
								MesCerr("specifier for vector",p);
								error = 1;
							}
						}
						w++;
						goto Sgetnum;
					}
					else {
						MesCerr("specifier for vector",p);
						while ( *p && *p != ')' && *p != ',' ) p++;
						error = 1;
						*w++ = VECTBIT | DOTPBIT | FUNBIT;
						goto Sgetnum;
					}
				case CDUBIOUS:
					goto skipfield;
				default:
					*p = 0;
					MesPrint("&%s is not a symbol, function, vector or dotproduct",inp);
					error = 1;
skipfield:			while ( *p && *p != ')' && *p != ',' ) p++;
					if ( *p && FG.cTable[p[1]] == 1 ) {
						p++;
						while ( *p && *p != ')' && *p != ',' ) p++;
					}
					break;
			}
		}
		else {
			MesCerr("name",p);
			while ( *p && *p != ',' ) p++;
			error = 1;
		}
	}
	to[1] = w-to;
	if ( *p == ')' ) p++;
	if ( *p ) { MesCerr("end of statement",p); return(0); }
	if ( error ) return(0);
	return(w);
}

/*
  	#] CountComp : 
  	#[ CoIf :

		Reads the if statement: There must be a pair of parentheses.
		Much work is delegated to the routines in compi2 and CountComp.
		The goto is kept hanging as it is forward.
		The address in which the label must be written is pushed on
		the AC.IfStack.

		Here we allow statements of the type
		if ( condition ) single statement;
		compile the if statement.
		test character at end
		if not ; or )
		copy the statement after the proper parenthesis to the
		beginning of the AC.iBuffer.
		Have it compiled.
		generate an endif statement.
*/

static UWORD *CIscratC = 0;

int CoIf ARG1(UBYTE *,inp)
{
	GETIDENTITY
	int error = 0, level;
	WORD *w, *ww, *u, *s, *OldWork, *OldSpace = AT.WorkSpace;
	WORD gotexp = 0;		/* Indicates whether there can be a condition */
	WORD lenpp, lenlev, ncoef, i, number;
	UBYTE *p, *pp, c;
	CBUF *C = cbuf+AC.cbufnum;
	LONG x;

	if ( *inp == '(' ) inp++;	/* Usually we enter at the bracket */

	if ( CIscratC == 0 )
		CIscratC = (UWORD *)Malloc1((AM.MaxTal+2)*sizeof(UWORD),"CoIf");
	lenpp = 0;
	lenlev = 1;
	if ( AC.IfLevel >= AC.MaxIf ) DoubleIfBuffers();
	AC.IfCount[lenpp++] = 0;
/*
	IfStack is used for organizing the 'go to' for the various if levels
*/
	*AC.IfStack++ = C->Pointer-C->Buffer+2;
/*
	IfSumCheck is used to test for illegal nesting of if, argument or repeat.
*/
	AC.IfSumCheck[AC.IfLevel++] = AC.RepLevel + AC.arglevel + AC.insidelevel
		+ AC.termlevel;
	w = OldWork = AT.WorkPointer;
	*w++ = TYPEIF;
	w += 2;
	p = inp;
	for(;;) {
		inp = p;
		level = 0;
ReDo:
		if ( FG.cTable[*p] == 1 ) {		/* Number */
			if ( gotexp == 1 ) { MesCerr("position for )",p); error = 1; }
			u = w;
			*w++ = LONGNUMBER;
			w += 2;
			if ( GetLong(p,(UWORD *)w,&ncoef) ) { ncoef = 1; error = 1; }
			w[-1] = ncoef;
			while ( FG.cTable[*++p] == 1 );
			if ( *p == '/' ) {
				p++;
				if ( FG.cTable[*p] != 1 ) {
					MesCerr("sequence",p); error = 1; goto OnlyNum;
				}
				if ( GetLong(p,CIscratC,&ncoef) ) {
					ncoef = 1; error = 1;
				}
				while ( FG.cTable[*++p] == 1 );
				if ( ncoef == 0 ) {
					MesPrint("&Division by zero!");
					error = 1;
				}
				else {
					if ( w[-1] != 0 ) {
						if ( Simplify(BHEAD (UWORD *)w,(WORD *)(w-1),
						CIscratC,&ncoef) ) error = 1;
						else {
							i = w[-1];
							if ( i >= ncoef ) {
								i = w[-1];
								w += i;
								i -= ncoef;
								s = (WORD *)CIscratC;
								NCOPY(w,s,ncoef);
								while ( --i >= 0 ) *w++ = 0;
							}
							else {
								w += i;
								i = ncoef - i;
								while ( --i >= 0 ) *w++ = 0;
								s = (WORD *)CIscratC;
								NCOPY(w,s,ncoef);
							}
						}
					}
				}
			}
			else {
OnlyNum:
				w += ncoef;
				if ( ncoef > 0 ) {
					ncoef--; *w++ = 1;
					while ( --ncoef >= 0 ) *w++ = 0;
				}
			}
			u[1] = WORDDIF(w,u);
			u[2] = (u[1] - 3)>>1;
			if ( level ) u[2] = -u[2];
			gotexp = 1;
		}
		else if ( *p == '+' ) { p++; goto ReDo; }
		else if ( *p == '-' ) { level ^= 1; p++; goto ReDo; }
		else if ( *p == 'c' || *p == 'C' ) {	/* Count or Coefficient */
			if ( gotexp == 1 ) { MesCerr("position for )",p); error = 1; }
			while ( FG.cTable[*++p] == 0 );
			c = *p; *p = 0;
			if ( !StrICmp(inp,(UBYTE *)"count") ) {
				*p = c;
				if ( c != '(' ) {
					MesPrint("&no ( after count");
					error = 1;
					goto endofif;
				}
				inp = p;
				SKIPBRA4(p);
				c = *++p; *p = 0; *inp = ',';
				w = CountComp(inp,w);
				*p = c; *inp = '(';
				if ( w == 0 ) { error = 1; goto endofif; }
				gotexp = 1;
			}
			else if ( ConWord(inp,(UBYTE *)"coefficient") && ( p - inp ) > 3 ) {
				*w++ = COEFFI;
				*w++ = 2;
				*p = c;
				gotexp = 1;
			}
			else goto NoGood;
			inp = p;
		}
		else if ( *p == 'm' || *p == 'M' ) {	/* match */
			if ( gotexp == 1 ) { MesCerr("position for )",p); error = 1; }
			while ( !FG.cTable[*++p] );
			c = *p; *p = 0;
			if ( !StrICmp(inp,(UBYTE *)"match") ) {
				*p = c;
				if ( c != '(' ) {
					MesPrint("&no ( after match");
					error = 1;
					goto endofif;
				} 
				p++; inp = p;
				SKIPBRA4(p);
				*p = '=';
/*
				Now we can call the reading of the lhs of an id statement.
				This has to be modified in the future.
*/
				AT.WorkSpace = AT.WorkPointer = w;
				AC.idoption = SUBMULTI;
				level = CoIdExpression(inp,TYPEIF);
				AT.WorkSpace = OldSpace;
				AT.WorkPointer = OldWork;
				if ( level != 0 ) {
					if ( level < 0 ) { error = -1; goto endofif; }
					error = 1;
				}
/*
				If we pop numlhs we are in good shape
*/
				s = u = C->lhs[C->numlhs];
				while ( u < C->Pointer ) *w++ = *u++;
				C->numlhs--; C->Pointer = s;
				*p++ = ')';
				inp = p;
				gotexp = 1;
			}
			else if ( !StrICmp(inp,(UBYTE *)"multipleof") ) {
			if ( gotexp == 1 ) { MesCerr("position for )",p); error = 1; }
				*p = c;
				if ( c != '(' ) {
					MesPrint("&no ( after multipleof");
					error = 1; goto endofif;
				}
				p++;
				if ( FG.cTable[*p] != 1 ) {
Nomulof:			MesPrint("&multipleof needs a short positive integer argument");
					error = 1; goto endofif;
				}
				ParseNumber(x,p)
				if ( *p != ')' || x <= 0 || x > MAXPOSITIVE ) goto Nomulof;
				p++;
				*w++ = MULTIPLEOF; *w++ = 3; *w++ = (WORD)x;
				inp = p;
				gotexp = 1;
			}
			else {
NoGood:			MesPrint("&Unrecognized word: %s",inp);
				*p = c;
				error = 1;
				level = 0;
				if ( c == '(' ) SKIPBRA4(p)
				inp = ++p;
				gotexp = 1;
			}
		}
		else if ( *p == 'f' || *p == 'F' ) {	/* FindLoop */
			if ( gotexp == 1 ) { MesCerr("position for )",p); error = 1; }
			while ( FG.cTable[*++p] == 0 );
			c = *p; *p = 0;
			if ( !StrICmp(inp,(UBYTE *)"findloop") ) {
				*p = c;
				if ( c != '(' ) {
					MesPrint("&no ( after findloop");
					error = 1;
					goto endofif;
				}
				inp = p;
				SKIPBRA4(p);
				c = *++p; *p = 0; *inp = ',';
				if ( CoFindLoop(inp) ) goto endofif;
				s = u = C->lhs[C->numlhs];
				while ( u < C->Pointer ) *w++ = *u++;
				C->numlhs--; C->Pointer = s;
				*p = c; *inp = '(';
				if ( w == 0 ) { error = 1; goto endofif; }
				gotexp = 1;
			}
			else goto NoGood;
			inp = p;
		}
		else if ( *p == 'e' || *p == 'E' ) { /* Expression */
			if ( gotexp == 1 ) { MesCerr("position for )",p); error = 1; }
			while ( FG.cTable[*++p] == 0 );
			c = *p; *p = 0;
			if ( !StrICmp(inp,(UBYTE *)"expression") ) {
				*p = c;
				if ( c != '(' ) {
					MesPrint("&no ( after expression");
					error = 1;
					goto endofif;
				}
				p++; ww = w; *w++ = IFEXPRESSION; w++;
				while ( *p != ')' ) {
					if ( *p == ',' ) { p++; continue; }
					if ( *p == '[' || FG.cTable[*p] == 0 ) {
						pp = p;
						if ( ( p = SkipAName(p) ) == 0 ) {
							MesPrint("&Improper name for an expression: '%s'",pp);
							error = 1;
							goto endofif;
						}
						c = *p; *p = 0;
						if ( GetName(AC.exprnames,pp,&number,NOAUTO) == CEXPRESSION ) {
							*w++ = number;
						}
						else if ( GetName(AC.varnames,pp,&number,NOAUTO) != NAMENOTFOUND ) {
							MesPrint("&%s is not an expression",pp);
							error = 1;
							*w++ = number;
						}
						*p = c;
					}
					else {
						MesPrint("&Illegal object in Expression in if-statement");
						error = 1;
						while ( *p && *p != ',' && *p != ')' ) p++;
						if ( *p == 0 || *p == ')' ) break;
					}
				}
				ww[1] = w - ww;
				p++;
				gotexp = 1;
			}
			else goto NoGood;
			inp = p;
		}
		else if ( *p == '$' ) {
			if ( gotexp == 1 ) { MesCerr("position for )",p); error = 1; }
			p++; inp = p;
			while ( FG.cTable[*p] == 0 || FG.cTable[*p] == 1 ) p++;
			c = *p; *p = 0;
			if ( ( i = GetDollar(inp) ) < 0 ) {
				MesPrint("&undefined dollar expression %s",inp);
				error = 1;
				i = AddDollar(inp,DOLUNDEFINED,0,0);
			}
			*p = c;
			*w++ = IFDOLLAR; *w++ = 3; *w++ = i;
			inp = p;
			gotexp = 1;
		}
		else if ( *p == '(' ) {
			if ( gotexp ) {
				MesCerr("parenthesis",p);
				error = 1;
				goto endofif;
			}
			gotexp = 0;
			if ( ++lenlev >= AC.MaxIf ) DoubleIfBuffers();
			AC.IfCount[lenpp++] = w-OldWork;
			*w++ = SUBEXPR;
			w += 2;
			p++;
		}
		else if ( *p == ')' ) {
			if ( gotexp == 0 ) { MesCerr("position for )",p); error = 1; }
			gotexp = 1;
			u = AC.IfCount[--lenpp]+OldWork;
			lenlev--;
			u[1] = w - u;
			if ( lenlev <= 0 ) {	/* End if condition */
				AT.WorkSpace = OldSpace;
				AT.WorkPointer = OldWork;
				AddNtoL(OldWork[1],OldWork);
				p++;
				if ( *p == ')' ) {
					MesPrint("&unmatched parenthesis in if/while ()");
					error = 1;
					while ( *++p == ')' );
				}
				if ( *p ) {
					level = CompileStatement(p);
					if ( level ) error = level;
					if ( CoEndIf(p) && error == 0 ) error = 1;
				}
				return(error);
			}
			p++;
		}
		else if ( *p == '>' ) {
			if ( gotexp == 0 ) goto NoExp;
			if ( p[1] == '=' ) { *w++ = GREATEREQUAL; *w++ = 2; p += 2; }
			else               { *w++ = GREATER;      *w++ = 2; p++; }
			gotexp = 0;
		}
		else if ( *p == '<' ) {
			if ( gotexp == 0 ) goto NoExp;
			if ( p[1] == '=' ) { *w++ = LESSEQUAL; *w++ = 2; p += 2; }
			else               { *w++ = LESS;      *w++ = 2; p++; }
			gotexp = 0;
		}
		else if ( *p == '=' ) {
			if ( gotexp == 0 ) goto NoExp;
			if ( p[1] == '=' ) p++;
			*w++ = EQUAL; *w++ = 2; p++;
			gotexp = 0;
		}
		else if ( *p == '!' && p[1] == '=' ) {
			if ( gotexp == 0 ) { p++; goto NoExp; }
			*w++ = NOTEQUAL; *w++ = 2; p += 2;
			gotexp = 0;
		}
		else if ( *p == '|' && p[1] == '|' ) {
			if ( gotexp == 0 ) { p++; goto NoExp; }
			*w++ = ORCOND; *w++ = 2; p += 2;
			gotexp = 0;
		}
		else if ( *p == '&' && p[1] == '&' ) {
			if ( gotexp == 0 ) {
				p++;
NoExp:			p++;
				MesCerr("sequence",p);
				error = 1;
			}
			else {
				*w++ = ANDCOND; *w++ = 2; p += 2;
				gotexp = 0;
			}
		}
		else if ( *p == 0 ) {
			MesPrint("&Unmatched parentheses");
			error = 1;
			goto endofif;
		}
		else {
			if ( FG.cTable[*p] == 0 ) {
				WORD ij;
				inp = p;
				while ( ( ij = FG.cTable[*++p] ) == 0 || ij == 1 );
				c = *p; *p = 0;
				goto NoGood;
			}
			MesCerr("sequence",p);
			error = 1;
			p++;
		}
	}
endofif:;
	return(error);
}

/*
  	#] CoIf :
  	#[ CoElse :
*/

int CoElse ARG1(UBYTE *,p)
{
	int error = 0;
	CBUF *C = cbuf+AC.cbufnum;
	if ( *p != 0 ) {
		while ( *p == ',' ) p++;
		if ( tolower(*p) == 'i' && tolower(p[1]) == 'f' && p[2] == '(' )
													return(CoElseIf(p+2));
	}
	if ( AC.IfLevel <= 0 ) { MesPrint("&else statement without if"); return(1); }
	if ( AC.IfSumCheck[AC.IfLevel-1] != AC.RepLevel + AC.arglevel + AC.insidelevel
		+ AC.termlevel ) {
		MesPrint("&Illegal nesting of if, repeat, argument"); error = 1;
	}
	Add3Com(TYPEELSE,AC.IfLevel)
	C->Buffer[AC.IfStack[-1]] = C->numlhs;
	AC.IfStack[-1] = C->Pointer - C->Buffer - 1;
	return(error);
}

/*
  	#] CoElse : 
  	#[ CoElseIf :
*/

int CoElseIf ARG1(UBYTE *,inp)
{
	CBUF *C = cbuf+AC.cbufnum;
	if ( AC.IfLevel <= 0 ) { MesPrint("&elseif statement without if"); return(1); }
	Add3Com(TYPEELSE,-AC.IfLevel)
	AC.IfLevel--;
	C->Buffer[*--AC.IfStack] = C->numlhs;
	return(CoIf(inp));
}

/*
  	#] CoElseIf : 
  	#[ CoEndIf :

		It puts a RHS-level at the position indicated in the AC.IfStack.
		This corresponds to the label belonging to a forward goto.
		It is the goto that belongs either to the failing condition
		of the if (no else statement), or the completion of the
		success path (with else statement)
		The code is a jump to the next statement. It is there to prevent
		problems with
		if ( .. )
			if ( .. ) 
			endif;
		elseif ( .. )
*/

int CoEndIf ARG1(UBYTE *,inp)
{
	CBUF *C = cbuf+AC.cbufnum;
	WORD i = C->numlhs, to, k = -AC.IfLevel;
	int error = 0;
	if ( AC.IfLevel <= 0 ) {
		MesPrint("&Endif statement without corresponding if"); return(1);
	}
	AC.IfLevel--;
	C->Buffer[*--AC.IfStack] = i+1;
	if ( AC.IfSumCheck[AC.IfLevel] != AC.RepLevel + AC.arglevel + AC.insidelevel
		+ AC.termlevel ) {
		MesPrint("&Illegal nesting of if, repeat, argument");
		error = 1;
	}
	Add3Com(TYPEENDIF,i+1)
/*
	Now the search for the TYPEELSE in front of the elseif statements
*/
	to = C->numlhs;
    while ( i > 0 ) {
		if ( C->lhs[i][0] == TYPEELSE && C->lhs[i][2] == to ) to = i;
		if ( C->lhs[i][0] == TYPEIF ) {
			if ( C->lhs[i][2] == to ) {
				i--;
				if ( i <= 0 || C->lhs[i][0] != TYPEELSE
				|| C->lhs[i][2] != k ) break;
				C->lhs[i][2] = C->numlhs;
				to = i;
			}
		}
		i--;
	}
	return(error);
}

/*
  	#] CoEndIf : 
  	#[ CoWhile :
*/

int CoWhile ARG1(UBYTE *,inp)
{
	CBUF *C = cbuf+AC.cbufnum;
	WORD startnum = C->numlhs + 1;
	int error;
	AC.WhileLevel++;
	error = CoIf(inp);
	if ( C->numlhs > startnum && C->lhs[startnum][2] == C->numlhs
							&& C->lhs[C->numlhs][0] == TYPEENDIF ) {
		C->lhs[C->numlhs][2] = startnum-1;
		AC.WhileLevel--;
	}
	else C->lhs[startnum][2] = startnum;
	return(error);
}

/*
  	#] CoWhile : 
  	#[ CoEndWhile :
*/

int CoEndWhile ARG1(UBYTE *,inp)
{
	int error = 0;
	WORD i;
	CBUF *C = cbuf+AC.cbufnum;
	if ( AC.WhileLevel <= 0 ) {
		MesPrint("&EndWhile statement without corresponding While"); return(1);
	}
	AC.WhileLevel--;
	i = C->Buffer[AC.IfStack[-1]];
	error = CoEndIf(inp);
	C->lhs[C->numlhs][2] = i - 1;
	return(error);
}

/*
  	#] CoEndWhile : 
  	#[ DoFindLoop :

	Function,arguments=number,loopsize=number,outfun=function,include=index;
*/

static char *messfind[] = {
	"Findloop(function,arguments=#,loopsize(=#|<#)[,include=index])"
   ,"Replaceloop,function,arguments=#,loopsize(=#|<#),outfun=function[,include=index]"
	};
static WORD comfindloop[7] = { TYPEFINDLOOP,7,0,0,0,0,0 };

int DoFindLoop ARG2(UBYTE *,inp,int,mode)
{
	UBYTE *s, c;
	WORD funnum, nargs = 0, nloop = 0, indexnum = 0, outfun = 0;
	int type, aflag, lflag, indflag, outflag, error = 0, sym;
	while ( *inp == ',' ) inp++;
	if ( ( s = SkipAName(inp) ) == 0 ) {
syntax:	MesPrint("&Proper syntax is:");
		MesPrint("%s",messfind[mode]);
		return(1);
	}
	c = *s; *s = 0;
	if ( ( ( type = GetName(AC.varnames,inp,&funnum,WITHAUTO) ) == NAMENOTFOUND )
		|| type != CFUNCTION || ( ( sym = (functions[funnum].symmetric) & ~REVERSEORDER )
		!= SYMMETRIC && sym != ANTISYMMETRIC ) ) {
		MesPrint("&%s should be a (anti)symmetric function or tensor",inp);
	}
	funnum += FUNCTION;
	*s = c; inp = s;
	aflag = lflag = indflag = outflag = 0;
	while ( *inp == ',' ) {
		while ( *inp == ',' ) inp++;
		s = inp;
		if ( ( s = SkipAName(inp) ) == 0 ) goto syntax;
		c = *s; *s = 0;
		if ( StrICont(inp,(UBYTE *)"arguments") == 0 ) {
			if ( c != '=' ) goto syntax;
			*s++ = c;
			NeedNumber(nargs,s,syntax)
			aflag++;
			inp = s;
		}
		else if ( StrICont(inp,(UBYTE *)"loopsize") == 0 ) {
			if ( c != '=' && c != '<' ) goto syntax;
			*s++ = c;
			if ( FG.cTable[*s] == 1 ) {
				NeedNumber(nloop,s,syntax)
				if ( nloop < 2 ) {
					MesPrint("&loopsize should be at least 2");
					error = 1;
				}
				if ( c == '<' ) nloop = -nloop;
			}
			else if ( tolower(*s) == 'a' && tolower(s[1]) == 'l'
			&& tolower(s[2]) == 'l' && FG.cTable[s[3]] > 1 ) {
				nloop = -1; s += 3;
				if ( c != '=' ) goto syntax;
			}
			inp = s;
			lflag++;
		}
		else if ( StrICont(inp,(UBYTE *)"include") == 0 ) {
			if ( c != '=' ) goto syntax;
			*s++ = c;
			if ( ( inp = SkipAName(s) ) == 0 ) goto syntax;
			c = *inp; *inp = 0;
			if ( ( type = GetName(AC.varnames,s,&indexnum,WITHAUTO) ) != CINDEX ) {
				MesPrint("&%s is not a proper index",s);
				error = 1;
			}
			else if ( indexnum < WILDOFFSET
			&& indices[indexnum].dimension == 0 ) {
				MesPrint("&%s should be a summable index",s);
				error = 1;
			}
			indexnum += AM.OffsetIndex;
			*inp = c;
			indflag++;
		}
		else if ( StrICont(inp,(UBYTE *)"outfun") == 0 ) {
			if ( c != '=' ) goto syntax;
			*s++ = c;
			if ( ( inp = SkipAName(s) ) == 0 ) goto syntax;
			c = *inp; *inp = 0;
			if ( ( type = GetName(AC.varnames,s,&outfun,WITHAUTO) ) != CFUNCTION ) {
				MesPrint("&%s is not a proper function or tensor",s);
				error = 1;
			}
			outfun += FUNCTION;
			outflag++;
			*inp = c;
		}
		else {
			MesPrint("&Unrecognized option in FindLoop or ReplaceLoop: %s",inp);
			*s = c; inp = s;
			while ( *inp && *inp != ',' ) inp++;
		}
	}
	if ( *inp != 0 && mode == REPLACELOOP ) goto syntax;
	if ( mode == FINDLOOP && outflag > 0 ) {
		MesPrint("&outflag option is illegal in FindLoop");
		error = 1;
	}
	if ( mode == REPLACELOOP && outflag == 0 ) goto syntax;
	if ( aflag == 0 || lflag == 0 ) goto syntax;
	comfindloop[3] = funnum;
	comfindloop[4] = nloop;
	comfindloop[5] = nargs;
	comfindloop[6] = outfun;
	comfindloop[1] = 7;
	if ( indflag ) {
		if ( mode == 0 ) comfindloop[2] =  indexnum + 5;
		else             comfindloop[2] = -indexnum - 5;
	}
	else comfindloop[2] = mode;
	AddNtoL(comfindloop[1],comfindloop);
	return(error);
}

/*
  	#] DoFindLoop : 
  	#[ CoFindLoop :
*/

int CoFindLoop ARG1(UBYTE *,inp)
{ return(DoFindLoop(inp,FINDLOOP)); }

/*
  	#] CoFindLoop : 
  	#[ CoReplaceLoop :
*/

int CoReplaceLoop ARG1(UBYTE *,inp)
{ return(DoFindLoop(inp,REPLACELOOP)); }

/*
  	#] CoReplaceLoop : 
  	#[ CoFunPowers :
*/

static UBYTE *FunPowOptions[] = {
	 (UBYTE *)"nofunpowers"
	,(UBYTE *)"commutingonly"
	,(UBYTE *)"allfunpowers"
	};

int CoFunPowers ARG1(UBYTE *,inp)
{
	UBYTE *option, c;
	int i, maxoptions = sizeof(FunPowOptions)/sizeof(UBYTE *);
	while ( *inp == ',' ) inp++;
	option = inp;
	inp = SkipAName(inp); c = *inp; *inp = 0;
	for ( i = 0; i < maxoptions; i++ ) {
		if ( StrICont(option,FunPowOptions[i]) == 0 ) {
			if ( c ) {
				*inp = c;
				MesPrint("&Illegal FunPowers statement");
				return(1);
			}
			AC.funpowers = i;
			return(0);
		}
	}
	MesPrint("&Illegal option in FunPowers statement: %s",option);
	return(1);
}

/*
  	#] CoFunPowers : 
  	#[ CoUnitTrace :
*/

int CoUnitTrace ARG1(UBYTE *,s)
{
	WORD num, type;
	if ( FG.cTable[*s] == 1 ) {
		ParseNumber(num,s)
		if ( *s != 0 ) {
nogood:		MesPrint("&Value of UnitTrace should be a (positive) number or a symbol");
			return(1);
		}
		AC.lUniTrace[0] = SNUMBER;
		AC.lUniTrace[2] = num;
	}
	else {
		if ( ( type = GetName(AC.varnames,s,&num,WITHAUTO) ) == CSYMBOL ) {
			AC.lUniTrace[0] = SYMBOL;
			AC.lUniTrace[2] = num;
			num = -num;
		}
		else goto nogood;
		s = SkipAName(s);
		if ( *s ) goto nogood;
	}
	AC.lUnitTrace = num;
	return(0);
}

/*
  	#] CoUnitTrace : 
  	#[ CoTerm :

	Note: termstack holds the offset of the term statement in the compiler
	buffer. termsortstack holds the offset of the last sort statement
		(or the corresponding term statement)
*/

int
CoTerm ARG1(UBYTE *,s) {
	GETIDENTITY
	WORD *w = AT.WorkPointer;
	int error = 0;
	while ( *s == ',' ) s++;
	if ( *s ) {
		MesPrint("&Illegal syntax for Term statement");
		return(1);
	}
	if ( AC.termlevel+1 >= AC.maxtermlevel ) {
		if ( AC.maxtermlevel <= 0 ) {
			AC.maxtermlevel = 20;
			AC.termstack = (LONG *)Malloc1(AC.maxtermlevel*sizeof(LONG),"termstack");
			AC.termsortstack = (LONG *)Malloc1(AC.maxtermlevel*sizeof(LONG),"termsortstack");
			AC.termsumcheck = (WORD *)Malloc1(AC.maxtermlevel*sizeof(WORD),"termsumcheck");
		}
		else {
			DoubleBuffer((void **)AC.termstack,(void **)AC.termstack+AC.maxtermlevel,
				sizeof(LONG),"doubling termstack");
			DoubleBuffer((void **)AC.termsortstack,
				(void **)AC.termsortstack+AC.maxtermlevel,
				sizeof(LONG),"doubling termsortstack");
			DoubleBuffer((void **)AC.termsumcheck,
				(void **)AC.termsumcheck+AC.maxtermlevel,
				sizeof(LONG),"doubling termsumcheck");
			AC.maxtermlevel *= 2;
		}
	}
	AC.termsumcheck[AC.termlevel] = AC.IfLevel + AC.RepLevel + AC.insidelevel
								+ AC.arglevel;
	AC.termstack[AC.termlevel] = cbuf[AC.cbufnum].Pointer
			                 - cbuf[AC.cbufnum].Buffer + 2;
	AC.termsortstack[AC.termlevel] = AC.termstack[AC.termlevel] + 1;
	AC.termlevel++;
	*w++ = TYPETERM;
	w++;
	*w++ = cbuf[AC.cbufnum].numlhs;
	*w++ = cbuf[AC.cbufnum].numlhs;
	AT.WorkPointer[1] = w - AT.WorkPointer;
	AddNtoL(AT.WorkPointer[1],AT.WorkPointer);
	return(error);
}

/*
  	#] CoTerm : 
  	#[ CoEndTerm :
*/

int
CoEndTerm ARG1(UBYTE *,s)
{
	CBUF *C = cbuf+AC.cbufnum;
	while ( *s == ',' ) s++;
	if ( *s ) {
		MesPrint("&Illegal syntax for EndTerm statement");
		return(1);
	}
	if ( AC.termlevel <= 0 ) {
		MesPrint("&EndTerm without corresponding Argument statement");
		return(1);
	}
	AC.termlevel--;
	cbuf[AC.cbufnum].Buffer[AC.termstack[AC.termlevel]] = C->numlhs;
	cbuf[AC.cbufnum].Buffer[AC.termsortstack[AC.termlevel]] = C->numlhs;
	if ( AC.termsumcheck[AC.termlevel] != ( AC.IfLevel + AC.RepLevel
		 + AC.insidelevel + AC.arglevel ) ) {
		MesPrint("&Illegal nesting of if, repeat, argument, term");
		return(1);
	}
	return(0);
}

/*
  	#] CoEndTerm : 
  	#[ CoSort :
*/

int
CoSort ARG1(UBYTE *,s) {
	GETIDENTITY
	WORD *w = AT.WorkPointer;
	int error = 0;
	while ( *s == ',' ) s++;
	if ( *s ) {
		MesPrint("&Illegal syntax for Sort statement");
		error = 1;
	}
	if ( AC.termlevel <= 0 ) {
		MesPrint("&The Sort statement can only be used inside a term environment");
		error = 1;
	}
	if ( error ) return(error);
	*w++ = TYPESORT;
	w++;
	w++;
	cbuf[AC.cbufnum].Buffer[AC.termsortstack[AC.termlevel-1]] =
										*w = cbuf[AC.cbufnum].numlhs+1;
	w++;
	AC.termsortstack[AC.termlevel-1] = cbuf[AC.cbufnum].Pointer
			                 - cbuf[AC.cbufnum].Buffer + 3;
	if ( AC.termsumcheck[AC.termlevel-1] != ( AC.IfLevel + AC.RepLevel
		 + AC.insidelevel + AC.arglevel ) ) {
		MesPrint("&Illegal nesting of if, repeat, argument, term");
		return(1);
	}
	AT.WorkPointer[1] = w - AT.WorkPointer;
	AddNtoL(AT.WorkPointer[1],AT.WorkPointer);
	return(error);
}

/*
  	#] CoSort : 
  	#[ CoPolyFun :

	Collect,functionname
*/

int
CoPolyFun ARG1(UBYTE *,s)
{
	GETIDENTITY
	WORD numfun;
	int type;
	UBYTE *t;
	if ( *s == 0 ) {
		AR.PolyFun = AC.lPolyFun = 0;
		return(0);
	}
	t = SkipAName(s);
	if ( t == 0 || *t != 0 ) {
		MesPrint("&PolyFun statement needs a single function for its argument");
		return(1);
	}
	if ( ( ( type = GetName(AC.varnames,s,&numfun,WITHAUTO) ) != CFUNCTION )
	|| ( functions[numfun].spec != 0 ) ) {
		MesPrint("&%s should be a regular function",s);
		if ( type < 0 ) {
			if ( GetName(AC.exprnames,s,&numfun,NOAUTO) == NAMENOTFOUND )
				AddFunction(s,0,0,0,0);
		}
		return(1);
	}
	AR.PolyFun = AC.lPolyFun = numfun+FUNCTION;
	return(0);
}

/*
  	#] CoPolyFun : 
  	#[ CoMerge :
*/

int
CoMerge ARG1(UBYTE *,inp)
{
	UBYTE *s = inp;
	int type;
	WORD numfunc, option = 0;
	if ( tolower(s[0]) == 'o' && tolower(s[1]) == 'n' && tolower(s[2]) == 'c' &&
	     tolower(s[3]) == 'e' && tolower(s[4]) == ',' ) {
		option = 1; s += 5;
	}
	else if ( tolower(s[0]) == 'a' && tolower(s[1]) == 'l' && tolower(s[2]) == 'l' &&
	     tolower(s[3]) == ',' ) {
		option = 0; s += 4;
	}
	if ( *s == '$' ) {
		if ( ( type = GetName(AC.dollarnames,s+1,&numfunc,NOAUTO) ) == CDOLLAR )
			numfunc = -numfunc;
		else {
			MesPrint("&%s is undefined",s);
			numfunc = AddDollar(s+1,DOLINDEX,&one,1);
			return(1);
		}
tests:	s = SkipAName(s);
		if ( *s != 0 ) {
			MesPrint("&Merge should have a single function or $variable for its argument");
			return(1);
		}
	}
	else if ( ( type = GetName(AC.varnames,s,&numfunc,WITHAUTO) ) == CFUNCTION ) {
		numfunc += FUNCTION;
		goto tests;
	}
	else if ( type != -1 ) {
		if ( type != CDUBIOUS ) {
			NameConflict(type,s);
			type = MakeDubious(AC.varnames,s,&numfunc);
		}
		return(1);
	}
	else {
		MesPrint("&%s is not a function",s);
		numfunc = AddFunction(s,0,0,0,0) + FUNCTION;
		return(1);
	}
	Add4Com(TYPEMERGE,numfunc,option);
	return(0);
}

/*
  	#] CoMerge : 
  	#[ CoSlavePatch :
*/

int CoSlavePatch ARG1(UBYTE *,s)
{
	LONG x;
	while ( *s == ',' || *s == '=' ) s++;
	ParseNumber(x,s)
	if ( *s && *s != ' ' && *s != '\t' ) {
		MesPrint("&Numerical value expected for SlavePatchSize");
		return(1);
	}
	AC.SlavePatchSize = x;
	return(0);
}

/*
  	#] CoSlavePatch : 
  	#[ CoThreadBucket :
*/

int CoThreadBucket ARG1(UBYTE *,s)
{
	LONG x;
	while ( *s == ',' || *s == '=' ) s++;
	ParseNumber(x,s)
	if ( *s && *s != ' ' && *s != '\t' ) {
		MesPrint("&Numerical value expected for ThreadBucketSize");
		return(1);
	}
	if ( x <= 0 ) {
		Warning("Negative of zero value not allowed for ThreadBucketSize. Adjusted to 1.");
		x = 1;
	}
	AC.ThreadBucketSize = x;
#ifdef WITHPTHREADS
	if ( AS.MultiThreaded ) MakeThreadBuckets(-1,1);
#endif
	return(0);
}

/*
  	#] CoThreadBucket : 
  	#[ CoModulusGCD :

	ModulusGCD,m,x,fun1,fun2;
*/

int
CoModulusGCD ARG1(UBYTE *,s)
{
	UBYTE *t, c;
	LONG x = 0;
	int sgn = 1;
	WORD numfun1, numfun2, numsym, type, buff[6];
	while ( *s == '+' || *s == '-' ) {
		if ( *s == '-' ) sgn = -sgn;
		s++;
	}
	while ( FG.cTable[*s] == 1 ) x = 10*x + *s++ - 0x30;
	while ( *s == ',' ) s++;
	t = SkipAName(s);
	c = *t; *t = 0;
	if ( *s == '$' ) {
		if ( ( type = GetName(AC.dollarnames,s+1,&numsym,NOAUTO) ) == CDOLLAR )
			numsym = -numsym;
		else {
			MesPrint("&%s is undefined",s);
			numsym = AddDollar(s+1,DOLINDEX,&one,1);
			*t = c;
			return(1);
		}
	}
	else if ( ( type = GetName(AC.varnames,s,&numsym,WITHAUTO) ) != CSYMBOL ) {
		MesPrint("&%s is not a symbol",s);
		numsym = AddSymbol(s,-MAXPOWER,MAXPOWER,0);
		*t = c;
		return(1);
	}
	*t = c;
	while ( *t == ',' ) t++;
	s = SkipAName(t);
	c = *s; *s = 0;
	if ( *t == '$' ) {
		if ( ( type = GetName(AC.dollarnames,t+1,&numfun1,NOAUTO) ) == CDOLLAR )
			numfun1 = -numfun1;
		else {
			MesPrint("&%s is undefined",t);
			numfun1 = AddDollar(t+1,DOLINDEX,&one,1);
			*s = c;
			return(1);
		}
	}
	else {
		if ( ( type = GetName(AC.varnames,t,&numfun1,WITHAUTO) ) != CFUNCTION ) {
			MesPrint("&%s is not a function",t);
			numfun1 = AddFunction(t,0,0,0,0) + FUNCTION;
			*s = c;
			return(1);
		}
		else numfun1 += FUNCTION;
	}
	*s = c;
	while ( *s == ',' ) s++;
	t = SkipAName(s);
	c = *t; *t = 0;
	if ( *s == '$' ) {
		if ( ( type = GetName(AC.dollarnames,s+1,&numfun2,NOAUTO) ) == CDOLLAR )
			numfun2 = -numfun2;
		else {
			MesPrint("&%s is undefined",s);
			numfun2 = AddDollar(s+1,DOLINDEX,&one,1);
			*t = c;
			return(1);
		}
	}
	else {
		if ( ( type = GetName(AC.varnames,s,&numfun2,WITHAUTO) ) != CFUNCTION ) {
			MesPrint("&%s is not a function",s);
			numfun2 = AddFunction(s,0,0,0,0) + FUNCTION;
			*t = c;
			return(1);
		}
		else numfun2 += FUNCTION;
	}
	*t = c;
	while ( *t == ',' ) t++;
	if ( *t ) {
		MesPrint("&Proper syntax is ModulusGCD,modval,symbol,function1,function2");
		return(1);
	}
	buff[0] = TYPEMODULUSGCD;
	buff[1] = 6;
	buff[2] = x;
	buff[3] = numsym;
	buff[4] = numfun1;
	buff[5] = numfun2;
	AddNtoL(6,buff);
	return(0);
}

/*
  	#] CoModulusGCD : 
  	#[ CoPolyNorm :
*/

int
CoPolyNorm ARG1(UBYTE *,s)
{
	UBYTE c, *t;
	int i, type, error = 0;
	WORD numfun, *pn;
	WORD polynormstring[4];
	polynormstring[0] = TYPEPOLYNORM; polynormstring[1] = 4;
	pn = polynormstring+2;
	for ( i = 0; i < 2; i++ ) {
		if ( *s ) {
			t = s;
			s = SkipAName(s);
			c = *s; *s = 0;
			if ( ( ( type = GetName(AC.varnames,t,&numfun,WITHAUTO) ) != CFUNCTION )
			&& type != CDUBIOUS ) {
nofunc:			MesPrint("&%s is not a CFunction",t);
				error = 4;
				if ( type < 0 ) numfun = AddFunction(t,0,0,0,0);
			}
			else if ( ( functions[numfun].commute != 0 ) &&
					  ( functions[numfun].spec != 0 ) ) goto nofunc;
			numfun += FUNCTION;
			*s = c;
			if ( *s == ',' ) s++;
		}
		else {
			if ( error == 0 )
				MesPrint("&The PolyNorm statement needs two CFunctions for its arguments");
			error++;
			numfun = 0;
		}
		*pn++ = numfun;
	}
	if ( *s ) {
		if ( error == 0 )
			MesPrint("&The PolyNorm statement needs two CFunctions for its arguments");
		error++;
	}
	else {
		AddNtoL(4,polynormstring);
	}
	return(error);
}

/*
  	#] CoPolyNorm : 

*/
/* temporary commentary for forcing cvs merge */
