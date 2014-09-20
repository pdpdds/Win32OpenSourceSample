// yamlSample.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#pragma warning(push)
#pragma warning(disable:4996)
#include "yaml-cpp/yaml.h"
#pragma warning(pop)

#include <fstream>
#include <string>
#ifdef _DEBUG
#pragma comment(lib, "libyaml-cppmdd.lib")
#else
#pragma comment(lib, "libyaml-cppmd.lib")
#endif

void SequenceTest();
int _tmain(int argc, _TCHAR* argv[])
{
	YAML::Node node = YAML::LoadFile("env.yaml");

	YAML::Node settingNode = node["settings"];
			
	std::cout << settingNode["graphics"]["vsync"].as<bool>() << "\n";
	std::cout << settingNode["graphics"]["quality"].as<std::string>() << "\n";

	std::cout << settingNode["graphics"]["resolution"]["width"].as<int>() << "\n";
	std::cout << settingNode["graphics"]["resolution"]["height"].as<int>() << "\n";
	

	return 0;
}
void SequenceTest()
{
	YAML::Node node = YAML::Load("[1, 2, 3]");
	assert(node.Type() == YAML::NodeType::Sequence);
	assert(node.IsSequence());  // a shortcut!
}

