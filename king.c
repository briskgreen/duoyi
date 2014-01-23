#include "king.h"
#include "tool.h"

char *king_translate(char *api,char *word)
{
	CURL *curl;
	CURLcode code;
	char *url;
	char *buf;
	char *res;
	DATA data;

	url=url_encode(word);
	buf=string_add("http://dict-co.iciba.com/api/dictionary.php?w=%s&key=%s",url,api);
	free(url);
	curl=curl_easy_init();
	data.data=NULL;

	curl_easy_setopt(curl,CURLOPT_URL,buf);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_read_to_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);

	code=curl_easy_perform(curl);

	if(code != 0)
		res=king_error("网络错误");
	
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
	root=xmlDocGetRootElement(doc);
	root=root->children;

	res=string_add("%s","翻译:\n\n");
	while(root)
	{
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
