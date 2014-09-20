// yamlSample2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#pragma warning(push)
#pragma warning(disable:4996)
#include "yaml-cpp/yaml.h"
#pragma warning(pop)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifdef _DEBUG
#pragma comment(lib, "libyaml-cppmdd.lib")
#else
#pragma comment(lib, "libyaml-cppmd.lib")
#endif

// our data types
struct Vec3 {
	float x, y, z;
};

struct Power {
	std::string name;
	int damage;
};

struct Monster {
	std::string name;
	Vec3 position;
	std::vector <Power> powers;
};

// now the extraction operators for these types
void operator >> (const YAML::Node& node, Vec3& v) {
	v.x = node[0].as<float>();
	v.y = node[1].as<float>();
	v.z = node[2].as<float>();
}

void operator >> (const YAML::Node& node, Power& power) {
	power.name = node["name"].as<std::string>();
	power.damage = node["damage"].as<int>();
}

void operator >> (const YAML::Node& node, Monster& monster) {
	monster.name = node["name"].as<std::string>();
	node["position"] >> monster.position;
	const YAML::Node& powers = node["powers"];
	for (unsigned i = 0; i<powers.size(); i++) {
		Power power;
		powers[i] >> power;
		monster.powers.push_back(power);
	}
}


	

int _tmain(int argc, _TCHAR* argv[])
{
	YAML::Node doc = YAML::LoadFile("monsters.yaml");

	for (unsigned i = 0; i < doc.size(); i++) {
		Monster monster;
		doc[i] >> monster;
		std::cout << monster.name << "\n";
		std::cout << monster.position.x << " "
				  << monster.position.y << " "
			      << monster.position.z << "\n";
	}

	return 0;
}

