/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Application:     CSE772 --- Test Pattern Generation for PLA's          *
 *                                                                         *
 *  Language:        C Language                                            *
 *                                                                         *
 *  Compiler:        (1) Borland C++ 3.1                                   *
 *                       BCC -ml -G pla.c                                  *
 *                   (2) Unix C                                            *
 *                       gcc pla.c -lm                                     *
 *                                                                         *
 *  Platform:        (1) i386, DOS 5.0                                     *
 *                   (2) Sun Sparc, UNIX V                                 *
 *                                                                         *
 *  Author:          Jordon Lin                                            *
 *  Date:            Dec. 13, 1993                                         *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
#include "pla.h"
#include <math.h>
Cover cube_init(Cover head, char name[])
{
    head = (Cover) malloc(sizeof *head);
    head->next = head;
    head->vertex = (char *) calloc(strlen(name)+1,sizeof *name);
    strcpy(head->vertex, name);

    return head;
}

void deletenext(Cover c)
{
    Cover t;

    t = c->next->next;
    free(c->next->vertex);
    free(c->next);
    c->next = t;
}

Cover insertafter(char w[], Cover t)
{
    Cover x;

    x = (Cover) malloc(sizeof *x);
    x->vertex = (char *) calloc(strlen(w)+1, sizeof *w);
    strcpy(x->vertex, w);
    x->next = t->next;
    t->next = x;

    return x;
}
/*------------------------------------------------------------------------*/
int print_cover(Cover c)
{
    Cover t;
    int i;

    i = 0;
    t = c->next;
    while (t != c)
    {
	printf("%s\n", t->vertex);
	t = t-> next;
	i++;
    }

    return i;
}

/*------------------------------------------------------------------------*/
void free_cover(Cover t)
{
    while (t->next != t)
	deletenext(t);
    free(t->vertex);
    free(t);
}
void free_bucket(Bucket b, int outputs)
{
    int i;

    for (i = 0; i < outputs; i++)
	free_cover(b[i]);
    free(b);
}
void free_buckets(Buckets b, int outputs)
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
	for (j = 0; j < outputs; j++)
	    free_cover(b[i][j]);
	free(b[i]);
    }
    free(b);
}
/*------------------------------------------------------------------------*/
void get_parm(FILE *fp, char *name, char *type, int *inputs, int *outputs)
{
    int ok;

    ok = 1;
    while (fgetc(fp) != '#');
    fscanf(fp, "%s\n", name);
    if ((ok = fscanf(fp, "#type %s\n",type)) == 0)
	puts("Type error");
    else if ((ok = fscanf(fp, "#input %i\n", inputs)) == 0)
	puts("Inputs variable error");
    else if ((ok = fscanf(fp, "#output %i\n", outputs)) == 0)
	puts("Outputs error");
    if (ok)
    /*printf("Name: %s\nType : %s\nInput variables: %i\nOutput variables: %i\n",
        name, type, *inputs, *outputs);*/
       ;
    else
    {
	fclose(fp);
	exit(-1);
    }
}

