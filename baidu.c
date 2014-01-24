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
	else
	{
		char *buf;

		buf=baidu_error_int(1);
		res=malloc(strlen(buf)+1);
		snprintf(res,strlen(buf)+1,"%s",buf);

		return res;
	}
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

char *baidu_error_int(int code)
{
	switch(code)
	{
		case 1:
			return "未知错误";
		case 2:
			return "服务器暂不可用";
		case 3:
			return "未知的方法";
		case 4:
			return "接口调用次数已达到设定上限";
		case 5:
			return "请求来自未经授权的IP地址";
		case 6:
			return "无权限访问该用户数据";
		case 7:
			return "来自refer的请求无访问权限";
		case 100:
			return "请求参数无效";
		case 101:
			return "api key无效";
		case 102:
			return "session key无效";
		case 103:
			return "call_id参数无效";
		case 104:
			return "无效签名";
		case 105:
			return "请求参数过多";
		case 106:
			return "未知的签名方法";
		case 107:
			return "timestamp参数无效";
		case 108:
			return "无效的user id";
		case 109:
			return "无效的用户资料段名";
		case 110:
			return "无效的access token";
		case 111:
			return "access token过期";
		case 112:
			return "session key过期";
		case 114:
			return "无效的ip参数";
		case 210:
			return "用户不可见";
		case 211:
			return "获取未授权的字段";
		case 212:
			return "没有权限用户的email";
		case 800:
			return "未知的存储操作错误";
		case 801:
			return "无效的操作方法";
		case 802:
			return "数据存储空间已超过设定上限";
		case 803:
			return "指定的对象不存在";
		case 804:
			return "指定的对象已存在";
		case 805:
			return "数据库操作出错，请重试";
		case 900:
			return "访问的应用不存在";
		case 950:
			return "批量操作已开始，请先调用end_batch接口结束前一个批量操作";
		case 951:
			return "结束批量操作的接口调用不应该在start_batch接口之前被调用";
		case 952:
			return "每个批量调用不能包含多于10个接口调用";
		case 953:
			return "该接口不适合在批量调用操作中被使用 ";
		default:
			return "未知错误";
	}
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

		//buf=baidu_error(json_reader_get_string_value(reader));
		buf=json_reader_get_string_value(reader);
		if(buf == NULL)
		{
			json_reader_end_member(reader);
			json_reader_read_member(reader,"error_code");
			buf=baidu_error_int(json_reader_get_int_value(reader));
		}
		else
			buf=baidu_error(buf);

		res=malloc(strlen(buf)+1);
		snprintf(res,strlen(buf)+1,"%s",buf);
		json_reader_end_member(reader);
	}

	return res;
}
