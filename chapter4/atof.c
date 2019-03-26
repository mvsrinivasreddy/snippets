#include <ctype.h>
#include <stdio.h>
/* atof: convert string s to double */
double atof(char s[])
{
    double val, power, ret;
    int i, sign=0;
    /* skip white space */
    for (i=0; isspace(s[i]); i++);

    sign = (s[i] == '-') ? -1 : 1;

    if (s[i] == '+' || s[i] == '-')
        i++;
    for (val = 0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');
    printf("VAL:%f\n",val);
    if (s[i] == '.')
        i++;
    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }
    printf("POW:%d\n",sign);
    
    ret =  sign*(val / power);
      
    printf("RES:%f\n",(ret));
    return sign * val / power;
}

int main(int argc, char* argv[]){
        printf("%s\n",argv[1]);
        atof(argv[1]);
        return 0;
}
