
/*
 *  GrendlConfiguration.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 5/6/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#include "Configuration.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <limits>

std::string instruction;

Configuration::Configuration(): initialized(false) {
	m_keyList.push_back("name");
	m_keyList.push_back("lol");
}

// Constructor loads configuration from source file
Configuration::Configuration (std::string path): initialized(false), messageSpecs(), commandSpecs(), ensembleMembers() {
	if (LoadConfiguration(path)) {
		initialized = true;
	}
}




Configuration::~Configuration() {
    
}

bool Configuration::HasKey (const std::string& key) {
	return (m_contents.count(key) > 0);
}


// Inserts the given key-value pair. If key already exists, it is replaced.
// NOTE: this is not the default behavior for C++ maps.
//	Currently does not work, and cannot until we realize a base class
void Configuration::SetValue (const std::string& key, const std::string& value) {
	m_contents.erase(key);
	m_contents[key] = value;
}

//	Currently does not work, and cannot until we realize a base class
void Configuration::GetValue(const std::string& key, std::string& value){
	if (m_contents.count(key) > 0) {
		value = m_contents[key];
	}
}


std::string Configuration::getOscPort() {
	return oscPortIn;
}

std::list<OscMessage>& Configuration::GetOscConfiguration() {
	return messageSpecs;
}

void Configuration::Initialize(std::string yamlString) {
	std::stringstream inStream (yamlString, std::stringstream::in);
	YAML::Parser parser1(inStream);
	YAML::Node confgrendlDoc;
	parser1.GetNextDocument(confgrendlDoc);
    
	// check for keys
	//std::cout << "Initializing Grendl Configuration..."  << std::endl;
	std::vector<std::string>::iterator it;
	for (it = m_keyList.begin(); it != m_keyList.end(); ++it) {
		//std::cout << "Seeking key " << *it << std::endl;
		if(const YAML::Node *pName = confgrendlDoc.FindValue(*it)) {
			std::string newValue;
			*pName >> newValue;
			m_contents[*it] = newValue;
			//std::cout << "Key " << *it << " exists, with value '" << newValue << "'\n";
		} else {
			//std::cout << "Key " << *it << " doesn't exist\n";
		}
        const YAML::Node& commandNode = confgrendlDoc["command"];
		//std::cout << "Command is " << commandNode.FindValue("command");
	}
	// look for player list
	//std::cout << "Reading player list " << std::endl;
	if(const YAML::Node *pName = confgrendlDoc.FindValue("ensemble")) {
		//std::cout << "List exists\n";
		parseEnsemble(*pName);
	}
	
}


/*
 Sprite input format:
 
 sample_sprite:
 update: true
 scale:
 - 0.0
 - 0.0
 - 0.0
 rgbcolor
 - 0.0
 - 0.0
 - 1.0
 - 1.0
 rotate
 - 0.0
 - 0.0
 - 0.0
 vbo_name: square
 int_in:
 dummy_int: 1
 dummy_int2: 5
 has_texture: false
 tex_name: cat.png
 shader_name: standard
 name: sample_sprite
 float_in:
 dummy_float: 1.0
 dummy_float2: 2.0
 translate:
 - 0.2
 - 0.0
 - 0.0
 instruction: add
 time: 2.0
 pixel:
 - 0
 - 0
 - 200
 - 200
 - 1
 alpha: 1.0
 */

 
 
 
 
