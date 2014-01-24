#include "king.h"
#include "tool.h"

void replace(char *word);

char *king_translate(char *api,char *word)
{
	CURL *curl;
	CURLcode code;
	char *buf;
	char *res;
	char *url;
	DATA data;

	/*替换掉<与>字符，不然解析时会是错误的xml文件*/
	replace(word);
	url=url_encode(word);
	buf=string_add("http://dict-co.iciba.com/api/dictionary.php?w=%s&key=%s",url,api);
	free(url);
	curl=curl_easy_init();
	data.data=NULL;

	curl_easy_setopt(curl,CURLOPT_URL,buf);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_read_to_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);

	code=curl_easy_perform(curl);
	free(buf);

	if(code != 0)
		res=king_error("网络错误");

/*如果有返回数据，则解析*/	
	if(data.data)
	{
		res=king_parser(data.data);
		free(data.data);
	}
	else
		res=king_error("翻译出错");

	return res;
}

char *king_parser(char *data)
{
	char *res;
	xmlDocPtr doc;
	xmlNodePtr root;
	xmlChar *str;

	doc=xmlParseDoc(data);
	xmlKeepBlanksDefault(0);
	root=xmlDocGetRootElement(doc);
	root=root->children;

	res=string_add("%s","翻译:\n\n");
	while(root)
	{
		if(!xmlStrcmp(root->name,"fy"))
		{
			str=xmlNodeGetContent(root);
			res=stradd(res,str);

			xmlFree(str);
		}

		if(!xmlStrcmp(root->name,"pos"))
		{
			str=xmlNodeGetContent(root);
			res=stradd(res,str);

			xmlFree(str);
		}

		if(!xmlStrcmp(root->name,"acceptation"))
		{
			str=xmlNodeGetContent(root);
			res=stradd(res,str);

			xmlFree(str);
		}

		if(!xmlStrcmp(root->name,"sent"))
			break;

		root=root->next;
	}

	res=stradd(res,"\n\n例句:\n\n");
	while(root)
	{
		if(!xmlStrcmp(root->name,"sent"))
		{
			str=xmlNodeGetContent(root);
			res=stradd(res,str);

			xmlFree(str);
		}

		root=root->next;
	}

	xmlFreeDoc(doc);

	return res;
}

char *king_error(char *code)
{
	char *res;
	
	res=malloc(strlen(code)+1);
	snprintf(res,strlen(code)+1,"%s",code);

	return res;
}

void replace(char *word)
{
	int i;

	for(i=0;word[i];++i)
	{
		switch(word[i])
		{
			case '<':
				word[i]='[';
				break;
			case '>':
				word[i]=']';
				break;
		}
	}
}
