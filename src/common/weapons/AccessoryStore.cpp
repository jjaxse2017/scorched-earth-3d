////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2011
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#include <XML/XMLFile.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Weapon.h>
#include <lang/LangResource.h>
#include <math.h>
#include <stdio.h>

AccessoryStore::AccessoryStore() : muzzleFlash_(0)
{

}

AccessoryStore::~AccessoryStore()
{
	clearAccessories();
}

void AccessoryStore::clearAccessories()
{
	AccessoryPart::resetAccessoryPartIds();
	Accessory::resetAccessoryIds();
	muzzleFlash_ = 0;
	deathAnimation_ = 0;
	while (!accessories_.empty())
	{
		Accessory *accessory = accessories_.front();
		accessories_.pop_front();
		delete accessory;
	}
	accessoriesById_.clear();
	accessoryParts_.clear();
	tabGroups_.clear();
}

bool AccessoryStore::parseFile(
	ScorchedContext &context, ProgressCounter *counter)
{
	if (counter) counter->setNewOp(LANG_RESOURCE("LOADING_WEAPONS", "Loading Weapons"));

	std::string fileName = S3D::getModFile("data/accessories.xml");
	clearAccessories();

	XMLFile file;
	if (!file.readFile(fileName.c_str()))
	{
		S3D::dialogMessage("AccessoryStore", S3D::formatStringBuffer(
					  "Failed to parse \"%s\"\n%s", 
					  fileName.c_str(),
					  file.getParserError()));
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		S3D::dialogMessage("AccessoryStore", S3D::formatStringBuffer(
					"Failed to find accessory file \"%s\"",
					fileName.c_str()));
		return false;		
	}

	// Itterate all of the accessories in the file
	int noChildren = (int) file.getRootNode()->getChildren().size();
	int childCount = 0;
	XMLNode *currentNode = 0;
	while (file.getRootNode()->getNamedChild("accessory", currentNode, false))
	{
		if (counter) counter->setNewPercentage(
			float(++childCount) / float(noChildren) * 100.0f);

		// Parse the accessory
		AccessoryCreateContext createContext(context);
		Accessory *accessory = new Accessory();
		if (!accessory->parseXML(createContext, currentNode))
		{
			return currentNode->returnError(
				S3D::formatStringBuffer("Failed to create accessory \"%s\"",
				accessory->getName()));
		}

		// Check uniqueness
		if (findByPrimaryAccessoryName(accessory->getName()))
		{
			return currentNode->returnError(
				S3D::formatStringBuffer("Accessory \"%s\" already exists",
				accessory->getName()));
		}

		if (!accessory->getNoBuy() &&
			accessory->getMaximumNumber() != 0)
		{
			tabGroups_.insert(accessory->getTabGroupName());
		}

		// Add the accessory
		accessoriesById_[accessory->getAccessoryId()] = accessory;
		accessories_.push_back(accessory);

		// Add weapons to death animations, weighted by arms level
		if (accessory->getAction()->getType() == AccessoryPart::AccessoryWeapon)
		{
			if (0 == strcmp(accessory->getName(), "WeaponMuzzle"))
			{
				muzzleFlash_ = accessory;
			}
			else if (0 == strcmp(accessory->getName(), "WeaponDeathAnimation"))
			{
				deathAnimation_ = accessory;
			}
		}	

		// Add to the map so references can find it
		parsingNodes_[accessory->getName()] = currentNode;
	}

	if (!muzzleFlash_)
	{
		return file.getRootNode()->returnError(
			"Failed to find WeaponMuzzle weapon used for muzzle flash.");
	}
	if (!deathAnimation_)
	{
		return file.getRootNode()->returnError(
			"Failed to find WeaponDeathAnimation weapon used for tank explosions.");
	}

	// Clear mapping as it now contains invalid pointers
	parsingNodes_.clear();
	return file.getRootNode()->failChildren();
}

AccessoryPart *AccessoryStore::createAccessoryPart(
	AccessoryCreateContext &context, 
	Accessory *parent, XMLNode *currentNode)
{
	XMLNode *typeNode = 0;
	if (!currentNode->getNamedParameter("type", typeNode)) return 0;

	AccessoryPart *accessoryPart = 
		AccessoryMetaRegistration::getNewAccessory(typeNode->getContent(), this);
	if (!accessoryPart)
	{
		S3D::dialogMessage("AccessoryStore", S3D::formatStringBuffer(
						"Failed to find accessory part type \"%s\"",
						typeNode->getContent()));
		return 0;
	}
	// Set the parent accessory
	accessoryPart->setParent(parent);
	
	// Tell this accessory instance to initialize its settings from
	// the current accessory xml definition node
	if (!accessoryPart->parseXML(context, currentNode)) return 0;

	// There should not be any children left
	// Any that are, are children that have not been
	// handled by the parse routine
	if (!currentNode->failChildren()) return 0;
	DIALOG_ASSERT(accessoryPart->getParent());

	// Add the accessory
	accessoryParts_.push_back(accessoryPart);
	return accessoryPart;
}