void operator >> (const YAML::Node& node, Sprite& s )
{
	std::string instruction, update, vbo_name, shader_name, tex_name;
	float duration;
	bool use_pixels;
    
	//following three variables must always be in the sprite message
    
	//need to know instruction ahead of time
	node["instruction"] >> instruction;
    
	//need to know time duration ahead of time
	node["time"] >> duration;
    
	//need to know ahead of time whether to use pixel functions or unit functions
	std::string dummy;
	node["use_pixels"] >> dummy;
	use_pixels = (dummy.compare("false")==0) ? false : true;
    
	
	std::map<std::string,int> int_in;
	std::map<std::string,float> float_in;
	GLfloat rx, ry, rz, sx, sy, sz, tx, ty, tz;
	GLfloat r, g, b;
    GLfloat alpha;
	bool has_texture;
	
	//these variables may or may not be a part of the message
	for(YAML::Iterator it = node.begin(); it != node.end(); ++it)
	{
		std::string key;
		it.first() >> key;
		if(key.compare("vbo_name")==0)
		{
		 	it.second() >> vbo_name;
            
		 	//for(YAML::Iterator it = node.begin(); it != node.end(); ++it )
		 	//	sprite_map[curr_sprite_name].update_vbo(vbo_name);
            
		 	s.update_vbo(vbo_name);
		}
		else if(key.compare("shader_name")==0)
		{
		 	it.second() >> shader_name;
		 	s.update_shader(shader_name);
		}
		else if(key.compare("tex_name")==0)
		{
			it.second() >> tex_name;
			s.update_texture(tex_name);
		}
		else if(key.compare("int_in")==0)
		{
			it.second() >> int_in;
			s.update_int_in(int_in);
		}
		else if(key.compare("float_in")==0)
		{
			it.second() >> float_in;
			s.update_float_in(float_in);
		}
		else if(key.compare("rotate")==0)
		{
			it.second()[0] >> rx;
			it.second()[1] >> ry;
			it.second()[2] >> rz;
			if(instruction.compare("add")==0)
				s.add_rotate(rx,ry,rz);
			else if(instruction.compare("gradual")==0)
				s.gradual_rotate(rx,ry,rz,duration);
			else
				s.update_rotate(rx,ry,rz);
		}
		else if(key.compare("scale")==0)
		{
			it.second()[0] >> sx;
			it.second()[1] >> sy;
			it.second()[2] >> sz;
			if(!use_pixels)
			{
				if(instruction.compare("add")==0)
					s.add_scale(sx,sy,sz);
				else if(instruction.compare("gradual")==0)
					s.gradual_scale(sx,sy,sz,duration);
	            else
	            	s.update_scale(sx,sy,sz);
        	}
		}
		else if(key.compare("translate")==0)
		{
			it.second()[0] >> tx;
			it.second()[1] >> ty;
			it.second()[2] >> tz;
			if(!use_pixels)
			{
				if(instruction.compare("add")==0)
					s.add_translate(tx,ty,tz);
				else if(instruction.compare("gradual")==0)
					s.gradual_translate(tx,ty,tz,duration);
	            else
	            	s.update_translate(tx,ty,tz);
	        }
		}
		else if(key.compare("rgbcolor")==0)
		{
			it.second()[0] >> r;
			it.second()[1] >> g;
			it.second()[2] >> b;
			if(instruction.compare("gradual")==0)
				s.gradual_color(r, g, b, duration);
			else
				s.update_color(r, g, b);
		}
		else if(key.compare("alpha")==0)
		{
			it.second() >> alpha;
			if(instruction.compare("gradual")==0)
				s.gradual_fade(alpha, duration);
			else
				s.update_alpha(alpha);
		}
		else if(key.compare("has_texture")==0)
		{
			std::string temp;
			it.second() >> temp;
			if(temp.compare("true")==0)
				has_texture = true;
			else has_texture = false;
			s.use_texture(has_texture);
		}
		else if(key.compare("pixel")==0)
		{
			/*pixel format:
             [0] = x position
             [1] = y position
             [2] = width
             [3] = height
             [4] = layer*/
            
			int x, y, width, height, layer;
			it.second()[0] >> x;
			it.second()[1] >> y;
			it.second()[2] >> width;
			it.second()[3] >> height;
			it.second()[4] >> layer;
			//window width = 768+768/15.4*1.235*2+768, window height = 1366
			if (use_pixels)
			{
				if(instruction.compare("add")==0)
				{
					s.add_size(width, height);
					s.add_position(x,y);
				}
				else if(instruction.compare("gradual")==0)
				{
					s.gradual_size(width, height, duration);
					s.gradual_position(x,y, duration);
				}
				else
				{
					s.update_size(width, height);
					s.update_position(x, y, layer);
				}
			}
		}
	}
}

