///////////////////////////////////////////////////////////////////////////////////
// Header Name: myCString.h
// Name:
// Class:
//
///////////////////////////////////////////////////////////////////////////////////

//int my_strlen(const char *s);
#define NULL 0
char* my_strcpy(char *dest, const char *src){
	int size = my_strlen(src);
	int i = 0;
	while(i < size){
		dest[i] = src[i];
		i++;
	}
	dest[size] = '\0';
	return dest;
}
char* my_strncpy(char *dest, const char *src, int n){
	int size = my_strlen(src);
	int i = 0;
	while(i < size && i < n){
		dest[i] = src[i];
		i++;
	}
	if(size > n)
		dest[n] = '\0';
	else
		dest[size] = '\0';

	return dest;
}

char * my_strncat(char *dest, const char *src, int n)
{
     int src_size = my_strlen(src);
     int dest_size = my_strlen(dest);
     int x = 0;
     while(x < src_size && x < n){
	dest[dest_size + x] = src[x];
	x++;
	}
	if(src_size > n)
		dest[n + dest_size] = '\0';
	else
		dest[dest_size + src_size - 1] = '\0';


}

char *my_strcat(char *dest, const char *src)
{
 int src_size = my_strlen(src);
 return(my_strncat(dest,src,src_size));
}

int my_strlen(const char *s){
	int len = 0;
	int i =0;
	while(s[i] != 0 ){
		len++;
		i++;
	}
	return len;
}

char* my_strchr(char* str, int character)
{
        if(str==0) return NULL;
        if(str[0]==character) return str;
        return my_strchr(str+1, character);
}
 
char* my_strstr(char* str1,char* str2)
{
        if(my_strlen(str1)<my_strlen(str2)) return NULL;
        if(*str1==*str2)
        {
                int i=1;
                while(i<my_strlen(str2))
                {
                        if(*(str1+i)!=*(str2+i)) return my_strstr(str1+i,str2);
                        i++;   
                }
                return str1;
        }
        return my_strstr(str1+1,str2);
}
 
int my_strcmp(char * str1,char * str2)
{
        if(my_strlen(str1)==1 && my_strlen(str2)==1 && *str1==*str2) return 0;
        if(*str1<*str2) return -1;
        else if(*str1>*str2) return 1;
        return my_strcmp(str1+1,str2+1);
}

//#endif
 
