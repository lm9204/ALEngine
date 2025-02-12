#pragma once

#include "Physics/BroadPhase.h"
#include "Physics/Contact/Contact.h"

namespace ale
{
class ContactManager
{
  public:
	ContactManager();
	void addPair(void *proxyUserDataA, void *proxyUserDataB);
	void findNewContacts();
	bool isSameContact(ContactLink *link, Fixture *fixtureA, Fixture *fixtureB, int32_t indexA, int32_t indexB);
	void collide();

	BroadPhase m_broadPhase;
	Contact *m_contactList;
	int32_t m_contactCount;
};
} // namespace ale