/*containers coming in the form of:
 
 - name: cont1
 vbo: square
 shader: standard
 tex: cat1.png
 transform: [1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1]
 color: [1,0,1]
 has_texture: true
 update: true
 instruction: none
 int_in:
 test_int: 5
 float_in
 test_float: 3.4
 -name: cont2
 ...etc
 
 */

void operator >> (const YAML::Node& node, Container& c)
{
	
	node["instruction"] >> c.instruction;
	node["update"] >> c.update;
	node["vbo_name"] >> c.vbo_name;
	node["shader_name"] >> c.shader_name;
	node["tex_name"] >> c.tex_name;
	node["int_in"] >> c.int_in;
	node["float_in"] >> c.float_in;
	c.transform_matrix = new float[16];
	for(int i=0; i<16; i++)
		node["transform"][i] >> c.transform_matrix[i];
	c.rgbcolor = new float[3];
	for(int i=0; i<3; i++)
		node["rgbcolor"][i] >> c.rgbcolor[i];
	std::string temp;
	node["has_texture"] >> temp;
	if(temp.compare("true") == 0) c.has_texture = true;
	else c.has_texture = false;
}

void printContainer(Container cont)
{
	//std::cout << "Container:" << std::endl;
	//std::cout << "\tvbo name: " << cont.vbo_name << std::endl;
	//std::cout << "\tshader name: " << cont.shader_name << std::endl;
	//std::cout << "\tHas texture? " << cont.has_texture << std::endl;
	if(cont.has_texture) //std::cout << "\t\ttexture name: " << cont.tex_name << std::endl;
	//std::cout << "\tColors: [" << cont.rgbcolor[0] << "," << cont.rgbcolor[1] << "," << cont.rgbcolor[2] << "]" << std::endl;
	//std::cout << "\tTransform matrix: \n\t\t";
	for(int i=0; i < 16; i++)
		//std::cout << cont.transform_matrix[i] << ",";
	for(std::map<std::string, int>::iterator map_iter = cont.int_in.begin(); map_iter != cont.int_in.end(); map_iter++)
	{
		std::string int_name = map_iter->first;
		int int_val = map_iter->second;
		//std::cout<<int_name<<": "<<int_val<<std::endl;
	}
	for(std::map<std::string, float>::iterator map_iter = cont.float_in.begin(); map_iter != cont.float_in.end(); map_iter++)
	{
		std::string float_name = map_iter->first;
		float float_val = map_iter->second;
		//std::cout<<float_name<<": "<<float_val<<std::endl;
	}
}

void UpdateContainers(std::map<std::string,Container> *cm, std::map<std::string,Container> *um)
{
	for(std::map<std::string,Container>::iterator map_iter = um->begin(); map_iter != um->end(); map_iter++)
	{
		(*cm)[map_iter->first] = map_iter->second;
	}
	um->clear();
}

void Configuration::ParseContainers(std::string yamlString, std::map<std::string,Container> *cm, std::map<std::string,Container> *um)
{
	//std::cout<<"parsing containers";
	std::stringstream inStream (yamlString, std::stringstream::in);
	YAML::Parser parser(inStream);
	YAML::Node doc;
	////std::cout<<"Got YAML set up. getting next document";
	parser.GetNextDocument(doc);
	std::vector<string> sprite_names;
	
	for(YAML::Iterator it=doc.begin(); it !=doc.end(); it++)
	{
		//std::cout<<"loop";
		
		std::string cont_name;
		it.first() >> cont_name;
		if(cont_name.compare("address") == 0) continue;
		if(cont_name.compare("sprite_names") == 0) continue;
		it.second() >> (*um)[cont_name];
		if((*um)[cont_name].update.compare("true") == 0) UpdateContainers(cm,um);
        //	printContainer((*cm)[cont_name]);
	}
}


