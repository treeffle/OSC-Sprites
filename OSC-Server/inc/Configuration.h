/*
 *  GrendlConfiguration.h
 *  GrendlServer
 *
 *  Created by Chris Branton on 5/6/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *	
 *	Encapsulates the configuration information for the GRENDL Server.
 * 
 */
#ifndef _YAML_CONFIGURATION_H
#define _YAML_CONFIGURATION_H

#include <fstream>
#include <iostream>
#include <list>

#include "yaml-cpp/yaml.h"
#include "SpriteView.h"

struct string {
    std::string sprite1;
};

	class OscMessage {
	public:
		std::string address;
		std::string typeString;
		std::string event;
	};


class Configuration {
protected:
	
	bool initialized;
	std::string oscPortIn;
	std::vector<std::string> m_keyList;
	std::map<std::string, std::string > m_contents;
	
	void parseListener (const YAML::Node& listenerNode);
	void parseCommands (const YAML::Node& commandNode);
	void parseEnsemble (const YAML::Node& ensembleNode);

//	void operator >> (const YAML::Node& node, OscMessage& msg);
//	void operator >> (const YAML::Node& node, GrendlConfiguration::Command& cmd);
	
public:
	Configuration();
	Configuration (std::string path); // Creates a general configuration from a file
	
	~Configuration();
	
	bool LoadConfiguration (std::string path);
	void Initialize(std::string yamlString);
	void ParseContainers(std::string yamlString, std::map<std::string, Container> *, std::map<std::string,Container>*);
	void ParseSprites(std::string yamlString, std::map<std::string, Sprite> *);
	bool HasKey (const std::string& key);
	void SetValue (const std::string& key, const std::string& value);
	void GetValue(const std::string& key, std::string& value);
	std::string getOscPort();
    void ParseTodo(const YAML::Node &node, std::vector<std::string> string_names , std::map<std::string, Sprite> *sprite_map);
    void ParseVertexData(std::string yamlString, SpriteView *view);
	
	

	
	struct GrendlCommand {
		std::string event;
		std::string name;
		std::string file;
	};
	
	class EnsembleMember {
	public:
		std::string name;
		std::string username;
		std::string hostname;
		std::string hostip;
		std::string receiverport;
		std::string userAtHost;
	};
	
	std::list<OscMessage>& GetOscConfiguration();

	std::list<OscMessage> messageSpecs;
	std::list<GrendlCommand> commandSpecs;
	std::list<EnsembleMember> ensembleMembers;
} ;



#endif //_GRENDL_CONFIGURATION_H
