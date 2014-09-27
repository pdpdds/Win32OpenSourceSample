// libxml2Ex.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#pragma comment(lib, "libxml2.lib")
#pragma comment(lib, "ws2_32.lib")


#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

static void print_element_names(xmlNode * a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf("node type: Element, name: %s\n", cur_node->name);
		}

		print_element_names(cur_node->children);
	}
}

int main(int argc, char **argv)
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	LIBXML_TEST_VERSION

	doc = xmlReadFile("libxml2.xml", NULL, 0);

	if (doc == NULL) 
	{
		printf("error: could not parse file %s\n", "libxml2.xml");
		return 0;
	}

	root_element = xmlDocGetRootElement(doc);
	print_element_names(root_element);

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}