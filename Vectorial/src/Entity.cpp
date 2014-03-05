/*
 * Entity.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: maferrar
 */

#include "Entity.h"

Entity::Entity(std::string name, EntityType type):
	name_(name),type_(type)
{
	// TODO Auto-generated constructor stub

}

Entity::~Entity() {
}

std::string Entity::getName()
{
	return name_;
}
EntityType Entity::getType()
{
	return type_;
}
