#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    uint16_t sot;
    uint16_t id;
    uint16_t nb;
    double  volt;
    double  curr;
    double  amp;
    uint16_t eot;
} ht_msg_t;

// Show C types length on your arch
void showtypessizes(){
    printf("char %d\n", sizeof(char));
    printf("short %d\n", sizeof(short));
    printf("int %d\n", sizeof(int));
    printf("long %d\n", sizeof(long));
    printf("long long %d\n", sizeof(long long));
    printf("float %d\n", sizeof(float));
    printf("double %d\n", sizeof(double));
}

// print all bytes of a char array
// begin with lower adress
// which is less significant byte first on a small endian
// achitecture
void pc(unsigned char *s, size_t length){
    int i;
    printf( "%d bytes: ", length);
    for( i=0; i<length; i++){
        printf("%02x", s[i]);
    }
    printf("\n");
}

// int to char conversion
// union way
void ui2c_union(unsigned int i) {
    printf("ui2c_union: ");

    union {
    unsigned int num;
    unsigned char ch[sizeof(int)];
    }  theValue;

    theValue.num = i;
    pc( theValue.ch, sizeof(int));
}

// int to char conversion
// casting way
void ui2c_casting(unsigned int k, unsigned char **c) {
    printf("ui2c_casting: ");
    *c = (unsigned char *) &k;
    pc( *c, sizeof(unsigned int));
}

// int to char conversion
// memcpy way
void ui2c_memcpy(unsigned int k, unsigned char **c) {
    printf("ui2c_memcpy: ");
    *c = (unsigned char *) malloc(sizeof( unsigned int));
    memcpy(  *c, &k, sizeof(k));
    pc( *c, sizeof(unsigned int));
}

// double to char conversion, (memcpy)
void d2c(double d, unsigned char **s ){
    *s=(unsigned char *) malloc(sizeof(double));
    memcpy( *s, &d, sizeof(double));
}

// append anything to the given char pointer, offset
// char pointer s has to be sufficiently allocated before
size_t any2char( unsigned char **s, void *any, size_t size, size_t *index){
    memcpy( *s+*index, any, size);
    *index+=size;
    return *index;
}

// serializing a structure, field by field,
// append in one unique bytearray
// return total bytes serialized
size_t commute(ht_msg_t *theMsg, unsigned char **s){
    *s=(unsigned char *) malloc(sizeof(double));
    size_t length = 3*16+3*64+16;
    size_t index = 0;
    //push sot;
    memcpy( *s+index, &theMsg->sot, sizeof(theMsg->sot));
    index+=sizeof(theMsg->sot);
    //push id;
    memcpy( *s+index, &theMsg->id, sizeof(theMsg->id));
    index+=sizeof(theMsg->id);
    //push nb;
    memcpy( *s+index, &theMsg->nb, sizeof(theMsg->nb));
    index+=sizeof(theMsg->nb);
    //pushvolt;
    memcpy( *s+index, &theMsg->volt, sizeof(theMsg->volt));
    index+=sizeof(theMsg->volt);
    //pushcurr;
    memcpy( *s+index, &theMsg->curr, sizeof(theMsg->curr));
    index+=sizeof(theMsg->curr);
    //pushamp;
    memcpy( *s+index, &theMsg->amp, sizeof(theMsg->amp));
    index+=sizeof(theMsg->amp);
    //push eot;
    memcpy( *s+index, &theMsg->eot, sizeof(theMsg->eot));
    index+=sizeof(theMsg->eot);
    return index;
}

// serializing a structure, field by field,
// append in one unique bytearray
// return total bytes serialized
size_t commute2(ht_msg_t *theMsg, unsigned char **s){
    *s=(unsigned char *) malloc(sizeof(double));
    size_t length = 3*16+3*64+16;
    size_t index = 0;
    //push sot;
    any2char(s, &theMsg->sot,  sizeof(theMsg->sot), &index);
    //push id;
    any2char(s, &theMsg->id,  sizeof(theMsg->id), &index);
    //push nb;
    any2char(s, &theMsg->nb,  sizeof(theMsg->nb), &index);
    //pushvolt;
    any2char(s, &theMsg->volt,  sizeof(theMsg->volt), &index);
    //pushcurr;
    any2char(s, &theMsg->curr,  sizeof(theMsg->curr), &index);
    //pushamp;
    any2char(s, &theMsg->amp,  sizeof(theMsg->amp), &index);
    //push eot;
    any2char(s, &theMsg->eot,  sizeof(theMsg->eot), &index);
    return index;
}

void d2cV2(double *d, unsigned char **s, size_t nb){
    *s=(unsigned char *) malloc(sizeof(double));
    size_t i=0;
    any2char(s, d, sizeof(double), &i);
    any2char(s, d+1, sizeof(double), &i);
    any2char(s, d+2, sizeof(double), &i);
}

// can we just serialize a double array ?
void d2cV(double *d, unsigned char **s, size_t nb){
    int   i;
    *s=(unsigned char *) malloc(nb*sizeof(d));

    for( i=0; i<nb; i++){
        memcpy(  *s+(i*sizeof(d) ) , &(d[i]), sizeof(d[i]));
    }
}

int main( int argc, char **argv){
    unsigned char *s, *t, *c;
    uint32_t k;
    double f = 3.1415;
    double r[3] = { 3.1415, 6.181098, 7.2434325};
    ht_msg_t mymsg = {
        0xAA55,
        0x0001,
        32,
        r[0],
        r[1],
        r[2],
        0xBB66};

    if( argc < 2){
        perror("give arg\n");
        exit(1);
    }


    // get 32 bits integer from stdin:
    k=atol(argv[1]);
    // or you wanna try big endian instead ?
    //k=htonl(atol(argv[1]));

    // 3 different ways for same result
    // (presumably)
    ui2c_union(k);
    ui2c_casting(k, &s);
    ui2c_memcpy(k, &s);


    // serialize a double
    d2c( r[0], &s);
    printf("double ");
    pc( s, sizeof(r[0]));
    // serialize an  array of double
    printf("double array 1 ");
    d2cV( r, &s, 3);
    pc( s, 3*sizeof(f));
    printf("double array 2 ");
    d2cV2( r, &s, 3);
    pc( s, 3*sizeof(f));

    // commute and print a structure
    printf("struct 1 ");
    if ( ! mymsg.nb == commute(&mymsg, &s)){
        return 1;
    }
    pc( s, mymsg.nb);
    printf("struct 2 ");
    if ( ! mymsg.nb == commute2(&mymsg, &s)){
        return 1;
    }
    pc( s, mymsg.nb);


    return 0;
}
