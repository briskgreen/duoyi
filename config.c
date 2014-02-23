/*配置文件使用json数据格式*/
#include "config.h"

void duoyi_read_config(DuoYiData *data)
{
	JsonParser *parser;
	JsonReader *reader;
	DuoYiData *duoyi_data=(DuoYiData *)data;

	g_type_init();
	parser=json_parser_new();
	/*配置文件为当前目录下的.duoyirc文件*/
	if(!json_parser_load_from_file(parser,".duoyirc",NULL))
	{
		printf("读取配置文件.duoyirc出错!\n");
		return;
	}
	reader=json_reader_new(json_parser_get_root(parser));

	/*解析出默认词典*/
	json_reader_read_member(reader,"default");
	duoyi_data->dic=json_reader_get_int_value(reader);
	json_reader_end_member(reader);

	/*解析出字体*/
	json_reader_read_member(reader,"font");
	duoyi_data->font=json_reader_get_string_value(reader);
	json_reader_end_member(reader);

	/*百度API*/
	json_reader_read_member(reader,"api");
	json_reader_read_element(reader,0);
	json_reader_read_member(reader,"BaiDu");
	data->api[0]=json_reader_get_string_value(reader);
	json_reader_end_member(reader);

	/*必应API*/
	json_reader_read_member(reader,"Bing");
	data->api[1]=json_reader_get_string_value(reader);
	json_reader_end_member(reader);

	/*金山API*/
	json_reader_read_member(reader,"King");
	data->api[2]=json_reader_get_string_value(reader);
	json_reader_end_member(reader);

	/*有道API*/
	json_reader_read_member(reader,"YouDao");
	data->api[3]=json_reader_get_string_value(reader);
	json_reader_end_member(reader);

	json_reader_end_element(reader);
}

void duoyi_write_config(DuoYiData *data)
{
	JsonBuilder *builder;
	JsonGenerator *generator;
	int i;
	char *name[]={"BaiDu","Bing","King","YouDao"};

	g_type_init();

	builder=json_builder_new();
	json_builder_begin_object(builder);

	/*默认词典*/
	json_builder_set_member_name(builder,"default");
	json_builder_add_int_value(builder,data->dic);

	/*字体设置*/
	json_builder_set_member_name(builder,"font");
	if(data->font)
		json_builder_add_string_value(builder,data->font);
	else
		json_builder_add_null_value(builder);

	/*API设置*/
	json_builder_set_member_name(builder,"api");
	json_builder_begin_array(builder);
	json_builder_begin_object(builder);

	for(i=0;i != 4;++i)
	{
		json_builder_set_member_name(builder,name[i]);
		if(data->api[i])
			json_builder_add_string_value(builder,data->api[i]);
		else
			json_builder_add_null_value(builder);
	}

	json_builder_end_object(builder);
	json_builder_end_array(builder);
	json_builder_end_object(builder);

	generator=json_generator_new();
	json_generator_set_root(generator,json_builder_get_root(builder));
	json_generator_to_file(generator,".duoyirc",NULL);
}
