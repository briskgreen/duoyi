#include "baidu.h"
#include "tool.h"

char *baidu_tran[]=
{
	"自动识别","中文","英语","日语","韩语","西班牙语","法语",
	"泰语","阿拉伯语","粤语","俄语","葡萄牙语",NULL
};

char *baidu_code[]=
{
	"auto","zh","en","jp","kor","spa","fra",
	"th","ara","yue","ru","pt",NULL
};

char *baidu_translate(char *from,char *to,char *api,char *word)
{
	CURL *curl;
	CURLcode code;
	char *res;
	char *url;
	char *buf;
	DATA data;

	data.data=NULL;
	url=url_encode(word);
	if(url == NULL)
		return;

	buf=string_add("http://openapi.baidu.com/public/2.0/bmt/translate?client_id=%s&q=%s&from=%s&to=%s",api,url,from,to);
	free(url);

	curl=curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,buf);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_read_to_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);

	free(buf);
	code=curl_easy_perform(curl);
	if(code != 0)
	{
		res=malloc(strlen("网络错误")+1);
		snprintf(res,strlen("网络错误")+1,"网络错误");
		return res;
	}

	curl_easy_cleanup(curl);
	if(data.data)
		return baidu_parser(data.data);
}

char *baidu_error(char *code)
{
	if(strcmp(code,"52001") == 0)
		return "超时";
	if(strcmp(code,"52002") == 0)
		return "翻译系统错误";
	if(strcmp(code,"52003") == 0)
		return "未授权用户";

	return "未知错误";
}

char *baidu_parser(char *data)
{
	JsonParser *parser;
	JsonReader *reader;
	char *res=NULL;
	char *temp;
	char *buf;

	g_type_init();

	parser=json_parser_new();
	json_parser_load_from_data(parser,data,-1,NULL);
	reader=json_reader_new(json_parser_get_root(parser));

	if(json_reader_read_member(reader,"trans_result"))
	{
		int len;
		int i;

		len=json_reader_count_elements(reader);

		for(i=0;i != len;++i)
		{
			json_reader_read_element(reader,i);
			json_reader_read_member(reader,"dst");
			buf=json_reader_get_string_value(reader);

			if(res)
			{
				int len;

				len=strlen(res);
				temp=malloc(len+1);
				snprintf(temp,len+1,"%s",res);
				free(res);

				len+=strlen(buf);
				res=malloc(len+1);
				snprintf(res,len+1,"%s%s",temp,buf);
				free(temp);
			}
			else
			{
				res=malloc(strlen(buf)+1);
				snprintf(res,strlen(buf)+1,"%s",buf);
			}

			json_reader_end_member(reader);
			json_reader_end_element(reader);
		}

		json_reader_end_member(reader);
	}
	else
	{
		json_reader_end_member(reader);

		json_reader_read_member(reader,"error_code");

		buf=baidu_error(json_reader_get_string_value(reader));
		res=malloc(strlen(buf)+1);
		snprintf(res,strlen(buf)+1,"%s",buf);
	}

	return res;
}
