#include <stdio.h>
#include <math.h>

int is_digit(char ch)
{ 
  return (ch >= '0' && ch <= '9');
}

float ft_atof(char *str)
{
    float n = 0.0f;
    float sign = 1.0f;
    float div = 10.0f;
    
    if (*str == '-')
      sign *= -1.0f;
    if (*str == '-' || *str == '+')
      str++;
    while (*str && *str != '.')
    { 
        n = n * 10 + *str - '0';
        str++;
    }
    if (*str == '.')
      str++;
    while (*str)
    {
      n += (*str - '0') / div; 
      div *= 10.0f; 
      str++;
    }
    return (n * sign);
}
        
int main()
{
 	float x = ft_atof("123.12");
	printf("%f\n", x);
 	return 0;
}
