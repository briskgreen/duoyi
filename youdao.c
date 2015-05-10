#include "youdao.h"
#include "tool.h"

char *youdao_translate(char *api,char *word)
{
	CURL *curl;
	CURLcode code;
	char *res;
	char *url;
	char *buf;
	DATA data;
	char yd_from[512]={0};
	char yd_api[512]={0};

	/*输入api与keyfrom
	 * 有道词典接口需要这两个参数
	 * 为了能够与其它词典API一样使用一行进行存储
	 * 所以有道词典API的存储方法是keyfrom api*/
	sscanf(api,"%s%s",yd_from,yd_api);
	/*url转码，不然无法翻译中文*/
	url=url_encode(word);
	buf=string_add("http://fanyi.youdao.com/openapi.do?keyfrom=%s&key=%s&type=data&doctype=json&version=1.1&q=%s",yd_from,yd_api,url);
	free(url);
	data.data=NULL;
	curl=curl_easy_init();

	curl_easy_setopt(curl,CURLOPT_URL,buf);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_read_to_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);

	code=curl_easy_perform(curl);
	free(buf);

	if(code != 0)
		res=youdao_error(-1);
	if(data.data)
	{
		res=youdao_parser(data.data);
		free(data.data);
	}
	else
		res=youdao_error(-2);

	return res;
}

char *youdao_parser(char *data)
{
	JsonParser *parser;
	JsonReader *reader;
	char *res;
	int code;

	g_type_init();

	parser=json_parser_new();
	json_parser_load_from_data(parser,data,-1,NULL);
	reader=json_reader_new(json_parser_get_root(parser));

	json_reader_read_member(reader,"errorCode");
	code=json_reader_get_int_value(reader);
	if(code != 0)
	{
		res=youdao_error(code);
		json_reader_end_member(reader);
	}
	else
	{
		json_reader_end_member(reader);
		json_reader_read_member(reader,"query");
		res=string_add("%s\r\n",json_reader_get_string_value(reader));
		json_reader_end_member(reader);

		if(json_reader_read_member(reader,"translation"))
		{
			int count;
			int i;

			count=json_reader_count_elements(reader);
			for(i=0;i != count;++i)
			{
				json_reader_read_element(reader,i);
				res=stradd(res,json_reader_get_string_value(reader));
				res=stradd(res,"\r\n");

				json_reader_end_element(reader);
			}

			json_reader_end_member(reader);
		}

		if(json_reader_read_member(reader,"basic"))
		{
			int count;
			int i;

			res=stradd(res,"\r\n简单翻译:\r\n");

			if(json_reader_read_member(reader,"phonetic"))
			{
				res=stradd(res,json_reader_get_string_value(reader));
				res=stradd(res,"\r\n");
				//json_reader_end_member(reader);
			}
		/*	else
			{
				json_reader_end_member(reader);
				json_reader_read_member(reader,"basic");
			}*/

			json_reader_end_member(reader);
			json_reader_read_member(reader,"explains");
			count=json_reader_count_elements(reader);

			for(i=0;i != count;++i)
			{
				json_reader_read_element(reader,i);
				res=stradd(res,json_reader_get_string_value(reader));
				res=stradd(res,"\r\n");
				json_reader_end_element(reader);
			}

			json_reader_end_member(reader);
			json_reader_end_member(reader);
		}

		if(json_reader_read_member(reader,"web"))
		{
			int count;
			int i;
			int j;
			int n;

			res=stradd(res,"\r\n网络翻译:\r\n");
			count=json_reader_count_elements(reader);

			for(i=0;i != count;++i)
			{
				json_reader_read_element(reader,i);

				json_reader_read_member(reader,"key");
				res=stradd(res,json_reader_get_string_value(reader));
				res=stradd(res,"\r\n");
				json_reader_end_member(reader);

				json_reader_read_member(reader,"value");
				n=json_reader_count_elements(reader);

				for(j=0;j != n;++j)
				{
					json_reader_read_element(reader,j);
					res=stradd(res,json_reader_get_string_value(reader));
					res=stradd(res,"\r\n");
					json_reader_end_element(reader);
				}

				json_reader_end_member(reader);
				json_reader_end_element(reader);
				res=stradd(res,"\r\n\r\n");
			}

			json_reader_end_member(reader);
		}

	}

	return res;
}

char *youdao_error(int code)
{
	char *res;

	switch(code)
	{
		case 20:
			res=string_add("要翻译的文本过长");
			break;
		case 30:
			res=string_add("无法进行有效的翻译");
			break;
		case 40:
			res=string_add("不支持的语言类型");
			break;
		case 50:
			res=string_add("无效的key");
			break;
		case -1:
			res=string_add("网络错误");
			break;
		case -2:
			res=string_add("翻译出错");
			break;
	}

	return res;
}