void Configuration::ParseTodo(const YAML::Node &node, std::vector<std::string> string_names, std::map<std::string, Sprite> *sprite_map)
{
	//transfer void operator >> (const YAML::Node& node, Sprite& s)
    std::string instruction, update, vbo_name, shader_name, tex_name;
	float duration;
	bool use_pixels;
    std::string dummy;
    
    std::map<std::string,int> int_in;
	std::map<std::string,float> float_in;
	GLfloat rx, ry, rz, sx, sy, sz, tx, ty, tz;
	GLfloat r, g, b;
    GLfloat alpha;
	bool has_texture;
    std::string sprite1;
    
    for (YAML::Iterator it = node.begin(); it != node.end(); ++it)
    {
        std::string key;
        it.first() >> key;
        if(key.compare("instruction")==0)
        {
            it.second() >> instruction;
        }
        else if(key.compare("time")==0)
        {
            it.second()>>duration;
        }
        else if (key.compare("use_pixels") ==0)
        {
            it.second() >> dummy;
            if(dummy.compare("false")==0)
                use_pixels = false;
            else use_pixels = true;
        }
        
    }

    if(instruction.compare("delete")==0)
    {
    	//delete all these sprites, then return
    	for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm)
    	{
            std::string sprite_name = *tm;
            std::map<std::string, Sprite>::iterator it = sprite_map->find(sprite_name);
            sprite_map->erase(it);
        }
        return;

    }
    
    //these variables may or may not be a part of the message
	for(YAML::Iterator it = node.begin(); it != node.end(); ++it)
	{
		std::string key;
		it.first() >> key;
		if(key.compare("vbo_name")==0)
		{
		 	it.second() >> vbo_name;
            for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                ////std::cout<< *tm; // output the current value that It is *pointing to
                std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                s->update_vbo(vbo_name);
                
            }
		}
        
		else if(key.compare("shader_name")==0)
		{
		 	it.second() >> shader_name;
		 	//s.update_shader(shader_name);
            for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                ////std::cout<< *tm; // output the current value that It is *pointing to
                std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                s->update_shader(shader_name);

            }

            
		}
		else if(key.compare("tex_name")==0)
		{
			it.second() >> tex_name;
			//s.update_texture(tex_name);
            for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                ////std::cout<< *tm; // output the current value that It is *pointing to
                std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                s->update_texture(tex_name);
            }
		}
		else if(key.compare("int_in")==0)
		{
			it.second() >> int_in;
			//s.update_int_in(int_in);
            for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                ////std::cout<< *tm; // output the current value that It is *pointing to
                std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                s->update_int_in(int_in);
            }
            
		}
		else if(key.compare("float_in")==0)
		{
			it.second() >> float_in;
			//s.update_float_in(float_in);
            for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                ////std::cout<< *tm; // output the current value that It is *pointing to
                std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                s->update_float_in(float_in);
            }
		}
		else if(key.compare("rotate")==0)
		{
			it.second()[0] >> rx;
			it.second()[1] >> ry;
			it.second()[2] >> rz;
			if(instruction.compare("add")==0)
				//s.add_rotate(rx,ry,rz);
                for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                    ////std::cout<< *tm; // output the current value that It is *pointing to
                    std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                    Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                    s->add_rotate(rx,ry,rz);
                }
			else if(instruction.compare("gradual")==0)
				//s.gradual_rotate(rx,ry,rz,duration);
                for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                    ////std::cout<< *tm; // output the current value that It is *pointing to
                    std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                    Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                    s->gradual_rotate(rx,ry,rz,duration);
                }
			else
				//s.update_rotate(rx,ry,rz);
                for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                    ////std::cout<< *tm; // output the current value that It is *pointing to
                    std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                    Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                    s->update_rotate(rx,ry,rz);
                }
		}
		else if(key.compare("scale")==0)
		{
			it.second()[0] >> sx;
			it.second()[1] >> sy;
			it.second()[2] >> sz;
			if(!use_pixels)
			{
				if(instruction.compare("add")==0)
					//s.add_scale(sx,sy,sz);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->add_scale(sx,sy,sz);
                    }
				else if(instruction.compare("gradual")==0)
					//s.gradual_scale(sx,sy,sz,duration);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->gradual_scale(sx,sy,sz,duration);
                    }
	            else
	            	//s.update_scale(sx,sy,sz);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->update_scale(sx,sy,sz);
                    }
        	}
		}
		else if(key.compare("translate")==0)
		{
			it.second()[0] >> tx;
			it.second()[1] >> ty;
			it.second()[2] >> tz;
			if(!use_pixels)
			{
				if(instruction.compare("add")==0)
					//s.add_translate(tx,ty,tz);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->add_translate(tx,ty,tz);
                    }
				else if(instruction.compare("gradual")==0)
					//s.gradual_translate(tx,ty,tz,duration);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->gradual_translate(tx,ty,tz,duration);
                    }
	            else
	            	//s.update_translate(tx,ty,tz);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->update_translate(tx,ty,tz);
                    }
	        }
		}
		else if(key.compare("rgbcolor")==0)
		{
			it.second()[0] >> r;
			it.second()[1] >> g;
			it.second()[2] >> b;
			if(instruction.compare("gradual")==0)
				//s.gradual_color(r, g, b, duration);
                for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                    ////std::cout<< *tm; // output the current value that It is *pointing to
                    std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                    Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                    s->gradual_color(r, g, b, duration);
                }
			else
				//s.update_color(r, g, b);
                for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                    ////std::cout<< *tm; // output the current value that It is *pointing to
                    std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                    Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                    s->update_color(r,g,b);
                }
		}
		else if(key.compare("alpha")==0)
		{
			it.second() >> alpha;
			if(instruction.compare("gradual")==0)
				//s.gradual_fade(alpha, duration);
                for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                    ////std::cout<< *tm; // output the current value that It is *pointing to
                    std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                    Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                    s->gradual_fade(alpha, duration);
                }
			else
				//s.update_alpha(alpha);
                for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                    ////std::cout<< *tm; // output the current value that It is *pointing to
                    std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                    Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                    s->update_alpha(alpha);
                }
		}
		else if(key.compare("has_texture")==0)
		{
			std::string temp;
			it.second() >> temp;
			if(temp.compare("true")==0)
				has_texture = true;
			else has_texture = false;
			//s.use_texture(has_texture);
            for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                ////std::cout<< *tm; // output the current value that It is *pointing to
                std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                s->use_texture(has_texture);
            }
		}
		else if(key.compare("pixel")==0)
		{
			/*pixel format:
             [0] = x position
             [1] = y position
             [2] = width
             [3] = height
             [4] = layer*/
            
			int x, y, width, height, layer;
			it.second()[0] >> x;
			it.second()[1] >> y;
			it.second()[2] >> width;
			it.second()[3] >> height;
			it.second()[4] >> layer;
			//window width = 768+768/15.4*1.235*2+768, window height = 1366
			if (use_pixels)
			{
				if(instruction.compare("add")==0)
				{
					//s.add_size(width, height);
					//s.add_position(x,y);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->add_size(width, height);
                    }
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->add_position(x,y);
                    }
				}
				else if(instruction.compare("gradual")==0)
				{
					//s.gradual_size(width, height, duration);
					//s.gradual_position(x,y, duration);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->gradual_size(width, height, duration);
                    }
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->gradual_position(x,y, duration);
                    }
				}
				else
				{
					//s.update_size(width, height);
					//s.update_position(x, y, layer);
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->update_size(width, height);
                    }
                    for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm){
                        ////std::cout<< *tm; // output the current value that It is *pointing to
                        std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                        Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                        s->update_position(x, y, layer);
                    }
				}
    
            }
            
        } 
        else if(key.compare("rotate_around_point")==0)
        {
        	bool rotate_around_point;
        	it.second() >> dummy;
            if(dummy.compare("false")==0)
                rotate_around_point = false;
            else rotate_around_point = true;

            for(std::vector<std::string>::iterator tm = string_names.begin(); tm != string_names.end(); ++tm)
            {
                ////std::cout<< *tm; // output the current value that It is *pointing to
                std::string sprite_name = *tm;// it will iterate through and give out sprite1, square1,........ it doesnt matter whats the name
                Sprite *s = &(*sprite_map)[sprite_name];// map has strings as key so u cannot access sprite that has vector u will have to do it take a string
                s->use_rotate_point(rotate_around_point);
            }	
        }  
    }

    //test
    for(std::map<std::string, Sprite>::iterator it = (*sprite_map).begin(); it != (*sprite_map).end(); ++it)
    {
    	//std::cout<<"sprite name = "<< it->first <<std::endl;
    }

}






