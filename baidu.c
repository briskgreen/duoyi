#include "baidu.h"
#include "tool.h"

char *baidu_tran[]=
{
	"自动识别","中文","英语","日语","韩语","西班牙语","法语",
	"泰语","阿联酋语","粤语","俄语","葡萄牙语",NULL
};

char *baidu_code[]=
{
	"auto","zh","en","jp","kor","spa","fra",
	"th","ara","yue","ru","pt",NULL
};

char *baidu_translate(char *from,char *to,char *api,char *word)
{
	CURL *curl;
	CURLCode code;
	char *res;
	char *url;
	char *buf;

	url=url_encode(word);
	buf=string_add(
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
