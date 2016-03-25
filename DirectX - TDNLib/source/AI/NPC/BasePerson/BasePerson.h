#pragma once
#include "AI\State\StateMachine.h"
#include "AI\Entity\EntityManager.h"
#include "AI\Message\MessageDispatcher.h"



/*
	Šî–{‚Ìl
*/
class BasePerson :public BaseGameEntity
{
public:
	BasePerson(int id);
	~BasePerson();

private:

};


BasePerson::BasePerson(int id):BaseGameEntity(id)
{



}

BasePerson::~BasePerson()
{



}
