#include "tool.h"

char *string_add(const char *format,...)
{
	va_list arg_ptr;
	int len;
	char *res=NULL;
	static char empty='\0';

	va_start(arg_ptr,format);
	len=vsnprintf(&empty,0,format,arg_ptr);
	va_end(arg_ptr);

	if(len < 0)
		return NULL;

	res=malloc(len+1);
	if(res == NULL)
		return NULL;

	va_start(arg_ptr,format);
	vsnprintf(res,len+1,format,arg_ptr);
	va_end(arg_ptr);

	return res;
}

char *stradd(char *from,const char *add)
{
	char *res;

	int len=strlen(from)+strlen(add);
	res=malloc(len+1);
	snprintf(res,len+1,"%s%s",from,add);
	free(from);

	return res;
}

char *url_encode(char *str)
{
	char *res;
	int len;
	int i,j;

	len=strlen(str)*3;
	res=malloc(len+1);
#ifdef _WIN32
	memset(res,0,len+1);
#else
	bzero(res,len+1);
#endif

	for(i=0,j=0;str[i];++i,j+=3)
		sprintf(res+j,"%%%02x",(unsigned char)str[i]);

	return res;
}

size_t curl_read_to_data(char *ptr,size_t size,size_t nmemb,DATA *data)
{
	if(data->data)
	{
		char *temp;
		int len;

		len=strlen(data->data);
		temp=malloc(sizeof(char)*len+1);
		snprintf(temp,len+1,"%s",data->data);
		free(data->data);

		data->data=malloc(sizeof(char)*len+nmemb+1);
		snprintf(data->data,len+nmemb+1,"%s%s",temp,ptr);
		free(temp);
	}
	else
	{
		data->data=malloc(sizeof(char)*nmemb+1);
		snprintf(data->data,nmemb+1,"%s",ptr);
	}

	return nmemb;
}