void Configuration::ParseSprites(std::string yamlString, std::map<std::string,Sprite> *sprite_map)
{
	//std::cout<<"parsing sprites";
	std::stringstream inStream (yamlString, std::stringstream::in);
	YAML::Parser parser(inStream);
	YAML::Node doc;
	parser.GetNextDocument(doc);
	std::vector<std::string> sprite_names;
    doc["sprite_names"] >> sprite_names;
    
	for(YAML::Iterator it=doc.begin(); it !=doc.end(); it++)
	{
		std::string node_name;
		it.first() >> node_name;
        
		if(node_name.compare("todo") == 0)
        //YAML::Node todo_list;
		{
            /*
			YAML::Node todo_list;
            todo_list = it.second();
            it.second() >> todo_list;*/
 			ParseTodo(it.second(), sprite_names, sprite_map); //yaml node   I am a little confused about how to call the vector.
		}
        
		if(node_name.compare("address") == 0) continue;
		if(node_name.compare("sprite_names") == 0) continue;
		//it.second() >> (*sprite_map)[node_name];
		//if((*um)[sprite_name].update.compare("true") == 0) UpdateContainers(cm,um);
        //printContainer((*cm)[sprite_name]);
	}
}


// Parse yaml configuration file and create data structures.
// TODO: all of this is hard coded at the moment for type safety.
bool Configuration::LoadConfiguration(std::string path) {
	std::ifstream configFile(path.c_str());
	if(configFile.fail()) {
		//std::cout << "Failed to open configuration file " << path << std::endl;
        return false;
	}
	try {
		//parse the file
		YAML::Parser parser1(configFile);
		YAML::Node currentDocument;
		// We currently assume one document per file
		parser1.GetNextDocument(currentDocument);
		const YAML::Node& listener = currentDocument["oscListener"];
		parseListener (listener);
		const YAML::Node& commandNode = currentDocument["instructions"];
		parseCommands(commandNode);
		const YAML::Node& ensembleNode = currentDocument["players"];
		parseEnsemble(ensembleNode);
		return true;
	}
	catch (YAML::ParserException& ex) {
		//std::cout << ex.what() << "\n";
        return false;
		//TODO: do something besides report
	}
	
	return true;
}