Buckets read_pla(FILE *fp, Buckets b, Cover array, int inputs, int outputs)
{
    int i, j;
    char *fin, *fout;
    char line[255];
    Cover t;
    char *name[] = { "ON SET", "OFF SET", "DC SET" };

    fin = (char *) calloc(inputs+1, sizeof(char));
    fout = (char *) calloc(outputs+1, sizeof(char));
    for (j = ON; j <= DC; j++)
	b[j] = (Bucket) calloc(outputs, sizeof *b);
    for (i = 0; i < outputs; i++)
	for (j = ON; j <= DC; j++)
	    b[j][i] = cube_init(b[j][i], name[j]);
    fgets(line, 254, fp);
    t = array;
    while (!feof(fp) && strcmp(line, "#end\n") != 0)
    {
	strcpy(fin, strtok(line, " "));
	strcpy(fout, strtok(NULL, "\n"));
	insertafter(fin, t);
	t = t->next;
	for (i = 0; i < outputs; i++)
	switch (fout[i])
	{
	    case '1':
		    /*insertafter(fin, b[ON][i]);*/
		cube_union(fin, b[ON][i]);
		break;
	    case '0':
		    /*insertafter(fin, b[OFF][i]);*/
		cube_union(fin, b[OFF][i]);
		break;
	    case '-':
	    case '2':
		    /*insertafter(fin, b[DC][i]);*/
		cube_union(fin, b[DC][i]);
		break;
	    default: break;
	}
	fgets(line, 254, fp);
    }
    free(fin);
    free(fout);

    return b;
}
/*------------------------------------------------------------------------*/
/*
   -1 = does not contain
    0 = strictly contain
    1 = contain
*/
int contain(char a, char b)
{
    char c[3];
    int r;

    c[0] = a;
    c[1] = b;
    c[2] = '\0';
    switch (a)
    {
	case '0':
	case '1':
	case '2':
	case '-':
	     if (strcmp(c, "00") == 0)
		 r = 1;
	     else if (strcmp(c, "01") == 0)
		      r = -1;
	     else if (strcmp(c, "02") == 0)
		      r = -1;
	     else if (strcmp(c, "10") == 0)
		      r = -1;
	     else if (strcmp(c, "11") == 0)
		      r = 1;
	     else if (strcmp(c, "12") == 0)
		      r = -1;
	     else if (strcmp(c, "20") == 0)
		      r = 0;
	     else if (strcmp(c, "21") == 0)
		      r = 0;
	     else if (strcmp(c, "22") == 0)
		      r = 1;
	     break;
	case '3':
	case '4':
	     if (strcmp(c, "33") == 0)
		 r = 1;
	     else if (strcmp(c, "34") == 0)
		      r = -1;
	     else if (strcmp(c, "43") == 0)
		      r = 0;
	     else if (strcmp(c, "44") == 0)
		      r = 1;
	     break;
	default:
	     r = -1;
	     break;
    }

    return r;
}
int cube_contain(char *a, char *b)
{
    int i, j, k, len;

    len = strlen(a);
    for (i = 0, j = 1; i < len; i++)
    {
	k = contain(a[i], b[i]);
	if (k == -1)
	{
	    j = -1;
	    break;
	}
	j = j * k;
    }

    return j;
}
/*------------------------------------------------------------------------*/
/* n cover union                                                          */
/*------------------------------------------------------------------------*/
Cover bucket2cover(Bucket b, int outputs)
{
    Cover r, r1;
    int i;

    r = cube_init(r, b[0]->vertex);
    for (i = 0; i < outputs; i++)
    {
	r1 = b[i]->next;
	while (r1 != b[i])
	{
	    /*insertafter(r1->vertex, r);*/
	    cube_union(r1->vertex, r);
	    r1 = r1->next;
	}
    }

    return r;
}
/*------------------------------------------------------------------------*/
Cover cube_union(char v[], Cover c)
{
    Cover t;
    int only;

    only = 1;
    t = c->next;
    while (t != c)
    {
	if ((only = strcmp(t->vertex, v)) == 0)
	    break;
	t = t->next;
    }
    if (only)
	insertafter(v, c);

    return c;
}

Cover cover_union(Cover r1, Cover r2)
{
    Cover r, r3;

    r3 = cube_init(r3, "UNION");
    r = r1;
    while (r->next != r1)
    {
	r = r->next;
	cube_union(r->vertex, r3);
    }
    r = r2;
    while (r->next != r2)
    {
	r = r->next;
	cube_union(r->vertex, r3);
    }

    return r3;
}

