#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char **argv){

    double towait_f;
    useconds_t towait_i;
    double t0, t1;
    struct timeval now;

    towait_f=atof(argv[1]);

    towait_i = floor((towait_f*1000000));

    gettimeofday(&now, NULL);
    t0 = now.tv_sec+(now.tv_usec/1000000.0);

    usleep(towait_i);

    gettimeofday(&now, NULL);
    t1 = now.tv_sec+(now.tv_usec/1000000.0);

    printf("%f\n", t1-t0);


}