void Configuration::parseListener (const YAML::Node& listenerNode) {
	listenerNode["port"] >> oscPortIn;
	OscMessage messageSpec;
	std::string address, typeString, event;
	//std::cout << "OSC port listed in Configuration is " << oscPortIn << "\n";
	const YAML::Node& messageNode = listenerNode["messages"];
    
	for (unsigned i = 0; i<messageNode.size(); i++) {
		const YAML::Node& msg = messageNode[i];
		msg["address"] >> messageSpec.address;
		msg["typeString"] >> messageSpec.typeString;
		msg["event"] >> messageSpec.event;
		////std::cout << "Address: " << messageSpec.address << ", typeString: " << messageSpec.typeString
		//	<< ", event: " << messageSpec.event << std::endl;
		messageSpecs.push_back(messageSpec);
	}
	////std::cout << messageSpecs.size() << " message types defined " << std::endl;
} // end parseListener

// TODO: much more error handling and optional elements
// parses "Commands" node of configuration file
void Configuration::parseCommands (const YAML::Node& commandNode) {
	GrendlCommand cmdSpec;
	////std::cout << commandNode.size() << " grendl commands in the file" << std::endl;
	for (unsigned i = 0; i<commandNode.size(); i++) {
		//GrendlConfiguration::OscMessage msg;
		const YAML::Node& cmd = commandNode[i];
		cmd["event"] >> cmdSpec.event;
		cmd["command"] >> cmdSpec.name;
		cmd["file"] >> cmdSpec.file;
		////std::cout << "Event: " << cmdSpec.event << ", name: " << cmdSpec.name
		//<< ", file: " << cmdSpec.file << std::endl;
		commandSpecs.push_back(cmdSpec);
	}
	////std::cout << commandSpecs.size() << " command types defined " << std::endl;
	
}

