/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Application:     Test Pattern Generation for PLA's                     *
 *                                                                         *
 *  Language:        C Language                                            *
 *                                                                         *
 *  Platform:        (1) i386, DOS 5.0                                     *
 *                   (2) Sun Sparc, UNIX V                                 *
 *                                                                         *
 *  Author:          Jordon Lin                                            *
 *  Date:            March 2, 1993                                         *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#ifdef UNIX
#include <memory.h>
#include <alloca.h>
#else
#include <mem.h>
#include <alloc.h>
#endif

enum nameofset { ON = 0, OFF, DC };
enum boolean { FALSE = 0, TRUE };
typedef struct node Cube;
typedef Cube * Cover;
typedef Cube ** Bucket;
typedef Bucket * Buckets;
struct node
{
    char *vertex;
    Cover next;
};

/*-------------------------------------------------------------------------*/
Cover cube_init(Cover head, char name[]);
void deletenext(Cover c);
Cover insertafter(char w[], Cover t);
/*------------------------------------------------------------------------*/
int print_cover(Cover c);
int print_bucket(Bucket b, int outputs);
void print_buckets(Buckets b, int outputs);
/*------------------------------------------------------------------------*/
void free_cover(Cover t);
void free_bucket(Bucket b, int outputs);
void free_buckets(Buckets b, int outputs);
/*------------------------------------------------------------------------*/
void get_parm(FILE *fp, char *name, char *type, int *inputs, int *outputs);
Buckets get_buckets(FILE *fp, Buckets b, int inputs, int outputs);
/*------------------------------------------------------------------------*/
int distance(char a, char b);
int cube_distance(char *a, char *b);
/*------------------------------------------------------------------------*/
char *cube_cofactor(char *c, char *p);
Cover cover_cofactor(Cover c, char *p);
/*------------------------------------------------------------------------*/
char *consensus(char *a, char *b);
int contain(char a, char b);
int cube_contain(char *a, char *b);
/*------------------------------------------------------------------------*/
char cube_and(char a, char b);
char *cube_intersect(char *a, char *b);
Bucket bucket_intersect(Bucket p, Bucket q, int inputs, int outputs);
Buckets buckets_intersect(Buckets bs1, Buckets bs2, int inputs, int outputs);
Cover intersect(Bucket p, Bucket q, int inputs, int outputs);
/*------------------------------------------------------------------------*/
Cover bucket2cover(Bucket b, int outputs);
Cover cube_union(char v[], Cover c);
Cover cover_union(Cover r1, Cover r2);
Cover buckets_union(Buckets b, int outputs);
/*------------------------------------------------------------------------*/
int binate_select(Cover c);
Cover *split_cover(Cover c, int i);
int special_cases(Cover c);
int tautology(Cover c, int count);
/*------------------------------------------------------------------------*/
Cover merge(int flag, Cover *c, int i);
Cover unate_simplify(Cover c);
Cover simplify(Cover c, int count);
/*------------------------------------------------------------------------*/
int all2row(Cover c, int size);
int complement_special(Cover c, int size);
Cover *comp_split_cover(Cover c, int i);
int ucomp_select(Cover c);
Cover DeMorgan(Cover c);
Cover p_unate_complement(Cover c, char Vbar[], int size, int count);
char *monotone(Cover c);
Cover unate_complement(Cover c, int size);
Cover comp1(Cover c, int inputs, int count);
Bucket complement(Bucket f, Bucket d, int outputs);
