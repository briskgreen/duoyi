#include "bing.h"
#include "tool.h"

char *bing_tran[]=
{
	"阿拉伯语","保加利亚语","加泰罗尼亚语","简体中文","繁体中文","捷克语",
	"丹麦语","荷兰语","英语","爱沙尼亚语","波斯语","芬兰语","法国诘","德语",
	"希腊语","海地语","希伯来语","印地语","匈牙利语","印度尼西亚语","意大利语","日语",
	"韩语","拉脱维亚语","立陶宛语","马来语","苗语","挪威语","波兰语","葡萄牙语",
	"罗马尼亚语","俄语","斯洛伐克语","斯洛文尼亚语","西班牙语","瑞典语","泰国语","土耳语",
	"乌克兰语","乌尔都语","越南语",NULL
};

char *bing_code[]=
{
	"ar","bg","ca","zh-CHS","zh-CHT","cs",
	"da","nl","en","et","fa","fi","fr","de",
	"el","ht","he","hi","hu","id","it","ja",
	"ko","lv","lt","ms","mww","no","pl","pt",
	"ro","ru","sk","sl","es","sv","th","tr",
	"uk","ur","vi",NULL
};

char *bing_translate(char *from,char *to,char *api,char *word)
{
	CURL *curl;
	CURLcode code;
	char *res;
	char *buf;
	char *url;
	DATA data;
	int retcode;

	url=url_encode(word);
	data.data=NULL;
	buf=string_add("https://api.datamarket.azure.com/Bing/MicrosoftTranslator/v1/Translate?Text=%%27%s%%27&From=%%27%s%%27&To=%%27%s%%27&$skip=0&$top=1&$format=json",url,from,to);
	free(url);
	url=string_add(":%s",api);

	curl=curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,buf);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_read_to_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);
	curl_easy_setopt(curl,CURLOPT_HTTPAUTH,CURLAUTH_BASIC);
	curl_easy_setopt(curl,CURLOPT_USERPWD,url);

	code=curl_easy_perform(curl);
	free(buf);
	free(url);

	if(code != 0)
		res=bing_error("网络错误");
	curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&retcode);
	curl_easy_cleanup(curl);

	if(retcode != 200)
		res=bing_error(data.data);
	else
		res=bing_parser(data.data);

	free(data.data);
	return res;

}

char *bing_parser(char *data)
{
	JsonParser *parser;
	JsonReader *reader;
	char *res;
	char *buf;

	g_type_init();

	parser=json_parser_new();
	json_parser_load_from_data(parser,data,-1,NULL);
	reader=json_reader_new(json_parser_get_root(parser));

	json_reader_read_member(reader,"d");
	json_reader_read_member(reader,"results");
	json_reader_read_element(reader,0);
	json_reader_read_member(reader,"Text");
	
	buf=json_reader_get_string_value(reader);
	res=malloc(strlen(buf)+1);
	snprintf(res,strlen(buf)+1,"%s",buf);

	json_reader_end_member(reader);
	json_reader_end_element(reader);
	json_reader_end_member(reader);
	json_reader_end_member(reader);

	return res;
}

char *bing_error(char *code)
{
	char *res;

	int len=strlen(code);
	res=malloc(len+1);
	snprintf(res,len+1,"%s",code);

	return res;
}