// TODO: much more error handling and optional elements
// TODO: currrently we empty the list every time. Make sure this is what we want.
// parses "Ensemble" node of configuration file
void Configuration::parseEnsemble (const YAML::Node& ensembleNode) {
	EnsembleMember player;
	ensembleMembers.clear();
	////std::cout << ensembleNode.size() << " ensemble members in the file" << std::endl;
	for (unsigned i = 0; i<ensembleNode.size(); i++) {
		const YAML::Node& member = ensembleNode[i];
		member["name"] >> player.name;
		member["username"] >> player.username;
		member["hostname"] >> player.hostname;
		member["hostip"] >> player.hostip;
		member["receiverport"] >> player.receiverport;
		//member["name"] >> player.userAtHost;
		player.userAtHost = std::string (player.username + "@" + player.hostip);
		////std::cout << "Member: " << player.name << " host: " << player.userAtHost << std::endl;
		ensembleMembers.push_back(player);
	}
	////std::cout << ensembleMembers.size() << " members defined " << std::endl;
	
}


/**
	name: vsample
	numverts: 3
	numelems: 3
	vertices:
	 - 1.0
	 - 1.0
	 - 0.0
	 - 0.0
	 - 0.0
	 - 0.0
	 - 1.0
	 - 0.0
	 - 0.0
	elements:
	 - 0
	 - 1
	 - 2
**/

void Configuration::ParseVertexData(std::string yamlString, SpriteView *view)
{
	std::stringstream inStream (yamlString, std::stringstream::in);
	YAML::Parser parser(inStream);
	YAML::Node doc;
	parser.GetNextDocument(doc);

	std::string name;
	int numElems, numVerts;
	vert *verts;
	GLushort *elems;

	doc["numverts"] >> numVerts;
	doc["numelems"] >> numElems;

	verts = new vert[numVerts];
	elems = new GLushort[numElems];

	for(YAML::Iterator it=doc.begin(); it !=doc.end(); it++)
	{
		std::string node_name;
		it.first() >> node_name;
		if(node_name.compare("vertices")==0)
		{
			for(int i=0; i<numVerts*3; i+=3)
			{
				std::cout<<"Getting vert "<<i<<std::endl;
				it.second()[i+0] >> verts[i/3].v[0];
				it.second()[i+1] >> verts[i/3].v[1];
				it.second()[i+2] >> verts[i/3].v[2];
				verts[i/3].n[0] = verts[i/3].n[1] = 0.f;
				verts[i/3].n[2] = 1.f;
				verts[i/3].t[0] = verts[i/3].t[1] = 1.f;
			}
		}
		else if(node_name.compare("elements")==0)
		{
			for(int i=0; i<numElems; i++)
			{
				it.second()[i] >> elems[i];
			}
		}
		else if(node_name.compare("name")==0)
		{
			it.second() >> name;
		}
	}

	view->init_vbo(numVerts, numElems, verts, elems, name);

}