Cover buckets_union(Buckets b, int outputs)
{
    Cover c1, c2, c3;

    c1 = bucket2cover(b[ON], outputs);
    c2 = bucket2cover(b[OFF], outputs);
    c3 = cover_union(c1, c2);
    free_cover(c1);
    free_cover(c2);
    c2 = bucket2cover(b[DC], outputs);
    c1 = cover_union(c2, c3);
    free_cover(c2);

    return c1;
}
/*--------------------------------------------------------------------------*/
/*
     E : Empty Set
     e : null

    sharp table

    # | 0  1  x
    --+---------+
    0 | e  E  e
    1 | E  e  e
    x | 1  0  e
/*
/*--------------------------------------------------------------------------*/
char sharp(char a, char b)
{
    char c[3];

    c[0] = a;
    c[1] = b;
    c[2] = '\0';
    if (strcmp(c, "00") == 0)
	c[2] = 'e';
    else if (strcmp(c, "01") == 0)
	     c[2] = 'E';
    else if (strcmp(c, "0-") == 0 || strcmp(c, "02") == 0)
	     c[2] = 'e';
    else if (strcmp(c, "10") == 0)
	     c[2] = 'E';
    else if (strcmp(c, "11") == 0)
	     c[2] = 'e';
    else if (strcmp(c, "1-") == 0 || strcmp(c, "12") == 0)
	     c[2] = 'e';
    else if (strcmp(c, "-0") == 0 || strcmp(c, "20") == 0)
	     c[2] = '1';
    else if (strcmp(c, "-1") == 0 || strcmp(c, "21") == 0)
	     c[2] = '0';
    else if (strcmp(c, "--") == 0 || strcmp(c, "22") == 0)
	     c[2] = 'e';

    return c[2];
}
/*--------------------------------------------------------------------------*/
/*
          a    ai # bi = E for some i
 a # b =  E    ai # bi = e for all i
          U(a1, a2,...,~bi,...an) otherwise
*/
/*--------------------------------------------------------------------------*/
Cover cube_sharp(char *a, char *b)
{
    Cover c;
    char *t, *e;
    int i, len;

    len = strlen(a);
    t = strdup(a);
    e = (char *) calloc(len+1, sizeof (char));
    memset(e, 'e', len);
    e[len] = '\0';
    for (i = 0; i < len; i++)
    {
	t[i] = sharp(a[i], b[i]);
	if (t[i] == 'E')
	{
	    strcpy(t,a);
	    break;
	}
    }
    c = cube_init(c, "SHARP");
    if (strcmp(t, a) == 0)
	insertafter(t, c);
    else if (strcmp(t, e) == 0)
	     ;
    else
    {
	for (i = 0; i < len; i++)
	{
	    if (t[i] == '0' || t[i] == '1')
	    {
		strcpy(e, a);
		e[i] = t[i];
		cube_union(e, c);
	    }
	}
    }
    free(t);
    free(e);

    return c;
}
/*--------------------------------------------------------------------------*/
/*
 an array # a cube    : C # b = { { c1 # b } { c2 # b } ... }
 a cube # an array    : b # C = { ... { { b # c1 # } c2 # } ... }
 an arrary # an array : B # C = { ... { { B # c1 # } c2 } ... }
*/
/*--------------------------------------------------------------------------*/
Cover cover_sharp(Cover a, Cover b)
{
    Cover r1, r2, r3, t, t1;

    r1 = a;
    r2 = b;
    if (r2->next->next == b)
    {
	r3 = cube_init(r3, "SHARP");
	r2 = r2->next;
	while (r1->next != a)
	{
	    r1 = r1->next;
	    t = cube_sharp(r1->vertex, r2->vertex);
	    t1 = cover_union(r3, t);
	    free_cover(r3);
	    r3 = t1;
	    free_cover(t);
	}
    }
    else if (r1->next->next == a)
    {
	r1 = r1->next;
	r2 = r2->next;
	r3 = cube_sharp(r1->vertex, r2->vertex);
	while (r2->next != b)
	{
	    r2 = r2->next;
	    t1 = cube_init(t1, "SHARP");
	    insertafter(r2->vertex, t1);
	    t = cover_sharp(r3, t1);
	    free_cover(r3);
	    free_cover(t1);
	    r3 = t;
	}
    }
    else
    {
	r2 = r2->next;
	t1 = cube_init(t1, "SHARP");
	insertafter(r2->vertex, t1);
	r3 = cover_sharp(a, t1);
	while (r2->next != b)
	{
	    r2 = r2->next;
	    t1 = cube_init(t1, "SHARP");
	    insertafter(r2->vertex, t1);
	    t = cover_sharp(r3, t1);
	    free_cover(r3);
	    free_cover(t1);
	    r3 = t;
	}
    }

    return r3;
}
/*------------------------------------------------------------------------*/
int cube_find(char *v, Cover c)
{
    Cover t;
    int flag = 0;

    t = c;
    while (t->next != c)
    {
	t = t->next;
	if (cube_contain(t->vertex, v) != -1)
	{
	    flag = 1;
	    break;
	}
    }

    return flag;
}
void set_matrix(char **m, int r, int c, char v)
{
    char x[3];

    x[0] = m[r][c];
    x[1] = v;
    x[2] = '\0';
    if (v == '2')
	m[r][c] = '2';
    if (x[0] != '0' && x[0] != '1' && x[0] != '2') /* . G S D A */
	m[r][c] = v;
    if (strcmp(x, "00") == 0)
	m[r][c] = '0';
    else if (strcmp(x, "01") == 0)
	m[r][c] = '2';
    else if (strcmp(x, "10") == 0)
	m[r][c] = '2';
    else if (strcmp(x, "11") == 0)
	m[r][c] = '1';
    else if (strcmp(x, "20") == 0)
	m[r][c] = '2';
    else if (strcmp(x, "21") == 0)
	m[r][c] = '2';

}
int row_in_matrix(Cover c, char *v)
{
    int j;
    Cover t2;

    t2 = c;
    for (j = 0; t2->next != c;j++)
    {
	t2 = t2->next;
	if (strcmp(t2->vertex, v) == 0)
	    break;
    }

    return j;
}
void check_test(char *test, Cover c, Buckets b, int inputs, int outputs,
		   char **matrixC, char **matrixD)
{
    Cover t, t1, t2, c2;
    Buckets b2;
    int n, i, j, k;
    int conflict;

    t = c;
    c2 = cube_init(c2, "contain test");
    t2 = c2;
    n = 0;
    while (t->next != c)
    {
	t = t->next;
	if (cube_contain(t->vertex, test) != -1)
	{
	    insertafter(t->vertex, t2);
	    t2 = t2->next;
	    n++;
	}
    }
    for (j = ON; j <= DC; j++)
	b2[j] = (Bucket) calloc(outputs, sizeof *b2);
    for (i = 0; i < outputs; i++)
	for (j = ON; j <= DC; j++)
	    b2[j][i] = cube_init(b2[j][i], "contain test");
    t2 = c2;
    while (t2->next != c2)
    {
	t2 = t2->next;
	for (i = 0; i < outputs; i++)
	    for (j = ON; j <= OFF; j++)
	    {
		t1 = b[j][i]; /* error */
		t = t1;
		while(t->next != t1)
		{
		    t = t->next;
		    if (strcmp(t2->vertex, t->vertex) == 0)
		    {
			insertafter(t2->vertex,b2[j][i]);
			break;
		    }
		}
	    }
    }
    /*  a column in D with only one 1 */
    for (i = 0; i < outputs; i++)
    {
	t = b2[ON][i];
	if (t->next != t && t->next->next == t) /* only one element */
	{
	    /* case 1: disappear fault */
	    j = row_in_matrix(c, t->next->vertex);
	    set_matrix(matrixD, j, i, '0');
	    printf("d(%i,%i)->0 ", j+1, i+1);
	    /* case 2: shrinking fault */
	    for (k = 0; k < inputs; k++)
	    {
		if (t->next->vertex[k] == '2')
		    switch (test[k])
		    {
			case '0':
			    set_matrix(matrixC, j, k, '1');
			    printf("c(%i,%i)->1 ", j+1, k+1);
			    break;
			case '1':
			    set_matrix(matrixC, j, k, '0');
			    printf("c(%i,%i)->0 ", j+1, k+1);
			    break;
			default :
			    break;
		    }
	    }
	}
    }
    /*-------------------------- all 0's in column i */
    for (i = 0; i < outputs; i++)
    {
	 t = b2[OFF][i];
	 t2 = c2;
	 j = 0;
	 while(t2->next != c2)
	 {
	     t2 = t2->next;
	     if (cube_find(t2->vertex, t) == 0) /* not found in off set */
	     {
		 j = -1;
		 break;
	     }
	     else
		 j++;
	 }
	 if (j == n) /* found all 0's in column i */
	 {
	    /* case 1: appearance fault */
	    t2 = c2;
	    while(t2->next != c2)
	    {
	       t2 = t2->next;
	       j = row_in_matrix(c, t2->vertex);
	       set_matrix(matrixD, j, i, '1');
	       printf("d(%i,%i)->1 ", j+1, i+1);
	    }

	    /* case 2: growth fault */
	    t2 = c2;
	    while(t2->next != c2)
	    {
		t2 = t2->next;
		conflict = 0;
		for (k = 0; k < inputs; k++)
		{
		    if (t2->vertex[k] != '2')
		    {
			if ((t2->vertex[k] == '0' && test[k] == '1') ||
			    (t2->vertex[k] == '1' && test[k] == '0'))
			{
			    conflict++;
			    if (conflict > 1)
				break;
			}
		    }
		}
		if (conflict == 1)
		{
		    j = row_in_matrix(c, t2->vertex);
		    set_matrix(matrixC, j, i, '2');
		    printf("c(%i,%i)->x ", j+1, i+1);
		}
	    }

	 }
    }
    free_cover(c2);
    free_buckets(b2, outputs);

}
/*--------------------------------------------------------------------------*/
void a_cube(char *v, int len)
{
    int i;

    for (i = 0; i < len; i++)
	v[i] = (v[i] == '2') ? '0' : v[i];
}
Cover G_fault(Cover c, Buckets b, int inputs, int outputs,
		   char **matrixC, char **matrixD)
{
    Cover cc, t, t1, r, rL, rLt, Test;
    int i, j, k, undetect;
    char *v;
    Bucket on_set;

    on_set = b[ON];
    Test = cube_init(Test, "GROWTH");
    cc = c;
    for (j = 0; cc->next != c; j++)
    {
	cc = cc->next;
	/* pick up a cube in c */
	v = strdup(cc->vertex);
	for (i = 0; i < inputs ; i++)
	{
	    if (matrixC[j][i] != '2' && (v[i] == '0' || v[i] == '1'))
	    for (k = 0; k < outputs; k++)
	    {
		if (cube_find(v, on_set[k]))
		{
		    rL = cube_init(rL, "growth");
		    /* an cover consisting all those c such that dik = 1 */
		    rLt = on_set[k];
		    while (rLt->next != on_set[k])
		    {
			rLt = rLt->next;
			if (strcmp(v, rLt->vertex) != 0)
			    insertafter(rLt->vertex, rL);
		    }
		    /* complement a bit */
		    v[i] = (v[i] == '0') ? '1' : '0';
		    t1 = cube_init(t1, "single");
		    insertafter(v, t1);
		    r = cover_sharp(t1, rL);
		    free_cover(t1);
		    /* recover this bit */
		    v[i] = (v[i] == '0') ? '1' : '0';
		    if (r->next != r) /* find a test set ? */
		    {
			matrixC[j][i] = '2';
			undetect = 0;
			//insertafter(r->next->vertex, Test);
			a_cube(r->next->vertex, inputs);
			cube_union(r->next->vertex, Test);
			printf("G = %s : c(%i,%i)->x ", r->next->vertex, j+1, i+1);
			check_test(r->next->vertex, c, b, inputs, outputs, matrixC, matrixD);
			printf("\n");
		    }
		    else
		    {
			undetect = 1;
			matrixC[j][i] = 'G';
		    }
		    free_cover(r);
		    free_cover(rL);
		    if (undetect == 0)
			break;
		}

	    }
	    if (matrixC[j][i] == 'G')
		printf("G : c(%i,%i)->x is undetected.\n", j+1, i+1);
	}
	free(v);
    }

    return Test;
}
/*------------------------------------------------------------------------*/
Cover S_fault(Cover c, Buckets b, int inputs, int outputs,
		   char **matrixC, char **matrixD)
{
    Cover cc, t, t1, r, rL, rLt, Test;
    int i, j, k, undetect;
    char *v, vc;
    Bucket on_set;

    on_set = b[ON];
    Test = cube_init(Test, "SHRINKING");
    cc = c;
    for (j = 0; cc->next != c; j++)
    {
	cc = cc->next;
	/* pick up a cube in c */
	v = strdup(cc->vertex);
	for (i = 0; i < inputs ; i++)
	{
	    if (matrixC[j][i] != '2')
	    {
		if (matrixC[j][i] != '0')
		{
		for (k = 0; k < outputs; k++)
		{
		    if (cube_find(v, on_set[k]))
		    {
			rL = cube_init(rL, "shrinking");
			/* an cover consisting all those c such that dik = 1 */
			rLt = on_set[k];
			while (rLt->next != on_set[k])
			{
			    rLt = rLt->next;
			    if (strcmp(v, rLt->vertex) != 0)
				insertafter(rLt->vertex, rL);
			}
			/* set a bit */
			vc = v[i];
			v[i] = '0';
			t1 = cube_sharp(cc->vertex, v);
			r = cover_sharp(t1, rL);
			free_cover(t1);
			/* recover this bit */
			v[i] = vc;
			if (r->next != r) /* find a test set ? */
			{
			    set_matrix(matrixC,j,i,'0');
			    undetect = 0;
			    //insertafter(r->next->vertex, Test);
			    a_cube(r->next->vertex, inputs);
			    cube_union(r->next->vertex, Test);
			    printf("S = %s : c(%i,%i)->0 ", r->next->vertex, j+1, i+1);
			    check_test(r->next->vertex, c, b, inputs, outputs, matrixC, matrixD);
			    printf("\n");
			}
			else
			{
			    undetect = 1;
			    set_matrix(matrixC,j,i,'S');

			}
			free_cover(r);
			free_cover(rL);
			if (undetect == 0)
			    break;
		    }
		}
		if (matrixC[j][i] == 'S')
		    printf("S : c(%i,%i)->0 is undetected.\n", j+1, i+1);
		}
		/*---------------------------------------------------*/
		if (matrixC[j][i] != '1' && matrixC[j][i] != '2')
		{
		for (k = 0; k < outputs; k++)
		{
		    if (cube_find(v, on_set[k]))
		    {
			rL = cube_init(rL, "shrinking");
			/* an cover consisting all those c such that dik = 1 */
			rLt = on_set[k];
			while (rLt->next != on_set[k])
			{
			    rLt = rLt->next;
			    if (strcmp(v, rLt->vertex) != 0)
				insertafter(rLt->vertex, rL);
			}
			/* set a bit */
			vc = v[i];
			v[i] = '1';
			t1 = cube_sharp(cc->vertex, v);
			r = cover_sharp(t1, rL);
			free_cover(t1);
			/* recover this bit */
			v[i] = vc;
			if (r->next != r) /* find a test set ? */
			{
			    set_matrix(matrixC,j,i,'1');
			    undetect = 0;
			    //insertafter(r->next->vertex, Test);
			    a_cube(r->next->vertex, inputs);
			    cube_union(r->next->vertex, Test);
			    printf("S = %s : c(%i,%i)->1 ", r->next->vertex, j+1, i+1);
			    check_test(r->next->vertex, c, b, inputs, outputs, matrixC, matrixD);
			    printf("\n");
			}
			else
			{
			    undetect = 1;
			    set_matrix(matrixC,j,i,'S');
			}
			free_cover(r);
			free_cover(rL);
			if (undetect == 0)
			    break;
		    }
		}
		if (matrixC[j][i] == 'S')
		    printf("S : c(%i,%i)->1 is undetected.\n", j+1, i+1);
		}
	    }
	}
	free(v);
    }

    return Test;
}
/*------------------------------------------------------------------------*/
Cover D_fault(Cover c, Buckets b, int inputs, int outputs,
		   char **matrixC, char **matrixD)
{
    Cover cc, t, t1, r, rL, rLt, Test;
    int i, j, undetect;
    char *v;
    Bucket on_set;

    on_set = b[ON];
    Test = cube_init(Test, "DISAPPEARANCE");
    cc = c;
    for (j = 0; cc->next != c; j++)
    {
	cc = cc->next;
	/* pick up a cube in c */
	v = strdup(cc->vertex);
	for (i = 0; i < outputs ; i++)
	{
	    if (matrixD[j][i] == '.')
	    {
		if (cube_find(v, on_set[i]))
		{
		    rL = cube_init(rL, "disappear");
		    /* an cover consisting all those c such that dik = 1 */
		    rLt = on_set[i];
		    while (rLt->next != on_set[i])
		    {
			rLt = rLt->next;
			if (strcmp(v, rLt->vertex) != 0)
			    insertafter(rLt->vertex, rL);
		    }
		    t1 = cube_init(t1, "single");
		    insertafter(v, t1);
		    r = cover_sharp(t1, rL);
		    free_cover(t1);
		    if (r->next != r) /* find a test set ? */
		    {
			set_matrix(matrixD,j,i,'0');
			undetect = 0;
			//insertafter(r->next->vertex, Test);
			a_cube(r->next->vertex, inputs);
			cube_union(r->next->vertex, Test);
			printf("D = %s : d(%i,%i)->0 ", r->next->vertex, j+1, i+1);
			check_test(r->next->vertex, c, b, inputs, outputs, matrixC, matrixD);
			printf("\n");
		    }
		    else
		    {
			undetect = 1;
			set_matrix(matrixD,j,i,'D');
		    }
		    free_cover(r);
		    free_cover(rL);
		    if (undetect == 0)
			break;
		}
	    }
	    if (matrixD[j][i] == 'D')
		printf("D : d(%i,%i)->0 is undetected.\n", j+1, i+1);
	}
	free(v);
    }

    return Test;
}
/*------------------------------------------------------------------------*/
Cover A_fault(Cover c, Buckets b, int inputs, int outputs,
		   char **matrixC, char **matrixD)
{
    Cover cc, t, t1, r, rL, rLt, Test;
    int i, j, undetect;
    char *v;
    Bucket on_set, off_set;

    off_set = b[OFF];
    on_set = b[ON];
    Test = cube_init(Test, "APPEAR");
    cc = c;
    for (j = 0; cc->next != c; j++)
    {
	cc = cc->next;
	/* pick up a cube in c */
	v = strdup(cc->vertex);
	for (i = 0; i < outputs ; i++)
	{
	    if (matrixD[j][i] == '.')
	    {
		if (cube_find(v, off_set[i]))
		{
		    rL = cube_init(rL, "appear");
		    /* an cover consisting all those c such that dik = 1 */
		    rLt = on_set[i];
		    while (rLt->next != on_set[i])
		    {
			rLt = rLt->next;
			if (strcmp(v, rLt->vertex) != 0)
			    insertafter(rLt->vertex, rL);
		    }
		    t1 = cube_init(t1, "single");
		    insertafter(v, t1);
		    r = cover_sharp(t1, rL);
		    free_cover(t1);
		    if (r->next != r) /* find a test set ? */
		    {
			set_matrix(matrixD,j,i,'1');
			undetect = 0;
			//insertafter(r->next->vertex, Test);
			a_cube(r->next->vertex, inputs);
			cube_union(r->next->vertex, Test);
			printf("A = %s : d(%i,%i)->1 ", r->next->vertex, j+1, i+1);
			check_test(r->next->vertex, c, b, inputs, outputs, matrixC, matrixD);
			printf("\n");
		    }
		    else
		    {
			undetect = 1;
			set_matrix(matrixD,j,i,'A');
		    }
		    free_cover(r);
		    free_cover(rL);
		    if (undetect == 0)
			break;
		}
	    }
	    if (matrixD[j][i] == 'A')
		printf("A : d(%i,%i)->1 is undetected.\n", j+1, i+1);
	}
	free(v);
    }

    return Test;
}
/*------------------------------------------------------------------------*/
void print_matrix(char **m, int row, int col)
{
    int i, j;

    for (i = 0; i < row; i++)
    {
	for (j = 0; j < col; j++)
	    printf("%c ",m[i][j]);
	printf("\n");
    }
}
void undetect(char **C, char **D, int row, int inputs, int outputs)
{
    int i, j;

    printf("============================== REPORT ==============================\n");
    for (i = 0; i < row; i++)
	for (j = 0; j < inputs; j ++)
	     switch (C[i][j])
	     {
		 case 'G' :
		     printf("c(%i,%i)->x is undetectable.\n", i+1, j+1);
		     break;
		 case '0' :
		     printf("c(%i,%i)->1 is undetectable.\n", i+1, j+1);
		     break;
		 case '1' :
		     printf("c(%i,%i)->0 is undetectable.\n", i+1, j+1);
		     break;
		 default:
		     break;
	     }
    for (i = 0; i < row; i++)
	for (j = 0; j < outputs; j ++)
	     switch (D[i][j])
	     {
		 case 'A' :
		     printf("d(%i,%i)->1 is undetectable.\n", i+1, j+1);
		     break;
		 case 'D' :
		     printf("d(%i,%i)->0 is undetectable.\n", i+1, j+1);
		     break;
		 default:
		     break;
	     }
    printf("--------------------------------------------------------------------\n");
}
Cover generate(Cover array, Buckets b, int inputs, int outputs)
{
    Cover c, t, c1, c2, c3, c4, Test;
    int i, row;
    char **matrixC, **matrixD;

//    c = bucket2cover(b[ON], outputs);
      c = array;

//    outputs--;
//    c = b[DC][outputs];

//    print_cover(c);
    row = 0;
    t = c;
    while (t->next != c)
    {
	t = t->next;
	row++;
    }
    matrixC = (char **) calloc(row, sizeof *matrixC);
    for (i = 0; i < row; i++)
    {
	matrixC[i] = (char *) calloc(inputs, sizeof(**matrixC));
	memset((void *) matrixC[i], '.', sizeof(char) * inputs);
    }
    matrixD = (char **) calloc(row, sizeof *matrixD);
    for (i = 0; i < row; i++)
    {
	matrixD[i] = (char *) calloc(outputs, sizeof(**matrixD));
	memset((void *) matrixD[i], '.', sizeof(char) * outputs);
    }
    c1 = G_fault(c, b, inputs, outputs, matrixC, matrixD);
//    print_matrix(matrixC, row, inputs);
//    print_matrix(matrixD, row, outputs);

    c2 = S_fault(c, b, inputs, outputs, matrixC, matrixD);
//    print_matrix(matrixC, row, inputs);
//    print_matrix(matrixD, row, outputs);

    c3 = cover_union(c1, c2);
    free_cover(c1);
    free_cover(c2);

    c4 = D_fault(c, b, inputs, outputs, matrixC, matrixD);
//    print_matrix(matrixC, row, inputs);
//    print_matrix(matrixD, row, outputs);

    c1 = A_fault(c, b, inputs, outputs, matrixC, matrixD);
//    print_matrix(matrixC, row, inputs);
//    print_matrix(matrixD, row, outputs);

    c2 = cover_union(c3, c4);
    free_cover(c3);
    free_cover(c4);

    Test = cover_union(c1, c2);
    free_cover(c1);
    free_cover(c2);

    undetect(matrixC, matrixD, row, inputs, outputs);
//    free_cover(c);

    return Test;
}
void test_pattern(Cover c, Bucket b, char *E, int outputs)
{
    int i, sum;
    char *space;
    Cover t;

    space = (char *) calloc(outputs+1, sizeof(char));
    memset(space, '0', outputs);
    space[outputs] = '\0';
    sum = 0;
    t = c;
    printf("        Test               Correct Output   \n");
    printf("--------------------    --------------------\n");
    while (t->next != c)
    {
	t = t->next;
	for (i = 0; i < outputs; i++)
	{
	    if (cube_find(t->vertex, b[i]))
		space[i] = '1';
	    if (E[i] == '1')
		space[i] = (space[i] == '0') ? '1' : '0';
	}
        printf("%-20s\t%-10s\n", t->vertex, space);
	memset(space, '0', outputs);
	sum++;
    }
    printf("====================    ====================\n");
    printf("number of cubes = %i\n", sum);
    free(space);
}
/*--------------------------------------------------------------------------*/
#ifdef SHARP
void main(int argc, char *argv[])
{
    Cover pp, p1, p2;

    pp = cube_sharp("0201","1022");
    print_cover(pp);
    free_cover(pp);
    puts("-------------");
    pp = cube_sharp("0201","0021");
    print_cover(pp);
    free_cover(pp);
    puts("-------------");
    pp = cube_sharp("212","111");
    print_cover(pp);
    free_cover(pp);
    puts("-------------");
    p1 = cube_init(p1, "COVER1");
    insertafter("0201", p1);
    p2 = cube_init(p2, "COVER2");
    insertafter("1022", p2);
    insertafter("0021",p2);
    pp = cover_sharp(p1, p2);
    print_cover(pp);
    free_cover(pp);
    free_cover(p1);
    free_cover(p2);
    puts("-------------");
}
#endif
/*------------------------------------------------------------------------*/
void main(int argc, char *argv[])
{
    Buckets b;
    Cover t, array;
    FILE *fp;
    char name[80], type[80];
    int inputs, outputs;
    int n;

    if (argc > 1)
    {
	if ((fp = fopen(argv[1], "r")) == NULL)
	    printf("File not found\n");
	else
	{
	    b = (Buckets) calloc(3, sizeof **b);
	    array = cube_init(array, "PLA");
	    get_parm(fp, name, type, &inputs, &outputs);
	    b = read_pla(fp, b, array, inputs, outputs);
	    t = generate(array, b, inputs, outputs);
	    test_pattern(t, b[ON], type, outputs);
	    free_cover(t);
	    free_buckets(b, outputs);
	    free_cover(array);
	}
	fclose(fp);
    }
    else
	printf("Usage: PLA <filename>\n");
}
