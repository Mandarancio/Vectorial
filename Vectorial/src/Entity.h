/*
 * Entity.h
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>

enum EntityType{
	NONE,GRAPHICAL
};

class Entity {
public:
	Entity(std::string name,EntityType type=NONE);
	virtual ~Entity();
	std::string getName();
	EntityType 	getType();
private:
	std::string name_;
	EntityType type_;
};

#endif /* ENTITY_H_ */