void AccessoryStore::sortList(std::list<Accessory *> &accList, int sortKey)
{
	if (sortKey)
	{
		std::vector<Accessory *> accVector;
		std::list<Accessory *>::iterator itor;
		for (itor = accList.begin();
			itor != accList.end();
			++itor)
		{
			accVector.push_back(*itor);
		}

		// Crudely sort by name or price
		// stl sort method list is broken in visual c 6
		// bubble sort
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (int i=0; i<int(accVector.size())-1; i++)
			{
				bool swap = false;

				// When sorting by price, use accessory name as a
				// secondary sort key.

				if ((sortKey == SortName) ||
				    (sortKey == SortPrice && accVector[i]->getPrice() == accVector[i + 1]->getPrice()))
				{
					swap = strcmp(accVector[i]->getName(), accVector[i + 1]->getName()) < 0;
				}
				else if (sortKey == SortPrice)
				{
					swap = accVector[i]->getPrice() < accVector[i + 1]->getPrice();
				}

				if (swap)
				{
					Accessory *tmp = accVector[i];
					accVector[i] = accVector[i+1];
					accVector[i+1] = tmp;
					changed = true;
					break;
				}
			}
		} 
		
		accList.clear();
		for (int i=0; i<int(accVector.size()); i++)
		{
			accList.push_front(accVector[i]);
		}
	}
	else
	{
		std::set<Accessory *> accessorySet;
		std::list<Accessory *>::iterator setItor;
		for (setItor = accList.begin();
			setItor != accList.end();
			++setItor)
		{
			Accessory *accessory = *setItor;
			accessorySet.insert(accessory);
		}

		accList.clear();
		std::list<Accessory *>::iterator itor;
		for (itor = accessories_.begin();
			itor != accessories_.end();
			++itor)
		{
			Accessory *accessory = *itor;
			if (accessorySet.find(accessory) != accessorySet.end())
			{
				accList.push_back(accessory);
			}
		}
	}
}

std::list<Accessory *> AccessoryStore::getAllAccessoriesByTabGroup(
	const char *tabgroup, int sortKey)
{
	std::list<Accessory *> result;
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		++itor)
	{
		Accessory *accessory = (*itor);
		if (0 == strcmp(tabgroup, accessory->getTabGroupName()))
		{
			result.push_back(*itor);
		}
	}

	if (sortKey) sortList(result, sortKey);
	return result;
}

std::list<Accessory *> AccessoryStore::getAllAccessories(int sortKey)
{
	std::list<Accessory *> result;
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		++itor)
	{
		result.push_back(*itor);
	}

	if (sortKey) sortList(result, sortKey);
	return result;
}

Accessory *AccessoryStore::getMuzzelFlash()
{
	return muzzleFlash_;
}

Accessory *AccessoryStore::getDeathAnimation()
{
	return deathAnimation_;
}

Accessory *AccessoryStore::findByPrimaryAccessoryName(const char *name)
{
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		++itor)
	{
		Accessory *accessory = (*itor);
		if (strcmp(accessory->getName(), name) == 0)
		{
			return accessory;
		}
	}
	return 0;
}

Accessory *AccessoryStore::findByAccessoryId(unsigned int id)
{
	std::map<unsigned int, Accessory *>::iterator itor = 
		accessoriesById_.find(id);
	if (itor == accessoriesById_.end()) return 0;
	return itor->second;
}

AccessoryPart *AccessoryStore::findAccessoryPartByAccessoryId(unsigned int id, const char *type)
{
	std::list<AccessoryPart *>::iterator itor;
	for (itor = accessoryParts_.begin();
		itor != accessoryParts_.end();
		++itor)
	{
		AccessoryPart *accessoryPart = (*itor);
		if (accessoryPart->getParent()->getAccessoryId() == id &&
			0 == strcmp(accessoryPart->getAccessoryTypeName(), type))
		{
			return accessoryPart;
		}
	}
	return 0;
}

bool AccessoryStore::writeAccessory(NamedNetBuffer &buffer, Accessory *accessory)
{
	if (accessory) buffer.addToBufferNamed("accessoryId", accessory->getAccessoryId());
	else buffer.addToBufferNamed("accessoryId", (unsigned int) 0);
	return true;
}

bool AccessoryStore::readAccessory(NetBufferReader &reader, Accessory *&accessory)
{
	accessory = 0;

	unsigned int accessoryId;
	if (!reader.getFromBuffer(accessoryId)) return false;
	if (accessoryId == 0) return true;

	accessory = findByAccessoryId(accessoryId);
	if (accessory) return true;
	return false;
}

bool AccessoryStore::writeEconomyToBuffer(NetBuffer &buffer)
{
	std::list<Accessory *> accessories = getAllAccessories();
	buffer.addToBuffer((int) accessories.size());

	std::list<Accessory *>::iterator itor;
	for (itor = accessories.begin();
		itor != accessories.end();
		++itor)
	{
		Accessory *accessory = (*itor);
		buffer.addToBuffer(accessory->getAccessoryId());
		buffer.addToBuffer(accessory->getPrice());
		buffer.addToBuffer(accessory->getSellPrice());
	}
	return true;
}

bool AccessoryStore::readEconomyFromBuffer(NetBufferReader &reader)
{
	int noAccessories = 0;
	if (!reader.getFromBuffer(noAccessories)) return false;
	for (int a=0; a<noAccessories; a++)
	{
		unsigned int accessoryId = 0;
		int price = 0, sellPrice = 0;
		if (!reader.getFromBuffer(accessoryId)) return false;
		if (!reader.getFromBuffer(price)) return false;
		if (!reader.getFromBuffer(sellPrice)) return false;

		Accessory *accessory = findByAccessoryId(accessoryId);
		if (!accessory) return false;
		accessory->setPrice(price);
		accessory->setSellPrice(sellPrice);
	}
	return true;
}
